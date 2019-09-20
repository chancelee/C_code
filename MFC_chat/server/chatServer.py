import threading
from MsgHandler import *
import time

from Database import Database

'''
    msg format:
    type:1 forward
    1 : sockFrom : sockTo : fromName : toName : time : msg
    struct{
        BYTE type
    }
'''

ip = "127.0.0.1"
port = 8888





class Server:

    def __init__(self,ip,port):

        self.dictOnlineClientAddr = {}
        '''
            dictOnlineClientAddr format:
                {
                    nickname : socket
                }
        '''

        self.dictFriend = {}
        '''
            dictFriend format:
                {
                    nickname : [friendname1, ...]
                }
        '''
        self.dictGroupInfo = getGroupInfo()
        '''
            dictFriend format:
                {
                    groupname : [creator, member...]
                }
        '''

        self.sockServ = socket.socket()
        self.sockServ.bind((ip,port))
        self.sockServ.listen(socket.SOMAXCONN)


    def run(self):
        print("server start.")
        while True:
            sockClient, clientAddr = self.sockServ.accept()
            if sockClient:
                print(time.strftime("%Y-%m-%d %H:%M:%S  ", time.localtime()) ,clientAddr, " connected.")

                threading.Thread(
                    target=self.recvData,
                    args=(sockClient,)
                ).start()

    def recvData(self,sockClient:socket.socket):
        try:
            while True:
                # dataBody has been decoded
                dataType, listDataBody = recvFrom(sockClient)

                Server.dictHandler[dataType](self, sockClient, listDataBody)
        finally:
            for username in self.dictOnlineClientAddr.keys():
                if self.dictOnlineClientAddr[username] == sockClient:
                    del(self.dictOnlineClientAddr[username])
                    sockClient.close()

    def __register(self, sockClient, listDataBody):
        #
        #   listDataBody format:
        #       [username, password]
        #
        register(sockClient, listDataBody)

    def __login(self, sockClient, listDataBody):
        #
        #   listDataBody format:
        #       [nickname, password]
        #
        ret = login(sockClient, listDataBody)
        nickname = listDataBody[0]
        if ret:
            self.dictOnlineClientAddr[nickname] = sockClient;
            self.dictFriend[nickname] = []

            friendlist = getFriendItem(nickname)
            self.dictFriend[nickname].extend(friendlist)
            for friendname in self.dictFriend[nickname]:
                sendTo(self.dictOnlineClientAddr[nickname],
                       SHOWFRIEND,
                       friendname)
            for groupname in self.dictGroupInfo.keys():
                if nickname in self.dictGroupInfo[groupname]:
                    sendTo(self.dictOnlineClientAddr[nickname],
                       SHOWGROUP,
                       groupname
                    )

    def __sendToFriend(self, sockClient, listDataBody):
        #
        #   listDataBody format:
        #       [fromname, friendname, data]
        #
        friendname = listDataBody[1]
        if friendname in self.dictOnlineClientAddr.keys():
            sockFriend = self.dictOnlineClientAddr[friendname]
            fromname = listDataBody[0]
            data = listDataBody[2]
            sendTo(sockFriend, SENDTOFRIEND, fromname + ":" + data)

    def __addFriend(self, sockClient, listDataBody):
        #
        #   listDataBody format:
        #       [fromname, status, friendname]
        #   status:
	    #        "0" : add request
        #        "1" : agree
        #       "-1" : disagree
        fromname = listDataBody[0]
        status = listDataBody[1]
        friendname = listDataBody[2]
        if status == REQUEST:
            if friendname in self.dictOnlineClientAddr.keys():
                sendTo(self.dictOnlineClientAddr[friendname],
                       ADDFRIEND,
                       fromname)
        elif status == AGREE:
            self.dictFriend[fromname].append(friendname)
            self.dictFriend[friendname].append(fromname)
            addFriendItem(fromname, friendname)
            sendTo(self.dictOnlineClientAddr[fromname],
                   SHOWFRIEND,
                   friendname)
            sendTo(self.dictOnlineClientAddr[friendname],
                   SHOWFRIEND,
                   fromname)

    def __createGroup(self, sockClient, listDataBody):
        #
        #   listDataBody format:
        #       [creatorname, groupname]
        #
        creatorname = listDataBody[0]
        groupname = listDataBody[1]
        if createGroup(groupname, creatorname):
            self.dictGroup[groupname] = [creatorname]
            #for i in self.dictGroupInfo[groupname]:
            sendTo(self.dictOnlineClientAddr[creatorname],
                   SHOWGROUP,
                   groupname)
        else:
            pass

    def __joinGroup(self, sockClient, listDataBody):
        #
        #   listDataBody format:
        #       [fromname, status, groupname]
        #   status:
        #        "0" : add request
        #        "1" : agree
        #       "-1" : disagree
        fromname = listDataBody[0]
        status = listDataBody[1]
        groupname = listDataBody[2]
        if status == REQUEST:
            creatorname = self.dictGroupInfo[groupname][0]
            if creatorname in self.dictOnlineClientAddr.keys():
                sendTo(self.dictOnlineClientAddr[creatorname],
                       JOINGROUP,
                       fromname + ":" + groupname)
        elif status == AGREE:
            addGroupMember(groupname, fromname)
            self.dictGroupInfo[groupname].append(fromname)
            sendTo(self.dictOnlineClientAddr[fromname],
                   SHOWGROUP,
                   groupname)

    def __sendToGroup(self, sockClient, listDataBody):
        #
        #   listDataBody format:
        #       [fromname, groupname, data]
        #
        groupname = listDataBody[1]
        if groupname in self.dictGroupInfo.keys():
            fromname = listDataBody[0]
            data = listDataBody[2]
            for membername in self.dictGroupInfo[groupname]:
                if membername != fromname:
                    sendTo(self.dictOnlineClientAddr[membername],
                           GROUPMSG,
                           ":".join(listDataBody))

    dictHandler = {
        REGISTER: __register,
        LOGIN: __login,
        SENDTOFRIEND: __sendToFriend,
        ADDFRIEND: __addFriend,
        CREATEGROUP:__createGroup,
        GROUPMSG: __sendToGroup,
        JOINGROUP: __joinGroup
    }


if __name__ == "__main__":
    server = Server("127.0.0.1", 8888)
    server.run()
