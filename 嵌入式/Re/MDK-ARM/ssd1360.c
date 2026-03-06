#include "ssd1360.h"
#include "spi.h"
#include <stdio.h>

// ????(??CubeMX??)
#define OLED_CS_PIN     GPIO_PIN_4
#define OLED_CS_PORT    GPIOA
#define OLED_DC_PIN     GPIO_PIN_3
#define OLED_DC_PORT    GPIOA

#define OLED_CS_L()     HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_RESET)
#define OLED_CS_H()     HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_SET)
#define OLED_DC_CMD()   HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_RESET)  // ????
#define OLED_DC_DATA()  HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_SET)    // ????

extern SPI_HandleTypeDef hspi1;

// 6x8 ASCII??(??????????,????????)
static const uint8_t font6x8[][6] = {
    {0x00,0x00,0x00,0x00,0x00,0x00}, // ??
    {0x00,0x00,0x5F,0x00,0x00,0x00}, // !
    {0x00,0x07,0x00,0x07,0x00,0x00}, // "
    // ... ???,???????0-9???,???????
    {0x3E,0x51,0x49,0x45,0x3E,0x00}, // 0
    {0x00,0x42,0x7F,0x40,0x00,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46,0x00}, // 2
    {0x21,0x41,0x45,0x4B,0x31,0x00}, // 3
    {0x18,0x14,0x12,0x7F,0x10,0x00}, // 4
    {0x27,0x45,0x45,0x45,0x39,0x00}, // 5
    {0x3C,0x4A,0x49,0x49,0x30,0x00}, // 6
    {0x01,0x71,0x09,0x05,0x03,0x00}, // 7
    {0x36,0x49,0x49,0x49,0x36,0x00}, // 8
    {0x06,0x49,0x49,0x29,0x1E,0x00}, // 9
    // ????? A-Z ?...
};

void SSD1306_Init(void) {
    OLED_CS_L();  // ????
    OLED_DC_CMD(); // ????

    uint8_t init_cmd[] = {
        0xAE,       // ????
        0xD5, 0x80, // ??????
        0xA8, 0x3F, // ??????
        0x20, 0x00, // ???????????
        0x8D, 0x14, // ?????
        0xA1,       // ????(?127???SEG0)
        0xC8,       // ????(COM0~COM63)
        0xD3, 0x00, // ??????
        0x40,       // ?????
        0xA6,       // ????
        0xA4,       // ????(??RAM??)
        0xAF        // ????
    };
    HAL_SPI_Transmit(&hspi1, init_cmd, sizeof(init_cmd), HAL_MAX_DELAY);
    OLED_CS_H();  // ????
}

void SSD1306_Clear(void) {
    OLED_CS_L();
    // ??????? 0~127
    OLED_DC_CMD();
    uint8_t col_range[] = {0x21, 0x00, 0x7F};
    HAL_SPI_Transmit(&hspi1, col_range, sizeof(col_range), HAL_MAX_DELAY);
    // ??????? 0~7
    uint8_t page_range[] = {0x22, 0x00, 0x07};
    HAL_SPI_Transmit(&hspi1, page_range, sizeof(page_range), HAL_MAX_DELAY);

    OLED_DC_DATA();  // ???????(??:?0)
    uint8_t clear_data[128] = {0};
    for (int page = 0; page < 8; page++) {
        HAL_SPI_Transmit(&hspi1, clear_data, 128, HAL_MAX_DELAY);
    }
    OLED_CS_H();
}

void SSD1306_SetCursor(uint8_t x, uint8_t y) {
    // x: 0~127, y: 0~7 (?)
    OLED_CS_L();
    OLED_DC_CMD();
    uint8_t cmd[] = {0xB0 + y, (uint8_t)(x & 0x0F), (uint8_t)(0x10 | (x >> 4))};
    HAL_SPI_Transmit(&hspi1, cmd, sizeof(cmd), HAL_MAX_DELAY);
    OLED_CS_H();
}

// ??????(6x8??)
static void SSD1306_WriteChar(char ch) {
    OLED_CS_L();
    OLED_DC_DATA();
    // ????:?????0-9
    if (ch >= '0' && ch <= '9') {
        HAL_SPI_Transmit(&hspi1, (uint8_t*)font6x8[ch - '0' + 10], 6, HAL_MAX_DELAY);
    } else {
        // ????????
        HAL_SPI_Transmit(&hspi1, (uint8_t*)font6x8[0], 6, HAL_MAX_DELAY);
    }
    OLED_CS_H();
}

void SSD1306_WriteString(char *str) {
    while (*str) {
        SSD1306_WriteChar(*str++);
    }
}

void SSD1306_WriteInt(uint16_t value) {
    char buffer[6];
    sprintf(buffer, "%d", value);
    SSD1306_WriteString(buffer);
}