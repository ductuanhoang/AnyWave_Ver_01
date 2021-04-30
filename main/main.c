/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "../Common.h"
#include "../task/ota_task.h"
#include "../task/bluetooth.h"
#include "../task/uart_task.h"
#include "../task/oled_task.h"
#include "../task/wifi_task.h"
#include "../task/mqtt_task.h"
#include "../task/plan_task.h"

#include "../components/logger_file/logger_file.h"
#include "../components/peripheral/user_spi.h"
#include "../components/peripheral/user_adc.h"
#include "../components/json_parser/json_parser.h"

#include "../user_driver/stm32_com/stm32_com.h"
#include "../user_driver/oled_driver/oled_display.h"
#include "../user_driver/mpu9250/mpu9250.h"
/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

void GPIO_Init(void)
{
  gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  //bit mask of the pins that you want to set,e.g.GPIO18/19
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  //disable pull-down mode
  io_conf.pull_down_en = 0;
  //disable pull-up mode
  io_conf.pull_up_en = 0;
  //configure GPIO with the given settings
  gpio_config(&io_conf);

  //interrupt of rising edge
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  //bit mask of the pins, use GPIO0 here
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  //set as input mode
  io_conf.mode = GPIO_MODE_INPUT;
  //enable pull-up mode
  io_conf.pull_up_en = 1;
  esp_err_t error = gpio_config(&io_conf); //configure GPIO with the given settings

  if (error != ESP_OK)
  {
    APP_LOGE("error configuring inputs\n");
  }
}

void app_main(void)
{
  APP_LOGI("--- APP_MAIN: Smart Anywave Update 09/03/2019......");
  APP_LOGI("--- APP_MAIN: Free memory: %d bytes", esp_get_free_heap_size());
  APP_LOGI("--- Firmware version : %.1f", FIRMWARE_VERSION);
  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  GPIO_Init();

  user_adc_init();
  logger_file_init();
  logger_LoadInfo_WifiStation(false);

  vTaskDelay(200 / portTICK_PERIOD_MS);
  ssd1306_Init();

  uart_com2stm32_init();

  char lcd_buf[20];
  sprintf(lcd_buf, "init device"); //convert temperature to string
  ssd1306_SetCursor(0, 5);
  ssd1306_WriteString(lcd_buf, Font_11x18, White);
  ssd1306_UpdateScreen(); //Update the LCD
  ssd1306_Fill(Black);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  ssd1306_start();

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen(); //Update the LCD

  uart_com2stm32_start();
  ble_create_task();
  lcd_create_task();
  plan_task();
  // ota_task();
}
