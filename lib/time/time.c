#include "time.h"

static uint16_t last_tick_count = 0;
static uint64_t overflow_count = 0;

// Assuming 32768 ticks per second
#define TICKS_PER_SECOND 32768
#define TICKS_PER_MILLISECOND (TICKS_PER_SECOND / 1000)

UINT64 millis(void) {
    uint16_t current_tick_count = R16_RTC_CNT_32K;
    UINT64 elapsed_ticks;

    // Check for overflow
    if (current_tick_count < last_tick_count) {
        // An overflow has occurred
        overflow_count++;
    }

    // Calculate the total elapsed ticks accounting for overflow
    elapsed_ticks = ((UINT64)overflow_count << 16) + current_tick_count;

    // Update the last tick count
    last_tick_count = current_tick_count;

    // Convert ticks to milliseconds
    return elapsed_ticks / TICKS_PER_MILLISECOND;
}


UINT8 weekDay(int day, int month, int year)
{
    if (month < 3)
    {
        month += 12;
        year -= 1;
    }

    int K = year % 100;
    int J = year / 100;
    int f = day + 13 * (month + 1) / 5 + K + K/5 + J/4 + 5*J;

    return f % 7;
}

void delay(UINT32 millis)
{
    DelayMs(millis);
}
