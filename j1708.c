#include "j1708.h"
//This function parse input FIFO buffer at J1708 packet
bool GetPacket(FIFO* buf, J1708* j1708Packet){
	bool status = FALSE;
	if(GetSize(buf)!= 0){
		status = TRUE;
		j1708Packet->Size = GetSize(buf);
		for(uint16_t i = 0; i < j1708Packet->Size; ++i){
			if(i == 0){//It's a MID data
				j1708Packet->MID =(uint8_t) Pull(buf);
			}
			else if (i == (j1708Packet-> Size -1)){//It's a CRC data
				uint8_t cCRC = (uint8_t) Pull(buf);
				GetCRC(j1708Packet);
				if(cCRC == j1708Packet->CRC){
					//TODO: Send packet
				}
				else{
					//TODO: Send packet with invalid marker
				}
			}
			else{//It's a data packets
				j1708Packet->Data[i - 1] =(uint8_t) Pull(buf);
			}
		}
	}
	else{
		status = FALSE;
		Clear(buf);
	}
	return status;
}

void GetCRC(J1708* packet){
uint8_t sum = 0;
	for(uint8_t i = 0; i < packet->Size; ++i){
		sum+=packet->Data[i];
	}
	packet->CRC= sum^0xFF;
}
