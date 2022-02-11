#pragma once
#include "main.h"
#include "fifo.h"

//Parser for UART_RX_PC
void UART_PC_RX(const volatile uint32_t data, FIFO* buf);
//Parser for UART_TX_PC
void UART_J1708_RX(const volatile uint32_t data, FIFO* buf, bool isEnd);
