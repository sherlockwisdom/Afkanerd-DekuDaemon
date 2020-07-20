-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Jul 20, 2020 at 11:52 AM
-- Server version: 5.7.30-0ubuntu0.18.04.1
-- PHP Version: 7.2.24-0ubuntu0.18.04.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `__DEKU__`
--
CREATE DATABASE IF NOT EXISTS `__DEKU__` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `__DEKU__`;

-- --------------------------------------------------------

--
-- Table structure for table `MODEMS`
--

CREATE TABLE `MODEMS` (
  `IMEI` bigint(20) NOT NULL,
  `PHONENUMBER` int(20) DEFAULT NULL,
  `TYPE` enum('ssh','mmcli') NOT NULL,
  `STATE` enum('active','exhausted') NOT NULL,
  `POWER` enum('plugged','not_plugged') NOT NULL,
  `DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `MODEM_SMS_RECEIVED`
--

CREATE TABLE `MODEM_SMS_RECEIVED` (
  `IMEI` bigint(20) NOT NULL,
  `ISP` varchar(12) NOT NULL,
  `PHONENUMBER` varchar(24) NOT NULL,
  `MESSAGE` text NOT NULL,
  `DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `MODEM_SMS_RECEIVED`
--

INSERT INTO `MODEM_SMS_RECEIVED` (`IMEI`, `ISP`, `PHONENUMBER`, `MESSAGE`, `DATE`) VALUES
(358812037638331, 'MTN', '+237672872115', 'Message 1', '2020-06-28 18:03:16');

-- --------------------------------------------------------

--
-- Table structure for table `MODEM_WORK_LOAD`
--

CREATE TABLE `MODEM_WORK_LOAD` (
  `IMEI` bigint(20) NOT NULL,
  `WORK_LOAD` int(11) NOT NULL,
  `DATE` date DEFAULT NULL,
  `MDATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `MODEMS`
--
ALTER TABLE `MODEMS`
  ADD PRIMARY KEY (`IMEI`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
