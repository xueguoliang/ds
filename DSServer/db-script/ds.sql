CREATE DATABASE  IF NOT EXISTS `ds` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `ds`;
-- MySQL dump 10.13  Distrib 5.5.50, for debian-linux-gnu (x86_64)
--
-- Host: 127.0.0.1    Database: ds
-- ------------------------------------------------------
-- Server version	5.5.50-0ubuntu0.14.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tloginlog`
--

DROP TABLE IF EXISTS `tloginlog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tloginlog` (
  `f1` varchar(16) DEFAULT NULL COMMENT 'username',
  `f2` varchar(16) DEFAULT NULL COMMENT 'login application, DSView, DS, or DSPlay',
  `f3` bigint(20) DEFAULT NULL COMMENT 'login timestamp'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tuser`
--

DROP TABLE IF EXISTS `tuser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tuser` (
  `f1` int(11) NOT NULL AUTO_INCREMENT COMMENT 'auto id',
  `f2` varchar(16) NOT NULL COMMENT 'username',
  `f3` varchar(16) NOT NULL COMMENT 'password',
  `f4` varchar(16) DEFAULT NULL COMMENT 'session',
  `f5` varchar(16) DEFAULT NULL COMMENT 'qq number',
  `f6` varchar(11) DEFAULT NULL COMMENT 'mobile',
  `f7` varchar(16) DEFAULT NULL COMMENT 'verify key, use in register',
  PRIMARY KEY (`f1`,`f2`),
  UNIQUE KEY `f2_UNIQUE` (`f2`),
  UNIQUE KEY `f6_UNIQUE` (`f6`),
  UNIQUE KEY `f5_UNIQUE` (`f5`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-10-02 15:47:51
