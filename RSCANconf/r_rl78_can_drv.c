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
* File Name    : r_rl78_can_drv.c 
* Version      : 1.0 
* Description  : This is source file for CAN driver code.
******************************************************************************/ 
/***************************************************************************** 
* History      : DD.MM.YYYY Version Description 
*              : 29.03.2013 1.00     First Release 
******************************************************************************/

/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
// #pragma sfr
#include "r_rl78_can_drv.h"
#include "r_rl78_can_sfr.h"
#include "RSCANlite.h"
#include "r_cg_macrodriver.h"
/*****************************************************************************
 Typedef definitions
 *****************************************************************************/

/*****************************************************************************
 Macro definitions
 *****************************************************************************/

#define CAN_FIFO_PTR_INC                    0xffUL

#define CAN_8_BITS_MASK                     0x00FFU

/*****************************************************************************
 Private variables and functions
 *****************************************************************************/

/*****************************************************************************
 Exported global variables and functions (to be accessed by other files)
 *****************************************************************************/

/****************************************************************************** 
* Function Name: R_Can_Init
* Description  : Initialize CAN controller after reset
* Arguments    : none 
* Return Value : CAN_RTN_OK - 
*                    normal completion
*                CAN_RTN_RAM_INIT -
*                    CAN RAM initializing
*                CAN_RTN_MODE_WAIT -
*                    wait to change global mode or channel mode
******************************************************************************/
Can_RtnType R_CAN_Init(void)
{

    /* ==== CAN RAM initialization ==== */
    if ((GSTS & CAN_RAM_INIT_BIT_ON) != 0U)
    {
        return CAN_RTN_RAM_INIT;
    }

    /* ==== global mode switch (stop->reset) ==== */
    if ((GSTS & CAN_GLB_STP_STS_BIT_ON) != 0U)
    {
        /* exit global stop mode */
        GCTRL &= (uint16_t)~(CAN_GLB_STP_BIT_ON);
        if ((GSTS & CAN_GLB_STP_STS_BIT_ON) != 0U)
        {
            return CAN_RTN_MODE_WAIT;
        }
    }

    /* ==== channel mode switch ==== */
    /* --- switch from channel stop mode ---- */
    if ((C0STSL & CAN_STP_STS_BIT_ON) != 0U)
    {
        /* exit channel stop mode */
        C0CTRL &= (uint16_t)~(CAN_STP_BIT_ON);
        if ((C0STSL & CAN_STP_STS_BIT_ON) != 0U)
        {
            return CAN_RTN_MODE_WAIT;
        }
    }

    /* ==== global function setting ==== */
    GCFGL = CAN_CFG_GLB_CFGL;
    GCFGH = CAN_CFG_GLB_CFGH;

    /* ==== communication speed setting ==== */
    C0CFGL = CAN_CFG_C0_BAUDRATE_L;
    C0CFGH = CAN_CFG_C0_BAUDRATE_H;

    /* ==== Rx rule setting ==== */
#if CAN_RX_RULE_NUM > 0
    {
        volatile __near can_rxrule_sfr_t * p_RxRuleSfr;
        uint16_t temp_rpage;
        uint16_t rxrule_idx;

        /* ---- Set Rx rule number per channel ---- */
        GAFLCFG = CAN_CFG_RN0;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 0 ---- */
        GRWCR &= (uint16_t)~(CAN_RAM_WINDOW_BIT_ON);

        /* ---- Copy Rx rule one by one ---- */
        p_RxRuleSfr = (volatile __near can_rxrule_sfr_t *)&GAFLIDL0;
        for (rxrule_idx = 0U; rxrule_idx < CAN_RX_RULE_NUM; rxrule_idx++)
        {
            /* Set a single Rx rule */
            p_RxRuleSfr->IDL = g_rxrule_table[rxrule_idx][0];
            p_RxRuleSfr->IDH = g_rxrule_table[rxrule_idx][1];
            p_RxRuleSfr->ML  = g_rxrule_table[rxrule_idx][2];
            p_RxRuleSfr->MH  = g_rxrule_table[rxrule_idx][3];
            p_RxRuleSfr->PL  = g_rxrule_table[rxrule_idx][4];
            p_RxRuleSfr->PH  = g_rxrule_table[rxrule_idx][5];

            /* Next sfr */
            p_RxRuleSfr++;
        }

        /* ---- restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }
#endif /* CAN_RX_RULE_NUM > 0 */

    /* ==== buffer setting ==== */
    /* ---- Set Rx buffer number ---- */
    RMNB = CAN_CFG_RBNUM;

    /* ---- Set Rx FIFO buffer ---- */
    RFCC0 = CAN_CFG_RXFIFO0;
    RFCC1 = CAN_CFG_RXFIFO1;

    /* ---- Set common (Tx/Rx) FIFO buffer ---- */
    CFCCL0 = CAN_CFG_C0_TRFIFO0_L;
    CFCCH0 = CAN_CFG_C0_TRFIFO0_H;

    /* ---- Tx buffer transmission complete interrupt ---- */
    TMIEC = CAN_CFG_C0_TXBUF_IE;

    /* ==== global error interrupt setting ==== */
    GCTRL = (GCTRL & (CAN_GLB_STP_BIT_ON | CAN_GLB_MODE_BITS_ON)) + CAN_CFG_GLB_ERR_INT;

    /* ==== channel function setting ==== */
    C0CTRL = (C0CTRL & (CAN_STP_BIT_ON | CAN_MODE_BITS_ON)) + CAN_CFG_C0_FUNC_L;
    C0CTRH = C0CTRH + CAN_CFG_C0_FUNC_H;

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_GlobalStart
* Description  : Start global operation
* Arguments    : none 
* Return Value : CAN_RTN_OK -
*                    normal completion
*                CAN_RTN_MODE_WAIT -
*                    wait to change global mode
******************************************************************************/
Can_RtnType R_CAN_GlobalStart(void)
{

    /* ==== switch to global operation mode from global reset mode ==== */
    if ((GSTS & CAN_GLB_RST_STS_BIT_ON) != 0U)
    {
        GCTRL = ((GCTRL & (uint16_t)~CAN_GLB_MODE_BITS_ON) |
                    CAN_GLB_OPERATION_MODE);
        if ((GSTS & CAN_GLB_RST_STS_BIT_ON) != 0U)
        {
            return CAN_RTN_MODE_WAIT;
        }
    }

    /* ==== Global error ==== */
    GERFLL = 0x0U;

    /* ==== enable reception FIFO ==== */
    if (g_rxfifo0_use_mode != CAN_NOUSE)
    {
        RFCC0 |= CAN_RFIFO_EN_BIT_ON;
    }

    if (g_rxfifo1_use_mode != CAN_NOUSE)
    {
        RFCC1 |= CAN_RFIFO_EN_BIT_ON;
    }

    /* ==== Tx/Rx FIFO setting ==== */
    /* ---- enable Tx/Rx FIFO (Rx mode) ---- */
    if (g_trfifo_use_mode == CAN_USE_RX_MODE)
    {
        CFCCL0 |= CAN_TRFIFO_EN_BIT_ON;
    }

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_ChStart_CH0
* Description  : Start channel operation (Channel 0)
* Arguments    : none
* Return Value : CAN_RTN_OK -
*                    normal completion
*                CAN_RTN_MODE_WAIT -
*                    wait to change channel mode
******************************************************************************/
Can_RtnType R_CAN_ChStart_CH0(void)
{
    /* ---- switch to channel operation mode ---- */
    if ((C0STSL & CAN_RST_STS_BIT_ON) != 0U)
    {
        C0CTRL = (C0CTRL & (uint16_t)~CAN_MODE_BITS_ON) | CAN_MODE_CH_COMM_MODE;
        if ((C0STSL & CAN_RST_STS_BIT_ON) != 0U)
        {
            return CAN_RTN_MODE_WAIT;
        }
    }

    /* ---- enable Tx/Rx FIFO (Tx mode) ---- */
    if (g_trfifo_use_mode == CAN_USE_TX_MODE)
    {
        CFCCL0 |= CAN_TRFIFO_EN_BIT_ON;
    }

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_TrmByTxBuf_CH0
* Description  : Transmit a frame by Tx buffer (Channel 0)
* Arguments    : txbuf_idx -
*                    Tx buffer index
*                pFrame -
*                    pointer to frame to be transmitted
* Return Value : CAN_RTN_OK -
*                    normal completion
*                CAN_RTN_STS_ERROR -
*                    failure to clear Tx buffer status
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_TrmByTxBuf_CH0(can_txbuf_t txbuf_idx,
                                 const can_frame_t * pFrame)
{
#if defined(__CHECK__)
    /* ----  Check Tx buffer index ---- */
    if (txbuf_idx >= CAN_MAX_TXBUF_NUM)
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ---- Clear Tx buffer status ---- */
    {
        volatile __near uint8_t * p_TMSTSp;

        p_TMSTSp = CAN_ADDR_TMSTSp(txbuf_idx);
        *p_TMSTSp = 0x0U;
        if (*p_TMSTSp != 0x0U)
        {
            return CAN_RTN_STS_ERROR;
        }
    }

    /* ---- Store message to tx buffer ---- */
    {
        volatile __near can_frame_sfr_t * p_TxMsgSfr;
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Set frame data ---- */
        p_TxMsgSfr = CAN_ADDR_TMIDLp(txbuf_idx);
        p_TxMsgSfr->IDL = ((can_frame_sfr_t *)pFrame)->IDL;
        p_TxMsgSfr->IDH = ((can_frame_sfr_t *)pFrame)->IDH;
        p_TxMsgSfr->PTR = ((can_frame_sfr_t *)pFrame)->PTR;
        p_TxMsgSfr->DF0 = ((can_frame_sfr_t *)pFrame)->DF0;
        p_TxMsgSfr->DF1 = ((can_frame_sfr_t *)pFrame)->DF1;
        p_TxMsgSfr->DF2 = ((can_frame_sfr_t *)pFrame)->DF2;
        p_TxMsgSfr->DF3 = ((can_frame_sfr_t *)pFrame)->DF3;

        /* ---- Restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Set transmission request ---- */
    TMCp(txbuf_idx) = CAN_TXBUF_TRM_BIT_ON;

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_AbortTrm_CH0
* Description  : Abort a CAN transmission (Channel 0)
* Arguments    : txbuf_idx -
*                    Tx buffer index
* Return Value : CAN_RTN_OK -
*                    normal completion
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_AbortTrm_CH0(can_txbuf_t txbuf_idx)
{
#if defined(__CHECK__)
    /* ----  Check Tx buffer index ---- */
    if (txbuf_idx >= CAN_MAX_TXBUF_NUM)
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ---- Set transmission abort request ---- */
    TMCp(txbuf_idx) |= CAN_TXBUF_ABT_BIT_ON;

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_CheckTxBufResult_CH0
* Description  : Read the result of transmission from Tx buffer (Channel 0)
* Arguments    : txbuf_idx -
*                    Tx buffer index
* Return Value : CAN_RTN_TRANSMITTING -
*                    Transmission is in progress
*                    or no transmit request is present.
*                CAN_RTN_TX_ABORT_OVER -
*                    Transmit abort has been completed.
*                CAN_RTN_TX_END -
*                    Transmission has been completed
*                    (without transmit abort request).
*                CAN_RTN_TX_END_WITH_ABORT_REQ -
*                    Transmission has been completed
*                    (with transmit abort request).
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_CheckTxBufResult_CH0(can_txbuf_t txbuf_idx)
{
    Can_RtnType rtn_value;

    volatile __near uint8_t * p_TMSTSp;

#if defined(__CHECK__)
    /* ----  Check Tx buffer index ---- */
    if (txbuf_idx >= CAN_MAX_TXBUF_NUM)
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    p_TMSTSp = CAN_ADDR_TMSTSp(txbuf_idx);

    rtn_value = (Can_RtnType)((*p_TMSTSp & CAN_TXBUF_RSLT_BITS_ON)
                              >> CAN_TXBUF_RSLT_BITS_POS);

    /* ---- Tx transmission completed/abort? ---- */
    if (rtn_value != CAN_RTN_TRANSMITTING)
    {
        /* Clear Tx buffer status */
        *p_TMSTSp = 0x0U;
    }

    return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_TrmByTRFIFO0_CH0
* Description  : Transmit a frame by common (Tx/Rx) FIFO 0 (Channel 0)
* Arguments    : ch_idx -
*                    channel index
*                trfifo_idx -
*                    Tx/Rx FIFO index
*                pFrame -
*                    pointer to frame to be transmitted
* Return Value : CAN_RTN_OK -
*                    Frame is successfully pushed into FIFO.
*                CAN_RTN_FIFO_FULL -
*                    Specified FIFO is full.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_TrmByTRFIFO0_CH0(const can_frame_t * pFrame)
{
#if defined(__CHECK__)
    /* ----  Check Tx/Rx FIFO 0 mode ---- */
    if (g_trfifo_use_mode != CAN_USE_TX_MODE)
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ---- Return if Tx/Rx FIFO is full ---- */
    if ((CFSTS0 & CAN_TRFIFO_FULL_BIT_ON) != 0)
    {
        return CAN_RTN_FIFO_FULL;
    }

    /* ---- Send message into Tx/Rx FIFO if it is not full ---- */
    {
        uint16_t temp_rpage;

        /* ---- save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Set frame data ---- */
        CFIDL0 = ((can_frame_sfr_t *)pFrame)->IDL;
        CFIDH0 = ((can_frame_sfr_t *)pFrame)->IDH;
        CFPTR0 = ((can_frame_sfr_t *)pFrame)->PTR;
        CFDF00 = ((can_frame_sfr_t *)pFrame)->DF0;
        CFDF10 = ((can_frame_sfr_t *)pFrame)->DF1;
        CFDF20 = ((can_frame_sfr_t *)pFrame)->DF2;
        CFDF30 = ((can_frame_sfr_t *)pFrame)->DF3;

        /* ---- restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Increment Tx/Rx FIFO buffer pointer ---- */
    CFPCTR0 = CAN_FIFO_PTR_INC;

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxBuffer
* Description  : Read message from Rx buffer
* Arguments    : p_rxbuf_idx -
*                    pointer to Rx buffer that receives frame
*                pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_STS_ERROR -
*                    failure to clear Rx complete flag
*                CAN_RTN_OVERWRITE -
*                    A frame is overwritten.
******************************************************************************/
Can_RtnType R_CAN_ReadRxBuffer(uint8_t * p_rxbuf_idx, can_frame_t * pFrame)
{
    uint8_t  buf_idx;
    uint16_t temp_rbrcf;
    uint16_t pattern;

    /* ---- Judge if new messages are available ---- */
    temp_rbrcf = RMND0;
    if (temp_rbrcf == 0)
    {
        return CAN_RTN_BUFFER_EMPTY;
    }

    /* ---- Get Rx buffer that has new message ---- */
    if (temp_rbrcf != 0)
    {
        pattern = 1;
        for (buf_idx = 0U; buf_idx < 16U; ++buf_idx)
        {
            if ((temp_rbrcf & pattern) != 0)
            {
                *p_rxbuf_idx = buf_idx;
                break;
            }
            pattern <<= 1;
        }
    }

    /* ---- Clear Rx complete flag of corresponding Rx buffer ---- */
    RMND0 &= (uint16_t)~pattern;
    if ((RMND0 & pattern) != 0)
    {
        return CAN_RTN_STS_ERROR;
    }

    /* ---- Read out message from Rx buffer ---- */
    {
        volatile __near can_frame_sfr_t * p_RxBufSfr;
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Read frame data ---- */
        p_RxBufSfr = CAN_ADDR_RMIDLp(*p_rxbuf_idx);
        ((can_frame_sfr_t *)pFrame)->IDL = p_RxBufSfr->IDL;
        ((can_frame_sfr_t *)pFrame)->IDH = p_RxBufSfr->IDH;
        ((can_frame_sfr_t *)pFrame)->TS  = p_RxBufSfr->TS; 
        ((can_frame_sfr_t *)pFrame)->PTR = p_RxBufSfr->PTR;
        ((can_frame_sfr_t *)pFrame)->DF0 = p_RxBufSfr->DF0;
        ((can_frame_sfr_t *)pFrame)->DF1 = p_RxBufSfr->DF1;
        ((can_frame_sfr_t *)pFrame)->DF2 = p_RxBufSfr->DF2;
        ((can_frame_sfr_t *)pFrame)->DF3 = p_RxBufSfr->DF3;

        /* ---- restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Judge if current message is overwritten ---- */
    if ((RMND0 & pattern) != 0)
    {
        return CAN_RTN_OVERWRITE;
    }

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxFIFO
* Description  : Read message from Rx FIFO
* Arguments    : rxfifo_idx -
*                    Rx FIFO index
*                pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
extern Can_RtnType R_CAN_ReadRxFIFO0(can_frame_t *);
extern Can_RtnType R_CAN_ReadRxFIFO1(can_frame_t *);
Can_RtnType R_CAN_ReadRxFIFO(can_rxfifo_t rxfifo_idx, can_frame_t * pFrame)
{
    if (rxfifo_idx == 0) {
        return R_CAN_ReadRxFIFO0(pFrame);
    } else if (rxfifo_idx == 1) {
        return R_CAN_ReadRxFIFO1(pFrame);
#if defined(__CHECK__)
    } else {
        return CAN_RTN_ARG_ERROR;
#endif
    }

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxFIFO0
* Description  : Read message from Rx FIFO 0
* Arguments    : pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_ReadRxFIFO0(can_frame_t * pFrame)
{
    uint16_t    temp_status;
    Can_RtnType rtn_value;

#if defined(__CHECK__)
    /* ----  Check Rx FIFO 0 mode ---- */
    if ((g_rxfifo0_use_mode == CAN_NOUSE))
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ----  Check if any unread message is available in Rx FIFO ---- */
    temp_status = RFSTS0;
    if ((temp_status & CAN_RFIFO_EMPTY_BIT_ON) != 0)
    {
        return CAN_RTN_BUFFER_EMPTY;
    }

    /* ----  Set return value ---- */
    rtn_value = CAN_RTN_OK;

    /* ---- Check if Rx FIFO has message lost ---- */
    if ((temp_status & CAN_RFIFO_MSGLST_BIT_ON) != 0)
    {
        /* ---- Clear message lost flag ---- */
        RFSTS0 = CAN_CLR_WITHOUT_RX_INT;

        /* ----  Set return value ---- */
        rtn_value = CAN_RTN_OK_WITH_LOST;
    }

    /* ---- Read out message from Rx FIFO ---- */
    {
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Read frame data ---- */
        ((can_frame_sfr_t *)pFrame)->IDL = RFIDL0;
        ((can_frame_sfr_t *)pFrame)->IDH = RFIDH0;
        ((can_frame_sfr_t *)pFrame)->TS  = RFTS0; 
        ((can_frame_sfr_t *)pFrame)->PTR = RFPTR0;
        ((can_frame_sfr_t *)pFrame)->DF0 = RFDF00;
        ((can_frame_sfr_t *)pFrame)->DF1 = RFDF10;
        ((can_frame_sfr_t *)pFrame)->DF2 = RFDF20;
        ((can_frame_sfr_t *)pFrame)->DF3 = RFDF30;

        /* ---- Restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Increment Rx FIFO buffer pointer ---- */
    RFPCTR0 = CAN_FIFO_PTR_INC;

    return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxFIFO1
* Description  : Read message from Rx FIFO 1
* Arguments    : pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_ReadRxFIFO1(can_frame_t * pFrame)
{
    uint16_t    temp_status;
    Can_RtnType rtn_value;

#if defined(__CHECK__)
    /* ----  Check Rx FIFO 1 mode ---- */
    if ((g_rxfifo1_use_mode == CAN_NOUSE))
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ----  Check if any unread message is available in Rx FIFO ---- */
    temp_status = RFSTS1;
    if ((temp_status & CAN_RFIFO_EMPTY_BIT_ON) != 0)
    {
        return CAN_RTN_BUFFER_EMPTY;
    }

    /* ----  Set return value ---- */
    rtn_value = CAN_RTN_OK;

    /* ---- Check if Rx FIFO has message lost ---- */
    if ((temp_status & CAN_RFIFO_MSGLST_BIT_ON) != 0)
    {
        /* ---- Clear message lost flag ---- */
        RFSTS1 = CAN_CLR_WITHOUT_RX_INT;

        /* ----  Set return value ---- */
        rtn_value = CAN_RTN_OK_WITH_LOST;
    }

    /* ---- Read out message from Rx FIFO ---- */
    {
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Read frame data ---- */
        ((can_frame_sfr_t *)pFrame)->IDL = RFIDL1;
        ((can_frame_sfr_t *)pFrame)->IDH = RFIDH1;
        ((can_frame_sfr_t *)pFrame)->TS  = RFTS1; 
        ((can_frame_sfr_t *)pFrame)->PTR = RFPTR1;
        ((can_frame_sfr_t *)pFrame)->DF0 = RFDF01;
        ((can_frame_sfr_t *)pFrame)->DF1 = RFDF11;
        ((can_frame_sfr_t *)pFrame)->DF2 = RFDF21;
        ((can_frame_sfr_t *)pFrame)->DF3 = RFDF31;

        /* ---- Restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Increment Rx FIFO buffer pointer ---- */
    RFPCTR1 = CAN_FIFO_PTR_INC;

    return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadTRFIFO
* Description  : Read message from common (Tx/Rx) FIFO
* Arguments    : ch_idx -
*                    channel index
*                trfifo_idx -
*                    common (Tx/Rx) FIFO index
*                pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_ReadTRFIFO0_CH0(can_frame_t * pFrame)
{
    uint16_t    temp_status;
    Can_RtnType rtn_value;

#if defined(__CHECK__)
    /* ----  Check Tx/Rx FIFO 0 mode ---- */
    if (g_trfifo_use_mode != CAN_USE_RX_MODE)
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ----  Check if any unread message is available in common (Tx/Rx) FIFO ---- */
    temp_status = CFSTS0;
    if ((temp_status & CAN_TRFIFO_EMPTY_BIT_ON) != 0)
    {
        return CAN_RTN_BUFFER_EMPTY;
    }

    /* ----  Set return value ---- */
    rtn_value = CAN_RTN_OK;

    /* ---- Check if common (Tx/Rx) FIFO has message lost ---- */
    if ((temp_status & CAN_TRFIFO_MSGLST_BIT_ON) != 0)
    {
        /* ---- Clear message lost flag ---- */
        CFSTS0 = CAN_CLR_WITHOUT_TX_RX_INT;

        /* ----  Set return value ---- */
        rtn_value = CAN_RTN_OK_WITH_LOST;
    }

    /* ---- Read out message from common (Tx/Rx) FIFO ---- */
    {
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Read frame data ---- */
        ((can_frame_sfr_t *)pFrame)->IDL = CFIDL0;
        ((can_frame_sfr_t *)pFrame)->IDH = CFIDH0;
        ((can_frame_sfr_t *)pFrame)->TS  = CFTS0;
        ((can_frame_sfr_t *)pFrame)->PTR = CFPTR0;
        ((can_frame_sfr_t *)pFrame)->DF0 = CFDF00;
        ((can_frame_sfr_t *)pFrame)->DF1 = CFDF10;
        ((can_frame_sfr_t *)pFrame)->DF2 = CFDF20;
        ((can_frame_sfr_t *)pFrame)->DF3 = CFDF30;

        /* ---- Restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Increment common (Tx/Rx) FIFO buffer pointer ---- */
    CFPCTR0 = CAN_FIFO_PTR_INC;

    return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadChStatus
* Description  : Read channel status
* Arguments    : none
* Return Value : channel status (<= 0xFF) -
*                    normal completion
******************************************************************************/
Can_RtnType R_CAN_ReadChStatus_CH0(void)
{
    return (Can_RtnType)(C0STSL & CAN_8_BITS_MASK);
}
