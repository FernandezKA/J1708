#ifndef _j1708_h_
#define _j1708_h_
#include "main.h"
// User variables and struct
struct j1708
{
	uint8_t MID;
	uint8_t Data[128];
	uint8_t CRC;
	uint16_t Size;
	uint16_t Priority;
};
typedef struct j1708 J1708;
// It's enum for parsing RS232 packet
enum RS232_FSM
{
	getPriority,
	getSize,
	getMID,
	getData,
	Complete
};
// User functions
// This function parse FIFO buffer into J1708 stuct
bool GetPacket(FIFO *buf, J1708 *j1708Packet);
// This function send parsed J1708 structure into j1708 bus
void SendPacket(J1708 *pckt, FIFO *buf);
// This function calculate CRC
void GetCRC(J1708 *packet);
#endif
