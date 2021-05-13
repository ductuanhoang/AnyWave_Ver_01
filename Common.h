/*
 * Common.h
 *
 *  Created on: Apr 24, 2021
 *      Author: ductu
 */

#ifndef MAIN_COMMON_H_
#define MAIN_COMMON_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "main.h"

#define FIRMWARE_VERSION 1.4

enum
{
	E_LOG_LVL_NONE,
	E_LOG_LVL_INFO,
	E_LOG_LVL_ERROR,
	E_LOG_LVL_WARNING,
	E_LOG_LVL_DEBUG,
	E_LOG_LVL_NEVER
};

/* Console color */
#define RESET "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
/*Background colors*/
#define BG_KOLORS_BLK "\x1b[40m" //Black
#define BG_KOLORS_RED "\x1b[41m" //Red
#define BG_KOLORS_GRN "\x1b[42m" //Green
#define BG_KOLORS_YEL "\x1b[43m" //Yellow
#define BG_KOLORS_BLU "\x1b[44m" //Blue

#define LOG_SHOULD_I(level) (level <= LOG_BUILD_LEVEL && level <= E_LOG_LVL_DEBUG)
#define LOG(level, tag, ...)                                       \
	do                                                             \
	{                                                              \
		if (LOG_SHOULD_I(level))                                   \
		{                                                          \
			printf("[%s] %s:%d: " RESET, tag, __func__, __LINE__); \
			printf(__VA_ARGS__);                                   \
			printf("\r\n");                                        \
		}                                                          \
	} while (0)

#define APP_LOGE(...) LOG(E_LOG_LVL_ERROR, KRED "ERROR" RESET, __VA_ARGS__)
#define APP_LOGI(...) LOG(E_LOG_LVL_INFO, KGRN "INFOR" RESET, __VA_ARGS__)
#define APP_LOGD(...) LOG(E_LOG_LVL_DEBUG, KYEL "DEBUG" RESET, __VA_ARGS__)
#define APP_LOGW(...) LOG(E_LOG_LVL_WARNING, BG_KOLORS_YEL "ALARM" RESET, __VA_ARGS__)

#define LOG_BUILD_LEVEL E_LOG_LVL_DEBUG

#define GPIO_OUTPUT_LED_RED 19
#define GPIO_OUTPUT_LED_GREEN 18
#define GPIO_OUTPUT_LED_BLUE 5
#define GPIO_BACK_LIGHT_ENABLE 4

#define PIN_NUM_DC_OLED 22
#define PIN_NUM_REST_OLED 19

#define PIN_NUM_CS 5

#define GPIO_OUTPUT_PIN_CS (1ULL << PIN_NUM_CS)
#define GPIO_OUTPUT_PIN_DC_OLED (1ULL << PIN_NUM_DC_OLED)
#define GPIO_OUTPUT_PIN_REST_OLED (1ULL << PIN_NUM_REST_OLED)

#define GPIO_OUTPUT_NANO_TIMER 23

#define GPIO_BATTERY_DETECT_PG 32
#define GPIO_BATTERY_DETECT_START1 33
#define GPIO_BATTERY_DETECT_START2 25

#define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_OUTPUT_LED_RED) | (1ULL << GPIO_OUTPUT_LED_GREEN) | (1ULL << GPIO_OUTPUT_LED_BLUE) | (1ULL << GPIO_OUTPUT_NANO_TIMER) | (1ULL << GPIO_BACK_LIGHT_ENABLE)

#define GPIO_USER_BUTTON 0
#define GPIO_INPUT_PIN_SEL (1ULL << GPIO_USER_BUTTON)

typedef enum
{
	E_BLUETOOTH_DISCONNECT,
	E_BLUETOOTH_CONNECT
} bluetooth_status_t;

typedef enum
{
	E_WIFI_DISCONNECT,
	E_WIFI_CONNECTED
} wifi_status_t;

typedef enum
{
	E_OTA_NONE,
	E_OTA_START,
	E_OTA_RUNNING,
	E_OTA_FAIL,
	E_OTA_DONE
} ota_status_t;

typedef enum
{
	E_MQTT_DISCONNECT,
	E_MQTT_CONNECTED
} mqtt_status_t;

typedef struct
{
	uint8_t battery_level;
	uint8_t battery_status;
} device_battery_t;

typedef enum
{
	E_MODE_PWM,
	E_MODE_PWM_GCG,
	E_MODE_PIEZO,
} e_device_mode;

typedef enum
{
	E_INTENSITY_LOW,
	E_INTENSITY_MID,
	E_INTENSITY_HIGH,
	E_INTENSITY_ULTRA
} e_level;

typedef enum
{
	e_PWM_LEVEL_LOW = 25, // set 25 % duty cycle
	e_PWM_LEVEL_MID = 50,
	e_PWM_LEVEL_HIGH = 75,
	e_PWM_LEVEL_ULTRA = 90
} e_pwm_level;

typedef enum
{
	e_PWM_GCG_LEVEL_LOW = 50, // 50 Hz
	e_PWM_GCG_LEVEL_MID = 100,
	e_PWM_GCG_LEVEL_HIGH = 150,
	e_PWM_GCG_LEVEL_ULTRA = 200
} e_pwm_gcg_level;

typedef enum // gavanlic
{
	e_PIEZO_LEVEL_LOW = 50, // 50 Hz
	e_PIEZO_LEVEL_MID = 100,
	e_PIEZO_LEVEL_HIGH = 150,
	e_PIEZO_LEVEL_ULTRA = 200
} e_piezo_level;

typedef enum
{
	E_UV_OFF,
	E_UV_ON
} e_Uv_led_status;

typedef enum
{
	E_POWER_OFF,
	E_POWER_ON
} e_device_power;

typedef struct
{
	uint8_t error;
	e_device_power power;
	e_level level;
	e_Uv_led_status uv_led;
	e_device_mode mode;
} device_state_t;

typedef struct
{
	char wifi_name[32];
	char wifi_pass[32];
	char wifi_mac[18];
} wifi_info_t;

typedef struct
{
	device_battery_t device_battery;
	device_state_t device_state;
	bluetooth_status_t bluetooth_status;
	wifi_status_t wifi_status;
	mqtt_status_t mqtt_status;
	ota_status_t ota_status;
} device_data_t;

wifi_info_t wifi_info;
device_data_t device_data;
bool wifi_connected;

TaskHandle_t tcp_handle_task;
TaskHandle_t mqtt_handle_task;
TaskHandle_t ir_tx_handle_task;
TaskHandle_t ir_rx_handle_task;
TaskHandle_t bluetooth_handle_task;

#endif /* MAIN_COMMON_H_ */
