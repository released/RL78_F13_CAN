/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_rl78_can_drv.h"                  /* Header file for CAN software driver */
#include "r_rl78_can_sfr.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
// #pragma interrupt INTCANGRFR r_can0_g_receive_isr
// #pragma interrupt INTCANGERR r_can0_g_error_isr
// #pragma interrupt INTCAN0CFR r_can0_receive_isr
// #pragma interrupt INTCAN0TRM r_can0_transmit_isr
// #pragma interrupt INTCAN0ERR r_can0_error_isr
// #pragma interrupt INTCAN0WUP dummy_isr

#pragma interrupt r_can0_g_receive_isr(vect=INTCANGRFR)
#pragma interrupt r_can0_g_error_isr(vect=INTCANGERR)
#pragma interrupt r_can0_receive_isr(vect=INTCAN0CFR)
#pragma interrupt r_can0_transmit_isr(vect=INTCAN0TRM)
#pragma interrupt r_can0_error_isr(vect=INTCAN0ERR)
#pragma interrupt dummy_isr(vect=INTCAN0WUP)

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/****************************************************************************** 
* Function Name: r_can0_g_receive_isr()
* Description  : Receive FIFO interrupt service routine
******************************************************************************/
static void r_can0_g_receive_isr( void )
{
    uint16_t temp_status;
    
    /* --- Check Rx FIFO0 --- */  
    temp_status = RFSTS0;
    if ( ( temp_status & (0x0001 << CAN_RFIF_BIT_POS) ) != 0 )
    {
        /* --- Clear Receive FIFO0 request flag --- */
        RFSTS0 &= ~(0x0001 << CAN_RFIF_BIT_POS);
        
        /* [ISR] */
    }
    
    /* --- Check Rx FIFO1 --- */  
    temp_status = RFSTS1;
    if ( ( temp_status & (0x0001 << CAN_RFIF_BIT_POS) ) != 0 )
    {
        /* --- Clear Receive FIFO0 request flag --- */
        RFSTS1 &= ~(0x0001 << CAN_RFIF_BIT_POS);
        
        /* [ISR] */
        
    }
}

/****************************************************************************** 
* Function Name: r_can0_g_error_isr()
* Description  : Global error interrupt service routine
*   b7-b3 reserved
*   b2  THLES   - Transmit History Buffer overflow
*   b1  MES     - FIFO Message Lost Status Flag
*   b0  DEF     - DLC Error Flag
******************************************************************************/
static void r_can0_g_error_isr( void )
{
    uint8_t temp_status;
    
    temp_status = GERFLL;
    /* --- Is DLC error present? --- */
    if ( (temp_status & 0x01) != 0 )
    {
        /* --- Clear DLC Error flag --- */
        GERFLL &= ~(0x01);
        
        /* [ISR] */
        
    }
    
    /* --- Is FIFO message lost error? --- */
    if ( (temp_status & 0x02) != 0 )
    {
        /* --- Check RFIFO0 message lost --- */
        if ( (RFSTS0 & (0x0001 << CAN_RFMLT_BIT_POS)) != 0 )
        {
            /* --- Clear RFIFO0 message lost error --- */
            RFSTS0 &= ~(0x0001 << CAN_RFMLT_BIT_POS);
            
            /* [ISR] */
            
        }
        
        /* --- Check RFIFO1 message lost --- */
        if ( (RFSTS1 & (0x0001 << CAN_RFMLT_BIT_POS)) != 0 )
        {
            /* --- Clear RFIFO0 message lost error --- */
            RFSTS1 &= ~(0x0001 << CAN_RFMLT_BIT_POS);
            
            /* [ISR] */
            
        }
        
        /* --- Check TFIFO0 message lost --- */
        if ( (CFSTS0 & (0x0001 << CAN_CFMLT_BIT_POS)) != 0 )
        {
            /* --- Clear RFIFO0 message lost error --- */
            CFSTS0 &= ~(0x0001 << CAN_CFMLT_BIT_POS);
            
            /* [ISR] */
            
        }
    }
    
    /* --- Is transmit history buffer overflow? --- */
    if ( (temp_status & 0x01) != 0 )
    {
        /* --- Clear transmit history buffer overflow Error flag --- */
        THLSTS0 &= ~(0x04);
        
        /* [ISR] */
        
    }
}

/****************************************************************************** 
* Function Name: r_can0_transmit_isr()
* Description  : Transmit interrupt service routine for TBuffer, TRFIFO
******************************************************************************/
static void r_can0_transmit_isr( void )
{
    uint16_t temp_status;

    /* --- Check TRFIFO status --- */
    temp_status = CFSTS0;       /* Read Common FIFO status */
    if ( (0x0001 & (temp_status >> CAN_CFTXIF_BIT_POS)) != 0 )
    {
        /* --- Clear transmit flag --- */
        CFSTS0 &= ~(0x0001 << CAN_CFTXIF_BIT_POS);
        
        /* [ISR] */
    }
    
    /* --- Check TrmBuffer0 status --- */
    temp_status = ((TMSTS0 & 0x0006) >> CAN_TMTRF_BIT_POS);     /* Read TrmBuffer0 status */
    TMSTS0 &= ~(0x0006UL);                                      /* Clear TMTRF[1:0] flag to 0x00 */
    switch ( temp_status )
    {
        case CAN_RTN_TRANSMITTING:
            /* transmiting or idle */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_ABORT_OVER:
            /* Transmission aborted */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END:
            /* Transmission completed w/o abort request */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END_WITH_ABORT_REQ:
            /* Transmission completed w/ abort request */
            /* [ISR] */
            break;
            
        default:
            break;
    }

    /* --- Check TrmBuffer1 status --- */
    temp_status = ((TMSTS1 & 0x0006) >> CAN_TMTRF_BIT_POS);     /* Read TrmBuffer1 status */
    TMSTS1 &= ~(0x0006UL);                                      /* Clear TMTRF[1:0] flag to 0x00 */
    switch ( temp_status )
    {
        case CAN_RTN_TRANSMITTING:
            /* transmiting or idle */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_ABORT_OVER:
            /* Transmission aborted */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END:
            /* Transmission completed w/o abort request */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END_WITH_ABORT_REQ:
            /* Transmission completed w/ abort request */
            /* [ISR] */
            break;
            
        default:
            break;
    }


    /* --- Check TrmBuffer2 status --- */
    temp_status = ((TMSTS2 & 0x0006) >> CAN_TMTRF_BIT_POS);     /* Read TrmBuffer2 status */
    TMSTS2 &= ~(0x0006UL);                                      /* Clear TMTRF[1:0] flag to 0x00 */
    switch ( temp_status )
    {
        case CAN_RTN_TRANSMITTING:
            /* transmiting or idle */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_ABORT_OVER:
            /* Transmission aborted */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END:
            /* Transmission completed w/o abort request */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END_WITH_ABORT_REQ:
            /* Transmission completed w/ abort request */
            /* [ISR] */
            break;
            
        default:
            break;
    }

    /* --- Check TrmBuffer3 status --- */
    temp_status = ((TMSTS3 & 0x0006) >> CAN_TMTRF_BIT_POS);     /* Read TrmBuffer3 status */
    TMSTS3 &= ~(0x0006UL);                                      /* Clear TMTRF[1:0] flag to 0x00 */
    
    switch ( temp_status )
    {
        case CAN_RTN_TRANSMITTING:
            /* transmiting or idle */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_ABORT_OVER:
            /* Transmission aborted */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END:
            /* Transmission completed w/o abort request */
            /* [ISR] */
            break;
            
        case CAN_RTN_TX_END_WITH_ABORT_REQ:
            /* Transmission completed w/ abort request */
            /* [ISR] */
            break;
            
        default:
            break;
    }
}

/****************************************************************************** 
* Function Name: r_can0_receive_isr()
* Description  : Receive interrupt service routine for TRFIFO
******************************************************************************/
static void r_can0_receive_isr( void )
{
    uint16_t temp_status;
    
    /* --- Check TRFIFO status --- */
    temp_status = CFSTS0;       /* Read Common FIFO status */
    if ( (0x0001 & (temp_status >> CAN_CFRXIF_BIT_POS)) != 0 )
    {
        /* --- Clear receive flag --- */
        CFSTS0 &= ~(0x0001 << CAN_CFRXIF_BIT_POS);
        
        /* [ISR] */
        
    }
}

/****************************************************************************** 
* Function Name: r_can0_error_isr()
* Description  : Channel error interrupt service routine
*   b15 reseved 
*   b14 ADERR   - ACK Delimiter Error Flag
*   b13 B0ERR   - Dominant Bit Error Flag
*   b12 B1ERR   - Recessive Bit Error Flag
*   b11 CERR    - CRC Error Flag
*   b10 AERR    - ACK Error Flag
*   b9  FERR    - Form Error Flag
*   b8  SERR    - Stuff Error Flag
*   b7  ALF     - Arbitration Lost Flag
*   b6  BLF     - Bus Lock Flag
*   b5  OVLF    - Overload Flag
*   b4  BORF    - Buf Off Recovery Flag
*   b3  BOEF    - Buf Off Entry Flag
*   b2  EPF     - Error Passive Flag
*   b1  EWF     - Error Warning Flag
*   b0  BEF     - Buf Error Flag
******************************************************************************/
static void r_can0_error_isr( void )
{
    uint16_t temp_status;
    
    /* --- Check Channel0 error flag --- */
    temp_status = C0ERFLL;    
    if ( temp_status != 0x00 )
    {
        /* --- Clear Channel0 error flag --- */
        C0ERFLL &= ~temp_status;
        
        /* [ISR] */
        
    }
}

/****************************************************************************** 
* Function Name: dummy_isr()
* Description  : Dummy interrupt service routine
******************************************************************************/
static void dummy_isr( void )
{
    NOP();
}
