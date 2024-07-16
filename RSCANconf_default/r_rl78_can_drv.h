/******************************************************************************* 
* DISCLAIMER 
* This software is supplied by Renesas Electronics Corporation and is only  
* intended for use with Renesas products. No other uses are authorized. This  
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS  
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software. By using this software, 
* you agree to the additional terms and conditions found by accessing the  
* following link: 
* http://www.renesas.com/disclaimer 
* 
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.     
*******************************************************************************/
/******************************************************************************* 
* File Name    : r_rl78_can_drv.h 
* Version      : 1.0 
* Description  : This is header file for CAN driver code.
******************************************************************************/ 
/***************************************************************************** 
* History      : DD.MM.YYYY Version Description 
*              : 29.03.2013 1.00     First Release 
******************************************************************************/
#ifndef R_RL78_CAN_DRV_H
#define R_RL78_CAN_DRV_H

typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef signed short        int16_t;
typedef unsigned short      uint16_t;

typedef unsigned int  Can_RtnType;

typedef uint8_t can_ch_t;
typedef uint8_t can_rxfifo_t;
typedef uint8_t can_trfifo_t;
typedef uint8_t can_txbuf_t;

/* ---- Channel ---- */
#define CAN_CH0             0U

/* ---- Rx FIFO ---- */
#define CAN_RXFIFO0         0U
#define CAN_RXFIFO1         1U
#define CAN_MAX_RXFIFO_NUM  2U

/* ---- Common (Tx/Rx) FIFO ---- */
#define CAN_TRFIFO0         0U
#define CAN_MAX_TRFIFO_NUM  1U

/* ---- Tx buffers ---- */
#define CAN_TXBUF0          0U
#define CAN_TXBUF1          1U
#define CAN_TXBUF2          2U
#define CAN_TXBUF3          3U
#define CAN_MAX_TXBUF_NUM   4U

/* ---- CAN frame ----- */
typedef struct
{
    uint16_t IDL:16;   /* ID Data (low)                      */
    uint16_t IDH:13;   /* ID Data (high)                     */
    uint16_t THDSE :1; /* Transmit History Data Store Enable */
    uint16_t RTR :1;   /* RTR  0:Data 1:Remote               */
    uint16_t IDE :1;   /* IDE  0:Standard 1:Extend           */
    uint16_t TS :16;   /* Timestamp Data                     */
    uint16_t LBL :12;  /* Label Data                         */
    uint16_t DLC :4;   /* DLC Data                           */
    uint8_t  DB[8];    /* Data Byte                          */
} can_frame_t;

extern const uint8_t       g_rxfifo0_use_mode;
extern const uint8_t       g_rxfifo1_use_mode;
extern const uint8_t       g_trfifo_use_mode;
extern const uint8_t  g_fifo_lost_isr;
extern const uint16_t g_rxrule_table[][6];

/* ---- Configuration ---- */
#define CAN_NOUSE                           0x00U
#define CAN_USE                             0x80U
#define CAN_USE_RX_MODE                     0x81U
#define CAN_USE_TX_MODE                     0x82U
#define CAN_USE_CH_MASK                     0x07U

/* ---- function return value ---- */
#define CAN_RTN_OK                          0U
#define CAN_RTN_OK_WITH_LOST                1U
#define CAN_RTN_FIFO_FULL                   2U
#define CAN_RTN_BUFFER_EMPTY                3U
#define CAN_RTN_MODE_WAIT                   4U
#define CAN_RTN_RAM_INIT                    5U

#define CAN_RTN_TRANSMITTING                0U
#define CAN_RTN_TX_ABORT_OVER               1U
#define CAN_RTN_TX_END                      2U
#define CAN_RTN_TX_END_WITH_ABORT_REQ       3U

#define CAN_RTN_ERROR                       (0x8000U)
#define CAN_RTN_ARG_ERROR                   (CAN_RTN_ERROR + 0x7FFF)
#define CAN_RTN_STS_ERROR                   (CAN_RTN_ERROR + 0x7FFE)
#define CAN_RTN_OVERWRITE                   (CAN_RTN_ERROR + 0x7FFD)
#define CAN_RTN_MODE_ERROR                  (CAN_RTN_ERROR + 0x7FFC)

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
extern Can_RtnType R_CAN_Init(void);
extern Can_RtnType R_CAN_GlobalStart(void);
extern Can_RtnType R_CAN_ChStart_CH0(void);
extern Can_RtnType R_CAN_TrmByTxBuf_CH0(can_txbuf_t, const can_frame_t *);
extern Can_RtnType R_CAN_AbortTrm_CH0(can_txbuf_t);
extern Can_RtnType R_CAN_CheckTxBufResult_CH0(can_txbuf_t);
extern Can_RtnType R_CAN_TrmByTRFIFO0_CH0(const can_frame_t *);
extern Can_RtnType R_CAN_ReadRxBuffer(uint8_t *, can_frame_t *);
extern Can_RtnType R_CAN_ReadRxFIFO(can_rxfifo_t, can_frame_t *);
extern Can_RtnType R_CAN_ReadTRFIFO0_CH0(can_frame_t *);
extern Can_RtnType R_CAN_ReadChStatus_CH0(void);

#define R_CAN_ChStart(val1)                 (R_CAN_ChStart_CH0())
#define R_CAN_TrmByTxBuf(val1, val2, val3)  (R_CAN_TrmByTxBuf_CH0(val2, val3))
#define R_CAN_AbortTrm(val1, val2)          (R_CAN_AbortTrm_CH0(val2))
#define R_CAN_CheckTxBufResult(val1, val2)  (R_CAN_CheckTxBufResult_CH0(val2))
#define R_CAN_TrmByTRFIFO(val1, val2, val3) (R_CAN_TrmByTRFIFO_CH0(val3))
#define R_CAN_ReadTRFIFO(val1, val2, val3)  (R_CAN_ReadTRFIFO0_CH0(val3))
#define R_CAN_ReadChStatus(val1)            (R_CAN_ReadChStatus_CH0())

#endif /* R_RL78_CAN_DRV_H */
