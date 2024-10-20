

#include "CH59x_common.h"


#define OLED_ADDR 0x78


void oled_send(uint8_t cmd, uint8_t data);
void oled_send_mult(uint8_t *data, int len);
void oled_cursor(uint8_t row, uint8_t col);
void oled_clear(void);
void oled_checkered(void);
void oled_init(void);
void oled_addr(int x1, int x2, int y1, int y2);
