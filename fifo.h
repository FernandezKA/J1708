#ifndef _fifo_h_
#define _fifo_h_
#include "main.h"

struct fifo{
	uint32_t Data[256];
	uint16_t Head;
	uint16_t Tail;
};

typedef struct fifo FIFO;

void FifoInit(FIFO* buf);

void Push(FIFO* buf, uint32_t data);

uint32_t Pull(FIFO* buf);

uint16_t GetSize(FIFO* buf);

void Clear(FIFO* buf);
#endif
