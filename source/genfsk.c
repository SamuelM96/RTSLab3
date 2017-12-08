#include "MemManager.h"
#include "RNG_Interface.h"
#include "TimersManager.h"
#include "genfsk_interface.h"
#include "xcvr_test_fsk.h"
#include "SerialManager.h"
#include "LED.h"

#include "genfsk.h"
#include "genfsk_states.h"
#include "genfsk_defs.h"

#include "ppp-webserver.h"

/*! *********************************************************************************
* Public memory declarations
********************************************************************************** */
/*serial interface id*/
uint8_t mAppSerId;
/*timers manager app timer id*/
uint8_t mAppTmrId;
/*GENFSK instance id*/
uint8_t mAppGenfskId;
/*configuration params*/
ct_config_params_t gaConfigParams[5];

/*! *********************************************************************************
* Private macros
********************************************************************************** */
#define gRadioOpcode1 (0xBE)
#define gRadioOpcode2 (0xEF)

/************************************************************************************
* Private memory declarations
************************************************************************************/
/* buffers for interaction with Generic FSK */
static uint8_t* gRxBuffer;
static uint8_t* gTxBuffer;

/* Generic FSK packets to get formatted data*/
static GENFSK_packet_t gRxPacket;
static GENFSK_packet_t gTxPacket;

/*hook to notify app thread*/
static pHookAppNotification pNotifyAppThread = NULL;
/*hook to notify app thread*/
static pTmrHookNotification pTmrCallback = NULL;

/*packet configuration*/
static GENFSK_packet_config_t pktConfig = 
{
    .preambleSizeBytes = 0, /*1 byte of preamble*/
    .packetType = gGenfskFormattedPacket,
    .lengthSizeBits = gGenFskDefaultLengthFieldSize_c,
    .lengthBitOrder = gGenfskLengthBitLsbFirst,
    .syncAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .lengthAdjBytes = 3, /*length field not including CRC so adjust by crc len*/
    .h0SizeBits = gGenFskDefaultH0FieldSize_c,
    .h1SizeBits = gGenFskDefaultH1FieldSize_c,
    .h0Match = gGenFskDefaultH0Value_c, /*match field containing zeros*/
    .h0Mask = gGenFskDefaultH0Mask_c,
    .h1Match = gGenFskDefaultH1Value_c,
    .h1Mask = gGenFskDefaultH1Mask_c
};

/*CRC configuration*/
static GENFSK_crc_config_t crcConfig =
{
    .crcEnable = gGenfskCrcEnable,
    .crcSize = 3,
    .crcStartByte = 4,
    .crcRefIn = gGenfskCrcInputNoRef,
    .crcRefOut = gGenfskCrcOutputNoRef,
    .crcByteOrder = gGenfskCrcLSByteFirst,
    .crcSeed = 0x00555555,
    .crcPoly = 0x0000065B,
    .crcXorOut = 0
};

/*whitener configuration*/
static GENFSK_whitener_config_t whitenConfig = 
{
    .whitenEnable = gGenfskWhitenEnable,
    .whitenStart = gWhitenStartWhiteningAtH0,
    .whitenEnd = gWhitenEndAtEndOfCrc,
    .whitenB4Crc = gCrcB4Whiten,
    .whitenPolyType = gGaloisPolyType,
    .whitenRefIn = gGenfskWhitenInputNoRef,
    .whitenPayloadReinit = gGenfskWhitenNoPayloadReinit,
    .whitenSize = 7,
    .whitenInit = 0x53,
    .whitenPoly = 0x11, /*x^7 + x^4 + 1! x^7 is never set*/
    .whitenSizeThr = 0,
    .manchesterEn = gGenfskManchesterDisable,
    .manchesterStart = gGenfskManchesterStartAtPayload,
    .manchesterInv = gGenfskManchesterNoInv,
};

/*radio configuration*/
static GENFSK_radio_config_t radioConfig = 
{
    .radioMode = gGenfskGfskBt0p5h0p5,
    .dataRate = gGenfskDR1Mbps
};

/*bit processing configuration*/

/*network / sync address configuration*/
static GENFSK_nwk_addr_match_t ntwkAddr = 
{
    .nwkAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .nwkAddrThrBits = 0,
    .nwkAddr = gGenFskDefaultSyncAddress_c,
}; 

// Current send/receive state: U for uninitialised
static char radioState = 'U';

/**********************************************************************************/
void GenFskInit(pHookAppNotification pFunc, pTmrHookNotification pTmrFunc)
{
    /*configure hook*/
    pNotifyAppThread = pFunc;
    
    /*configure timer callback*/
    pTmrCallback = pTmrFunc;
    
    /* populate shortcut array */
    gaConfigParams[0].paramType = gParamTypeString_c;
    FLib_MemCpy(gaConfigParams[0].paramName, "Mode", 5);
    FLib_MemCpy(gaConfigParams[0].paramValue.stringValue, "RX", 3);
    if(gDefaultMode_c == gModeTx_c)
    {
        gaConfigParams[0].paramValue.stringValue[0] = 'T';
    }
    gaConfigParams[1].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[1].paramName, "Channel", 8);
    gaConfigParams[1].paramValue.decValue = gGenFskDefaultChannel_c;
    
    gaConfigParams[2].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[2].paramName, "Power", 6);
    gaConfigParams[2].paramValue.decValue = gGenFskDefaultTxPowerLevel_c;
    
    gaConfigParams[3].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[3].paramName, "Payload", 8);
    gaConfigParams[3].paramValue.decValue = gGenFskDefaultPayloadLen_c;
    
    gaConfigParams[4].paramType = gParamTypeMaxType_c;
    /* allocate once to use for the entire application */
    gRxBuffer  = MEM_BufferAlloc(gGenFskDefaultMaxBufferSize_c + 
                                 crcConfig.crcSize);
    gTxBuffer  = MEM_BufferAlloc(gGenFskDefaultMaxBufferSize_c);
    
    gRxPacket.payload = (uint8_t*)MEM_BufferAlloc(gGenFskMaxPayloadLen_c  + 
                                                       crcConfig.crcSize);
    gTxPacket.payload = (uint8_t*)MEM_BufferAlloc(gGenFskMaxPayloadLen_c);
    
    /*prepare the part of the tx packet that is common for all tests*/
    gTxPacket.addr = gGenFskDefaultSyncAddress_c;
    gTxPacket.header.h0Field = gGenFskDefaultH0Value_c;
    gTxPacket.header.h1Field = gGenFskDefaultH1Value_c;
    
    /*set bitrate*/
    GENFSK_RadioConfig(mAppGenfskId, &radioConfig);
    /*set packet config*/
    GENFSK_SetPacketConfig(mAppGenfskId, &pktConfig);
    /*set whitener config*/
    GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
    /*set crc config*/
    GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);
    
    /*set network address at location 0 and enable it*/
    GENFSK_SetNetworkAddress(mAppGenfskId, 0, &ntwkAddr);
    GENFSK_EnableNetworkAddress(mAppGenfskId, 0);
    
    /*set tx power level*/
    GENFSK_SetTxPowerLevel(mAppGenfskId, gGenFskDefaultTxPowerLevel_c);
    /*set channel: Freq = 2360MHz + ChannNumber*1MHz*/
    GENFSK_SetChannelNumber(mAppGenfskId, gGenFskDefaultChannel_c);
}

/*! *********************************************************************************
* \brief  Handles the Packet error rate RX test
********************************************************************************** */
bool_t Genfsk_Receive(ct_event_t evType, void* pAssociatedValue)
{
    static uint8_t message;
    static uint8_t u8PacketIndex;
    
    ct_rx_indication_t* pIndicationInfo = NULL;
    uint8_t* pRxBuffer = NULL;
    bool_t bRestartRx = FALSE;
    bool_t bReturnFromSM = FALSE;
    
    if(radioState != 'R') /* Reset the state machine */
    {
        u8PacketIndex = 0;
        message = 0;

        Serial_Print(mAppSerId, "\n\rRADIO Rx Running\r\n\r\n", gAllowToBlock_d);

        if(gGenfskSuccess_c != GENFSK_StartRx(mAppGenfskId, gRxBuffer, gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize, 0, 0)) {
        	GENFSK_AbortAll();
            Serial_Print(mAppSerId, "\n\rRADIO Rx failed.\r\n\r\n", gAllowToBlock_d);
        }

        radioState = 'R';
    }

    /*check if RX related events are fired */
    if(gCtEvtRxDone_c == evType || gCtEvtRxFailed_c == evType || gCtEvtSeqTimeout_c == evType) {
    	/*if rx successful, get packet information */
    	if (gCtEvtRxDone_c == evType) {
                pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
                pRxBuffer = pIndicationInfo->pBuffer; /*same as gRxBuffer*/
                
                /*map rx buffer to generic fsk packet*/
                GENFSK_ByteArrayToPacket(mAppGenfskId, pRxBuffer, &gRxPacket);
                MeshProtocol *payload = (MeshProtocol*)gRxPacket.payload;
                if(payload->opcode1 == gRadioOpcode1 && payload->opcode2 == gRadioOpcode2) /* check if packet payload is RADIO type */
                {

                    u8PacketIndex = payload->packetID;
                    message = payload->message;
                    
                    if (message == MESHMESSAGE_ACK) {
                    	if (addNode(payload->nodeID) == -1) {
                    		// Code here
                    	}
                    }
                    
                    bRestartRx = TRUE;
                } 
                else
                {
                    bRestartRx = TRUE;
                }
            }
            else
            {
                bRestartRx = TRUE;
            }

    	/*restart RX immediately with no timeout*/
            if(bRestartRx) {
                if(gGenfskSuccess_c != GENFSK_StartRx(mAppGenfskId, gRxBuffer, gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize, 0, 0)) {
                    GENFSK_AbortAll();
                    Serial_Print(mAppSerId, "\n\rRADIO Rx failed.\r\n\r\n", gAllowToBlock_d);
                }
            }  
        }

    return bReturnFromSM;      
}

/*! *********************************************************************************
* \brief  Handles the Packet error rate TX test
********************************************************************************** */
bool_t Genfsk_Send(ct_event_t evType, uint8_t nodeID, uint8_t message)
{
	// static bool_t initialised = false;
    static ct_radio_tx_states_t radioTxState = gRadioTxStateInit_c;
    static uint32_t microSecDelay;

    static uint8_t u8PacketIndex = 0;
    
    uint16_t buffLen = 0;
    bool_t bReturnFromSM = FALSE;

	radioTxState = gRadioTxStateInit_c;
	microSecDelay = 10000;

	u8PacketIndex++;

	gTxPacket.header.lengthField = (uint16_t)gaConfigParams[3].paramValue.decValue;

	MeshProtocol payload = {
			nodeID,
			5,
			u8PacketIndex,
			message,
			gRadioOpcode1,
			gRadioOpcode2
	};

	gTxPacket.payload = (uint8_t*)&payload;

	/*pack everything into a buffer*/
	GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
	/*calculate buffer length*/
	buffLen = gTxPacket.header.lengthField+
				(gGenFskDefaultHeaderSizeBytes_c)+
					(gGenFskDefaultSyncAddrSize_c + 1);

	/*start tx at current time + input delay*/
	if(gGenfskSuccess_c != GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, 0)) {
		GENFSK_AbortAll();
		Serial_Print(mAppSerId, "\r\n\r\nRadio TX failed.\r\n\r\n", gAllowToBlock_d);
		radioTxState = gRadioTxStateIdle_c;
	}

	Serial_Print(mAppSerId, "\r\n Running RADIO Tx, Number of packets: ", gAllowToBlock_d);
	Serial_PrintDec(mAppSerId, (uint32_t)u8PacketIndex);
	Serial_Print(mAppSerId, ", Message: ", gAllowToBlock_d);
	Serial_PrintDec(mAppSerId, (uint32_t)message);
	Serial_Print(mAppSerId, ", NodeID: ", gAllowToBlock_d);
	Serial_PrintDec(mAppSerId, (uint32_t)nodeID);

	radioTxState = gRadioTxStateRunning_c;
	radioState = 'T';
    
    return bReturnFromSM;      
}

