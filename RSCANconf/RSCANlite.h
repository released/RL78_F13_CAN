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
* File Name    : RSCANlite.h
* Version      : 1.0 
* Description  : This is include file for CAN configuration.
******************************************************************************/ 
/***************************************************************************** 
* History      : DD.MM.YYYY Version Description 
*              : 03.29.2013 1.00     First Release 
******************************************************************************/
#ifndef RSCANLITE_H
#define RSCANLITE_H

#define CAN_ENABLE                  1U
#define CAN_DISABLE                 0U

/* ---- Global setting ---- */
#define CAN_CFG_TX_PRIORITY         CAN_TX_ID_FIRST
#define CAN_CFG_DLC_CHECK           CAN_DISABLE
#define CAN_CFG_DLC_REPLACE         CAN_DISABLE
#define CAN_CFG_MIRROR              CAN_DISABLE
#define CAN_CFG_CLOCK               CAN_SOURCE_PCLOCK_2
#define CAN_CFG_TS_CLOCK            CAN_TS_PCLOCK_2
#define CAN_CFG_TS_PR               CAN_TS_NO_DIV
#define CAN_CFG_IT_PR               0U

#define CAN_CFG_OVERFLOW_IE         CAN_DISABLE
#define CAN_CFG_MSG_LOST_IE         CAN_DISABLE
#define CAN_CFG_DLC_ERROR_IE        CAN_DISABLE

/* ---- Rx rule ---- */
#define CAN_RX_RULE_NUM_CH0         (1U)
#define CAN_RX_RULE_NUM             CAN_RX_RULE_NUM_CH0

/* ---- Rx buffer ---- */
#define CAN_CFG_RBNUM               (4U)

/* ---- RxFIFO ---- */
#define CAN_CFG_RXFIFO0 \
    CAN_CFG_RXFIFO(CAN_FIFO_DEPTH_4, CAN_ENABLE, \
                   CAN_FIFO_INT_THRESHOLD, CAN_FIFO_THRESHOLD_2_8)

#define CAN_CFG_RXFIFO1 \
    0x0000U

/* ---- Register value ---- */
#define CAN_CFG_GLB_CFGL \
    CAN_CFG_GCFGL(CAN_CFG_TX_PRIORITY, \
                  CAN_CFG_DLC_CHECK, CAN_CFG_DLC_REPLACE, \
                  CAN_CFG_MIRROR, CAN_CFG_CLOCK, \
                  CAN_CFG_TS_CLOCK, CAN_CFG_TS_PR)

#define CAN_CFG_GLB_CFGH \
    CAN_CFG_GCFGH(CAN_CFG_IT_PR)

#define CAN_CFG_GLB_ERR_INT \
    CAN_CFG_GCTRL(CAN_CFG_OVERFLOW_IE, \
                  CAN_CFG_MSG_LOST_IE, CAN_CFG_DLC_ERROR_IE)

#define CAN_CFG_RN0  (CAN_RX_RULE_NUM_CH0)

/* ---- Channel 0 setting ---- */
#define CAN_CFG_C0_FUNC_L \
    CAN_CFG_FUNCL(CAN_DISABLE, CAN_DISABLE, CAN_DISABLE, CAN_DISABLE, \
                  CAN_DISABLE, CAN_DISABLE, CAN_DISABLE, CAN_DISABLE)

#define CAN_CFG_C0_FUNC_H \
    CAN_CFG_FUNCH(CAN_DISABLE, CAN_BOFF_RECOVERY_ISO11898, CAN_ERRDISP_FIRST_ERR_ONLY)

#define CAN_CFG_C0_BAUDRATE_L \
    CAN_CFG_BAUDRATEL(3U)

#define CAN_CFG_C0_BAUDRATE_H \
    CAN_CFG_BAUDRATEH(CAN_TSEG1_11TQ, CAN_TSEG2_4TQ, CAN_SJW_4TQ)

#define CAN_CFG_C0_TRFIFO0_L \
    CAN_CFG_TRFIFO_TXL(CAN_FIFO_DEPTH_4, CAN_ENABLE, \
                       CAN_FIFO_INT_EACH_MSG)

#define CAN_CFG_C0_TRFIFO0_H \
    CAN_CFG_TRFIFO_TXH(CAN_TX_BUFFER_3, 0, 0)

#define CAN_CFG_C0_TXBUF_IE  (0x0007U)

#endif
