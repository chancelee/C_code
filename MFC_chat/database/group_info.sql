CREATE TABLE `group_info` (
  `groupID` int(11) NOT NULL AUTO_INCREMENT,
  `groupname` varchar(20) NOT NULL,
  `creatorID` int(11) DEFAULT NULL,
  PRIMARY KEY (`groupID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 