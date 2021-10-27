/***********************************************************
** Copyright (C), 2008-2019, OPPO Mobile Comm Corp., Ltd.
** VENDOR_EDIT
** File: - oppo_smart_bw_decision.h
** Description: Source file for Smart Band-Width Decision feature.
**           It stands for choose STA P2P 2.4G HT40 based on environment or other scenarios.
** Version: 2.0
** Date : 2019/10/1
** Author: Fenghua.Xu@PSW.TECH.WiFi.Connect
**
** ------------------------------- Revision History: -------------------------------
**  	<author>		<data> 	   <version >	       <desc>
**  Xufenghua       2019/7/31     1.0     1st versoin on P70 18531 Gen3 main branch: step 1.5 & not euclid
**  Xufenghua       2019/10/31    2.0     2nd versoin on 5G-L Gen4m main branch: euclid
****************************************************************/
#ifdef OPLUS_FEATURE_WIFI_SMART_BW
#ifndef _OPPO_SMART_BW_DECISION_H
#define _OPPO_SMART_BW_DECISION_H
#include <linux/oplus_kevent.h>

//@2019/10/28: Gen4m missing too many define, define here to let core code needn't modify
typedef struct BSS_DESC BSS_DESC_T, *P_BSS_DESC_T;
typedef struct ADAPTER ADAPTER_T, *P_ADAPTER_T;
typedef enum ENUM_BAND ENUM_BAND_T, *P_ENUM_BAND_T;
typedef enum ENUM_CHANNEL_WIDTH ENUM_CHANNEL_WIDTH_T, *P_ENUM_CHANNEL_WIDTH_P;
typedef enum ENUM_CHNL_EXT ENUM_CHNL_EXT_T, *P_ENUM_CHNL_EXT_T;
typedef unsigned char BOOL, *PBOOL, BOOLEAN, *PBOOLEAN;
#ifndef _OSAL_TYPEDEF_H_
typedef void VOID, *PVOID, **PPVOID;
#endif
typedef struct LINK LINK_T, *P_LINK_T;
typedef struct RF_CHANNEL_INFO RF_CHANNEL_INFO_T, *P_RF_CHANNEL_INFO_T;
typedef unsigned int WLAN_STATUS, *P_WLAN_STATUS;

/* for air busy status during scan done */
/* WM_BUSY_STATUS_1CH_T: one channel busy status
 * WM_BUSY_STATUS_1Time_T: one scan busy status
 * WM_BUSY_STATUS_T: store latest 3 time's WM scan result to let it more smooth
 */
typedef struct _WM_BUSY_STATUS_1CH_T {
        unsigned char  Ch;
        unsigned char  cHIdleSlotRatio; /* store numerator: store value not value% */
        unsigned int u4ScanResult2GScanDuration; /* FW already update ms, not only slot number, need caculate number * 9 us */
        unsigned int u4ScanResult2GMdrdyCount;
        unsigned int u4ScanResult2GIdleSlotTime; /* FW already update ms, not only slot number */
} WM_BUSY_STATUS_1CH_T, *P_WM_BUSY_STATUS_1CH_T;
typedef struct _WM_BUSY_STATUS_1Time_T {
        OS_SYSTIME u4SysScanTime;
        unsigned char u4ScanResult2GTotalCh;
        unsigned int u4ScanResult2GTotalScanDuration;
        unsigned int u4ScanResult2GTotalMdrdyCount;
        unsigned int u4ScanResult2GTotalIdleSlotTime; /* ms */
        WM_BUSY_STATUS_1CH_T arSR2G1CHWMBS[14];
} WM_BUSY_STATUS_1Time_T, *P_WM_BUSY_STATUS_1Time_T;
typedef struct _WM_BUSY_STATUS_T {
        /* need record this number since it will round trip, index is from 0 */
        unsigned char u4OldestIndex;  /* the 1st record index */
        unsigned char u4LatestIndex;  /* the latest record index */
        unsigned char u4StoreNum; /* the total store number, should be 3 in most cases */
        WM_BUSY_STATUS_1Time_T arSR2G1TimeWMBS[3]; /* store latest 3 time's WM scan result */
} WM_BUSY_STATUS_T, *P_WM_BUSY_STATUS_T;

/* smart_bw_params, driver will hold default value and replace by RUS update */
/* we use hardcode in the 1st version, now use RUS, keep this code for comment only */
#if 0
#ifndef SMART_BW_FEATURE
#define SMART_BW_FEATURE TRUE
#define SMART_BW_DBG TRUE /* better set FALSE in release version */
#define LQ_CHECK_NUM 5 /* link quality total store number for smooth */
#define LQ_MIN_ACTIVE_NEED_NUM 3 /* link quality info maybe phase out,  this is min active number needed*/
#define LQ_CHECK_EXPIRE 5 /* >= LQ_CHECK_EXPIRE's link quality will be ignored */
        /* TODO: experience number may need fine tune: auth/assoc/4way/DHDP/HTTP etc normally should >20 in 5s */
        /* < this number after LQ_CHECK_EXPIRE will set AP 40M IOT issue*/
#define LQ_AP_40M_IOT_L2_REACH_PACKET_COUNT 20
#define LQ_AP_40M_IOT_L3_REACH_PACKET_COUNT 12 /* EAPOL DHCP ARP HTTP etc should > it in 5s */
#define LQ_40M_BAD_RATE 135 /* this is the real MCS rate *10, 1X1 40M 800ns MCS0: 13.5 */
#define LQ_PER_HIGH 35 /* 35 stands for 35%, MTK FW 11N MCS0 PER is 28% */
#define LQ_IDLE_SLOT_RATIO_ACCEPT 15 /* 15%, experience from connect based on scan WM busy status */
#define LQ_L3_SPEED_LOW  120 /* stands for 120KB, near 1Mbps, 1500bytes per packet,  about 82 packets/s */

/* kernel offical timeout is 30s, OPPO FWK period scan is 15s, set to multiple */
#define SCAN_RESULT_EXPIRE 60
#define SAVE_CONNECT_LOG_THRESHOLD 30
#define CLEAN_AP_NUMBER 7 /* active AP number < CLEANAPNUMBER stands for environment clean */
#define GOOD_RSSI (-75)
#define CLEAN_CH_IDLE_SLOT_RATIO 10 /* > CLEAN_CH_IDLE_SLOT_RATIO stands for the CH is busy */
#define CLEAN_BAND_IDLE_SLOT_RATIO 15  /* > CLEAN_BAND_IDLE_SLOT_RATIO stands for the band(related 8 CH) is busy */
#endif
#endif
/* data come from FWK rom update => kernel oppo sla => WIFI driver, they must have the same data member order */
typedef struct _smart_bw_params{
        int SELECT_BW_WHEN_CONNECT_FEATRUE_ENABLE; //step1 config, 1
        int DYNAMIC_SW_BW_FEATRUE_ENABLE; //step2 config, 0
        int SMART_BW_DBG; // debug 1, release 1
        //step 1 related threshold
        int SCAN_RESULT_EXPIRE; // 60
        int SAVE_CONNECT_LOG_THRESHOLD; // 30
        int CLEAN_AP_NUMBER; //25
        int GOOD_RSSI;//-75
        int CLEAN_CH_IDLE_SLOT_RATIO;//10
        int CLEAN_BAND_IDLE_SLOT_RATIO;//15
        //step 2 related threshold
        int LQ_CHECK_NUM;//5
        int LQ_MIN_ACTIVE_NEED_NUM;//3
        int LQ_CHECK_EXPIRE;//5
        int LQ_AP_40M_IOT_L2_REACH_PACKET_COUNT;//20
        int LQ_AP_40M_IOT_L3_REACH_PACKET_COUNT;//12=>6
        int LQ_40M_BAD_RATE;//13.5X10
        int LQ_PER_HIGH;//35=>20
        int LQ_IDLE_SLOT_RATIO_ACCEPT;//15
        int LQ_L3_SPEED_LOW;//120KB, 1Mbps~=>200KB
}SMART_BW_PARAMS_T, *P_SMART_BW_PARAMS_T;

typedef enum _ENUM_SMART_BW_SWITCH_STATE_T {
	SMART_BW_SWITCH_IDLE = 0,	        /* Idle state, ex: not trigger or already stopped */
	SMART_BW_SWITCH_ONGOING,		/* Switching. */
} ENUM_SMART_BW_SWITCH_STATE_T, *P_ENUM_SMART_BW_SWITCH_STATE_T;

enum WCN_KEY_LOG_MODE{
        TYPE_LOG_SCAN_BAND_RATIO, /* scan band distribution & ratio */
        TYPE_LOG_SCAN_RSSI_RATIO,  /* scan rssi distribution & ratio */
        TYPE_LOG_CONNECT_BAND_RATIO,  /* connect band distribution & ratio */
        TYPE_LOG_MAX_NUM
};

typedef struct _smart_bw{
        /* Variable Group 1 */
        /* parameter RUS related */
        bool getSmartBWRomUpdate; //false
        SMART_BW_PARAMS_T smart_bw_params;

        /* Variable Group 2 */
        /* Summary 2G 5G BSS number, show all buffered BSS & now active BSS */
        /* every time's scan result is not accurate */
        unsigned int u4ScanResultTotalNum;
        unsigned int u4ScanResult2GNum;
        unsigned int u4ScanResult5GNum;
        unsigned int u4ScanResult2G40MNum;
        unsigned int u4ScanResult5G160MNum;
        /* active BSS get from buffered BSS via time */
        unsigned int u4ScanResultActiveTotalNum;
        unsigned int u4ScanResultActive2GNum;
        unsigned int u4ScanResultActive5GNum;
        unsigned int u4ScanResultActive2G40MNum;
        unsigned int u4ScanResultActive5G160MNum;

        /* Variable Group 3 */
        /* for air busy status during scan done */
        WM_BUSY_STATUS_T arScanResult2GWMBusyStatus;

        /* Variable Group 4 */
        /* connect and scan, AP IOT, LQ monitor statistics log purpose */
        /* this value will be set to 0 in driver init state */
        unsigned int u4ConnectTotalTime;
        unsigned int u4ConnectTotal2GTime;
        unsigned int u4ConnectTotal2GHT40Time;
        unsigned int u4ConnectTotal5GVHT160Time;
        OS_SYSTIME rRecordCnnSysTime; /* the log recorded connect sys time, won't record log for each connect */
        unsigned char needSaveLog; /* control all DCS logs including: scan/connect/AP IOT/LQ monitor etc...*/

        /* Variable Group 5 */
        /* Smooth link quality monitor related, default link quality only keep 1 time, use 5 times for smooth use purpose */
        OS_SYSTIME rCnnSysTime; /* the connect sys time, AP IOT monitor will start after + 5s */
        BOOLEAN fgIsNeedMonitorLink; /* for 20M 40M band switch after link established */
        BOOLEAN fgIsNeedMonitorAPIOT; /* monitor AP IOT issue just after link establish */
        /* need record this number since it will round trip, index is from 0 */
        unsigned char u4LQOldestIndex;  /* the 1st record index */
        unsigned char u4LQLatestIndex;  /* the latest record index */
        unsigned char u4LQStoreNum; /* the total store number */
        struct WIFI_LINK_QUALITY_INFO rLinkQualityInfoSmooth[5]; /* can't update via RUS, hard code to 5 */

        /* Variable Group 6 */
        /* band-width switch related */
        int i24GBWManualSet; /* manual set 2.4G BW 0: code original behavior, 1: force HT20, 2: force HT40, 1001: very special usage */
        /* i24GBWManualSet none 0 is similar with SMART_BW_SWITCH_ONGOING in some states */
        /* but i24GBWManualSet will reset after BW select */
        /* eSmartBWSwitchState will set to ongoing when trigger switch and reset to idle after trigger done */
        ENUM_SMART_BW_SWITCH_STATE_T eSmartBWSwitchState;
        unsigned int u4SmartBWSwitchStart;
        unsigned int u4SmartBWSwitchEnd;
        unsigned char ucSelectBW; /* 0: 20M, 1: 40M, 2: 80M */
}SMART_BW_T, *P_SMART_BW_T;
#define WLAN0_BSS_INDEX 0 //wlan0 is always bss index 0 now

extern bool get_L3_LQ_status(int payload[], int len);
void smartBWGenericInit(P_ADAPTER_T prAdapter);
void smartBWGenericUnInit(void);
BOOLEAN isBSS24GHT40(ENUM_BAND_T eBand, ENUM_CHANNEL_WIDTH_T eChannelWidth, ENUM_CHNL_EXT_T eSco);
BOOLEAN isBSS5GVHT160(ENUM_BAND_T eBand, ENUM_CHANNEL_WIDTH_T eChannelWidth);
void wlanSetSmartBWParams(void);
BOOLEAN preCheckBeforeConnect(uint8_t ucBssIndex);
VOID connectLogActiveBSSRSSI(IN P_BSS_DESC_T prSPBssDesc);
VOID connectLogSummary(IN P_BSS_DESC_T prBssDesc, uint8_t ucBssIndex);
VOID scanLogAllBufferedScanResult(void);
VOID printScanResultDetail(unsigned char *aucSSID, unsigned char ucSSIDLen,
        unsigned char ucChannelNum, ENUM_BAND_T eBand,
        ENUM_CHANNEL_WIDTH_T eChannelWidth, ENUM_CHNL_EXT_T eSco,
        PARAM_RSSI rRssi, OS_SYSTIME rTimeSec, unsigned char* aucBSSID);
VOID printWMBusyStatus(void);
BOOLEAN checkEnvironmentClean(IN P_BSS_DESC_T prSPBssDesc);
BOOLEAN isCTEnvironment(unsigned char* aucSSID, unsigned char ucSSIDLen);
BOOLEAN extraCheck2GSupportBW(IN P_BSS_DESC_T prSPBssDesc);
void wlanResetLinkQualityMonitorStatus(void);
VOID printLQSmoothStatus(void);
void wlanSaveLinkQualitySmooth(struct WIFI_LINK_QUALITY_INFO *prLinkQualityInfo);
BOOLEAN wlanGetActiveAverageLQStatus(
        unsigned int* u4GetAVGCurTxRate,
        unsigned long long * u8GetRetAVGTxTotalCount,
        unsigned long long * u8GetAVGTxRetryCount,
        unsigned long long * u8GetAVGTxFailCount,
        unsigned int* u4GetAVGCurTxPer,
        unsigned int* u4GetAVGCurRxRate,
        unsigned int* u4GetAVGRxDupCount,
        unsigned long long * u8GetAVGRxTotalCount,
        unsigned char* u8GetAVGIdleSlotRatio,
        int* iGetAVGL3CurSpped,
        unsigned char* u1GetActiveTotalNum);
void wlanMonitorAPIOTIssue(void);
void wlanMonitorLQStatus(void);
int trigger2GBWSwitch(int dstBW);
void handleSmartBWSwitchResult(WLAN_STATUS eStatus);
signed int htKalWCNKeyLogWrite(IN const char *log, IN enum WCN_KEY_LOG_MODE mode);
BOOLEAN tuneSTABW(struct BSS_INFO* prBssInfo, struct BSS_DESC* prBssDesc, u8* pUcAPBandwidth, uint8_t ucBssIndex);
void saveScanWMBusyStatus(struct EVENT_SCAN_DONE* prScanDone);
#endif /* _OPPO_SMART_BW_DECISION_H */
#endif /* OPLUS_FEATURE_WIFI_SMART_BW*/
