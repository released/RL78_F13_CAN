/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_rl78_can_drv.h"                  /* Header file for CAN software driver */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

void R_CAN_Create( void )
{
    Can_RtnType retval;

    /* supply CAN clock */
    CAN0EN = 1;
    
    /* CAN Initialize */
    retval = CAN_RTN_RAM_INIT;
    while (retval != CAN_RTN_OK) {
        retval = R_CAN_Init();
    }

    /* Set global operating mode */
    retval = CAN_RTN_MODE_WAIT;
    while (retval == CAN_RTN_MODE_WAIT) {
        retval = R_CAN_GlobalStart();
    }

    /* CH0 -> Channel communication mode */
    retval = CAN_RTN_MODE_WAIT;
    while (retval == CAN_RTN_MODE_WAIT) {
        retval = R_CAN_ChStart(CAN_CH0);
    }
    
    /* Set interrupt */
    CAN0TRMMK = 0;      /* Enable transmit interrup mask flag(by TBuffer, TRBuffer) */
    //CAN0CFRMK = 0;    /* Enable receive  interrupt mask flag(by TRBuffer) */
    CAN0ERRMK = 0;      /* Enable channel error interrupt mask flag */
    CANGRFRMK = 0;      /* Enable receive  interrupt mask flag(by RFIFO0, RFIFO1) */
    CANGERRMK = 0;      /* Enable global error interrupt mask flag */
    
}
