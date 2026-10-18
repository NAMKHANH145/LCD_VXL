#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "i2c_lcd.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    // Khởi tạo LCD và kiểm tra lỗi
    esp_err_t ret = lcd_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "LCD Init failed. Check wiring and address!");
        return;
    }

    // Test hiển thị chuỗi
    lcd_clear();
    lcd_put_cur(0, 0);
    lcd_send_string("LCD Driver OK!");

    // Test hiển thị số
    lcd_put_cur(1, 0);
    lcd_send_string("Count: ");
    
    int count = 0;
    while(1) {
        lcd_put_cur(1, 7); // Di chuyển con trỏ đến vị trí sau chữ "Count: "
        lcd_send_int(count++);
        
        // Nháy đèn nền mỗi 5 giây để test
        if (count % 10 == 0) {
            lcd_backlight_off();
            vTaskDelay(200 / portTICK_PERIOD_MS);
            lcd_backlight_on();
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
