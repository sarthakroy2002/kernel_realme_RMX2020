/***********************************************************
** Copyright (C), 2008-2019, OPPO Mobile Comm Corp., Ltd.
** VENDOR_EDIT
** File: - oppo_smart_bw_decision.c
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
#include "gl_os.h"
#include "gl_kal.h"
#include "gl_wext.h"
#include "precomp.h"

extern bool get_smart_bw_rom_update(int payload[], int len);
static P_SMART_BW_T prSmartBW = NULL;
static struct ADAPTER* prAdapter = NULL;

/* Init some global variables, let code more easy porting to different platform later */
void smartBWGenericInit(P_ADAPTER_T prAdapterIn)
{
        prAdapter = prAdapterIn;
        prSmartBW = &(prAdapter->rSmartBW);
        wlanSetSmartBWParams();

        return;
}

void smartBWGenericUnInit(void)
{
        prAdapter = NULL;
        prSmartBW = NULL;

        return;
}


BOOLEAN isBSS24GHT40(ENUM_BAND_T eBand, ENUM_CHANNEL_WIDTH_T eChannelWidth, ENUM_CHNL_EXT_T eSco)
{
        BOOLEAN ret = FALSE;

        if ((eBand == BAND_2G4) && (eChannelWidth == CW_20_40MHZ) && ((eSco == CHNL_EXT_SCA) || (eSco == CHNL_EXT_SCB))) {
                ret = TRUE;
        }

        return ret;
}

BOOLEAN isBSS5GVHT160(ENUM_BAND_T eBand, ENUM_CHANNEL_WIDTH_T eChannelWidth)
{
        BOOLEAN ret = FALSE;

        if ((eBand == BAND_5G) && ((eChannelWidth == CW_160MHZ) || (eChannelWidth == CW_80P80MHZ))) {
                ret = TRUE;
        }

        return ret;
}

void wlanSetSmartBWParams(void)
{
        SMART_BW_PARAMS_T *p_params;

        /* xufenghua modify for bug:2267281 2019/08/22, sometimes wlan may probe fail, cause prAdapter null */
        if (!prAdapter) {
                return;
        }
        //else
        p_params = &(prSmartBW->smart_bw_params);

        /* rWlanInfo will reset when wlan power on or power off, need set defaule value and get RUS */
        prSmartBW->getSmartBWRomUpdate = false;
        //2019/11/19, modify from real parameter to dummy parameter for easy release out later
        p_params->SELECT_BW_WHEN_CONNECT_FEATRUE_ENABLE = 0;
        p_params->DYNAMIC_SW_BW_FEATRUE_ENABLE = 0;
        p_params->SMART_BW_DBG = 1;
        //step 1 related threshold
        p_params->SCAN_RESULT_EXPIRE = 0;
        p_params->SAVE_CONNECT_LOG_THRESHOLD = 0;
        p_params->CLEAN_AP_NUMBER = 0;
        p_params->GOOD_RSSI = 0;
        p_params->CLEAN_CH_IDLE_SLOT_RATIO = 0;
        p_params->CLEAN_BAND_IDLE_SLOT_RATIO = 0;
        //step 2 related threshold
        p_params->LQ_CHECK_NUM = 0;
        p_params->LQ_MIN_ACTIVE_NEED_NUM = 0;
        p_params->LQ_CHECK_EXPIRE = 0;
        p_params->LQ_AP_40M_IOT_L2_REACH_PACKET_COUNT = 0;
        p_params->LQ_AP_40M_IOT_L3_REACH_PACKET_COUNT = 0;
        p_params->LQ_40M_BAD_RATE = 0;
        p_params->LQ_PER_HIGH = 0;
        p_params->LQ_IDLE_SLOT_RATIO_ACCEPT = 0;
        p_params->LQ_L3_SPEED_LOW = 0;

        if(get_smart_bw_rom_update((int*)p_params, sizeof(SMART_BW_PARAMS_T))) {
                prSmartBW->getSmartBWRomUpdate = true;
        }
        if(p_params->SMART_BW_DBG) {
                DBGLOG(INIT, TRACE, "wlanSetSmartBWParams:set params romupdate = %d, params[0] = %d,params[1] = %d,params[2] = %d,params[3] = %d,"
                                "params[4] = %d,params[5] = %d,params[6] = %d,params[7] = %d,params[8] = %d,"
                                "params[9] = %d,params[10] = %d,params[11] = %d,params[12] = %d,params[13] = %d,"
                                "params[14] = %d,params[15] = %d,params[16] = %d,params[17] = %d\n",
                                prSmartBW->getSmartBWRomUpdate, ((int*)p_params)[0],((int*)p_params)[1],((int*)p_params)[2],((int*)p_params)[3],
                                ((int*)p_params)[4],((int*)p_params)[5],((int*)p_params)[6],((int*)p_params)[7],((int*)p_params)[8],
                                ((int*)p_params)[9],((int*)p_params)[10],((int*)p_params)[11],((int*)p_params)[12],((int*)p_params)[13],
                                ((int*)p_params)[14],((int*)p_params)[15],((int*)p_params)[16],((int*)p_params)[17]);
        }
}

/*
 * This is a misc function before really connect:
 * 1. Check whether need to save the connect log and set the flag for later use
 * Only update time & set needSaveLog to 1 when 1st time or after some dedicated time before last connect
 * 2. recored the connect time for later use
 * 3. try to get rom update if hasn't updated
 */
BOOLEAN preCheckBeforeConnect(uint8_t ucBssIndex)
{
        SMART_BW_PARAMS_T *p_params = &(prSmartBW->smart_bw_params);
        OS_SYSTIME rCurSysTime = kalGetTimeTick();
        //@2019/10/30: Dual STA need import ucBssIndex, now only check wlan0
        if (ucBssIndex != WLAN0_BSS_INDEX) return FALSE;

        prSmartBW->rCnnSysTime = rCurSysTime;
        prSmartBW->needSaveLog = 0;

        /* SLA set rom update maybe later than the 1st WIFI auto connect during WIFI turn on, cover this scenario here */
        if(!prSmartBW->getSmartBWRomUpdate && get_smart_bw_rom_update((int*)p_params, sizeof(SMART_BW_PARAMS_T))){
                prSmartBW->getSmartBWRomUpdate = true;
        }
        if(p_params->SMART_BW_DBG) {
                DBGLOG(AIS, TRACE, "wlanSetSmartBWParams:set params romupdate = %d, params[0] = %d,params[1] = %d,params[2] = %d,params[3] = %d,"
                                "params[4] = %d,params[5] = %d,params[6] = %d,params[7] = %d,params[8] = %d,"
                                "params[9] = %d,params[10] = %d,params[11] = %d,params[12] = %d,params[13] = %d,"
                                "params[14] = %d,params[15] = %d,params[16] = %d,params[17] = %d\n",
                                prSmartBW->getSmartBWRomUpdate, ((int*)p_params)[0],((int*)p_params)[1],((int*)p_params)[2],((int*)p_params)[3],
                                ((int*)p_params)[4],((int*)p_params)[5],((int*)p_params)[6],((int*)p_params)[7],((int*)p_params)[8],
                                ((int*)p_params)[9],((int*)p_params)[10],((int*)p_params)[11],((int*)p_params)[12],((int*)p_params)[13],
                                ((int*)p_params)[14],((int*)p_params)[15],((int*)p_params)[16],((int*)p_params)[17]);
        }

        /* 1st time or after pass some time */
        /* SAVE_CONNECT_LOG_THRESHOLD avoid driver/FWK quickly reconnect cases */
        if (prSmartBW->rRecordCnnSysTime == 0 ||
                TIME_AFTER(rCurSysTime, prSmartBW->rRecordCnnSysTime + SEC_TO_SYSTIME(p_params->SAVE_CONNECT_LOG_THRESHOLD))) {
                        DBGLOG(AIS, TRACE, "1st time connect, or connect time > %d than last save log connect.\n", p_params->SAVE_CONNECT_LOG_THRESHOLD);
                        prSmartBW->rRecordCnnSysTime = rCurSysTime;
                        prSmartBW->needSaveLog = 1;
        }

        return TRUE;
}

/*
 * log active BSS RSSI when active BSS number <= threshold
 * prSPBssDesc: the specail BSS want to print
 */
VOID connectLogActiveBSSRSSI(IN P_BSS_DESC_T prSPBssDesc)
{
        P_BSS_DESC_T prBssDesc = NULL;
        P_LINK_T prBSSDescList = &prAdapter->rWifiVar.rScanInfo.rBSSDescList;
        unsigned int u4ScanResultActive2GNum = prSmartBW->u4ScanResultActive2GNum;
        int CLEAN_AP_NUMBER = prSmartBW->smart_bw_params.CLEAN_AP_NUMBER;
        int SCAN_RESULT_EXPIRE = prSmartBW->smart_bw_params.SCAN_RESULT_EXPIRE;
        PARAM_RSSI rRssi;
        int i = 0;
        char buf[512];
        int bufStrLen = 0;
        /* performance better than memset, and the latter uninit byte will be replaced by snprintf */
        buf[0] = '\0';

        /* print RSSI only when active AP number <= AP_CLEAN_NUMBER_RANGE */
        if (u4ScanResultActive2GNum > CLEAN_AP_NUMBER) {
                return;
        }

        if (prSPBssDesc) {
                /* end '\0' will be replaced with next time's snprintf, last '\0' still be kept */
                rRssi = RCPI_TO_dBm(prSPBssDesc->ucRCPI);
                snprintf(buf, sizeof(buf), "[SCN]: SPSSID: %s, SPRSSI= %d, ", prSPBssDesc->aucSSID, rRssi);
        }
        bufStrLen = strlen(buf);

        LINK_FOR_EACH_ENTRY(prBssDesc, prBSSDescList, rLinkEntry, BSS_DESC_T) {
		if (prBssDesc->ucChannelNum > 214)
			continue;

	        if ((!CHECK_FOR_TIMEOUT(kalGetTimeTick(), prBssDesc->rUpdateTime, SEC_TO_SYSTIME(SCAN_RESULT_EXPIRE))) &&
	                (prBssDesc->eBand == BAND_2G4)) {
                                rRssi = RCPI_TO_dBm(prBssDesc->ucRCPI);
                                /* the previous end '\0' will be replaced with this time's snprintf, last '\0' still be kept */
                                snprintf(buf + bufStrLen, sizeof(buf) - bufStrLen, "SSID: %s, RSSI = %d ", prBssDesc->aucSSID, rRssi);
                                bufStrLen = strlen(buf);
                                if (++i >= CLEAN_AP_NUMBER) {
                                        break;
                                }
                                if (sizeof(buf) - bufStrLen <= 0) {
                                        break;
                                }
                }
        }

        if (prSmartBW->needSaveLog) {
                DBGLOG(AIS, TRACE, "%s\n", buf);
                htKalWCNKeyLogWrite(buf, TYPE_LOG_SCAN_RSSI_RATIO);
        }
}

/* log Bss band ratio when connect */
VOID connectLogSummary(IN P_BSS_DESC_T prBssDesc, uint8_t ucBssIndex)
{
        char buf[512];

        /* @2019/7/30, record the latest scan WM busy status in DCS */
        int i;
        unsigned char  ucNumOfChannel = 0;
        P_WM_BUSY_STATUS_T prWMBS = &(prSmartBW->arScanResult2GWMBusyStatus);
        WM_BUSY_STATUS_1Time_T* WMBS_1Time = NULL;
        RF_CHANNEL_INFO_T aucChannelList[14];

        /* performance better than memset, and the latter uninit byte will be replaced by snprintf */
        buf[0] = '\0';

        if (!prBssDesc)
                return;

        prSmartBW->u4ConnectTotalTime++;
        if (prBssDesc->eBand == BAND_2G4) {
                prSmartBW->u4ConnectTotal2GTime++;
        }

        if (isBSS24GHT40(prBssDesc->eBand, prBssDesc->eChannelWidth, prBssDesc->eSco)) {
                prSmartBW->u4ConnectTotal2GHT40Time++;

                if (prBssDesc->ucSSIDLen) {
                        DBGLOG(AIS, TRACE, "2.4G HT40, %s, Channel = %d, Ratio: %d/100\n",
                                prBssDesc->aucSSID, prBssDesc->ucChannelNum,
                                prSmartBW->u4ConnectTotal2GHT40Time * 100 / prSmartBW->u4ConnectTotalTime);
                }
        } else if (isBSS5GVHT160(prBssDesc->eBand, prBssDesc->eChannelWidth)) {
                prSmartBW->u4ConnectTotal5GVHT160Time++;

                if (prBssDesc->ucSSIDLen) {
                        DBGLOG(AIS, TRACE, "5G VHT160, %s, Channel = %d, Ratio: %d/100\n",
                                prBssDesc->aucSSID, prBssDesc->ucChannelNum,
                                prSmartBW->u4ConnectTotal5GVHT160Time * 100 / prSmartBW->u4ConnectTotalTime);
                }
        }

        if (prSmartBW->needSaveLog) {
                snprintf(buf, sizeof(buf),
                        "[CNN]: CNN: %u, CNN2G: %u, CNN2G_40M: %u, CCNN2G_Ratio: %u/100, CNN2G_40M_Ratio: %u/100, CNN5G_160M_MRatio: %u/100, ESS: %u",
                        prSmartBW->u4ConnectTotalTime,
                        prSmartBW->u4ConnectTotal2GTime,
                        prSmartBW->u4ConnectTotal2GHT40Time,
                        prSmartBW->u4ConnectTotal2GTime * 100 / prSmartBW->u4ConnectTotalTime,
                        prSmartBW->u4ConnectTotal2GHT40Time * 100 / prSmartBW->u4ConnectTotalTime,
                        prSmartBW->u4ConnectTotal5GVHT160Time * 100 / prSmartBW->u4ConnectTotalTime,
                        prAdapter->rWifiVar.rAisSpecificBssInfo[ucBssIndex].rCurEssLink.u4NumElem);
                DBGLOG(AIS, TRACE, "%s\n", buf);
                htKalWCNKeyLogWrite(buf, TYPE_LOG_CONNECT_BAND_RATIO);

                /* @2019/7/30, record the latest scan WM busy status in DCS */
                buf[0] = '\0';
                WMBS_1Time = &(prWMBS->arSR2G1TimeWMBS[prWMBS->u4LatestIndex]);
                kalGetChannelList(prAdapter->prGlueInfo, BAND_2G4, 20, &ucNumOfChannel, aucChannelList);
                snprintf(buf, sizeof(buf),"[SMART_BW]: [WMBusyStatus]: 2GTotalScanCh: %d ", WMBS_1Time->u4ScanResult2GTotalCh);
                for (i = 0; i < ucNumOfChannel; i++) {
                        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                                "Ch:%d, %ldms, %ldms, %d ",
                                WMBS_1Time->arSR2G1CHWMBS[i].Ch,
                                WMBS_1Time->arSR2G1CHWMBS[i].u4ScanResult2GIdleSlotTime,
                                WMBS_1Time->arSR2G1CHWMBS[i].u4ScanResult2GScanDuration,
                                WMBS_1Time->arSR2G1CHWMBS[i].cHIdleSlotRatio);
                }
                DBGLOG(AIS, TRACE, "%s\n", buf);
                htKalWCNKeyLogWrite(buf, TYPE_LOG_CONNECT_BAND_RATIO);
        }
}

/*
 * log the buffered and now active Bss in driver, buffered Bss contains the history APs haven't been replaced or removed
 */
VOID scanLogAllBufferedScanResult(void)
{
        P_BSS_DESC_T prBssDesc = NULL;
        P_LINK_T prBSSDescList = &prAdapter->rWifiVar.rScanInfo.rBSSDescList;
        int SCAN_RESULT_EXPIRE = prSmartBW->smart_bw_params.SCAN_RESULT_EXPIRE;

        unsigned int u4ScanResultTotalNum = 0;
        unsigned int u4ScanResult2GNum = 0;
        unsigned int u4ScanResult5GNum = 0;
        unsigned int u4ScanResult2G40MNum = 0;
        unsigned int u4ScanResult5G160MNum = 0;

        /* active BSS get from buffered BSS via time */
        unsigned int u4ScanResultActiveTotalNum = 0;
        unsigned int u4ScanResultActive2GNum = 0;
        unsigned int u4ScanResultActive5GNum = 0;
        unsigned int u4ScanResultActive2G40MNum = 0;
        unsigned int u4ScanResultActive5G160MNum = 0;
        BOOL expired = TRUE;

        PARAM_RSSI rRssi;
        OS_SYSTIME rTimeSec;
        char buf[512];
        /* performance better than memset, and the latter uninit byte will be replaced by snprintf */
        buf[0] = '\0';

        LINK_FOR_EACH_ENTRY(prBssDesc, prBSSDescList, rLinkEntry, BSS_DESC_T) {
                if (prBssDesc->ucChannelNum > 214)
                        continue;

                /* Collect & print 2.4G HT40 & 5G VHT160 BSS info */
                /* Caculate totalNum/totalActiveNum/Active2G/Active5G here other than in the deep if else cases */
                /* this make code more easy to track, otherwise, need to get these Active values in lots of if else cases */
                expired = CHECK_FOR_TIMEOUT(kalGetTimeTick(), prBssDesc->rUpdateTime, SEC_TO_SYSTIME(SCAN_RESULT_EXPIRE));
                u4ScanResultTotalNum++;
                if (!expired) {
                        u4ScanResultActiveTotalNum++;
                        if (prBssDesc->eBand == BAND_2G4) {
                               u4ScanResultActive2GNum++;
                        } else if (prBssDesc->eBand == BAND_5G) {
                                u4ScanResultActive5GNum++;
                        }
                }

	        if (prBssDesc->eBand == BAND_2G4) {
                        u4ScanResult2GNum++;
                        if (isBSS24GHT40(prBssDesc->eBand, prBssDesc->eChannelWidth, prBssDesc->eSco)) {
                                u4ScanResult2G40MNum++;
                                if (!expired) {
                                        u4ScanResultActive2G40MNum++;
			        }
		        }
                } else if (prBssDesc->eBand == BAND_5G) {
                        u4ScanResult5GNum++;
                        if (isBSS5GVHT160(prBssDesc->eBand, prBssDesc->eChannelWidth)) {
                                u4ScanResult5G160MNum++;
                                if (!expired) {
                                        u4ScanResultActive5G160MNum++;
			        }
                        }
                }

                rRssi = RCPI_TO_dBm(prBssDesc->ucRCPI);
                /* jiffies maybe turn around, so new time will < old time in this case, we should handle this case theoretically */
                /* But we only use it as print info and it only happen once a round, and one round will last 50 or 500 days */
                /* So just let it go for easy handling */
                rTimeSec = SYSTIME_TO_SEC(kalGetTimeTick() - prBssDesc->rUpdateTime);

                printScanResultDetail(prBssDesc->aucSSID, prBssDesc->ucSSIDLen,
                        prBssDesc->ucChannelNum, prBssDesc->eBand, prBssDesc->eChannelWidth,
                        prBssDesc->eSco, rRssi, rTimeSec, prBssDesc->aucBSSID);
        } /* end LINK_FOR_EACH_ENTRY */

        /* Centralize the error handling to make things easy */
        if ((u4ScanResultTotalNum > CFG_MAX_NUM_BSS_LIST) || (u4ScanResult2GNum > CFG_MAX_NUM_BSS_LIST) ||
                (u4ScanResult2G40MNum > CFG_MAX_NUM_BSS_LIST) || (u4ScanResult5GNum > CFG_MAX_NUM_BSS_LIST) ||
                (u4ScanResult5G160MNum > CFG_MAX_NUM_BSS_LIST) || (u4ScanResultActiveTotalNum > CFG_MAX_NUM_BSS_LIST) ||
                (u4ScanResultActive2GNum > CFG_MAX_NUM_BSS_LIST) || (u4ScanResultActive2G40MNum > CFG_MAX_NUM_BSS_LIST) ||
                (u4ScanResultActive5GNum > CFG_MAX_NUM_BSS_LIST) || (u4ScanResult5G160MNum > CFG_MAX_NUM_BSS_LIST)) {
                        DBGLOG(SCN, ERROR, "Scan result summary number out of range, error!!!\n");
        }
        u4ScanResultTotalNum = (u4ScanResultTotalNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResultTotalNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResult2GNum = (u4ScanResult2GNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResult2GNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResult2G40MNum = (u4ScanResult2G40MNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResult2G40MNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResult5GNum = (u4ScanResult5GNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResult5GNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResult5G160MNum = (u4ScanResult5G160MNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResult5G160MNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResultActiveTotalNum = (u4ScanResultActiveTotalNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResultActiveTotalNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResultActive2GNum = (u4ScanResultActive2GNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResultActive2GNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResultActive2G40MNum = (u4ScanResultActive2G40MNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResultActive2G40MNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResultActive5GNum = (u4ScanResultActive5GNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResultActive5GNum: CFG_MAX_NUM_BSS_LIST;
        u4ScanResultActive5G160MNum = (u4ScanResultActive5G160MNum < CFG_MAX_NUM_BSS_LIST) ? u4ScanResultActive5G160MNum: CFG_MAX_NUM_BSS_LIST;

        prSmartBW->u4ScanResultTotalNum = u4ScanResultTotalNum;
        prSmartBW->u4ScanResult2GNum = u4ScanResult2GNum;
        prSmartBW->u4ScanResult2G40MNum = u4ScanResult2G40MNum;
        prSmartBW->u4ScanResult5GNum = u4ScanResult5GNum;
        prSmartBW->u4ScanResult5G160MNum = u4ScanResult5G160MNum;
        prSmartBW->u4ScanResultActiveTotalNum = u4ScanResultActiveTotalNum;
        prSmartBW->u4ScanResultActive2GNum = u4ScanResultActive2GNum;
        prSmartBW->u4ScanResultActive2G40MNum = u4ScanResultActive2G40MNum;
        prSmartBW->u4ScanResultActive5GNum = u4ScanResultActive5GNum;
        prSmartBW->u4ScanResultActive5G160MNum = u4ScanResultActive5G160MNum;

        /* snprintf is better than memcpy, strncpy etc.., since snprintf will make sure the dest has '\0' in the end */
        /* snprintf behavior is different in GCC & VC, gcc the size contains the '\0', so it's more safe */
#if DBG
        if (prSmartBW->needSaveLog) {
                snprintf(buf, sizeof(buf),
                        "[SCN]: %u query %u store buffered Bss, SCN2G: %u, SCN2G_40M: %u, SCN2G_40M_Ratio: %u/100, SCN5G: %u, SCN5G_160M: %u, SCN5G_160M_Ratio: %u/100",
                        prSmartBW->u4ScanResultTotalNum,
                        prAdapter->rWifiVar.rScanInfo.rBSSDescList.u4NumElem,
                        prSmartBW->u4ScanResult2GNum, prSmartBW->u4ScanResult2G40MNum,
                        prSmartBW->u4ScanResult2G40MNum * 100 / prSmartBW->u4ScanResult2GNum,
                        prSmartBW->u4ScanResult5GNum, prSmartBW->u4ScanResult5G160MNum,
                        prSmartBW->u4ScanResult5G160MNum * 100 / prSmartBW->u4ScanResult5GNum);
                DBGLOG(SCN, TRACE, "%s\n", buf);
                htKalWCNKeyLogWrite(buf, TYPE_LOG_SCAN_BAND_RATIO);
        }
#endif
        if (prSmartBW->needSaveLog) {
                snprintf(buf, sizeof(buf),
                        "[SCN]: %u active Bss, %u buffered Bss, SCN2G: %u, SCN2G_40M: %u, SCN2G_40M_Ratio: %u/100, SCN5G: %u, SCN5G_160M: %u, SCN5G_160M_Ratio: %u/100",
                        prSmartBW->u4ScanResultActiveTotalNum,
                        prSmartBW->u4ScanResultTotalNum,
                        prSmartBW->u4ScanResultActive2GNum, prSmartBW->u4ScanResultActive2G40MNum,
                        prSmartBW->u4ScanResultActive2G40MNum * 100 / prSmartBW->u4ScanResultActive2GNum,
                        prSmartBW->u4ScanResultActive5GNum, prSmartBW->u4ScanResultActive5G160MNum,
                        prSmartBW->u4ScanResultActive5G160MNum * 100 / prSmartBW->u4ScanResultActive5GNum);
                DBGLOG(SCN, TRACE, "%s\n", buf);
                htKalWCNKeyLogWrite(buf, TYPE_LOG_SCAN_BAND_RATIO);
        }
}

/* print all BSS important info, TRACE level is default off */
VOID printScanResultDetail(unsigned char  *aucSSID, unsigned char  ucSSIDLen,
        unsigned char  ucChannelNum, ENUM_BAND_T eBand,
        ENUM_CHANNEL_WIDTH_T eChannelWidth, ENUM_CHNL_EXT_T eSco,
        PARAM_RSSI rRssi, OS_SYSTIME rTimeSec, unsigned char * aucBSSID)
{
        if (aucSSID && ucSSIDLen) {
                DBGLOG(SCN, TRACE, "%s\n", aucSSID);
        }

        DBGLOG(SCN, TRACE, "Channel = %d, Band = %d, ChannelWidth = %d, Sco = %d, RSSI: %d, UpdateTimePassed: %u\n",
                        ucChannelNum, eBand, eChannelWidth, eSco, rRssi, rTimeSec);

        if (aucBSSID) {
                DBGLOG(SCN, TRACE, "BSSID: %x:%x:%x:%x:%x:%x\n",
                        aucBSSID[0], aucBSSID[1], aucBSSID[2],
                        aucBSSID[3], aucBSSID[4], aucBSSID[5]);
        }
}

/* print WM Busy Status */
VOID printWMBusyStatus(void)
{
        int i = 0;
        int x = 0;
        unsigned char  ucNumOfChannel = 0;
        RF_CHANNEL_INFO_T aucChannelList[14];
        P_WM_BUSY_STATUS_T prWMBS = &(prSmartBW->arScanResult2GWMBusyStatus);
        WM_BUSY_STATUS_1Time_T* WMBS_1Time = NULL;

        kalGetChannelList(prAdapter->prGlueInfo, BAND_2G4, 20, &ucNumOfChannel, aucChannelList);
        DBGLOG(SCN, TRACE, "oldIndex: %d, newIndex: %d, storeNum = %d\n",
                prWMBS->u4OldestIndex, prWMBS->u4LatestIndex, prWMBS->u4StoreNum);
        for(x = 0; x < prWMBS->u4StoreNum; x++) {
                DBGLOG(SCN, TRACE, "index: %d\n", (prWMBS->u4OldestIndex + x) % 3);
                WMBS_1Time =  &(prWMBS->arSR2G1TimeWMBS[(prWMBS->u4OldestIndex + x) % 3]); /* since it's round robin */
                for (i = 0; i < ucNumOfChannel; i++) {
                        DBGLOG(SCN, TRACE, "2GTotalScanCh: %d, Ch: %d, 2GIdleSlotTime:%ldms, 2GScanDuration:%ldms, 2GChIdleSlotRatio:%d\n",
                                WMBS_1Time->u4ScanResult2GTotalCh, WMBS_1Time->arSR2G1CHWMBS[i].Ch,
                                WMBS_1Time->arSR2G1CHWMBS[i].u4ScanResult2GIdleSlotTime,
                                WMBS_1Time->arSR2G1CHWMBS[i].u4ScanResult2GScanDuration,
                                WMBS_1Time->arSR2G1CHWMBS[i].cHIdleSlotRatio);
                }
        }
}

/*
 * 2019/11/19 gen3, gen4 W/WO DODC scan results are all different
 * gen3 prScanDone->aucChannelNum[0] doesn't store channel, so it start from index 1, but gen4 start from index 0
 * without DBDC, 2.4G is place in the front, with DBDC, 2.4G and 5G is placed alternately.
 * Try to find a general full loop function to handle all the cases
 */
BOOLEAN checkScanResult2GFullScan(struct EVENT_SCAN_DONE* prScanDone, u8* ucStartIndex, u8* ucEndIndex, u8* uc2GNumOfChannel)
{
        u8 i;
        RF_CHANNEL_INFO_T aucChannelList[14];
        u8 uc2GScandDoneChCnt = 0; //2.4G channel number in the scan done result
        BOOLEAN ret = FALSE;

#if (defined CFG_SUPPORT_CONNAC2X) || (defined CFG_SUPPORT_CONNAC2X_2x2)//stands for gen4m
        *ucStartIndex = 0;
        *ucEndIndex   = prScanDone->ucCompleteChanCount - 1;
#else
        *ucStartIndex = 1;
        *ucEndIndex   = prScanDone->ucCompleteChanCount;
#endif

        kalGetChannelList(prAdapter->prGlueInfo, BAND_2G4, 20, uc2GNumOfChannel, aucChannelList);
        // ucNumOfChannel: phone support 2.4G channel number, ucCompleteChanCount: all scan channel this time (2.4G + 5G)
        if (prScanDone->ucCompleteChanCount < *uc2GNumOfChannel) return FALSE;
        for (i = *ucStartIndex; i <= *ucEndIndex; i++) {
                if (prScanDone->aucChannelNum[i] <= *uc2GNumOfChannel) uc2GScandDoneChCnt++;

                if (uc2GScandDoneChCnt >= *uc2GNumOfChannel) {
                        ret = TRUE;
                        break;
                }
        }

        DBGLOG(SCN, TRACE, "ret: %d, ucStartIndex = %d, ucEndIndex = %d, uc2GScandDoneChCnt = %d, uc2GNumOfChannel = %d, aucChannelNum[0] = %d, aucChannelNum[1] = %d\n",
                ret, *ucStartIndex, *ucEndIndex, uc2GScandDoneChCnt, *uc2GNumOfChannel, prScanDone->aucChannelNum[0], prScanDone->aucChannelNum[1]);

        return ret;
}

/* only save the 2.4G full scan's WM busy status for later check WMBusyStatus usage */
void saveScanWMBusyStatus(struct EVENT_SCAN_DONE* prScanDone)
{
        P_WM_BUSY_STATUS_T prWMBS = &(prAdapter->rSmartBW.arScanResult2GWMBusyStatus);
        WM_BUSY_STATUS_1Time_T* WMBS_1Time = NULL;
        WM_BUSY_STATUS_1CH_T* WMBS_1CH = NULL;
        u8 uc2GScandDoneChCnt = 0;
        u8 ucStartIndex, ucEndIndex, uc2GNumOfChannel;
        u8 i;

        if (!checkScanResult2GFullScan(prScanDone, &ucStartIndex, &ucEndIndex, &uc2GNumOfChannel)) return;

        if (!prWMBS->u4StoreNum) {
                prWMBS->u4OldestIndex = 0;
                prWMBS->u4LatestIndex = 0;
        } else {
                /* must put ++ before prWMBS->u4LatestIndex */
                prWMBS->u4LatestIndex = (++prWMBS->u4LatestIndex) % 3;
                /* need forward u4OldestIndex since this means already round trip */
                if (prWMBS->u4LatestIndex == prWMBS->u4OldestIndex) {
                       prWMBS->u4OldestIndex = (++prWMBS->u4OldestIndex) % 3;
                }
        }
        prWMBS->u4StoreNum++;
        prWMBS->u4StoreNum = (prWMBS->u4StoreNum <= 3) ? prWMBS->u4StoreNum:3; /* buffer list is 3 */
        WMBS_1Time = &(prWMBS->arSR2G1TimeWMBS[prWMBS->u4LatestIndex]);
        kalMemSet(WMBS_1Time, 0, sizeof(WM_BUSY_STATUS_1Time_T));

        GET_CURRENT_SYSTIME(&WMBS_1Time->u4SysScanTime);
        for (i = ucStartIndex; i <= ucEndIndex; i++) {
                if (prScanDone->aucChannelNum[i] <= uc2GNumOfChannel) {
                        WMBS_1Time->u4ScanResult2GTotalCh++;
                        WMBS_1CH = &(WMBS_1Time->arSR2G1CHWMBS[uc2GScandDoneChCnt]);
                        WMBS_1CH->Ch = prScanDone->aucChannelNum[i];
                        WMBS_1CH->u4ScanResult2GScanDuration = prScanDone->au2ChannelScanTime[i];
                        WMBS_1CH->u4ScanResult2GIdleSlotTime = prScanDone->au2ChannelIdleTime[i];
                        if (WMBS_1CH->u4ScanResult2GScanDuration) {
                                /* save value not value% to avoid floating point */
                                /* 2019/11/19 gen4 different from gen3, gen4 store slot number not ms */
#if (defined CFG_SUPPORT_CONNAC2X) || (defined CFG_SUPPORT_CONNAC2X_2x2)
                                WMBS_1CH->cHIdleSlotRatio =
                                        (WMBS_1CH->u4ScanResult2GIdleSlotTime * 9) / (10 * WMBS_1CH->u4ScanResult2GScanDuration);
#else
                                WMBS_1CH->cHIdleSlotRatio =
                                        (WMBS_1CH->u4ScanResult2GIdleSlotTime * 100) / WMBS_1CH->u4ScanResult2GScanDuration;
#endif
                        }
                        uc2GScandDoneChCnt++;
                }
                if (uc2GScandDoneChCnt >= uc2GNumOfChannel) break;
        }
        printWMBusyStatus();
}

/* return TRUE: clean */
/* need satisfy:
 * 1. AP is 2.4G HT40
 * 2. not all stored scan is partial scan and also not all phased out (partial scan & phased out scan will just bypass)
 * All full scan and not phased out scan should match:
 * 3. related "8" CH each CH idle slot ratio >= 10%
 * 4. related "8" CH average idle slot ratio >= 15%
 */
BOOLEAN checkEnvironmentClean(IN P_BSS_DESC_T prSPBssDesc)
{
        int i = 0;
        P_WM_BUSY_STATUS_T prWMBS = &(prSmartBW->arScanResult2GWMBusyStatus);
        unsigned int u4IdleTotalSlotTime = 0;
        unsigned int u4IdleTotalSlotRatio = 0;
        unsigned int u4IdleTotalScanDuration = 0;
        int startCH, endCH;
        BOOLEAN ret = TRUE;
        RF_CHANNEL_INFO_T aucChannelList[14];
        unsigned char  ucNumOfChannel = 0;
        int x = 0;
        int matchedScanRound = 0;
        WM_BUSY_STATUS_1Time_T* WMBS_1Time = NULL;
        WM_BUSY_STATUS_1CH_T* WMBS_1CH = NULL;
        int SCAN_RESULT_EXPIRE = prSmartBW->smart_bw_params.SCAN_RESULT_EXPIRE;
        int CLEAN_CH_IDLE_SLOT_RATIO = prSmartBW->smart_bw_params.CLEAN_CH_IDLE_SLOT_RATIO;
        int CLEAN_BAND_IDLE_SLOT_RATIO = prSmartBW->smart_bw_params.CLEAN_BAND_IDLE_SLOT_RATIO;

        printWMBusyStatus();

        /* partial scan result or scan result too long isn't accurate */
	kalGetChannelList(prAdapter->prGlueInfo, BAND_2G4, 20, &ucNumOfChannel, aucChannelList);
        if (TIME_AFTER(kalGetTimeTick(), prWMBS->arSR2G1TimeWMBS[prWMBS->u4LatestIndex].u4SysScanTime + SEC_TO_SYSTIME(SCAN_RESULT_EXPIRE)) ||
                        !isBSS24GHT40(prSPBssDesc->eBand, prSPBssDesc->eChannelWidth, prSPBssDesc->eSco) ||
                        !prWMBS->u4StoreNum) {
                DBGLOG(SCN, TRACE, "condition not matched: %d\n", __LINE__);
                return FALSE;
        }

        /* get the related 8 CH */
        if (prSPBssDesc->eSco == CHNL_EXT_SCB) {
            startCH = prSPBssDesc->ucChannelNum -5;
            endCH = prSPBssDesc->ucChannelNum + 2;
        } else if (prSPBssDesc->eSco == CHNL_EXT_SCA) {
            startCH = prSPBssDesc->ucChannelNum -1;
            endCH = prSPBssDesc->ucChannelNum + 6;
        }
        if (startCH < 1) startCH = 1;
        if (endCH < 1) endCH = 1;
        if (startCH > ucNumOfChannel) startCH = ucNumOfChannel;
        if (endCH > ucNumOfChannel) endCH = ucNumOfChannel;
        if(endCH - startCH <= 1) return FALSE; /* the real related ch number may < 8 when in the channel boundary */
        DBGLOG(SCN, TRACE, "startCH = %d, endCH = %d, primary ch = %d\n", startCH, endCH, prSPBssDesc->ucChannelNum);

        /* loop into each scan run */
        for(x = 0; x < prWMBS->u4StoreNum; x++) {
                WMBS_1Time =  &(prWMBS->arSR2G1TimeWMBS[(prWMBS->u4OldestIndex + x) % 3]); /* since it's round robin */
                /* scan time too long or not full channel scan isn't accurate */
                if ((TIME_AFTER(kalGetTimeTick(), WMBS_1Time->u4SysScanTime + SEC_TO_SYSTIME(SCAN_RESULT_EXPIRE))) ||
                        WMBS_1Time->u4ScanResult2GTotalCh < ucNumOfChannel) {
                                continue;
                }

                /* loop for every channel */
                /* each CH idle slot ratio should match the requirement */
                for(i = startCH-1; i <= endCH-1; i++) { /* only search 2.4 1-13 CH, WMBusyStatus[0] store CH1*/
                        /* New FW already upload ms instead of the real slot number, so needn't use *9/1000 to get ms */
                        /* Just get 100% ratio, so it caculate as: (100 * (slottime * 9/1000))/scan duration */
                        /* So it's (slottime * 9) / (10 * scan duration ) */
                        WMBS_1CH = &(WMBS_1Time->arSR2G1CHWMBS[i]);
                        u4IdleTotalSlotTime += WMBS_1CH->u4ScanResult2GIdleSlotTime;
                        u4IdleTotalScanDuration += WMBS_1CH->u4ScanResult2GScanDuration;
                        DBGLOG(SCN, TRACE, "Ch = %d, 2GScanDuration = %ldms, 2GMdrdyCount = %ld, 2GIdleSlotTime= %ldms, 2GSlotRatio = %ld/100\n",
                                WMBS_1CH->Ch, WMBS_1CH->u4ScanResult2GScanDuration,
                                WMBS_1CH->u4ScanResult2GMdrdyCount,
                                WMBS_1CH->u4ScanResult2GIdleSlotTime, WMBS_1CH->cHIdleSlotRatio);
                        if (WMBS_1CH->cHIdleSlotRatio < CLEAN_CH_IDLE_SLOT_RATIO) {
                                DBGLOG(SCN, TRACE, "2GSlotRatio condition not matched: %ld/100\n", WMBS_1CH->cHIdleSlotRatio);
                                return FALSE;
                        }
                }

                /* toal CH idle slot ratio should match the requirement */
                if (ret == TRUE) {
                        //u4IdleTotalSlotRatio = (u4IdleTotalSlotTime * 100) / u4IdleTotalScanDuration;
                        u4IdleTotalSlotRatio = (u4IdleTotalSlotTime * 9) / (u4IdleTotalScanDuration * 10); //FenghuaHenry TODO, tmp modify since gen4 upload slot not ms
                        DBGLOG(SCN, TRACE, "u4IdleTotalSlotTime = %ldms, u4IdleTotalScanDuration = %ldms, u4IdleTotalSlotRatio = %ld\n",
                                u4IdleTotalSlotTime, u4IdleTotalScanDuration, u4IdleTotalSlotRatio);
                        if (u4IdleTotalSlotRatio < CLEAN_BAND_IDLE_SLOT_RATIO) {
                                DBGLOG(SCN, TRACE, "2GTotalSlotRatio condition not matched: %ld/100\n", u4IdleTotalSlotRatio);
                                return FALSE;
                        }
                }
                if (ret == TRUE) {
                        matchedScanRound++;
                }
        }

        if (!matchedScanRound) {
                ret = FALSE;
        }

        DBGLOG(SCN, INFO, "checkEnvironmentClean status: %d\n", ret);
        return ret;
}

/* return TRUE:CT */
BOOLEAN isCTEnvironment(unsigned char * aucSSID, unsigned char  ucSSIDLen)
{
        unsigned char  CTAPSSID[] = "CREAP";

        if (!aucSSID || ucSSIDLen < 5) {
                return FALSE;
        }

        if (!strncmp(CTAPSSID, aucSSID, 5)) {
                return TRUE;
        } else {
                return FALSE;
        }
}

//2020/1/14: Add Blacklist for 2.4G 40M APIOT issue, return TRUE when the BssDesc is in 2G40M Blacklist
BOOLEAN isIn2G40MBlacklist(IN P_BSS_DESC_T prSPBssDesc)
{
        BOOLEAN ret = FALSE;
        if (bssGetIotApAction(prAdapter, prSPBssDesc) == WLAN_IOT_AP_DIS_2GHT40) ret = TRUE;

        return ret;
}

/* return TRUE: 40M, FALSE: 20M */
BOOLEAN extraCheck2GSupportBW(IN P_BSS_DESC_T prSPBssDesc)
{
        BOOLEAN ret = FALSE;
        SMART_BW_PARAMS_T *p_params = &(prSmartBW->smart_bw_params);
        char buf[512];
        /* performance better than memset, and the latter uninit byte will be replaced by snprintf */
        buf[0] = '\0';

        if (isBSS24GHT40(prSPBssDesc->eBand, prSPBssDesc->eChannelWidth, prSPBssDesc->eSco)
                && !isIn2G40MBlacklist(prSPBssDesc)
                && prSmartBW->u4ScanResultActive2GNum <= p_params->CLEAN_AP_NUMBER
                && RCPI_TO_dBm(prSPBssDesc->ucRCPI) >= p_params->GOOD_RSSI
                && checkEnvironmentClean(prSPBssDesc)) {
                if (!isCTEnvironment(prSPBssDesc->aucSSID, prSPBssDesc->ucSSIDLen)) {
                        /* none crep need start link quality monitor */
                        prSmartBW->fgIsNeedMonitorLink = TRUE;
                        prSmartBW->fgIsNeedMonitorAPIOT = TRUE;
                }
                ret = TRUE;
        }

	snprintf(buf, sizeof(buf),
		"[SMART_BW]: [HT40_SELECT]: eBand = %d, eChannelWidth = %d, eSco = %d, Active2GNum = %d, RSSI: %d, SSID: %s, CH: %d, 2.4G HT40 status: %d, fgIsNeedMonitorLink: %d, fgIsNeedMonitorAPIOT: %d, isIn2G40MBlacklist: %d",
                prSPBssDesc->eBand, prSPBssDesc->eChannelWidth, prSPBssDesc->eSco,
                prSmartBW->u4ScanResultActive2GNum, RCPI_TO_dBm(prSPBssDesc->ucRCPI), prSPBssDesc->aucSSID, prSPBssDesc->ucChannelNum,
                ret, prSmartBW->fgIsNeedMonitorLink, prSmartBW->fgIsNeedMonitorAPIOT, isIn2G40MBlacklist(prSPBssDesc));

        DBGLOG(SCN, TRACE, "%s\n", buf);
        if (prSmartBW->needSaveLog) htKalWCNKeyLogWrite(buf, TYPE_LOG_CONNECT_BAND_RATIO); /* limit the DCS log */

        return ret;
}

void wlanResetLinkQualityMonitorStatus(void)
{
        prSmartBW->fgIsNeedMonitorLink = FALSE;
        prSmartBW->fgIsNeedMonitorAPIOT = FALSE;

        prSmartBW->u4LQOldestIndex = 0;
        prSmartBW->u4LQLatestIndex = 0;
        prSmartBW->u4LQStoreNum = 0;

        kalMemSet(prSmartBW->rLinkQualityInfoSmooth, 0, sizeof(prSmartBW->rLinkQualityInfoSmooth));
}

/* 2019/10/26 only for STA 2.4G now, pUcAPBandwidth valid only when return true */
BOOLEAN tuneSTABW(struct BSS_INFO* prBssInfo, struct BSS_DESC* prBssDesc, u8* pUcAPBandwidth, uint8_t ucBssIndex)
{
        /* now only change for STA mode */
        /* @7/25: move SELECT_BW_WHEN_CONNECT_FEATRUE_ENABLE here to make code same with origin when this feature disable */
        unsigned char  ucAPBandwidth = *pUcAPBandwidth;
        BOOLEAN ret = FALSE;

        DBGLOG(SCN, TRACE, "FenghuaHenry 3, ucBssIndex = %d, feature: %d, manual: %d, ucAPBandwidth: %d\n",
                ucBssIndex, prAdapter->rSmartBW.smart_bw_params.SELECT_BW_WHEN_CONNECT_FEATRUE_ENABLE,
                prAdapter->rSmartBW.i24GBWManualSet, ucAPBandwidth);

        //@2019/10/30, for Dual STA, only check WLAN0 currently
        if (ucBssIndex != WLAN0_BSS_INDEX) return ret;

        if (prAdapter->rSmartBW.smart_bw_params.SELECT_BW_WHEN_CONNECT_FEATRUE_ENABLE
                        && prBssDesc->eBand == BAND_2G4 && IS_BSS_AIS(prBssInfo)) {
                wlanResetLinkQualityMonitorStatus();
                /* when manual set BW, we won't check environment */
                if (ucAPBandwidth == MAX_BW_40MHZ
                                && !prAdapter->rSmartBW.i24GBWManualSet
                                && !extraCheck2GSupportBW(prBssDesc)) {
                        ucAPBandwidth = MAX_BW_20MHZ;
                }

                /* support manual control for both manual switch BW & debug purpose */
                /* under control of step 2(DYNAMIC_SW_BW_FEATRUE_ENABLE) feature option */
                if (prAdapter->rSmartBW.smart_bw_params.DYNAMIC_SW_BW_FEATRUE_ENABLE) {
                        if (prAdapter->rSmartBW.i24GBWManualSet == 1) {
                                ucAPBandwidth = MAX_BW_20MHZ;
                        } else if (prAdapter->rSmartBW.i24GBWManualSet == 2) {
                                ucAPBandwidth = MAX_BW_40MHZ;
                                /* force HT40 also need set start monitor */
                                prAdapter->rSmartBW.fgIsNeedMonitorLink = TRUE;
                                prAdapter->rSmartBW.fgIsNeedMonitorAPIOT = TRUE;
                        } else if (prAdapter->rSmartBW.i24GBWManualSet == 1001 && prAdapter->rSmartBW.smart_bw_params.SMART_BW_DBG) {
                                /* Debug purpose for monitor 2.4G HT20 LQ status, easy to compare with HT40 */
                                ucAPBandwidth = MAX_BW_20MHZ;
                                prAdapter->rSmartBW.fgIsNeedMonitorLink = TRUE;
                                prAdapter->rSmartBW.fgIsNeedMonitorAPIOT = TRUE;
                        }

                        prAdapter->rSmartBW.i24GBWManualSet = 0;
                }

                ret = TRUE;
                *pUcAPBandwidth = ucAPBandwidth;

                DBGLOG(SCN, TRACE, "FenghuaHenry 4, pUcAPBandwidth = %d\n", *pUcAPBandwidth);
        }
        if (ret) prAdapter->rSmartBW.ucSelectBW = ucAPBandwidth;

        return ret;
}

VOID printLQSmoothStatus(void)
{
        struct WIFI_LINK_QUALITY_INFO *prLQInfoSmooth = NULL;
        int i;
        int LQ_CHECK_NUM = prSmartBW->smart_bw_params.LQ_CHECK_NUM;
        /* loop into each run */
        DBGLOG(SW4, TRACE, "LQSmooth startInx:%d, endIdx: %d, totalCount: %d\n",
                prSmartBW->u4LQOldestIndex, prSmartBW->u4LQLatestIndex, prSmartBW->u4LQStoreNum);
        for (i = 0; i < prSmartBW->u4LQStoreNum; i++) {
                prLQInfoSmooth = &(prSmartBW->rLinkQualityInfoSmooth[(prSmartBW->u4LQOldestIndex + i) % LQ_CHECK_NUM]);/* since it's round robin */
                DBGLOG(SW4, TRACE,
                        "Link Quality: current Idx: %d, Tx(rate:%u, total:%lu, retry:%lu, fail:%u, RTS fail:%lu, ACK fail:%lu),"
                        "Rx(rate:%u, total:%lu, dup:%u, error:%lu), PER(%u), Congestion(idle slot:%u, diff:%u),"
                        "L3(CSpeed: %d, congestion: %d), recordTime: %u\n",
                        (prSmartBW->u4LQOldestIndex + i) % LQ_CHECK_NUM,
                        prLQInfoSmooth->u4CurTxRate, /* tx rate, current tx link speed */
                        prLQInfoSmooth->u8TxTotalCount, /* tx total packages */
                        prLQInfoSmooth->u8TxRetryCount, /* tx retry count */
                        prLQInfoSmooth->u8TxFailCount, /* tx fail count */
                        prLQInfoSmooth->u8TxRtsFailCount, /* tx RTS fail count */
                        prLQInfoSmooth->u8TxAckFailCount, /* tx ACK fail count */
                        prLQInfoSmooth->u4CurRxRate, /* rx rate */
                        prLQInfoSmooth->u8RxTotalCount, /* rx total packages */
                        prLQInfoSmooth->u4RxDupCount, /* rx duplicate package count */
                        prLQInfoSmooth->u8RxErrCount, /* rx error count */
                        prLQInfoSmooth->u4CurTxPer, /* PER */
                        prLQInfoSmooth->u8IdleSlotCount,  /* congestion stats: idle slot */
                        prLQInfoSmooth->u8DiffIdleSlotCount,
                        prLQInfoSmooth->iL3CSpeed,
                        prLQInfoSmooth->iL3CongestionFlag,
                        prLQInfoSmooth->rRecordLQSysTime
	        );
        }
}

void wlanSaveLinkQualitySmooth(struct WIFI_LINK_QUALITY_INFO *prLinkQualityInfo)
{
       struct WIFI_LINK_QUALITY_INFO *prLinkQualityInfoSmooth = NULL;
       int LQ_CHECK_NUM = prSmartBW->smart_bw_params.LQ_CHECK_NUM;

       if (!prSmartBW->u4LQStoreNum) {
               prSmartBW->u4LQOldestIndex = 0;
               prSmartBW->u4LQLatestIndex = 0;
       } else {
               /* must put ++ before */
               prSmartBW->u4LQLatestIndex = (++prSmartBW->u4LQLatestIndex) % LQ_CHECK_NUM;
               /* need forward u4LQOldestIndex since this means already round trip */
               if (prSmartBW->u4LQLatestIndex == prSmartBW->u4LQOldestIndex) {
                      prSmartBW->u4LQOldestIndex = (++prSmartBW->u4LQOldestIndex) % LQ_CHECK_NUM;
               }
       }
       prSmartBW->u4LQStoreNum++;
       prSmartBW->u4LQStoreNum = (prSmartBW->u4LQStoreNum <= LQ_CHECK_NUM) ? prSmartBW->u4LQStoreNum:LQ_CHECK_NUM;
       prLinkQualityInfoSmooth = &(prSmartBW->rLinkQualityInfoSmooth[prSmartBW->u4LQLatestIndex]);
       kalMemCopy(prLinkQualityInfoSmooth, prLinkQualityInfo, sizeof(struct WIFI_LINK_QUALITY_INFO));
       printLQSmoothStatus();
}

/*
 * return FALSE:  return value no useful
 * this may due to none phase out info too less since when screen off, link quality monitor timer will stop, LQ won't update.
 * active means the time saved should less than LQ_CHECK_EXPIRE compare with current time
 *
 * Get the useful active average link quality status
 */
BOOLEAN wlanGetActiveAverageLQStatus(
        unsigned int* u4GetAVGCurTxRate,
        unsigned long long* u8GetRetAVGTxTotalCount,
        unsigned long long* u8GetAVGTxRetryCount,
        unsigned long long* u8GetAVGTxFailCount,
        unsigned int* u4GetAVGCurTxPer,
        unsigned int* u4GetAVGCurRxRate,
        unsigned int* u4GetAVGRxDupCount,
        unsigned long long* u8GetAVGRxTotalCount,
        unsigned char * u8GetAVGIdleSlotRatio,
        int* iGetAVGL3CurSpped,
        unsigned char * u1GetActiveTotalNum)
{
        SMART_BW_PARAMS_T *p_params = &(prSmartBW->smart_bw_params);
        struct WIFI_LINK_QUALITY_INFO *prLQInfoSmooth = NULL;
        struct WIFI_LINK_QUALITY_INFO *prLQInfoSmoothStart = NULL;
        struct WIFI_LINK_QUALITY_INFO *prLQInfoSmoothEnd= NULL;
        unsigned int u4AVGCurTxRate = 0;
        unsigned long long u8AVGTxTotalCount = 0;
        unsigned long long u8AVGTxRetryCount = 0;
        unsigned long long u8AVGTxFailCount = 0;
        unsigned int u4AVGCurTxPer = 0;
        unsigned int u4AVGCurRxRate = 0;
        unsigned int u4AVGRxDupCount = 0;
        unsigned long long u8AVGRxTotalCount = 0;
        unsigned long long u8DiffIdleSlotCount = 0;
        unsigned long long u8HwMacAwakeDuration = 0;
        unsigned char   u8AVGIdleSlotRatio = 0;
        int iAVGL3CurSpped = 0;
        int i;
        int activeStartIndex; /* u4LQOldestIndex may store phase out data, need get the real active start index */
        int activeTotalNum; /* the active total number */
        int activeValidRxTotalNum = 0; /* the active valid rx total number, Rx rate maybe 0 */
        BOOLEAN fgIsGetStartIndex = FALSE;

        prLQInfoSmooth = &(prSmartBW->rLinkQualityInfoSmooth[prSmartBW->u4LQLatestIndex]);
        if (TIME_AFTER(kalGetTimeTick(), prLQInfoSmooth->rRecordLQSysTime + SEC_TO_SYSTIME(p_params->LQ_CHECK_EXPIRE)) ||
                        !prSmartBW->u4LQStoreNum) {
                DBGLOG(INIT, INFO, "condition not matched: %d\n", __LINE__);
                return FALSE;
        }

        /* loop into each run */
        for (i = 0; i < prSmartBW->u4LQStoreNum; i++) {
                prLQInfoSmooth = &(prSmartBW->rLinkQualityInfoSmooth[(prSmartBW->u4LQOldestIndex + i) % p_params->LQ_CHECK_NUM]);/* since it's round robin */
                /* time too long isn't accurate */
                if (TIME_AFTER(kalGetTimeTick(), prLQInfoSmooth->rRecordLQSysTime + SEC_TO_SYSTIME(p_params->LQ_CHECK_EXPIRE))) {
                                continue;
                }
                if (!fgIsGetStartIndex) {
                        activeStartIndex = (prSmartBW->u4LQOldestIndex + i) % p_params->LQ_CHECK_NUM;
                        activeTotalNum = (prSmartBW->u4LQLatestIndex >= activeStartIndex) ?
                                (prSmartBW->u4LQLatestIndex - activeStartIndex + 1):
                                (p_params->LQ_CHECK_NUM - (activeStartIndex - prSmartBW->u4LQLatestIndex) + 1);

                        fgIsGetStartIndex = TRUE;
                        /* too less info can't be referenced */
                        if (activeTotalNum < p_params->LQ_MIN_ACTIVE_NEED_NUM) {
                                DBGLOG(INIT, INFO, "condition not matched: %d, activeTotalNum: %d\n", __LINE__, activeTotalNum);
                                return FALSE;
                        }
                }
                if (prLQInfoSmooth->u4CurRxRate) activeValidRxTotalNum++; /* since Rx rate maybe 0 */

                /* now condition should be matched */
                u4AVGCurTxRate += prLQInfoSmooth->u4CurTxRate;
                u4AVGCurTxPer  += prLQInfoSmooth->u4CurTxPer;
                u4AVGCurRxRate += prLQInfoSmooth->u4CurRxRate;

                /* MTK told HwMacAwakeDuration may be 0 in the very beginning, need remove it from caculate */
                if (prLQInfoSmooth->u4HwMacAwakeDuration) {
                        u8DiffIdleSlotCount += prLQInfoSmooth->u8DiffIdleSlotCount;
                        u8HwMacAwakeDuration += prLQInfoSmooth->u4HwMacAwakeDuration;
                }

                iAVGL3CurSpped += prLQInfoSmooth->iL3CSpeed;
                /* needn't get AVG iL3Congestion since L3 congestion itself is already avg */
        }

        prLQInfoSmoothStart = &(prSmartBW->rLinkQualityInfoSmooth[activeStartIndex]);
        prLQInfoSmoothEnd = &(prSmartBW->rLinkQualityInfoSmooth[prSmartBW->u4LQLatestIndex]);

        u8AVGTxTotalCount = (prLQInfoSmoothEnd->u8TxTotalCount > prLQInfoSmoothStart->u8TxTotalCount) ?
                (prLQInfoSmoothEnd->u8TxTotalCount - prLQInfoSmoothStart->u8TxTotalCount) : 0;
        u8AVGTxRetryCount = (prLQInfoSmoothEnd->u8TxRetryCount > prLQInfoSmoothStart->u8TxRetryCount) ?
                (prLQInfoSmoothEnd->u8TxRetryCount - prLQInfoSmoothStart->u8TxRetryCount) : 0;
        u8AVGTxFailCount = (prLQInfoSmoothEnd->u8TxFailCount > prLQInfoSmoothStart->u8TxFailCount) ?
                (prLQInfoSmoothEnd->u8TxFailCount - prLQInfoSmoothStart->u8TxFailCount) : 0;
        u4AVGRxDupCount = (prLQInfoSmoothEnd->u4RxDupCount > prLQInfoSmoothStart->u4RxDupCount) ?
                (prLQInfoSmoothEnd->u4RxDupCount - prLQInfoSmoothStart->u4RxDupCount) : 0;
        u8AVGRxTotalCount = (prLQInfoSmoothEnd->u8RxTotalCount > prLQInfoSmoothStart->u8RxTotalCount) ?
                (prLQInfoSmoothEnd->u8RxTotalCount - prLQInfoSmoothStart->u8RxTotalCount) : 0;

        /* use all sum, then divide total number will be more accurate than divide each run and then sum */
        /* And the pervious algorithm seems shouldn't out of bonding */
        u4AVGCurTxRate /= activeTotalNum;
        u8AVGTxTotalCount /= activeTotalNum;
        u8AVGTxRetryCount /= activeTotalNum;
        u8AVGTxFailCount /= activeTotalNum;
        u4AVGCurTxPer /= activeTotalNum;
        u4AVGCurRxRate = (activeValidRxTotalNum != 0) ? u4AVGCurRxRate/activeValidRxTotalNum : 0;
        u4AVGRxDupCount /= activeTotalNum;
        u8AVGRxTotalCount /= activeTotalNum;
        iAVGL3CurSpped /= activeTotalNum;

        if(u4GetAVGCurTxRate) *u4GetAVGCurTxRate = u4AVGCurTxRate;
        if(u8GetRetAVGTxTotalCount) *u8GetRetAVGTxTotalCount = u8AVGTxTotalCount;
        if(u8GetAVGTxRetryCount) *u8GetAVGTxRetryCount = u8AVGTxRetryCount;
        if(u8GetAVGTxFailCount) *u8GetAVGTxFailCount = u8AVGTxFailCount;
        if(u4GetAVGCurTxPer) *u4GetAVGCurTxPer = u4AVGCurTxPer;
        if(u4GetAVGCurRxRate) *u4GetAVGCurRxRate = u4AVGCurRxRate;
        if(u4GetAVGRxDupCount) *u4GetAVGRxDupCount = u4AVGRxDupCount;
        if(u8GetAVGRxTotalCount) *u8GetAVGRxTotalCount = u8AVGRxTotalCount;
        /* Just get 100% ratio, so it caculate as: (100 * (slottime * 9/1000))/scan duration */
        /* So it's (slottime * 9) / (10 * scan duration(ms) ) */
        if(u8GetAVGIdleSlotRatio && u8HwMacAwakeDuration) {
                u8AVGIdleSlotRatio = (u8DiffIdleSlotCount * 9) / (10 * u8HwMacAwakeDuration);
                *u8GetAVGIdleSlotRatio = u8AVGIdleSlotRatio;
        }
        if(u1GetActiveTotalNum) *u1GetActiveTotalNum = activeTotalNum;
        if(iGetAVGL3CurSpped) *iGetAVGL3CurSpped = iAVGL3CurSpped;

	DBGLOG(SW4, TRACE,
	       "Link Quality Active AVG: activeStartIndex = %d, activeEndIndex = %d, activeTotalNum = %d,"
	       "Tx(rate:%u, total:%lu, retry:%lu, fail:%u), Rx(rate:%u, total:%lu, dup:%u), PER(%u), Congestion(idle slot ratio:%d)\n",
               activeStartIndex,
               prSmartBW->u4LQLatestIndex,
               activeTotalNum,
	       u4AVGCurTxRate,
	       u8AVGTxTotalCount,
	       u8AVGTxRetryCount,
	       u8AVGTxFailCount,
	       u4AVGCurRxRate,
	       u8AVGRxTotalCount,
	       u4AVGRxDupCount,
	       u4AVGCurTxPer,
	       u8AVGIdleSlotRatio
	);

        return TRUE;
}

/* Check the AP IOT issue just after connect with the AP */
/* condition: total L2 Tx Rx packets <= 100 || (Tx MCS rate && Tx Per bad) */
void wlanMonitorAPIOTIssue(void)
{
        SMART_BW_PARAMS_T *p_params = &(prSmartBW->smart_bw_params);

        unsigned int u4GetAVGCurTxRate;
        unsigned long long u8GetRetAVGTxTotalCount;
        unsigned int u4GetAVGCurTxPer;
        unsigned long long u8GetAVGRxTotalCount;
        unsigned char  u1GetActiveTotalNum;

        unsigned long long u8TotalCountDif;

        // workaround code for fix MTK FW total Tx Rx count number inaccurate issue
        const struct rtnl_link_stats64 *stats;
	struct rtnl_link_stats64 temp;
        static int fgRecordTxRx = 0;
        static unsigned long long startTxPackets, startRxPackets, endTxPackets, endRxPackets;
        unsigned long long L3PacketsTxDif, L3PacketsRxDif, L3PacketsTotalDif;

        char buf[512];
        /* performance better than memset, and the latter uninit byte will be replaced by snprintf */
        buf[0] = '\0';

        /* use prAdapter->prGlueInfo->prDevHandler->stats.tx_bytes & stats.rx_bytes seems not accurate */
        if (!fgRecordTxRx) {
                //@2019/10/30 prDevHandler in dual STA mode is set to wlan0
                stats = dev_get_stats(prAdapter->prGlueInfo->prDevHandler, &temp);
                startTxPackets = stats->tx_packets;
	        startRxPackets = stats->rx_packets;
	        fgRecordTxRx = 1;
        }
        //end

        if (prSmartBW->fgIsNeedMonitorAPIOT) {
                DBGLOG(SW4, TRACE, "%s running! 2GBW: %d\n", __func__, prSmartBW->ucSelectBW);
        }
        if (!prSmartBW->fgIsNeedMonitorAPIOT ||
                !TIME_AFTER(kalGetTimeTick(), prSmartBW->rCnnSysTime + SEC_TO_SYSTIME(p_params->LQ_CHECK_EXPIRE)) ||
                !wlanGetActiveAverageLQStatus(
                &u4GetAVGCurTxRate,
                &u8GetRetAVGTxTotalCount,
                NULL,
                NULL,
                &u4GetAVGCurTxPer,
                NULL,
                NULL,
                &u8GetAVGRxTotalCount,
                NULL,
                NULL,
                &u1GetActiveTotalNum)) {
                        return;
        }

        // workaround code for fix MTK FW total Tx Rx count number inaccurate issue
        stats = dev_get_stats(prAdapter->prGlueInfo->prDevHandler, &temp);
        endTxPackets = stats->tx_packets;
        endRxPackets = stats->rx_packets;

        L3PacketsTxDif = (endTxPackets > startTxPackets) ? (endTxPackets - startTxPackets) : endTxPackets;
        L3PacketsRxDif = (endRxPackets > startRxPackets) ? (endRxPackets - startRxPackets) : endRxPackets;
        L3PacketsTotalDif = L3PacketsTxDif + L3PacketsRxDif;

	DBGLOG(SW4, TRACE,
	        "%s: dev stats: startTxPackets = %lu, startRxPackets = %lu, endTxPackets = %lu, endRxPackets = %lu, Tx Diff: %lu, Rx Diff: %lu, Tx+Rx Diff: %lu\n",
                __func__, startTxPackets, startRxPackets, endTxPackets, endRxPackets,
                L3PacketsTxDif, L3PacketsRxDif, L3PacketsTotalDif);
        // end

        /* now match the requirement to judge */
        prSmartBW->fgIsNeedMonitorAPIOT = FALSE;
        u8TotalCountDif = (u8GetRetAVGTxTotalCount + u8GetAVGRxTotalCount) * (u1GetActiveTotalNum);

        DBGLOG(SW4, INFO, "%s: AP IOT check done!, L3TotalCountDif = %lu, L2TotalCountDif = %lu, AVGCurTxRate = %u, AVGCurTxPer = %u",
                __func__, L3PacketsTotalDif, u8TotalCountDif, u4GetAVGCurTxRate, u4GetAVGCurTxPer);

        /* MTK L2 total count isn't accurate, using L3 packets number + L2 LQ status */
        if (/*u8TotalCountDif < p_params->LQ_AP_40M_IOT_L2_REACH_PACKET_COUNT || */
                L3PacketsTotalDif < p_params->LQ_AP_40M_IOT_L3_REACH_PACKET_COUNT ||
                        (u4GetAVGCurTxRate < p_params->LQ_40M_BAD_RATE && u4GetAVGCurTxPer > p_params->LQ_PER_HIGH)) {
                snprintf(buf, sizeof(buf),
		        "[SMART_BW]: [AP_IOT_TRIGGER]: 2GBW = %d, L3PacketsTotalDif = %lu, L2TotalCountDif = %lu, AVGCurTxRate = %u, AVGCurTxPer = %u",
                        prSmartBW->ucSelectBW, L3PacketsTotalDif, u8TotalCountDif, u4GetAVGCurTxRate, u4GetAVGCurTxPer);
                DBGLOG(SW4, INFO, "%s\n", buf);
                if (prSmartBW->needSaveLog) {
                        htKalWCNKeyLogWrite(buf, TYPE_LOG_CONNECT_BAND_RATIO); /* limit the DCS log */
                }

                trigger2GBWSwitch(1);
        }
}

/* L3 && L2 link status monitor */
/*
 * L3 match condition: current congestion == high || L3 AVG current speed <= LQ_L3_SPEED_LOW
 * L3 match will check L2 match condition:
 * L3 speed >= LQ_L3_SPEED_LOW/2:  Tx/Rx MCS rate + Tx PER + idle slot ratio
 * L3 speed <  LQ_L3_SPEED_LOW/2:  Tx/Rx MCS rate + idle slot ratio
 */
void wlanMonitorLQStatus(void)
{
        struct WIFI_LINK_QUALITY_INFO *prLinkQualityInfoSmooth =
                &(prSmartBW->rLinkQualityInfoSmooth[prSmartBW->u4LQLatestIndex]);
        SMART_BW_PARAMS_T *p_params = &(prSmartBW->smart_bw_params);
        BOOLEAN fgIsNeedChangeBW = FALSE;
        unsigned int u4GetAVGCurTxRate;
        unsigned int u4GetAVGCurTxPer;
        unsigned int u4GetAVGCurRxRate;
        unsigned char  u1GetActiveTotalNum;
        int iGetAVGL3CurSpped;
        unsigned char  u8GetAVGIdleSlotRatio = 0;
        BOOLEAN fgGetAVGLQStatusRet = FALSE;

        char buf[512];
        /* performance better than memset, and the latter uninit byte will be replaced by snprintf */
        buf[0] = '\0';

        /* Let AP IOT monitor timer work first, then LQ monitor */
        if (prSmartBW->fgIsNeedMonitorAPIOT) {
                return;
        }

        if (prSmartBW->smart_bw_params.SMART_BW_DBG) {
                DBGLOG(SW4, TRACE, "%s running!\n",__func__);
        }

        /* prority is first =, then && (check fgGetAVGLQStatusRet) */
        if (prSmartBW->fgIsNeedMonitorLink &&
                        (fgGetAVGLQStatusRet = wlanGetActiveAverageLQStatus(
                                &u4GetAVGCurTxRate,
                                NULL,
                                NULL,
                                NULL,
                                &u4GetAVGCurTxPer,
                                &u4GetAVGCurRxRate,
                                NULL,
                                NULL,
                                &u8GetAVGIdleSlotRatio,
                                &iGetAVGL3CurSpped,
                                &u1GetActiveTotalNum))) {
                  if (prLinkQualityInfoSmooth->iL3CongestionFlag == 2 || iGetAVGL3CurSpped <= p_params->LQ_L3_SPEED_LOW) {
                        if (iGetAVGL3CurSpped >= p_params->LQ_L3_SPEED_LOW/2) { /* using Tx PER when packet more */
                                if (((u4GetAVGCurTxRate < p_params->LQ_40M_BAD_RATE && u4GetAVGCurTxPer > p_params->LQ_PER_HIGH)
                                                || (u4GetAVGCurRxRate && u4GetAVGCurRxRate < p_params->LQ_40M_BAD_RATE))
                                        && u8GetAVGIdleSlotRatio < p_params->LQ_IDLE_SLOT_RATIO_ACCEPT) fgIsNeedChangeBW = TRUE;
                        } else {
                                /* packet less, don't try Tx PER */
                                if (((u4GetAVGCurTxRate < p_params->LQ_40M_BAD_RATE) || (u4GetAVGCurRxRate && u4GetAVGCurRxRate < p_params->LQ_40M_BAD_RATE))
                                        && u8GetAVGIdleSlotRatio < p_params->LQ_IDLE_SLOT_RATIO_ACCEPT) fgIsNeedChangeBW = TRUE;
                        }
                 }
        }

        if (fgGetAVGLQStatusRet) {
                DBGLOG(SW4, TRACE, "%s: 2GBW: %d, rssi: %d, fgIsNeedChangeBW: %d, AVGL3CurSpped = %d, L3CongestionFlag = %d, AVGIdleSlotRatio= %d,  AVGCurTxRate = %u, AVGCurTxPer = %u, AVGCurRxRate = %u\n",
                                __func__, prSmartBW->ucSelectBW, prAdapter->rLinkQuality.rLq[WLAN0_BSS_INDEX].cRssi, fgIsNeedChangeBW, iGetAVGL3CurSpped, prLinkQualityInfoSmooth->iL3CongestionFlag,
                                u8GetAVGIdleSlotRatio, u4GetAVGCurTxRate, u4GetAVGCurTxPer, u4GetAVGCurRxRate);
        }

        if (fgIsNeedChangeBW) {
                prSmartBW->fgIsNeedMonitorLink = FALSE;

                snprintf(buf, sizeof(buf),
		        "[SMART_BW]: [LQ_TRIGGER]: 2GBW: %d, rssi: %d, AVGL3CurSpped = %d, L3CongestionFlag = %d, AVGIdleSlotRatio= %d,  AVGCurTxRate = %u, AVGCurTxPer = %u, AVGCurRxRate = %u",
                        prSmartBW->ucSelectBW, prAdapter->rLinkQuality.rLq[WLAN0_BSS_INDEX].cRssi, iGetAVGL3CurSpped, prLinkQualityInfoSmooth->iL3CongestionFlag,
                        u8GetAVGIdleSlotRatio, u4GetAVGCurTxRate, u4GetAVGCurTxPer, u4GetAVGCurRxRate);
                DBGLOG(SW4, INFO, "%s\n", buf);
                if (prSmartBW->needSaveLog) {
                        htKalWCNKeyLogWrite(buf, TYPE_LOG_CONNECT_BAND_RATIO); /* limit the DCS log */
                }

                trigger2GBWSwitch(1);
        }
}

/*----------------------------------------------------------------------------*/
/*!
* @brief This routine is trigger 2.4G HT20 40 BW dynamic switch
*
* @param
* dstBW: manual set 2.4G BW 0: code original behavior, 1: force HT20, 2: force HT40
* @retval 0:       successful
*         others:  failure
*/
/*----------------------------------------------------------------------------*/
int trigger2GBWSwitch(int dstBW)
{
        if (!prSmartBW->smart_bw_params.DYNAMIC_SW_BW_FEATRUE_ENABLE) return 1; /* feature disalbe */

        prSmartBW->i24GBWManualSet = dstBW; /* align with step 2 feature */
        DBGLOG(REQ, INFO, "trigger2GBWSwitch dstBW: %d!\n", dstBW);
        if (dstBW != 1 && dstBW != 2 && dstBW != 1001) return 1; /* only support 1,2 */

        if (prSmartBW->eSmartBWSwitchState != SMART_BW_SWITCH_IDLE) {
                DBGLOG(REQ, WARN, "more than 1 bw switch at the same time, error happened???\n");
        }
        prSmartBW->eSmartBWSwitchState = SMART_BW_SWITCH_ONGOING;
        prSmartBW->u4SmartBWSwitchStart = kalGetTimeTick();
        DBGLOG(REQ, TRACE, "Start Time: %u!\n", prSmartBW->u4SmartBWSwitchStart);

        wlanResetLinkQualityMonitorStatus(); /* stop monitor timer */

        //@2019/10/29, Gen4m aisBssBeaconTimeout=>aisBssBeaconTimeout_impl will also set u2DeauthReason, set other flag
        //if (dstBW == 1) prAdapter->prAisBssInfo->u2DeauthReason = BEACON_TIMEOUT_DUE_2_2G_40M_2_20M;
        //else if (dstBW == 2) prAdapter->prAisBssInfo->u2DeauthReason = BEACON_TIMEOUT_DUE_2_2G_20M_2_40M;

        /* it will at last call aisBssBeaconTimeout, but can't call directly, */
        /* otherwise may encounter with unsync issue under stress test. Need to set flag & trigger in tx_thread */
        //@2019/10/28: Gen4m is different from Gen3, Gen4m don't have kalSetResetConnEvent
	//kalSetResetConnEvent(prAdapter->prGlueInfo);
	aisBssBeaconTimeout(prAdapter, WLAN0_BSS_INDEX);
        return 0;
}

void handleSmartBWSwitchResult(WLAN_STATUS eStatus)
{
        unsigned int u4TimeLast;
        char buf[512];
        int result; /* 0: success, 1: disconnect, 2: join failure */
        /* performance better than memset, and the latter uninit byte will be replaced by snprintf */
        buf[0] = '\0';

        //2019/11/1 this function may call in wlanProbe before prSmartBW be set
        if (!prSmartBW || prSmartBW->eSmartBWSwitchState != SMART_BW_SWITCH_ONGOING) return; /* not related with bw swith */
        switch (eStatus) {
                case WLAN_STATUS_ROAM_OUT_FIND_BEST:
                case WLAN_STATUS_MEDIA_CONNECT:
                        result = 0;
                        break;

                case WLAN_STATUS_MEDIA_DISCONNECT:
                case WLAN_STATUS_MEDIA_DISCONNECT_LOCALLY:
                        result = 1;
                        break;

                case WLAN_STATUS_JOIN_FAILURE:
                        result = 2;
                        break;
                default:
                        return;
        }

        /* must be success, disconect or join failure */
        prSmartBW->eSmartBWSwitchState = SMART_BW_SWITCH_IDLE;
        prSmartBW->u4SmartBWSwitchEnd = kalGetTimeTick();
        /* jiffies will turn every round, if End < Start, it should turn around, discard this case for easy handling */
        if (prSmartBW->u4SmartBWSwitchEnd >= prSmartBW->u4SmartBWSwitchStart) {
                u4TimeLast = prSmartBW->u4SmartBWSwitchEnd - prSmartBW->u4SmartBWSwitchStart;
        }

        snprintf(buf, sizeof(buf), "[SMART_BW]: [TRIGGER_RESULT]: time = %lu ms, result = %d", u4TimeLast, result);
        DBGLOG(SW4, INFO, "%s\n", buf);
        if (prSmartBW->needSaveLog) { /* limit the DCS log */
                htKalWCNKeyLogWrite(buf, TYPE_LOG_CONNECT_BAND_RATIO);
        }

}

/* WCN kernel layer log save function
 * log:  the log string end with '\0';
 * mode: log type, it will affect database UI view

 * alloc memory each time since this function should not be called with high frequency,
 * and if re-use pre-alloc memory, then we need to do some spin-lock to make sure data won't be conflict
 */
signed int htKalWCNKeyLogWrite(IN const char *log, IN enum WCN_KEY_LOG_MODE mode)
{
        struct kernel_packet_info *user_msg_info;
        char log_tag_scn[32] = "wifi_fool_proof";
        char event_id_scan_band_ratio[20] = "scan_band_ratio"; /* length should less than 20 */
        char event_id_scan_rssi_ratio[20] = "scan_rssi_ratio";
        char event_id_connect_band_ratio[20] = "connect_band_ratio";

        void* buffer = NULL;
        int log_len = strlen(log);
        int kevent_size;

        kevent_size = sizeof(struct kernel_packet_info) + log_len + 1;   /* extra + 1 for '\0' */
        DBGLOG(SW4, TRACE, "kevent_send_to_user, size=%d, passed in log: %s\n", kevent_size, log);

        buffer = kmalloc(kevent_size, GFP_ATOMIC);
        memset(buffer, 0, kevent_size);
        user_msg_info = (struct kernel_packet_info *)buffer;
        user_msg_info->type = 1;

        /* snprintf is better than memcpy, strncpy etc.., since snprintf will make sure the dest has '\0' in the end */
        /* snprintf behavior is different in GCC & VC, gcc the size contains the '\0', so it's more safe */
        snprintf(user_msg_info->log_tag, sizeof(user_msg_info->log_tag), "%s", log_tag_scn);
        switch (mode) {
        case TYPE_LOG_SCAN_BAND_RATIO:
                snprintf(user_msg_info->event_id, sizeof(user_msg_info->event_id), "%s", event_id_scan_band_ratio);
                break;
        case TYPE_LOG_SCAN_RSSI_RATIO:
                snprintf(user_msg_info->event_id, sizeof(user_msg_info->event_id), "%s", event_id_scan_rssi_ratio);
                break;
        case TYPE_LOG_CONNECT_BAND_RATIO:
                snprintf(user_msg_info->event_id, sizeof(user_msg_info->event_id), "%s", event_id_connect_band_ratio);
                break;
        default:
                snprintf(user_msg_info->event_id, sizeof(user_msg_info->event_id), "%s", event_id_scan_band_ratio);
                break;
        }
        snprintf(user_msg_info->payload, log_len + 1, "%s", log); /* snprintf len uses sizeof(payload) */
        user_msg_info->payload_length = log_len + 1;

        kevent_send_to_user(user_msg_info);
        kfree(buffer);

        return 0;
}
#endif /* OPLUS_FEATURE_WIFI_SMART_BW*/
