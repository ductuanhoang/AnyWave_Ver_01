/*
 * oled_display.h
 *
 *  Created on: May 26, 2020
 *      Author: Yolo
 */

#ifndef _OLED_DISPLAY_H_
#define _OLED_DISPLAY_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "../../Common.h"
#include "fonts2/fonts.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
//
///****************************************************************************/
///***        Type Definitions                                              ***/
///****************************************************************************/
//
//
///****************************************************************************/
///***         Exported global functions                                     ***/
///****************************************************************************/
//// i2c1 is chosen. In case using I2C2, change to hi2c2
//#define SSD1306_I2C_PORT			hi2c1
//// SSD1306 I2C address
//#define SSD1306_I2C_ADDR        0x78
//// SSD1306 width in pixels
//#define SSD1306_WIDTH           128
//// SSD1306 LCD height in pixels
//#define SSD1306_HEIGHT          64
//
//typedef enum {
//	Black = 0x00, /*!< Black color, no pixel */
//	White = 0x01  /*!< Pixel is set. Color depends on LCD */
//} SSD1306_COLOR;
//
//
//typedef struct {
//	uint16_t CurrentX;
//	uint16_t CurrentY;
//	uint8_t Inverted;
//	uint8_t Initialized;
//} SSD1306_t;
//
//
//// extern I2C_HandleTypeDef SSD1306_I2C_PORT;
//
//
//uint8_t ssd1306_Init(void);
//
//
//void ssd1306_Fill(SSD1306_COLOR color);
//void ssd1306_UpdateScreen(void);
//void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
//char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
//char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
//void ssd1306_SetCursor(uint8_t x, uint8_t y);
//
//void ssd1306_WriteCommand(uint8_t command);
//void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR c);
//void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR c);
//uint8_t SSD1306_DrawSignalStrength(uint16_t x, uint16_t y, uint8_t bar);
//void SSD1306_DisplayOff(bool status);
//
//void ssd1306_SetContrast(const uint8_t value);
// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif

#ifndef SSD1306_BUFFER_SIZE
#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8
#endif

#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define YELLOW 0xFFE0

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

typedef enum {
    SSD1306_OK = 0x00,
    SSD1306_ERR = 0x01  // Generic error.
} SSD1306_Error_t;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
    uint8_t DisplayOn;
} SSD1306_t;
typedef struct {
    uint8_t x;
    uint8_t y;
} SSD1306_VERTEX;

// Procedure definitions
void ssd1306_Init(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);
void ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color);
void ssd1306_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_COLOR color);
void ssd1306_Polyline(const SSD1306_VERTEX *par_vertex, uint16_t par_size, SSD1306_COLOR color);
void ssd1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);
/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void ssd1306_SetContrast(const uint8_t value);
/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void ssd1306_SetDisplayOn(const uint8_t on);
/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t ssd1306_GetDisplayOn();

// Low-level procedures
void ssd1306_Reset(void);
void ssd1306_WriteCommand(uint8_t byte);
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size);
SSD1306_Error_t ssd1306_FillBuffer(uint8_t* buf, uint32_t len);

void ssd1306_drawBitmap(int16_t x, int16_t y, uint8_t bitmap[], int16_t w,
                              int16_t h, uint16_t color);
void ssd1306_start(void);


void ssd1306_bluetooth_wifi_icon(bool ble_status, bool wifi_status);

#endif /* _OLED_DISPLAY_H_ */
