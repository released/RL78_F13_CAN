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
* File Name    : r_rl78_can_sfr.h 
* Version      : 1.0 
* Description  : This is include file for CAN I/O registers.
******************************************************************************/ 
/***************************************************************************** 
* History      : DD.MM.YYYY Version Description 
*              : 29.03.2013 1.00     First Release 
******************************************************************************/
#ifndef R_RL78_CAN_SFR_H
#define R_RL78_CAN_SFR_H

#define BIT_ON(x)     (uint16_t)(1 << (x))
#define BITS_2_ON(x)  (uint16_t)(3 << (x))

/*
 * Address
 */

/* ---- Channel ---- */
typedef struct {
    volatile uint16_t CnCFGL;
    volatile uint16_t CnCFGH;
    volatile uint16_t CnCTRL;
    volatile uint16_t CnCTRH;
    volatile uint16_t CnSTSL;
    volatile uint16_t CnSTSH;
    volatile uint16_t CnERFLL;
    volatile uint16_t CnERFLH;
} can_ch_top_sfr_t;

typedef struct {
    volatile uint16_t IDL;
    volatile uint16_t IDH;
    volatile uint16_t TS;
    volatile uint16_t PTR;
    volatile uint16_t DF0;
    volatile uint16_t DF1;
    volatile uint16_t DF2;
    volatile uint16_t DF3;
} can_frame_sfr_t;

typedef struct {
    volatile uint16_t IDL;
    volatile uint16_t IDH;
    volatile uint16_t ML;
    volatile uint16_t MH;
    volatile uint16_t PL;
    volatile uint16_t PH;
} can_rxrule_sfr_t;

/* ---- Tx buffers ---- */
#define TMCp(txbuf)                 (*((volatile __near uint8_t *)((uint16_t)&TMC0 + txbuf)))
#define CAN_ADDR_TMSTSp(txbuf)      (volatile __near uint8_t *)((uint16_t)&TMSTS0 + txbuf)

/* ---- RAM ---- */
#define CAN_ADDR_RMIDLp(buf)        (volatile __near can_frame_sfr_t *)((uint16_t)&RMIDL0 + (0x10 * (buf)))
#define CAN_ADDR_TMIDLp(txbuf)      (volatile __near can_frame_sfr_t *)((uint16_t)&TMIDL0 + (0x10 * (txbuf)))


/* ==== CAN SFR register bit field position ==== */
/* ---- C0CFGL ---- */
#define CAN_BRP_BIT_POS                      0U
#define CAN_CFG_BAUDRATEL(brp)    ((brp)   << CAN_BRP_BIT_POS)

/* ---- C0CFGH ---- */
#define CAN_SJW_BIT_POS                      8U
#define CAN_TSEG2_BIT_POS                    4U
#define CAN_TSEG1_BIT_POS                    0U
#define CAN_CFG_BAUDRATEH(tseg1, tseg2, sjw)        \
    ( ((tseg1) << CAN_TSEG1_BIT_POS)                \
    + ((tseg2) << CAN_TSEG2_BIT_POS)                \
    + ((sjw)   << CAN_SJW_BIT_POS) )

/* ---- C0CTRL ---- */
#define CAN_ALIE_BIT_POS                    15U
#define CAN_BLIE_BIT_POS                    14U
#define CAN_OLIE_BIT_POS                    13U
#define CAN_BORIE_BIT_POS                   12U
#define CAN_BOEIE_BIT_POS                   11U
#define CAN_EPIE_BIT_POS                    10U
#define CAN_EWIE_BIT_POS                     9U
#define CAN_BEIE_BIT_POS                     8U
#define CAN_CSLPR_BIT_POS                    2U
#define CAN_CHMDC_BIT_POS                    0U

#define CAN_CFG_FUNCL(ie_bus, ie_ew, ie_ep, ie_boe, \
                     ie_bor, ie_ol, ie_bl, ie_al)   \
    ( ((ie_bus)    << CAN_BEIE_BIT_POS)             \
    + ((ie_ew)     << CAN_EWIE_BIT_POS)             \
    + ((ie_ep)     << CAN_EPIE_BIT_POS)             \
    + ((ie_boe)    << CAN_BOEIE_BIT_POS)            \
    + ((ie_bor)    << CAN_BORIE_BIT_POS)            \
    + ((ie_ol)     << CAN_OLIE_BIT_POS)             \
    + ((ie_bl)     << CAN_BLIE_BIT_POS)             \
    + ((ie_al)     << CAN_ALIE_BIT_POS) )

#define CAN_STP_BIT_ON                      BIT_ON(CAN_CSLPR_BIT_POS)
#define CAN_MODE_BITS_ON                    BITS_2_ON(CAN_CHMDC_BIT_POS)

/* ---- C0CTRH ---- */
#define CAN_ERRD_BIT_POS                     7U
#define CAN_BOM_BIT_POS                      5U
#define CAN_TAIE_BIT_POS                     0U

#define CAN_CFG_FUNCH(ie_ta, boff_mode, er_disp)    \
    ( ((ie_ta)     << CAN_TAIE_BIT_POS)             \
    + ((boff_mode) << CAN_BOM_BIT_POS)              \
    + ((er_disp)   << CAN_ERRD_BIT_POS) )

/* ---- C0STSL ---- */
#define CAN_CSLPSTS_BIT_POS                 2U
#define CAN_CRSTSTS_BIT_POS                 0U

#define CAN_STP_STS_BIT_ON                  BIT_ON(CAN_CSLPSTS_BIT_POS)
#define CAN_RST_STS_BIT_ON                  BIT_ON(CAN_CRSTSTS_BIT_POS)

/* ---- GCFGL ---- */
#define CAN_TSSS_BIT_POS                    12U
#define CAN_TSP_BIT_POS                      8U
#define CAN_DCS_BIT_POS                      4U
#define CAN_MME_BIT_POS                      3U
#define CAN_DRE_BIT_POS                      2U
#define CAN_DCE_BIT_POS                      1U
#define CAN_TPRI_BIT_POS                     0U

#define CAN_CFG_GCFGL(priority, dlc_check, dlc_replace, \
                     mirror, can_clock, ts_clock,       \
                     ts_pr)                             \
    ( ((priority)    << CAN_TPRI_BIT_POS)               \
    + ((dlc_check)   << CAN_DCE_BIT_POS)                \
    + ((dlc_replace) << CAN_DRE_BIT_POS)                \
    + ((mirror)      << CAN_MME_BIT_POS)                \
    + ((can_clock)   << CAN_DCS_BIT_POS)                \
    + ((ts_pr)       << CAN_TSP_BIT_POS)                \
    + ((ts_clock)    << CAN_TSSS_BIT_POS) )

/* ---- GCFGH ---- */
#define CAN_ITRCP_BIT_POS                    0U

#define CAN_CFG_GCFGH(it_pr)                           \
    ( (it_pr)       << CAN_ITRCP_BIT_POS )

/* ---- GCTRL ---- */
#define CAN_THLEIE_BIT_POS                  10U
#define CAN_MEIE_BIT_POS                     9U
#define CAN_DEIE_BIT_POS                     8U
#define CAN_GSLPR_BIT_POS                    2U
#define CAN_GMDC_BIT_POS                     0U

#define CAN_CFG_GCTRL(overflow_ie, msg_lost_ie, dlc_error_ie) \
    ( ((overflow_ie)  << CAN_THLEIE_BIT_POS)                  \
    + ((msg_lost_ie)  << CAN_MEIE_BIT_POS)                    \
    + ((dlc_error_ie) << CAN_DEIE_BIT_POS))

#define CAN_GLB_STP_BIT_ON                  BIT_ON(CAN_GSLPR_BIT_POS)
#define CAN_GLB_MODE_BITS_ON                BITS_2_ON(CAN_GMDC_BIT_POS)

/* ---- GSTS ---- */
#define CAN_GRAMINIT_BIT_POS                3U
#define CAN_GSLPSTS_BIT_POS                 2U
#define CAN_GRSTSTS_BIT_POS                 0U

#define CAN_RAM_INIT_BIT_ON                 BIT_ON(CAN_GRAMINIT_BIT_POS)
#define CAN_GLB_STP_STS_BIT_ON              BIT_ON(CAN_GSLPSTS_BIT_POS)
#define CAN_GLB_RST_STS_BIT_ON              BIT_ON(CAN_GRSTSTS_BIT_POS)

/* ---- GRWCR ---- */
#define CAN_RPAGE_BIT_POS                   0U

#define CAN_RAM_WINDOW_BIT_ON               BIT_ON(CAN_RPAGE_BIT_POS)



/* ---- RFCCm ---- */
#define CAN_RFIGCV_BIT_POS                  13U
#define CAN_RFIM_BIT_POS                    12U
#define CAN_RFDC_BIT_POS                     8U
#define CAN_RFIE_BIT_POS                     1U
#define CAN_RFE_BIT_POS                      0U

#define CAN_CFG_RXFIFO(dc, i_enable, i_factor, i_timing)    \
    ( ((i_timing) << CAN_RFIGCV_BIT_POS)                    \
    + ((i_factor) << CAN_RFIM_BIT_POS)                      \
    + ((i_enable) << CAN_RFIE_BIT_POS)                      \
    + ((dc)       << CAN_RFDC_BIT_POS) )

#define CAN_RFIFO_EN_BIT_ON                 BIT_ON(CAN_RFE_BIT_POS)

/* ---- RFSTSm ---- */
#define CAN_RFIF_BIT_POS                    3U
#define CAN_RFMLT_BIT_POS                   2U
#define CAN_RFEMP_BIT_POS                   0U

#define CAN_RFIFO_MSGLST_BIT_ON             BIT_ON(CAN_RFMLT_BIT_POS)
#define CAN_RFIFO_EMPTY_BIT_ON              BIT_ON(CAN_RFEMP_BIT_POS)

#define CAN_CLR_WITHOUT_RX_INT              BIT_ON(CAN_RFIF_BIT_POS)

/* ---- CFCCLk ---- */
#define CAN_CFIGCV_BIT_POS                  13U
#define CAN_CFIM_BIT_POS                    12U
#define CAN_CFDC_BIT_POS                     8U
#define CAN_CFTXIE_BIT_POS                   2U
#define CAN_CFRXIE_BIT_POS                   1U
#define CAN_CFE_BIT_POS                      0U

#define CAN_CFG_TRFIFO_TXL(dc, i_enable, i_factor)              \
    ( ((i_factor)           << CAN_CFIM_BIT_POS)                \
    + ((dc)                 << CAN_CFDC_BIT_POS)                \
    + ((i_enable)           << CAN_CFTXIE_BIT_POS) )

#define CAN_CFG_TRFIFO_RXL(dc, i_enable, i_factor, i_timing)    \
    ( ((i_timing)           << CAN_CFIGCV_BIT_POS)              \
    + ((i_factor)           << CAN_CFIM_BIT_POS)                \
    + ((dc)                 << CAN_CFDC_BIT_POS)                \
    + ((i_enable)           << CAN_CFRXIE_BIT_POS) )


#define CAN_TRFIFO_EN_BIT_ON                BIT_ON(CAN_CFE_BIT_POS)

/* ---- CFCCHk ---- */
#define CAN_CFITT_BIT_POS                    8U
#define CAN_CFTML_BIT_POS                    4U
#define CAN_CFITR_BIT_POS                    3U
#define CAN_CFITSS_BIT_POS                   2U
#define CAN_CFM_BIT_POS                      0U

#define CAN_CFG_TRFIFO_TXH(txbuf_idx, timer, time)              \
    ( ((time)               << CAN_CFITT_BIT_POS)               \
    + ((txbuf_idx)          << CAN_CFTML_BIT_POS)               \
    + ((timer)              << CAN_CFITSS_BIT_POS)              \
    + ((CAN_TRFIFO_TX_MODE) << CAN_CFM_BIT_POS) )

#define CAN_CFG_TRFIFO_RXH     ((CAN_TRFIFO_RX_MODE) << CAN_CFM_BIT_POS)

/* ---- CFSTSk ---- */
#define CAN_CFTXIF_BIT_POS                  4U
#define CAN_CFRXIF_BIT_POS                  3U
#define CAN_CFMLT_BIT_POS                   2U
#define CAN_CFFLL_BIT_POS                   1U
#define CAN_CFEMP_BIT_POS                   0U

#define CAN_TRFIFO_MSGLST_BIT_ON            BIT_ON(CAN_CFMLT_BIT_POS)
#define CAN_TRFIFO_FULL_BIT_ON              BIT_ON(CAN_CFFLL_BIT_POS)
#define CAN_TRFIFO_EMPTY_BIT_ON             BIT_ON(CAN_CFEMP_BIT_POS)

#define CAN_CLR_WITHOUT_TX_RX_INT \
    (BIT_ON(CAN_CFTXIF_BIT_POS) | BIT_ON(CAN_CFRXIF_BIT_POS))




/* ---- TMCp ---- */
#define CAN_TMTAR_BIT_ON                    1U
#define CAN_TMTR_BIT_ON                     0U

#define CAN_TXBUF_ABT_BIT_ON                BIT_ON(CAN_TMTAR_BIT_ON)
#define CAN_TXBUF_TRM_BIT_ON                BIT_ON(CAN_TMTR_BIT_ON)

/* ---- TMSTSp ---- */
#define CAN_TMTRF_BIT_POS                   1U

#define CAN_TXBUF_RSLT_BITS_POS             CAN_TMTRF_BIT_POS
#define CAN_TXBUF_RSLT_BITS_ON              BITS_2_ON(CAN_TXBUF_RSLT_BITS_POS)




/* ==== CAN SFR register value ==== */

/* ---- Resynchronization jump width ---- */
#define CAN_SJW_1TQ                      0U
#define CAN_SJW_2TQ                      1U
#define CAN_SJW_3TQ                      2U
#define CAN_SJW_4TQ                      3U

/* ---- Time segment 2 ---- */
#define CAN_TSEG2_2TQ                    1U
#define CAN_TSEG2_3TQ                    2U
#define CAN_TSEG2_4TQ                    3U
#define CAN_TSEG2_5TQ                    4U
#define CAN_TSEG2_6TQ                    5U
#define CAN_TSEG2_7TQ                    6U
#define CAN_TSEG2_8TQ                    7U

/* ---- Time segment 1 ---- */
#define CAN_TSEG1_4TQ                    3U
#define CAN_TSEG1_5TQ                    4U
#define CAN_TSEG1_6TQ                    5U
#define CAN_TSEG1_7TQ                    6U
#define CAN_TSEG1_8TQ                    7U
#define CAN_TSEG1_9TQ                    8U
#define CAN_TSEG1_10TQ                   9U
#define CAN_TSEG1_11TQ                  10U
#define CAN_TSEG1_12TQ                  11U
#define CAN_TSEG1_13TQ                  12U
#define CAN_TSEG1_14TQ                  13U
#define CAN_TSEG1_15TQ                  14U
#define CAN_TSEG1_16TQ                  15U

/* ---- Error display mode ---- */
#define CAN_ERRDISP_FIRST_ERR_ONLY       0U
#define CAN_ERRDISP_ALL_ERR              1U

/* ---- Bus off recovery mode ---- */
#define CAN_BOFF_RECOVERY_ISO11898       0U
#define CAN_BOFF_RECOVERY_START_ENTRY    1U
#define CAN_BOFF_RECOVERY_END_ENTRY      2U
#define CAN_BOFF_RECOVERY_MANUAL_ENTRY   3U

/* ---- Channel mode ---- */
#define CAN_MODE_CH_COMM_MODE            0U
#define CAN_MODE_CH_RESET_MODE           1U
#define CAN_MODE_CH_HALT_MODE            2U

/* ---- Time stamp clock source ---- */
#define CAN_TS_PCLOCK_2                  0U
#define CAN_TS_CAN0_BT_CLOCK             1U

/* ---- Time stamp clock source division ---- */
#define CAN_TS_NO_DIV                    0U
#define CAN_TS_2_DIV                     1U
#define CAN_TS_4_DIV                     2U
#define CAN_TS_8_DIV                     3U
#define CAN_TS_16_DIV                    4U
#define CAN_TS_32_DIV                    5U
#define CAN_TS_64_DIV                    6U
#define CAN_TS_128_DIV                   7U
#define CAN_TS_256_DIV                   8U
#define CAN_TS_512_DIV                   9U
#define CAN_TS_1024_DIV                 10U
#define CAN_TS_2048_DIV                 11U
#define CAN_TS_4096_DIV                 12U
#define CAN_TS_8192_DIV                 13U
#define CAN_TS_16384_DIV                14U
#define CAN_TS_32768_DIV                15U

/* ---- CAN clock source ---- */
#define CAN_SOURCE_PCLOCK_2              0U
#define CAN_SOURCE_MAIN_CLOCK            1U

/* ---- transmission priority ---- */
#define CAN_TX_ID_FIRST                  0U
#define CAN_TX_BUF_IDX_FIRST             1U

/* ---- global mode ---- */
#define CAN_GLB_OPERATION_MODE           0U
#define CAN_GLB_RESET_MODE               1U
#define CAN_GLB_TEST_MODE                2U

/* ---- FIFO (Rx, Common) interrupt generation timing ---- */
#define CAN_FIFO_THRESHOLD_1_8           0U
#define CAN_FIFO_THRESHOLD_2_8           1U
#define CAN_FIFO_THRESHOLD_3_8           2U
#define CAN_FIFO_THRESHOLD_4_8           3U
#define CAN_FIFO_THRESHOLD_5_8           4U
#define CAN_FIFO_THRESHOLD_6_8           5U
#define CAN_FIFO_THRESHOLD_7_8           6U
#define CAN_FIFO_THRESHOLD_FULL          7U

/* ---- FIFO (Rx, Common) interrupt factor ---- */
#define CAN_FIFO_INT_THRESHOLD           0U
#define CAN_FIFO_INT_EACH_MSG            1U

/* ---- FIFO (Rx, Common) depth define ---- */
#define CAN_FIFO_DEPTH_0                 0U
#define CAN_FIFO_DEPTH_4                 1U
#define CAN_FIFO_DEPTH_8                 2U
#define CAN_FIFO_DEPTH_16                3U

/* ---- Tx buffer ---- */
#define CAN_TX_BUFFER_0                  0U
#define CAN_TX_BUFFER_1                  1U
#define CAN_TX_BUFFER_2                  2U
#define CAN_TX_BUFFER_3                  3U

/* ---- Common FIFO interval time counter source ---- */
#define CAN_IT_PCLOCK_2                  0U
#define CAN_IT_CAN_BIT_CLK               1U
#define CAN_IT_PCLOCK_2_10_DIV           2U

/* ---- Common FIFO mode ---- */
#define CAN_TRFIFO_RX_MODE               0U
#define CAN_TRFIFO_TX_MODE               1U

#endif /* R_RL78_CAN_SFR_H */
