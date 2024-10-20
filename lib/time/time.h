#ifndef TIME_H
#define TIME_H

#include "CH59x_common.h"

// Function to get the elapsed milliseconds
UINT64 millis(void);

UINT8 weekDay(int day, int month, int year);

void delay(UINT32 millis);

#endif // TIME_H
