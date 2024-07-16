/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_PROJ_CTL;
#define FLAG_PROJ_TIMER_PERIOD_1000MS                 	(flag_PROJ_CTL.bit0)
#define FLAG_PROJ_TRIG_BTN                       	    (flag_PROJ_CTL.bit1)
#define FLAG_PROJ_REVERSE2                 	            (flag_PROJ_CTL.bit2)
#define FLAG_PROJ_REVERSE3                    		    (flag_PROJ_CTL.bit3)
#define FLAG_PROJ_REVERSE4                              (flag_PROJ_CTL.bit4)
#define FLAG_PROJ_REVERSE5                              (flag_PROJ_CTL.bit5)
#define FLAG_PROJ_REVERSE6                              (flag_PROJ_CTL.bit6)
#define FLAG_PROJ_TIMER_PERIOD_SPECIFIC                 (flag_PROJ_CTL.bit7)


#define FLAG_PROJ_TRIG_1                                (flag_PROJ_CTL.bit8)
#define FLAG_PROJ_TRIG_2                                (flag_PROJ_CTL.bit9)
#define FLAG_PROJ_TRIG_3                                (flag_PROJ_CTL.bit10)
#define FLAG_PROJ_TRIG_4                                (flag_PROJ_CTL.bit11)
#define FLAG_PROJ_TRIG_5                                (flag_PROJ_CTL.bit12)
#define FLAG_PROJ_REVERSE13                             (flag_PROJ_CTL.bit13)
#define FLAG_PROJ_REVERSE14                             (flag_PROJ_CTL.bit14)
#define FLAG_PROJ_REVERSE15                             (flag_PROJ_CTL.bit15)

/*_____ D E F I N I T I O N S ______________________________________________*/

volatile unsigned int counter_tick = 0;

Can_RtnType retval;
can_frame_t frame;
/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void set_TIMER_PERIOD_1000MS(void)
{
    FLAG_PROJ_TIMER_PERIOD_1000MS = 1;
}

void reset_TIMER_PERIOD_1000MS(void)
{
    FLAG_PROJ_TIMER_PERIOD_1000MS = 0;
}

bool Is_TIMER_PERIOD_1000MS_Trig(void)
{
    return FLAG_PROJ_TIMER_PERIOD_1000MS;
}

unsigned int get_tick(void)
{
	return (counter_tick);
}

void set_tick(unsigned int t)
{
	counter_tick = t;
}

void tick_counter(void)
{
	counter_tick++;
    if (get_tick() >= 60000)
    {
        set_tick(0);
    }
}

void delay_ms(unsigned int ms)
{
	#if 1
    unsigned int tickstart = get_tick();
    unsigned int wait = ms;
	unsigned int tmp = 0;
	
    while (1)
    {
		if (get_tick() > tickstart)	// tickstart = 59000 , tick_counter = 60000
		{
			tmp = get_tick() - tickstart;
		}
		else // tickstart = 59000 , tick_counter = 2048
		{
			tmp = 60000 -  tickstart + get_tick();
		}		
		
		if (tmp > wait)
			break;
    }
	
	#else
	TIMER_Delay(TIMER0, 1000*ms);
	#endif
}

void decode_rcv_frame(can_frame_t * pFrame)
{
    /*
        uint16_t IDL:16;   ID Data (low)                      
        uint16_t IDH:13;   ID Data (high)                     
        uint16_t THDSE :1; Transmit History Data Store Enable 
        uint16_t RTR :1;   RTR  0:Data 1:Remote               
        uint16_t IDE :1;   IDE  0:Standard 1:Extend           
        uint16_t TS :16;   Timestamp Data                     
        uint16_t LBL :12;  Label Data                         
        uint16_t DLC :4;   DLC Data                           
        uint8_t  DB[8];    Data Byte   
    */ 

    printf("IDE:");
    (pFrame->IDE == 0 ) ? (printf("Standard\r\n")) : (printf("Extend\r\n")) ;
    printf("RTR:");
    (pFrame->RTR == 0 ) ? (printf("Data\r\n")) : (printf("Remote\r\n")) ;
    printf("THDSE/Transmit History Data Store Enable:");
    (pFrame->THDSE == 1 ) ? (printf("Enable\r\n")) : (printf("Disable\r\n")) ;
    
    printf("IDL/ID Data (low):0x%02X\r\n" , pFrame->IDL);
    printf("IDH/ID Data (high):0x%02X\r\n" , pFrame->IDH);

    printf("DLC/DLC Data:0x%02X\r\n" , pFrame->DLC);
    printf("LBL/Label Data:0x%02X\r\n" , pFrame->LBL);
    printf("TS/Timestamp Data:0x%02X\r\n" , pFrame->TS);

    printf("Data Byte:");
    dump_buffer_hex((unsigned char *)frame.DB,pFrame->DLC);

}

void CAN_RxBuffer(void)
{
    /* Wait until receiving a CAN frame (Any STD-ID) */
    retval = R_CAN_ReadRxFIFO(CAN_RXFIFO0, &frame);
    if (retval != CAN_RTN_BUFFER_EMPTY)
    {
        decode_rcv_frame(&frame);
        /* Send out received CAN frame */
        retval = R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);            
        if (retval != CAN_RTN_OK)
        {
            while (1);
        }
    }
}


void CAN_TxBuffer_polling2(void)
{
    static uint8_t cnt = 0;

    frame.IDE = 1;     	        /* IDE  0:Standard 1:Extend           */
    frame.RTR = 0;     	        /* RTR  0:Data 1:Remote               */
    frame.THDSE = 0;   	        /* Transmit History Data Store Enable */
    frame.IDL = 0x110;          /* ID Data (low)                      */
    frame.IDH = 0;              /* ID Data (high)                     */
    frame.DLC = 8;     	        /* DLC Data                           */
    frame.LBL = 0;     	        /* Label Data                         */
    frame.TS = 0;      	        /* Timestamp Data                     */
    frame.DB[0] = 0x5A;         /* Data Byte                          */
    frame.DB[1] = 0x5A;         /* Data Byte                          */
    frame.DB[2] = 0x00 + cnt;   /* Data Byte                          */
    frame.DB[3] = 0x01 + cnt;   /* Data Byte                          */
    frame.DB[4] = 0x02 + cnt;   /* Data Byte                          */
    frame.DB[5] = 0x03 + cnt;   /* Data Byte                          */
    frame.DB[6] = 0xA5;         /* Data Byte                          */
    frame.DB[7] = 0xA5;         /* Data Byte                          */
    retval = R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);
    if (retval != CAN_RTN_OK)
    {
         /* Error handle */
         while (1)  NOP();
    }

    cnt = (cnt >= 0xFF) ? (0x00) : (cnt + 0x10);
}


void CAN_TxBuffer_polling1(void)
{
    static uint8_t cnt = 0;

    frame.IDE = 0;     	        /* IDE  0:Standard 1:Extend           */
    frame.RTR = 0;     	        /* RTR  0:Data 1:Remote               */
    frame.THDSE = 0;   	        /* Transmit History Data Store Enable */
    frame.IDL = 0x55;           /* ID Data (low)                      */
    frame.IDH = 0;              /* ID Data (high)                     */
    frame.DLC = 8;     	        /* DLC Data                           */
    frame.LBL = 0;     	        /* Label Data                         */
    frame.TS = 0;      	        /* Timestamp Data                     */
    frame.DB[0] = 0x5A;         /* Data Byte                          */
    frame.DB[1] = 0x5A;         /* Data Byte                          */
    frame.DB[2] = 0x00 + cnt;   /* Data Byte                          */
    frame.DB[3] = 0x01 + cnt;   /* Data Byte                          */
    frame.DB[4] = 0x02 + cnt;   /* Data Byte                          */
    frame.DB[5] = 0x03 + cnt;   /* Data Byte                          */
    frame.DB[6] = 0xA5;         /* Data Byte                          */
    frame.DB[7] = 0xA5;         /* Data Byte                          */
    retval = R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);
    if (retval != CAN_RTN_OK)
    {
         /* Error handle */
         while (1)  NOP();
    }

    cnt++;
}

void CAN_TxBuffer(void)
{
    frame.IDE = 1;     	 /* IDE  0:Standard 1:Extend           */
    frame.RTR = 0;     	 /* RTR  0:Data 1:Remote               */
    frame.THDSE = 0;   	 /* Transmit History Data Store Enable */
    frame.IDL = 0x120;   /* ID Data (low)                      */
    frame.IDH = 0;       /* ID Data (high)                     */
    frame.DLC = 8;     	 /* DLC Data                           */
    frame.LBL = 0;     	 /* Label Data                         */
    frame.TS = 0;      	 /* Timestamp Data                     */
    frame.DB[0] = 0x12;  /* Data Byte                          */
    frame.DB[1] = 0x34;  /* Data Byte                          */
    frame.DB[2] = 0x56;  /* Data Byte                          */
    frame.DB[3] = 0x78;  /* Data Byte                          */
    frame.DB[4] = 0x9A;  /* Data Byte                          */
    frame.DB[5] = 0xBC;  /* Data Byte                          */
    frame.DB[6] = 0xDE;  /* Data Byte                          */
    frame.DB[7] = 0xF0;  /* Data Byte                          */
    retval = R_CAN_TrmByTxBuf(CAN_CH0, CAN_TXBUF0, &frame);
    if (retval != CAN_RTN_OK)
    {
         /* Error handle */
         while (1)  NOP();
    }
}


void Timer_1ms_IRQ(void)
{
    tick_counter();

    if ((get_tick() % 1000) == 0)
    {
        set_TIMER_PERIOD_1000MS();
    }

    if ((get_tick() % 250) == 0)
    {
        FLAG_PROJ_TIMER_PERIOD_SPECIFIC = 1;
    }	

    if ((get_tick() % 50) == 0)
    {

    }	
}


/*
    F13 target board
    LED1 connected to P66, LED2 connected to P67
*/
void LED_Toggle(void)
{
    // PIN_WRITE(6,6) = ~PIN_READ(6,6);
    // PIN_WRITE(6,7) = ~PIN_READ(6,7);
    P6_bit.no6 = ~P6_bit.no6;
    P6_bit.no7 = ~P6_bit.no7;
}

void loop(void)
{
	// static unsigned int LOG1 = 0;

    if (Is_TIMER_PERIOD_1000MS_Trig())
    {
        reset_TIMER_PERIOD_1000MS();

        // printf("log(timer):%4d\r\n",LOG1++);
        LED_Toggle();             
    }

    if (FLAG_PROJ_TRIG_BTN)
    {
        FLAG_PROJ_TRIG_BTN = 0;
        printf("BTN pressed\r\n");
    }

    if (FLAG_PROJ_TRIG_1)
    {
        FLAG_PROJ_TRIG_1 = 0;
        CAN_TxBuffer();
    }

    if ((FLAG_PROJ_TRIG_2 ==1) && 
        (FLAG_PROJ_TIMER_PERIOD_SPECIFIC == 1))
    {        
        FLAG_PROJ_TIMER_PERIOD_SPECIFIC = 0;
        CAN_TxBuffer_polling1();
    }

    if ((FLAG_PROJ_TRIG_3 ==1) && 
        (FLAG_PROJ_TIMER_PERIOD_SPECIFIC == 1))
    {        
        FLAG_PROJ_TIMER_PERIOD_SPECIFIC = 0;
        CAN_TxBuffer_polling2();
    }


    CAN_RxBuffer();

}

// F13 EVB , P137/INTP0
void Button_Process_in_IRQ(void)    
{
    FLAG_PROJ_TRIG_BTN = 1;
}

void UARTx_Process(unsigned char rxbuf)
{    
    if (rxbuf > 0x7F)
    {
        printf("invalid command\r\n");
    }
    else
    {
        printf("press:%c(0x%02X)\r\n" , rxbuf,rxbuf);   // %c :  C99 libraries.
        switch(rxbuf)
        {
            case '1':
                FLAG_PROJ_TRIG_1 = 1;
                break;
            case '2':
                // FLAG_PROJ_TRIG_2 = 1;
                FLAG_PROJ_TRIG_2 ^= 1;
                break;
            case '3':
                // FLAG_PROJ_TRIG_3 = 1;
                FLAG_PROJ_TRIG_3 ^= 1;
                break;
            case '4':
                FLAG_PROJ_TRIG_4 = 1;
                break;
            case '5':
                FLAG_PROJ_TRIG_5 = 1;
                break;

            case 'X':
            case 'x':
                RL78_soft_reset(7);
                break;
            case 'Z':
            case 'z':
                RL78_soft_reset(1);
                break;
        }
    }
}

/*
    Reset Control Flag Register (RESF) 
    BIT7 : TRAP
    BIT6 : 0
    BIT5 : 0
    BIT4 : WDCLRF
    BIT3 : 0
    BIT2 : 0
    BIT1 : IAWRF
    BIT0 : LVIRF
*/
void check_reset_source(void)
{
    /*
        Internal reset request by execution of illegal instruction
        0  Internal reset request is not generated, or the RESF register is cleared. 
        1  Internal reset request is generated. 
    */
    uint8_t src = RESF;
    printf("Reset Source <0x%08X>\r\n", src);

    #if 1   //DEBUG , list reset source
    if (src & BIT0)
    {
        printf("0)voltage detector (LVD)\r\n");       
    }
    if (src & BIT1)
    {
        printf("1)illegal-memory access\r\n");       
    }
    if (src & BIT2)
    {
        printf("2)EMPTY\r\n");       
    }
    if (src & BIT3)
    {
        printf("3)EMPTY\r\n");       
    }
    if (src & BIT4)
    {
        printf("4)watchdog timer (WDT) or clock monitor\r\n");       
    }
    if (src & BIT5)
    {
        printf("5)EMPTY\r\n");       
    }
    if (src & BIT6)
    {
        printf("6)EMPTY\r\n");       
    }
    if (src & BIT7)
    {
        printf("7)execution of illegal instruction\r\n");       
    }
    #endif

}

/*
    7:Internal reset by execution of illegal instruction
    1:Internal reset by illegal-memory access
*/
//perform sofware reset
void _reset_by_illegal_instruction(void)
{
    static const unsigned char illegal_Instruction = 0xFF;
    void (*dummy) (void) = (void (*)(void))&illegal_Instruction;
    dummy();
}
void _reset_by_illegal_memory_access(void)
{
    #if 1
    const unsigned char ILLEGAL_ACCESS_ON = 0x80;
    IAWCTL |= ILLEGAL_ACCESS_ON;            // switch IAWEN on (default off)
    *(__far volatile char *)0x00000 = 0x00; //write illegal address 0x00000(RESET VECTOR)
    #else
    signed char __far* a;                   // Create a far-Pointer
    IAWCTL |= _80_CGC_ILLEGAL_ACCESS_ON;    // switch IAWEN on (default off)
    a = (signed char __far*) 0x0000;        // Point to 0x000000 (FLASH-ROM area)
    *a = 0;
    #endif
}

void RL78_soft_reset(unsigned char flag)
{
    switch(flag)
    {
        case 7: // do not use under debug mode
            _reset_by_illegal_instruction();        
            break;
        case 1:
            _reset_by_illegal_memory_access();
            break;
    }
}

// retarget printf
int __far putchar(int c)
{
    // F13 , UART0
    STMK0 = 1U;    /* disable INTST0 interrupt */
    SDR00L = (unsigned char)c;
    while(STIF0 == 0)
    {

    }
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    return c;
}

void hardware_init(void)
{
    // const unsigned char indicator[] = "hardware_init";
    EI();   //BSP_EI();
    R_UART0_Start();            // UART , P15 , P16
    R_TAU0_Channel1_Start();  
    R_INTC0_Start();            // BUTTON , P137 
    
    // RL78 F13 EVB - CAN transceiver PIN#8 , pull low
    P1_bit.no2 = 0;
    R_CAN_Create();             // CAN , P1.0/CTXD0 , P1.1/CRXD0
    
    // check_reset_source();
    printf("%s finish\r\n\r\n",__func__);
}
