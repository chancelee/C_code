import Database
import socket
import struct
import hashlib

REGISTER = 0
LOGIN = 1

SENDTOFRIEND = 2
GROUPMSG = 3

ADDFRIEND = 4
SHOWFRIEND = 5

CREATEGROUP = 6
JOINGROUP = 7
SHOWGROUP = 8

REQUEST = "0"
AGREE = "1"
DISAGREE = "2"

_db = Database.Database()

def encrypt(data):
    data = bytes(map(lambda x:x^88,data))
    return data
    # for i in range(len(data)):
    #     #ret += chr(data[i] ^ 88)
    #     data[i] ^= b'\x58'
    # return data


def sendTo(sockClient:socket.socket, type, data):
    data = data.encode("gb2312")
    size = len(data)
    data = encrypt(data)
    msg = struct.pack("ii%ds" % size, type, size, data)
    sockClient.send(msg)

def recvFrom(sockClient:socket.socket):
    head = sockClient.recv(8)
    if len(head) == 0:
        raise  Exception("client disconnected");

    dataType, dataSize = struct.unpack("ii", head)
    print("type: {0}\nsize: {1}".format(dataType, dataSize))

    dataBody = sockClient.recv(dataSize)
    dataBody = encrypt(dataBody)
    print("data:{}".format(dataBody.decode("utf-16")))
    #return dataType, dataBody.split(":")

    return dataType, dataBody.decode("utf-16").split(":")

def md5(s):
    sha256 = hashlib.sha256()
    sha256.update(s.encode("gb2312"))
    return sha256.hexdigest()

def register(sockClient : socket.socket, dataBody : list):
    #
    #   listDataBody format:
    #       [username, password]
    #
    FAIL = "0";
    SUCCESS = "1";

    print(dataBody)
    nickname = dataBody[0]

    sql = "select ID from user_info where nickname = '%s'" % nickname

    _db.cursor.execute(sql)
    result = _db.cursor.fetchone()
    if result != None:
        sendTo(sockClient, REGISTER, FAIL)
        return False


    password = md5(dataBody[1])

    sql = "INSERT INTO user_info (nickname, password) VALUES ('%s','%s');" % \
          (nickname, password)
    _db.cursor.execute(sql)
    _db.cursor.execute("commit;")
    sendTo(sockClient, REGISTER, SUCCESS)

def login(sockClient : socket.socket, dataBody : list):
    #
    #   dataBody format:
    #       [username, password]
    #
    FAIL = "0"
    SUCCESS = "1"
    sql = "select nickname,password from user_info where nickname = '%s'" % dataBody[0]
    _db.cursor.execute(sql)
    userinfo = _db.cursor.fetchone()
    if userinfo == None:
        sendTo(sockClient, LOGIN, FAIL)
        return
    if userinfo["nickname"] == dataBody[0] and \
                    userinfo["password"] == md5(dataBody[1]):
        sendTo(sockClient, LOGIN, SUCCESS)
        return True
    else:
        sendTo(sockClient, LOGIN, FAIL)
        return False

def addFriendItem(fromname, friendname):
    sql = "SELECT ID FROM user_info WHERE nickname = '%s' or nickname = '%s'" % (fromname, friendname)
    _db.cursor.execute(sql)
    ID1 = _db.cursor.fetchone()
    ID2 = _db.cursor.fetchone()

    sql = "INSERT INTO friend_info VALUES (%d,%d)" % (ID1["ID"], ID2["ID"])
    _db.cursor.execute(sql)
    sql = "INSERT INTO friend_info VALUES (%d,%d)" % (ID2["ID"], ID1["ID"])
    _db.cursor.execute(sql)
    _db.cursor.execute("commit;")

def getFriendItem(nickname):

    ret = []

    # 查找用户ID
    sql = "SELECT ID FROM user_info WHERE nickname = '%s'" % nickname
    _db.cursor.execute(sql)
    userID = _db.cursor.fetchone()

    # 根据用户ID查找朋友
    sql = "SELECT f2 FROM friend_info WHERE f1 = %d" % userID["ID"]
    _db.cursor.execute(sql)
    friendIDs = _db.cursor.fetchall()
    for friendID in friendIDs:
        sql = "SELECT nickname FROM user_info WHERE ID = %d" % friendID["f2"]
        _db.cursor.execute(sql)
        friendname = _db.cursor.fetchone()
        ret.append(friendname["nickname"])
    return ret
    pass

def createGroup(groupname:str, creator:str):

    # 是否存在群聊
    sql = "SELECT groupID FROM group_info WHERE groupname = '%s';" % groupname
    _db.cursor.execute(sql)
    exist = _db.cursor.fetchone()
    if exist:
        return False

    # 获取创建者ID
    sql = "SELECT ID from user_info WHERE nickname = '%s';" % creator
    _db.cursor.execute(sql)
    creatorID = _db.cursor.fetchone()

    # 添加群聊
    sql = "INSERT INTO group_info(creatorID, groupname) VALUES (%d, '%s');" % (creatorID["ID"], groupname)
    _db.cursor.execute(sql)
    _db.cursor.execute("commit;")

    # 获取群聊ID
    sql = "SELECT groupID from group_info WHERE groupname = '%s';" % groupname
    _db.cursor.execute(sql)
    groupID = _db.cursor.fetchone()


    # 第一个成员为创建者
    sql = "INSERT INTO group_member(groupID, memberID) VALUES (%d, %d);" % (groupID["groupID"], creatorID["ID"])
    _db.cursor.execute(sql)
    _db.cursor.execute("commit;")
    return True

def getGroupInfo():
    dictGroupInfo = {}

    # 获取群聊ID
    sql = "SELECT * FROM group_info;"
    _db.cursor.execute(sql)
    groupInfo = _db.cursor.fetchall()

    # 通过每个群聊ID获取成员
    for group in groupInfo:
        dictGroupInfo[group["groupname"]] = []

        # 获取创建者昵称
        sql = "SELECT nickname FROM user_info WHERE ID = %d;" % group["creatorID"]
        _db.cursor.execute(sql)
        creatorname = _db.cursor.fetchone()
        dictGroupInfo[group["groupname"]].append(creatorname["nickname"])

        # 获取组员昵称
        sql = "select nickname from user_info"\
              " where ID in "\
              "(select memberID from group_member where groupID = %d);"\
                % group["groupID"]
        _db.cursor.execute(sql)
        membernames = _db.cursor.fetchall()
        for member in membernames:
            if member["nickname"] != creatorname["nickname"]:
                dictGroupInfo[group["groupname"]].append(member["nickname"])

    return dictGroupInfo

def addGroupMember(groupname:str, newmember:str):
    # 获取群聊ID
    sql = "SELECT groupID from group_info WHERE groupname = '%s';" % groupname
    _db.cursor.execute(sql)
    groupID = _db.cursor.fetchone()

    # 获取新成员ID
    sql = "SELECT ID from user_info WHERE nickname = '%s';" % newmember
    _db.cursor.execute(sql)
    memberID = _db.cursor.fetchone()

    # 添加
    sql = "INSERT INTO group_member(groupID, memberID) VALUES (%d, %d);" % (groupID["groupID"], memberID["ID"])
    _db.cursor.execute(sql)
    _db.cursor.execute("commit;")

def sendToGroup():
    pass