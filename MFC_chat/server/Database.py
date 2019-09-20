import pymysql

class Database:
    def __init__(self):
        self.connecter = pymysql.connect(host='127.0.0.1',
                                         user='root',
                                         password='',
                                         db='chat',
                                         charset='utf8',
                                         cursorclass=pymysql.cursors.DictCursor)
        self.cursor = self.connecter.cursor(cursor=pymysql.cursors.DictCursor)

