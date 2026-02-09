/* ==========================================
   Lite Monitor 数据库初始化脚本
   分区命名：
     日分区：p_YYYYMMDD
     月分区：p_YYYY_MM
========================================== */

SET NAMES utf8mb4;
SET time_zone = '+08:00';

CREATE DATABASE IF NOT EXISTS lite_monitor
    DEFAULT CHARACTER SET utf8mb4
    DEFAULT COLLATE utf8mb4_general_ci;

USE lite_monitor;
/* ===========================
   hosts 表
=========================== */

CREATE TABLE IF NOT EXISTS hosts
(
    host_id     BIGINT PRIMARY KEY,
    host_name   VARCHAR(128),
    ip          VARCHAR(64),
    status      TINYINT,
    create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4;


/* ===========================
   items 表
=========================== */

CREATE TABLE IF NOT EXISTS items
(
    item_id      BIGINT PRIMARY KEY NOT NULL,
    host_id      BIGINT             NOT NULL,
    item_key     VARCHAR(128)       NOT NULL,
    value_type   TINYINT COMMENT '0=float 1=uint 2=string',
    units        VARCHAR(32),
    interval_sec INT,
    status       TINYINT,
    create_time  TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_host_id (host_id),
    UNIQUE KEY uk_host_item (host_id, item_key)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4;


/* ===========================
   history_uint（日分区）
=========================== */

CREATE TABLE IF NOT EXISTS history_uint
(
    itemid BIGINT(20) UNSIGNED NOT NULL,
    clock  INT(11)             NOT NULL DEFAULT 0,
    value  BIGINT(20) UNSIGNED NOT NULL DEFAULT 0,
    ns     INT(11)             NOT NULL DEFAULT 0,
    PRIMARY KEY (itemid, clock, ns)
) ENGINE = InnoDB
    PARTITION BY RANGE (clock) (
        PARTITION p_init values LESS THAN (0)
        );


/* ===========================
   history_float（日分区）
=========================== */

CREATE TABLE IF NOT EXISTS history_float
(
    itemid BIGINT(20) UNSIGNED NOT NULL,
    clock  INT(11)             NOT NULL DEFAULT 0,
    value  DOUBLE              NOT NULL DEFAULT 0.0000,
    ns     INT(11)             NOT NULL DEFAULT 0,
    PRIMARY KEY (itemid, clock, ns)
) ENGINE = InnoDB
    PARTITION BY RANGE (clock) (
        PARTITION p_init values LESS THAN (0)
        );


/* ===========================
   trends_uint（月分区）
=========================== */

CREATE TABLE IF NOT EXISTS trends_uint
(
    itemid    BIGINT(20) UNSIGNED NOT NULL,
    clock     INT(11)             NOT NULL DEFAULT 0 COMMENT '小时起始时间戳',
    num       INT(11)             NOT NULL DEFAULT 0 COMMENT '样本数量',
    value_min BIGINT(20) UNSIGNED NOT NULL DEFAULT 0,
    value_avg BIGINT(20) UNSIGNED NOT NULL DEFAULT 0,
    value_max BIGINT(20) UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (itemid, clock)
) ENGINE = InnoDB
    PARTITION BY RANGE (clock) (
        PARTITION p_init values LESS THAN (0)
        );


/* ===========================
   trends_float（月分区）
=========================== */

CREATE TABLE IF NOT EXISTS trends_float
(
    itemid    BIGINT(20) UNSIGNED NOT NULL,
    clock     INT(11)             NOT NULL DEFAULT 0 COMMENT '小时起始时间戳',
    num       INT(11)             NOT NULL DEFAULT 0 COMMENT '样本数量',
    value_min DOUBLE              NOT NULL DEFAULT 0.0000,
    value_avg DOUBLE              NOT NULL DEFAULT 0.0000,
    value_max DOUBLE              NOT NULL DEFAULT 0.0000,
    PRIMARY KEY (itemid, clock)
) ENGINE = InnoDB
    PARTITION BY RANGE (clock) (
        PARTITION p_init values LESS THAN (0)
        );

