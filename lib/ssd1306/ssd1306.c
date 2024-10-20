

#include "ssd1306.h"

void oled_send(uint8_t cmd, uint8_t data)
{
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET)
    {
    }
    I2C_GenerateSTART(ENABLE); // Send start signal
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
    }
    I2C_Send7bitAddress(OLED_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
    }
    if (I2C_GetFlagStatus(I2C_FLAG_TXE) != RESET) // Wait for the data register to be empty
        I2C_SendData(cmd);  // Fill in data to the data register DATAR
    I2C_SendData(data); // Fill in data to the data register DATAR
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    }
    I2C_GenerateSTOP(ENABLE); // Send stop signal
}

void oled_send_mult(uint8_t *data, int len)
{
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET)
    {
    }
    I2C_GenerateSTART(ENABLE); // Send start signal
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
    }
    I2C_Send7bitAddress(OLED_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
    }
    while (I2C_GetFlagStatus(I2C_FLAG_TXE) != RESET)
    {
    } // Wait for the data register to be empty
    for (int i = 0; i < len; i++)
    {
        I2C_SendData(data[i]);
        while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
        }
    } // Fill in data to the data register DATAR

    I2C_GenerateSTOP(ENABLE); // Send stop signal
}

void oled_cursor(uint8_t row, uint8_t col)
{
    oled_send(0x00, 0xB0 | row);                 // Set Y position
    oled_send(0x00, 0x10 | ((col & 0xF0) >> 4)); // Set X position lower 4 bits
    oled_send(0x00, 0x00 | (col & 0x0F));        // Set the X position to the upper 4 bits
}

void oled_clear(void)
{
    oled_send(0x00, 0xAE); // Turn off the display
    for (int j = 0; j < 8; j++)
    {
        oled_cursor(j, 0);
        {
            for (int i = 0; i < 128; i++)
            { // clear all columns
                oled_send(0x40, 0);
            }
        }
    }
    oled_send(0x00, 0xAF); // Turn on display
    oled_cursor(0, 0);
}

void oled_checkered(void)
{
    oled_send(0x00, 0xAE); // Turn off the display
    for (int j = 0; j < 8; j++)
    {

        for (int i = 0; i < 128; i++)
        { // clear all columns

            if ((i / 8) % 2 != j % 2)
            {
                continue;
            }
            oled_cursor(j, i);
            oled_send(0x40, 0xFF);
        }
    }
    oled_send(0x00, 0xAF); // Turn on display
    oled_cursor(0, 0);
}

void oled_init(void)
{
    oled_send(0x00, 0xAE); // Turn off the display
    oled_send(0x00, 0xD5); // Set the display clock division ratio/oscillator frequency
    oled_send(0x00, 0x80);
    oled_send(0x00, 0xA8); // Set the multiplexing rate
    oled_send(0x00, 0x3F);
    oled_send(0x00, 0xD3); // Set display offset
    oled_send(0x00, 0x00);
    oled_send(0x00, 0x40); // Set display start line
    oled_send(0x00, 0xA1); // Set left and right direction, 0xA1 is normal, 0xA0 is left and right inverted
    oled_send(0x00, 0xC8); // Set up and down direction, 0xC8 is normal, 0xC0 is up and down inverted
    oled_send(0x00, 0xDA); // Set COM pin hardware configuration
    oled_send(0x00, 0x12);
    oled_send(0x00, 0x81); // Set contrast control
    oled_send(0x00, 0xCF);
    oled_send(0x00, 0xD9); // Set precharge cycle
    oled_send(0x00, 0xF1);
    oled_send(0x00, 0xDB); // Set VCOMH deselect level
    oled_send(0x00, 0x30);
    oled_send(0x00, 0xA4); // Set the entire display on/off
    oled_send(0x00, 0xA6); // Set normal/inverted display
    oled_send(0x00, 0x8D); // Set the charge pump
    oled_send(0x00, 0x14); // Set the charge pump
    oled_send(0x00, 0xAF); // Turn on display

    oled_send(0x00, 0x20);
    oled_send(0x00, 0x00);

    oled_clear(); // OLED clears the screen
}

// void oled_clear()
// {
//     // Send commands to set the column and page addresses
//     oled_send(0x00, 0x21); // Set column address command
//     oled_send(0x00, 0x00); // Start column (0)
//     oled_send(0x00, 0x7F); // End column (127)

//     oled_send(0x00, 0x22); // Set page address command
//     oled_send(0x00, 0x00); // Start page (0)
//     oled_send(0x00, 0x07); // End page (7)

//     for (uint16_t b = 0; b < 1024; b++)
//     {
//         oled_send(0x40, b & 0x00); // 1bpp
//     }
// }


void oled_addr(int x1, int x2, int y1, int y2)
{
    oled_send(0x00, 0x21); // Set column address command
    oled_send(0x00, x1);   // Start column
    oled_send(0x00, x2);   // End column

    oled_send(0x00, 0x22); // Set page address command
    oled_send(0x00, y1);   // Start page
    oled_send(0x00, y2);   // End page
}
