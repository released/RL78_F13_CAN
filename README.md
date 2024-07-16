# RL78_F13_CAN
 RL78_F13_CAN

udpate @ 2024/07/16

1. init UART , P15 , P16 for printf log , 

2. init CAN BUS : P1.0/CTXD0 , P1.1/CRXD0 , test on RL78 F13 EVB

3. refer to RL78-RSCANLite_Manual.pdf and RS-CAN_Configurator.exe , to set up RL78 CAN driver 

4. below is CAN bus clock setting in CS+

![image](https://github.com/released/RL78_F13_CAN/blob/main/clock_setting.jpg)


5. below is baud rate/sampling setting in RS-CAN_Configurator.exe

![image](https://github.com/released/RL78_F13_CAN/blob/main/CAN_Configurator.jpg)


6. below is baud rate/sampling setting in PCAN 

![image](https://github.com/released/RL78_F13_CAN/blob/main/PCAN_baud_rate.jpg)


7. below is MCU receive CAN bus data from PCAN 

![image](https://github.com/released/RL78_F13_CAN/blob/main/log_PCAN.jpg)


8. below is MCU transmit CAN bus data to PCAN 

data increase 1 per packet

![image](https://github.com/released/RL78_F13_CAN/blob/main/MCU_TxBuffer_polling1.jpg)


9. below is MCU transmit CAN bus data to PCAN 

data increase 0x10 per packet

![image](https://github.com/released/RL78_F13_CAN/blob/main/MCU_TxBuffer_polling2.jpg)

