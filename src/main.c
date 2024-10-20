

#include <stdbool.h>
#include "CH59x_common.h"
#include "time.h"
#include "ssd1306.h"
#include "lvgl.h"
#include "ui/ui.h"

#define WIDTH 128
#define HEIGHT 64
#define BUF_SIZE 16
#define LVBUF (WIDTH * BUF_SIZE)

static lv_display_t *lvDisplay;
uint8_t lvBuffer[2][LVBUF];
uint8_t scrbuf[1024]; // 128x64 pixels = 1024 bytes

void setPixel(uint16_t x, uint16_t y, bool set)
{
    if ((x < 128) && (y < 64))
    {                                            // Ensure x and y are within bounds
        uint16_t byte_index = (y / 8) * 128 + x; // Calculate byte index in the buffer
        uint8_t bit_position = y % 8;            // Calculate bit index within the byte

        if (set)
        {
            scrbuf[byte_index] |= (1 << bit_position); // Set the corresponding bit
        }
        else
        {
            scrbuf[byte_index] &= ~(1 << bit_position); // Clear the corresponding bit
        }
    }
}

// Function to flush the display
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, unsigned char *data)
{
    // Set the column and page address according to the flush area

    for (int y = area->y1; y <= area->y2; y++)
    {
        for (int x = area->x1; x <= area->x2; x++)
        {
            // Get the grayscale value from LVGL's buffer (8-bit grayscale)
            uint8_t grayscale = *data;
            data++;

            // Convert grayscale to 1-bit (black or white) based on threshold
            setPixel(x, y, grayscale > 128);
        }
    }

    oled_addr(area->x1, area->x2, area->y1 / 8, area->y2 / 8);

    // Send the pixel data for the specific area (page by page, column by column)
    for (uint8_t page = area->y1 / 8; page <= area->y2 / 8; page++)
    {
        for (uint8_t col = area->x1; col <= area->x2; col++)
        {
            uint16_t byte_index = page * 128 + col; // Calculate the byte index for each column in the page
            oled_send(0x40, scrbuf[byte_index]);    // Send the byte (8 vertical pixels in the column)
        }
    }

    // Inform LVGL that flushing is complete
    lv_display_flush_ready(disp);
}

static uint32_t my_tick(void)
{
    return millis();
}

void debug_init(void)
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

lv_timer_t *show_timer = NULL;

int sc = 0;

void show_timer_cb(lv_timer_t *timer)
{
    switch (sc)
    {
    case 1:

        lv_disp_load_scr(ui_Screen1);

        break;
    case 2:
        lv_disp_load_scr(ui_Screen2);
        break;
    case 3:
        lv_disp_load_scr(ui_Screen3);
        break;
    }
    sc++;
    if (sc > 3)
    {
        sc = 1;
    }
}

int main()
{
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    debug_init();

    GPIOB_ModeCfg(GPIO_Pin_14 | GPIO_Pin_15, GPIO_ModeIN_PU);
    I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, 0x42); // Use IIC peripherals, set the clock frequency to 400Kbit/s, set the clock duty cycle to Tlow:Thigh=16:9, 7-bit address, set the host address

    PRINT("IIC Host mode\r\n"); /*IIC_Init*/

    oled_init();

    // oled_checkered();

    lv_init();

    lv_tick_set_cb(my_tick);

    lvDisplay = lv_display_create(WIDTH, HEIGHT);
    lv_display_set_flush_cb(lvDisplay, my_disp_flush);
    lv_display_set_buffers(lvDisplay, lvBuffer[0], lvBuffer[1], LVBUF, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    // lv_obj_t *label1 = lv_label_create(lv_scr_act());
    // lv_obj_set_align(label1, LV_ALIGN_CENTER);
    // lv_obj_set_width(label1, 120);
    // lv_label_set_text(label1, "Hello from CH592F, LVGL 9 here!");
    // lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(label1, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_init();


    show_timer = lv_timer_create(show_timer_cb, 3000, NULL);
    lv_timer_set_repeat_count(show_timer, -1);
    show_timer_cb(show_timer); // Trigger first run

    RTC_InitTime(2024, 7, 22, 11, 56, 45);

    uint16_t py, pmon, pd, ph, pm, ps;

    while (1)
    {
        lv_timer_handler(); // Update the UI-
        
        delay(1000);

        RTC_GetTime(&py, &pmon, &pd, &ph, &pm, &ps);

        // lv_label_set_text_fmt(label1, "%02d:%02d:%02d\n%02d/%02d/%d", ph, pm, ps, pd, pmon, py);

        lv_label_set_text_fmt(ui_Label1, "%02d:%02d:%02d", ph, pm, ps);
    }
}
