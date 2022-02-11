#ifndef _j1708_h_
#define _j1708_h_
#include "main.h"
struct j1708{
	uint8_t MID;
	uint8_t Data[128];
	uint8_t CRC;
	uint16_t Size;
};

typedef struct j1708 J1708;


bool GetPacket(FIFO* buf, J1708* j1708Packet);

void GetCRC(J1708* packet);
#endif
