#include "j1708.h"

// This function need to send parsed j1708 packet at PC with RS232
void SendJ1708Packet(J1708 *pckt, FIFO *Buf, bool isValid)
{
	if (!isValid)
	{
		Push(Buf, 0xFF);
	}
	for (uint16_t i = 0; i < pckt->Size; ++i)
	{
		if (i == 0)
		{
			Push(Buf, pckt->MID);
		}
		else if (i == pckt->Size - 1)
		{
			Push(Buf, pckt->CRC);
		}
		else
		{
			Push(Buf, pckt->Data[i - 1]);
		}
	}
}

// This function parse input FIFO buffer at J1708 packet
bool GetPacket(FIFO *buf, J1708 *j1708Packet)
{
	bool status = FALSE;
	if (GetSize(buf) != 0)
	{
		status = TRUE;
		j1708Packet->Size = GetSize(buf);
		for (uint16_t i = 0; i < j1708Packet->Size; ++i)
		{
			if (i == 0)
			{ // It's a MID data
				j1708Packet->MID = (uint8_t) Pull(buf);
			}
			else if (i == (j1708Packet->Size - 1))
			{ // It's a CRC data
				uint8_t cCRC = (uint8_t)Pull(buf);
				GetCRC(j1708Packet);
				if (cCRC == j1708Packet->CRC)
				{
					// TODO: Send packet
					SendJ1708Packet(j1708Packet, &TxBuf, TRUE);
				}
				else
				{
					// TODO: Send packet with invalid marker
					SendJ1708Packet(j1708Packet, &TxBuf, FALSE);
				}
			}
			else
			{ // It's a data packets
				j1708Packet->Data[i - 1] = (uint8_t)Pull(buf);
			}
		}
	}
	else
	{
		status = FALSE;
		Clear(buf);
	}
	return status;
}

// This function send J1708 structure to j1708 bus
void SendPacket(J1708 *pckt, FIFO *buf)
{
	Push(buf, pckt->MID);
	for (uint16_t i = 0; i < pckt->Size; ++i)
	{
		Push(buf, pckt->Data[i]);
	}
	GetCRC(pckt);
	Push(buf, pckt->CRC);
}

// This function calculate CRC and write into the current structure
void GetCRC(J1708 *packet)
{
	uint8_t sum = 0;
	for (uint8_t i = 0; i < packet->Size - 2; ++i)
	{
		sum += packet->Data[i];
	}
	packet->CRC = sum ^ 0xFF;
}
