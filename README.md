J1708 adapter
/**************************************/
MCU: GD32F103C8T6
/**************************************/
PinOut:
/**************************************/

J1708_UART BAUDRATE 9600
PA2 - TX
PA3 - RX

PC13 - LED activity
/**************************************/
Description: 
This project performs USBD_VCP -> j1708 interface conversion.

Admission with J1708 bus:

If the value of the CRC packet converges with the calculated one, then the packet is output without changes in the format
MID+Data+CRC

If the calculated CRC value does not match the received one, then the packet will be output in the format
0xFF+MID+Data+calc. CRC

Sending a packet to the J1708 bus

To send data to J1708, you need to send a packet via USBD_VCP in the following format

Priority [1-8] + Size of data + MID[0-68] + Data[max. 64]. The CRC value will be calculated by software. In the case of incorrect filling of the fields, the previous values ​​will be reset, you must repeat the entry from the beginning.
