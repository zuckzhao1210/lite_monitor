DELIMITER $$

DROP PROCEDURE IF EXISTS maintain_partitions$$
CREATE PROCEDURE maintain_partitions()
BEGIN
    /* ==========
       公共变量
    ========== */
    DECLARE v_today CHAR(8);
    DECLARE v_today_ts INT;
    DECLARE v_tomorrow_ts INT;

    DECLARE v_month CHAR(7);
    DECLARE v_next_month_ts INT;

    DECLARE v_cnt INT;
    DECLARE v_sql TEXT;

    /* ==========
       日期计算
    ========== */
    SET v_today = DATE_FORMAT(CURDATE(), '%Y%m%d');
    SET v_today_ts = UNIX_TIMESTAMP(CURDATE());
    SET v_tomorrow_ts = UNIX_TIMESTAMP(DATE_ADD(CURDATE(), INTERVAL 1 DAY));

    SET v_month = DATE_FORMAT(CURDATE(), '%Y_%m');
    SET v_next_month_ts = UNIX_TIMESTAMP(
            DATE_FORMAT(DATE_ADD(CURDATE(), INTERVAL 1 MONTH), '%Y-%m-01')
                          );

    /* =====================================================
       1. history_uint：今天的日分区
    ===================================================== */
    SELECT COUNT(*)
    INTO v_cnt
    FROM information_schema.PARTITIONS
    WHERE TABLE_SCHEMA = 'lite_monitor'
      AND TABLE_NAME = 'history_uint'
      AND PARTITION_NAME = CONCAT('p_', v_today);

    IF v_cnt = 0 THEN
        SET @v_sql = CONCAT(
                'ALTER TABLE history_uint ADD PARTITION (',
                'PARTITION p_', v_today,
                ' VALUES LESS THAN (', v_tomorrow_ts, ')',
                ')'
                     );
        PREPARE stmt FROM @v_sql;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END IF;

    /* =====================================================
       2. history_float：今天的日分区
    ===================================================== */
    SELECT COUNT(*)
    INTO v_cnt
    FROM information_schema.PARTITIONS
    WHERE TABLE_SCHEMA = 'lite_monitor'
      AND TABLE_NAME = 'history_float'
      AND PARTITION_NAME = CONCAT('p_', v_today);

    IF v_cnt = 0 THEN
        SET @v_sql = CONCAT(
                'ALTER TABLE history_float ADD PARTITION (',
                'PARTITION p_', v_today,
                ' VALUES LESS THAN (', v_tomorrow_ts, ')',
                ')'
                     );
        PREPARE stmt FROM @v_sql;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END IF;

    /* =====================================================
       3. trends_uint：本月的月分区
    ===================================================== */
    SELECT COUNT(*)
    INTO v_cnt
    FROM information_schema.PARTITIONS
    WHERE TABLE_SCHEMA = 'lite_monitor'
      AND TABLE_NAME = 'trends_uint'
      AND PARTITION_NAME = CONCAT('p_', v_month);

    IF v_cnt = 0 THEN
        SET @v_sql = CONCAT(
                'ALTER TABLE trends_uint ADD PARTITION (',
                'PARTITION p_', v_month,
                ' VALUES LESS THAN (', v_next_month_ts, ')',
                ')'
                     );
        PREPARE stmt FROM @v_sql;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END IF;

    /* =====================================================
       4. trends_float：本月的月分区
    ===================================================== */
    SELECT COUNT(*)
    INTO v_cnt
    FROM information_schema.PARTITIONS
    WHERE TABLE_SCHEMA = 'lite_monitor'
      AND TABLE_NAME = 'trends_float'
      AND PARTITION_NAME = CONCAT('p_', v_month);

    IF v_cnt = 0 THEN
        SET @v_sql = CONCAT(
                'ALTER TABLE trends_float ADD PARTITION (',
                'PARTITION p_', v_month,
                ' VALUES LESS THAN (', v_next_month_ts, ')',
                ')'
                     );
        PREPARE stmt FROM @v_sql;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END IF;

END $$

DELIMITER ;
