#include "i2c_lcd.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <unistd.h>

static const char *TAG = "LCD";

// Biến cục bộ lưu trạng thái đèn nền
static uint8_t lcd_backlight_state = LCD_BACKLIGHT;

static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ, 
    };
    i2c_param_config(I2C_NUM, &conf);
    return i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0);
}

static void lcd_write_nibble(uint8_t nibble, uint8_t mode)
{
    uint8_t data = (nibble & 0xF0) | mode | lcd_backlight_state;
    uint8_t tx_data[2];
    tx_data[0] = data | EN;
    tx_data[1] = data;
    
    esp_err_t err = i2c_master_write_to_device(I2C_NUM, LCD_ADDR, tx_data, 2, 100 / portTICK_PERIOD_MS);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C Write Failed (Addr: 0x%02X)", LCD_ADDR);
    }
}

static void lcd_send_byte(uint8_t data, uint8_t mode)
{
    uint8_t data_u = (data & 0xF0);
    uint8_t data_l = ((data << 4) & 0xF0);
    lcd_write_nibble(data_u, mode);
    lcd_write_nibble(data_l, mode);
}

void lcd_send_cmd(uint8_t cmd) { lcd_send_byte(cmd, 0); }
void lcd_send_data(uint8_t data) { lcd_send_byte(data, RS); }

void lcd_clear(void)
{
    lcd_send_cmd(LCD_CLEAR);
    vTaskDelay(20 / portTICK_PERIOD_MS); 
}

void lcd_put_cur(int row, int col)
{
    switch (row) {
        case 0: col |= 0x80; break;
        case 1: col |= 0xC0; break;
    }
    lcd_send_cmd(col);
}

void lcd_backlight_on(void) {
    lcd_backlight_state = LCD_BACKLIGHT;
    uint8_t data = lcd_backlight_state; 
    i2c_master_write_to_device(I2C_NUM, LCD_ADDR, &data, 1, 100 / portTICK_PERIOD_MS);
}

void lcd_backlight_off(void) {
    lcd_backlight_state = LCD_NOBACKLIGHT;
    uint8_t data = lcd_backlight_state;
    i2c_master_write_to_device(I2C_NUM, LCD_ADDR, &data, 1, 100 / portTICK_PERIOD_MS);
}

esp_err_t lcd_init(void)
{
    esp_err_t err = i2c_master_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C Init Failed");
        return err;
    }
    
    ESP_LOGI(TAG, "LCD Initialized at Address: 0x%02X", LCD_ADDR);

    // Initialization Sequence
    vTaskDelay(50 / portTICK_PERIOD_MS); 
    lcd_write_nibble(0x30, 0); vTaskDelay(5 / portTICK_PERIOD_MS); 
    lcd_write_nibble(0x30, 0); vTaskDelay(5 / portTICK_PERIOD_MS); 
    lcd_write_nibble(0x30, 0); vTaskDelay(5 / portTICK_PERIOD_MS);
    lcd_write_nibble(0x20, 0); vTaskDelay(5 / portTICK_PERIOD_MS);

    lcd_send_cmd(LCD_FUNCTION_SET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS); 
    vTaskDelay(2 / portTICK_PERIOD_MS);
    lcd_send_cmd(LCD_DISPLAY_CONTROL | LCD_DISPLAYOFF);
    vTaskDelay(2 / portTICK_PERIOD_MS);
    lcd_clear(); 
    lcd_send_cmd(LCD_ENTRY_MODE | LCD_ENTRY_INC | LCD_ENTRY_NOSHIFT); 
    vTaskDelay(2 / portTICK_PERIOD_MS);
    lcd_send_cmd(LCD_DISPLAY_CONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF); 
    
    return ESP_OK;
}

void lcd_send_string(char *str) { while (*str) lcd_send_data((uint8_t)(*str++)); }
void lcd_send_int(int number) { char buffer[16]; sprintf(buffer, "%d", number); lcd_send_string(buffer); }
void lcd_send_float(float number, int precision) {
    char buffer[16], format[10];
    sprintf(format, "%%.%df", precision);
    sprintf(buffer, format, number);
    lcd_send_string(buffer);
}