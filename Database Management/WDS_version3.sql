-- MySQL Script generated by MySQL Workbench
-- Fri May  1 15:55:16 2020
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
-- -----------------------------------------------------
-- Schema WDS
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema WDS
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `WDS` DEFAULT CHARACTER SET utf8 ;
USE `WDS` ;

-- -----------------------------------------------------
-- Table `WDS`.`user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `WDS`.`user` ;

CREATE TABLE IF NOT EXISTS `WDS`.`user` (
  `user_id` BIGINT NOT NULL AUTO_INCREMENT,
  `user_name` VARCHAR(45) NOT NULL,
  `user_username` VARCHAR(45) NOT NULL,
  `user_password` VARCHAR(256) NOT NULL,
  PRIMARY KEY (`user_id`))
ENGINE = InnoDB
AUTO_INCREMENT = 5
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `WDS`.`customer`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `WDS`.`customer` ;

CREATE TABLE IF NOT EXISTS `WDS`.`customer` (
  `cid` BIGINT NOT NULL AUTO_INCREMENT,
  `cfirstname` VARCHAR(30) NOT NULL,
  `clastname` VARCHAR(30) NOT NULL,
  `cgender` CHAR(1) NULL DEFAULT NULL,
  `cmaritality` CHAR(1) NOT NULL,
  `cinstype` CHAR(1) NOT NULL,
  `chouse` INT NOT NULL,
  `cstreet` VARCHAR(30) NOT NULL,
  `ccity` VARCHAR(30) NOT NULL,
  `cstate` VARCHAR(2) NOT NULL,
  `czipcode` INT NOT NULL,
  `user_id` BIGINT NOT NULL,
  PRIMARY KEY (`cid`),
  INDEX `user_id_idx` (`user_id` ASC) VISIBLE,
  CONSTRAINT `fk_user_customer`
    FOREIGN KEY (`user_id`)
    REFERENCES `WDS`.`user` (`user_id`))
ENGINE = InnoDB
AUTO_INCREMENT = 10
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `WDS`.`insurance`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `WDS`.`insurance` ;

CREATE TABLE IF NOT EXISTS `WDS`.`insurance` (
  `insid` BIGINT NOT NULL AUTO_INCREMENT,
  `insstartdate` DATETIME NOT NULL,
  `insenddate` DATETIME NOT NULL,
  `inspremium` DECIMAL(10,2) NOT NULL,
  `cid` BIGINT NOT NULL,
  PRIMARY KEY (`insid`),
  INDEX `cid_idx` (`cid` ASC) VISIBLE,
  CONSTRAINT `fk_customer_ins`
    FOREIGN KEY (`cid`)
    REFERENCES `WDS`.`customer` (`cid`))
ENGINE = InnoDB
AUTO_INCREMENT = 29
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `WDS`.`auto_ins`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `WDS`.`auto_ins` ;

CREATE TABLE IF NOT EXISTS `WDS`.`auto_ins` (
  `insid` BIGINT NOT NULL,
  `vin` VARCHAR(20) NOT NULL,
  `vmake` VARCHAR(30) NOT NULL,
  `vmodel` VARCHAR(30) NULL DEFAULT NULL,
  `vyear` INT NOT NULL,
  `vstatus` CHAR(1) NOT NULL,
  `licenseno` VARCHAR(10) NOT NULL,
  `driverfirstname` VARCHAR(30) NOT NULL,
  `driverlastname` VARCHAR(30) NOT NULL,
  `driverbirthdate` DATETIME NOT NULL,
  PRIMARY KEY (`insid`),
  CONSTRAINT `fk_ins_auto`
    FOREIGN KEY (`insid`)
    REFERENCES `WDS`.`insurance` (`insid`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `WDS`.`home_ins`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `WDS`.`home_ins` ;

CREATE TABLE IF NOT EXISTS `WDS`.`home_ins` (
  `insid` BIGINT NOT NULL,
  `hpurchasedate` DATETIME NOT NULL,
  `hvalue` DECIMAL(10,2) NOT NULL,
  `harea` DECIMAL(8,2) NOT NULL,
  `htype` CHAR(1) NOT NULL,
  `hfire` CHAR(1) NOT NULL,
  `hsecurity` CHAR(1) NOT NULL,
  `hpool` CHAR(1) NOT NULL,
  `hbasement` CHAR(1) NOT NULL,
  PRIMARY KEY (`insid`),
  CONSTRAINT `fk_ins_home`
    FOREIGN KEY (`insid`)
    REFERENCES `WDS`.`insurance` (`insid`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

USE `WDS` ;

-- -----------------------------------------------------
-- procedure sp_createUser
-- -----------------------------------------------------

USE `WDS`;
DROP procedure IF EXISTS `WDS`.`sp_createUser`;

DELIMITER $$
USE `WDS`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_createUser`(
    IN p_name VARCHAR(20),
    IN p_username VARCHAR(20),
    IN p_password VARCHAR(256)
)
BEGIN
    if ( select exists (select 1 from WDS.user where user_username = p_username) ) THEN
     
        select 'Username Exists !!';
     
    ELSE
     
        insert into WDS.user
        (
            user_name,
            user_username,
            user_password
        )
        values
        (
            p_name,
            p_username,
            p_password
        );
     
    END IF;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure sp_insertCarIns
-- -----------------------------------------------------

USE `WDS`;
DROP procedure IF EXISTS `WDS`.`sp_insertCarIns`;

DELIMITER $$
USE `WDS`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insertCarIns`(in p_insstartdate varchar(20), in p_insenddate varchar(20), 
						in p_inspremium decimal(10,2), in p_vin varchar(20), in p_vmake varchar(30), in p_vmodel varchar(30), 
                        in p_vyear int, in p_vstatus char(1), in p_licenseno varchar(10), in p_driverfirstname varchar(30),
                        in p_driverlastname varchar(30), in p_driverbirthdate varchar(20), in p_user_name varchar(45))
BEGIN
	set @temp_cid := (select c.cid from WDS.user u join WDS.customer c on u.user_id=c.user_id 
					where u.user_username = p_user_name);
	insert into insurance(insstartdate, insenddate, inspremium, cid) 
		values (p_insstartdate, p_insenddate, p_inspremium, @temp_cid);
        
    set @temp_ins_id := (select m.insid from (select i.insid from insurance i where i.insid not in (select insid from auto_ins)) m where m.insid not in (select insid from home_ins));
	insert into auto_ins(insid, vin, vmake, vmodel, vyear, vstatus, licenseno, driverfirstname, driverlastname, driverbirthdate) 
		values (@temp_ins_id, p_vin, p_vmake, p_vmodel, p_vyear, p_vstatus, p_licenseno, p_driverfirstname, p_driverlastname, 
        p_driverbirthdate);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure sp_insertCustomerInfo
-- -----------------------------------------------------

USE `WDS`;
DROP procedure IF EXISTS `WDS`.`sp_insertCustomerInfo`;

DELIMITER $$
USE `WDS`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insertCustomerInfo`(in p_cfirstname varchar(30), in p_clastname varchar(30), 
						in p_cgender char(1), in p_cmaritality char(1), in p_cinstype char(1), 
                        in p_chouse int, in p_cstreet varchar(30), in p_ccity varchar(30), 
                        in p_cstate varchar(2), in p_czipcode int, in p_user_name varchar(45))
BEGIN
    if ( select exists 
		(select c.user_id from WDS.user u join WDS.customer c on u.user_id=c.user_id 
        where u.user_username = p_user_name) ) then 
        set @temp_id := (select c.user_id from WDS.user u join WDS.customer c on u.user_id=c.user_id 
						where u.user_username = p_user_name);
        update customer set cfirstname=p_cfirstname,clastname=p_clastname,cgender=p_cgender,cmaritality=p_cmaritality,
							cinstype=p_cinstype,chouse=p_chouse,cstreet=p_cstreet,ccity=p_ccity,cstate=p_cstate,
                            czipcode=p_czipcode where user_id=@temp_id;
     
    ELSE
		set @temp_id := (select user_id from WDS.user where user_username=p_user_name);
        insert into customer(cfirstname, clastname, cgender, cmaritality, cinstype, chouse, cstreet, ccity,
						cstate, czipcode, user_id) 
		values (p_cfirstname, p_clastname, p_cgender, p_cmaritality, p_cinstype,
                        p_chouse, p_cstreet, p_ccity, p_cstate, p_czipcode, @temp_id);
     
    END IF;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure sp_insertHomeIns
-- -----------------------------------------------------

USE `WDS`;
DROP procedure IF EXISTS `WDS`.`sp_insertHomeIns`;

DELIMITER $$
USE `WDS`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insertHomeIns`(in p_insstartdate varchar(20), in p_insenddate varchar(20), 
						in p_inspremium decimal(10,2), in p_hpurchasedate varchar(20), in p_hvalue decimal(10,2), 
                        in p_harea decimal(8,2), in p_htype char(1), in p_hfire char(1), in p_hsecurity char(1), 
                        in p_hpool char(1), in p_hbasement char(1), in p_user_name varchar(45))
BEGIN
	set @temp_cid := (select c.cid from WDS.user u join WDS.customer c on u.user_id=c.user_id 
					where u.user_username = p_user_name);
	insert into insurance(insstartdate, insenddate, inspremium, cid) 
		values (p_insstartdate, p_insenddate, p_inspremium, @temp_cid);
	    
    set @temp_ins_id := (select m.insid from (select i.insid from insurance i where i.insid not in (select insid from auto_ins)) m where m.insid not in (select insid from home_ins));
    insert into home_ins(insid, hpurchasedate, hvalue, harea, htype, hfire, hsecurity, hpool, hbasement) 
		values (@temp_ins_id, p_hpurchasedate, p_hvalue, p_harea, p_htype, p_hfire, p_hsecurity, 
        p_hpool, p_hbasement);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure sp_validatelogin
-- -----------------------------------------------------

USE `WDS`;
DROP procedure IF EXISTS `WDS`.`sp_validatelogin`;

DELIMITER $$
USE `WDS`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_validatelogin`(
 IN p_username VARCHAR(20)
    )
BEGIN
 SELECT * FROM WDS.user WHERE user_username=p_username;
END$$

DELIMITER ;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
