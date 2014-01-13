#!/bin/sh -x
. ~/.profile
v_date=`date +'%Y%m'`

v_last_month=`/okcode/shell/gettime ${v_date}010000 day 1`
v_month=`echo ${v_last_month}|cut -c1-6`

if [ -d /okdata1/etl/shetlgather/hwwg/${v_last_month} ]
then
   cd /okdata1/etl/shetlgather/hwwg/${v_last_month} 
   if [ -s "sh_bts_config±í.csv" ]
    then
       cat sh_bts_config±í.csv |awk 'BEGIN{FS=",";OFS=","}{if(length($14) >0 && length($19) >0) {print $5,$6,$13,$14,$19}}' >/okcode/calc_dis/src/sh_bts_config.del
   fi
   
   if [ -s "TD_BTS_ÅäÖÃ.csv" ]
     then
       cat TD_BTS_ÅäÖÃ.csv|awk 'BEGIN{FS=",";OFS=","}{print $8,$9,$21,$22,$25}' >/okcode/calc_dis/src/sh_td_config.del
   fi
fi

cd /okcode/calc_dis
./calc_distance /okcode/calc_dis/src/sh_bts_config.del  /okcode/calc_dis/src/sh_td_config.del /okcode/calc_dis/src/wlan.txt /okcode/calc_dis/out/gsm_td.del /okcode/calc_dis/out/gsm_wlan.del  /okcode/calc_dis/out/td_wlan.del

conn77
db2 "CREATE TABLE DTOP_D.DWD_GSM_TD_DISTANCE_${v_month}
 (
  GSM_LAC_CELL        VARCHAR(15),
  GSM_C_BTS_TYPE      VARCHAR(50),
  GSM_LONGITUDE       DECIMAL(20, 10),
  GSM_LATITUDE        DECIMAL(20, 10),
  TD_LAC_CELL         VARCHAR(500),
  TD_TYPE_BTS         VARCHAR(50),
  TD_LONGITUDE        DECIMAL(20, 10),
  TD_LATITUDE         DECIMAL(20, 10),
  DISTANCE            DECIMAL(30, 10)
 )
   IN TBS_DW_DT
 INDEX IN TBS_DW_DT
 partitioning key (GSM_LAC_CELL,TD_LAC_CELL)
 not logged initially"

db2 " CREATE TABLE DTOP_D.DWD_GSM_WLAN_DISTANCE_${v_month}
 (
  GSM_LAC_CELL          VARCHAR(15),
  GSM_C_BTS_TYPE        VARCHAR(50),
  GSM_LONGITUDE         DECIMAL(20, 10),
  GSM_LATITUDE          DECIMAL(20, 10),
  HOT_ID                VARCHAR(64),
  LONGITUDE             DECIMAL(20, 10),
  LATITUDE              DECIMAL(20, 10),
  DISTANCE              DECIMAL(30, 10)
 )
   IN TBS_DW_DT
 INDEX IN TBS_DW_DT
 partitioning key (GSM_LAC_CELL,HOT_ID)
 not logged initially"

db2 " CREATE TABLE DTOP_D.DWD_TD_WLAN_DISTANCE_${v_month}
 (
  TD_LAC_CELL         VARCHAR(15),
  TD_C_BTS_TYPE       VARCHAR(50),
  TD_LONGITUDE        DECIMAL(20, 10),
  TD_LATITUDE         DECIMAL(20, 10),
  HOT_ID              VARCHAR(64),
  LONGITUDE           DECIMAL(20, 10),
  LATITUDE            DECIMAL(20, 10),
  DISTANCE            DECIMAL(30, 10)
 )
    IN TBS_DW_DT
 INDEX IN TBS_DW_DT
 partitioning key (TD_LAC_CELL,HOT_ID)
 not logged initially"
db2 "load client from /okcode/calc_dis/out/gsm_td.del of del modified by codepage=819 insert into DTOP_D.DWD_GSM_TD_DISTANCE_${v_month} nonrecoverable"
db2 "load client from /okcode/calc_dis/out/gsm_wlan.del of del modified by codepage=819  insert into DTOP_D.DWD_GSM_WLAN_DISTANCE_${v_month} nonrecoverable"
db2 "load client from /okcode/calc_dis/out/td_wlan.del of del   modified by codepage=819 insert into DTOP_D.DWD_TD_WLAN_DISTANCE_${v_month} nonrecoverable"
db2 "runstats on table DTOP_D.DWD_GSM_TD_DISTANCE_${v_month}"
db2 "runstats on table DTOP_D.DWD_GSM_WLAN_DISTANCE_${v_month}"
db2 "runstats on table DTOP_D.DWD_TD_WLAN_DISTANCE_${v_month}"
