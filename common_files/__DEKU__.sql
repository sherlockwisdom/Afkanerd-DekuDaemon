-- phpMyAdmin SQL Dump
-- version 5.0.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Mar 25, 2020 at 08:30 AM
-- Server version: 10.4.11-MariaDB
-- PHP Version: 7.4.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `__DEKU__`
--
CREATE DATABASE IF NOT EXISTS `__DEKU__` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE `__DEKU__`;

-- --------------------------------------------------------

--
-- Table structure for table `MODEMS`
--

CREATE TABLE `MODEMS` (
  `IMEI` bigint(20) NOT NULL,
  `PHONENUMBER` int(20) DEFAULT NULL,
  `TYPE` enum('ssh','mmcli') COLLATE utf8mb4_unicode_ci NOT NULL,
  `STATE` enum('active','exhausted') COLLATE utf8mb4_unicode_ci NOT NULL,
  `POWER` enum('plugged','not_plugged') COLLATE utf8mb4_unicode_ci NOT NULL,
  `DATE` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `MODEM_WORK_LOAD`
--

CREATE TABLE `MODEM_WORK_LOAD` (
  `IMEI` bigint(20) NOT NULL,
  `WORK_LOAD` int(11) NOT NULL DEFAULT 0,
  `DATE` date NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `__CLIENTS__`
--

CREATE TABLE `__CLIENTS__` (
  `__ID__` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `__TOKEN__` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `__DATE__` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `__REQUEST__`
--

CREATE TABLE `__REQUEST__` (
  `__ID__` int(11) NOT NULL,
  `REQ_ID` bigint(255) NOT NULL,
  `__MESSAGE__` text COLLATE utf8mb4_unicode_ci NOT NULL,
  `__PHONENUMBER__` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `__DATE__` timestamp NOT NULL DEFAULT current_timestamp(),
  `__STATUS__` enum('sent','not_sent') COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT 'not_sent'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `MODEMS`
--
ALTER TABLE `MODEMS`
  ADD PRIMARY KEY (`IMEI`);

--
-- Indexes for table `__REQUEST__`
--
ALTER TABLE `__REQUEST__`
  ADD PRIMARY KEY (`__ID__`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `__REQUEST__`
--
ALTER TABLE `__REQUEST__`
  MODIFY `__ID__` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
