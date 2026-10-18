#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "driver/i2c.h"
#include "esp_err.h"
#include <stdint.h>

// I2C Configuration
#define LCD_ADDR 0x27
#define I2C_SDA_PIN 21 
#define I2C_SCL_PIN 22
#define I2C_NUM I2C_NUM_0
#define I2C_FREQ_HZ 100000

// HD44780 Instruction Set
// Clear Display & Return Home
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02

// Entry Mode Set (0x04)
#define LCD_ENTRYMODESET 0x04
#define LCD_ENTRY_INC 0x02     // Increment cursor (I/D = 1) - Text flows Left to Right
#define LCD_ENTRY_DEC 0x00     // Decrement cursor (I/D = 0) - Text flows Right to Left
#define LCD_ENTRY_SHIFT 0x01   // Shift display (S = 1)
#define LCD_ENTRY_NOSHIFT 0x00 // Don't shift display (S = 0)

// Display On/Off Control (0x08)
#define LCD_DISPLAYCONTROL 0x08
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Cursor or Display Shift (0x10)
#define LCD_CURSORSHIFT 0x10
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// Function Set (0x20)
#define LCD_FUNCTIONSET 0x20
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// RAM Address Set
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// PCF8574 Pin Mapping
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00
#define EN 0b00000100  // Enable bit (P2)
#define RW 0b00000010  // Read/Write bit (P1)
#define RS 0b00000001  // Register select bit (P0)

// User-friendly aliases (Requested Mapping)
#define LCD_CLEAR LCD_CLEARDISPLAY
#define LCD_HOME LCD_RETURNHOME
#define LCD_ENTRY_MODE LCD_ENTRYMODESET
#define LCD_DISPLAY_CONTROL LCD_DISPLAYCONTROL
#define LCD_FUNCTION_SET LCD_FUNCTIONSET
#define LCD_SET_CGRAM_ADDR LCD_SETCGRAMADDR
#define LCD_SET_DDRAM_ADDR LCD_SETDDRAMADDR

// Public Functions
esp_err_t lcd_init(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_clear(void);
void lcd_put_cur(int row, int col);
void lcd_send_string(char *str);
void lcd_send_int(int number);
void lcd_send_float(float number, int precision);
void lcd_backlight_on(void);
void lcd_backlight_off(void);

#endif
