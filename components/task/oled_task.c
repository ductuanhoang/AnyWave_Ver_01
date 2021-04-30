/*
 * oled_task.c
 *
 *  Created on: Mar 29, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "oled_task.h"
#include "../../Common.h"

#include "../user_driver/oled_driver/oled_display.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static void oled_task(void *pvParameters);

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/
void lcd_create_task(void)
{
	xTaskCreatePinnedToCore(oled_task, "oled_task", 3 * 1024, NULL, 5 | portPRIVILEGE_BIT, NULL, 1);
}
/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
device_data_t device_data_old;
/***********************************************************************************************************************
* Static Functions
***********************************************************************************************************************/
static void oled_task(void *pvParameters)
{
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	while (1)
	{
		if (device_data.ota_status == E_OTA_NONE)
		{
			if (device_data.bluetooth_status != device_data_old.bluetooth_status && device_data.bluetooth_status == 1)
			{
				device_data_old.bluetooth_status = device_data.bluetooth_status;
				// draw bluetooth icon
				ssd1306_bluetooth_wifi_icon(1, 1);
				APP_LOGD("draw bluetooth icon");
			}
			else if (device_data.bluetooth_status != device_data_old.bluetooth_status && device_data.bluetooth_status == 0)
			{
				device_data_old.bluetooth_status = device_data.bluetooth_status;
				ssd1306_bluetooth_wifi_icon(0, 1);
				APP_LOGD("clear bluetooth icon");
			}

			if (
				((device_data.device_state.power != device_data_old.device_state.power) |
				 ((device_data.device_state.mode != device_data_old.device_state.mode) |
				  (device_data.device_state.level != device_data_old.device_state.level) |
				  (device_data.device_state.uv_led != device_data_old.device_state.uv_led))) &&
				(device_data.device_state.power == 1))
			{
				ssd1306_Fill(Black);
				if (device_data.bluetooth_status == 1)
				{
					ssd1306_bluetooth_wifi_icon(1, 1);
				}
				else
				{
					ssd1306_bluetooth_wifi_icon(0, 1);
				}
				APP_LOGD("power on");
				device_data_old.device_state.power = device_data.device_state.power;
				device_data_old.device_state.mode = device_data.device_state.mode;
				device_data_old.device_state.level = device_data.device_state.level;
				device_data_old.device_state.uv_led = device_data.device_state.uv_led;

				ssd1306_SetDisplayOn(1);
				// update mode
				char lcd_buf[50];
				memset(lcd_buf, 0x00, sizeof(lcd_buf));
				if (device_data_old.device_state.mode == E_MODE_PWM)
				{
					sprintf(lcd_buf, "mode :%s", "PWM"); //convert temperature to string
				}
				else if (device_data_old.device_state.mode == E_MODE_PWM_GCG)
				{
					sprintf(lcd_buf, "mode :%s", "GCG"); //convert temperature to string
				}
				else if (device_data_old.device_state.mode == E_MODE_PIEZO)
				{
					sprintf(lcd_buf, "mode :%s", "WAVE"); //convert temperature to string
				}

				ssd1306_SetCursor(0, 5);
				ssd1306_WriteString(lcd_buf, Font_7x10, White);

				memset(lcd_buf, 0x00, sizeof(lcd_buf));
				// level
				switch (device_data_old.device_state.level)
				{
				case E_INTENSITY_LOW:
					sprintf(lcd_buf, "level : %s", "low"); //convert temperature to string
					break;
				case E_INTENSITY_MID:
					sprintf(lcd_buf, "level : %s", "medium"); //convert temperature to string
					break;
				case E_INTENSITY_HIGH:
					sprintf(lcd_buf, "level : %s", "high"); //convert temperature to string
					break;
				case E_INTENSITY_ULTRA:
					sprintf(lcd_buf, "level : %s", "ultra"); //convert temperature to string
					break;
				default:
					break;
				}
				ssd1306_SetCursor(0, 20);
				ssd1306_WriteString(lcd_buf, Font_7x10, White);

				switch (device_data_old.device_state.uv_led)
				{
				case E_UV_ON:
					sprintf(lcd_buf, "Ultraviolet  : %s", "on"); //convert temperature to string
					break;
				case E_UV_OFF:
					sprintf(lcd_buf, "Ultraviolet  : %s", "off"); //convert temperature to string
					break;
				default:
					break;
				}
				ssd1306_SetCursor(0, 35);

				ssd1306_WriteString(lcd_buf, Font_7x10, White);
				ssd1306_UpdateScreen(); //Update the LCD
			}
			else if (device_data.device_state.power != device_data_old.device_state.power && device_data.device_state.power == 0)
			{
				APP_LOGD("power off");
				device_data_old.device_state.power = device_data.device_state.power;
				ssd1306_Fill(Black);
				char lcd_buf[20];
				sprintf(lcd_buf, "Power off ..."); //convert temperature to string
				ssd1306_SetCursor(0, 5);
				ssd1306_WriteString(lcd_buf, Font_11x18, White);
				ssd1306_UpdateScreen(); //Update the LCD
				vTaskDelay(2000 / portTICK_PERIOD_MS);
				ssd1306_SetDisplayOn(0);
			}
		}
		else
		{
			if (device_data.ota_status != E_OTA_NONE)
			{
				ssd1306_SetDisplayOn(1);
				switch (device_data.ota_status)
				{
				case E_OTA_RUNNING:
				{
					APP_LOGD("E_OTA_RUNNING");
					ssd1306_Fill(Black);
					char lcd_buf[20];
					sprintf(lcd_buf, "ota update ..."); //convert temperature to string
					ssd1306_SetCursor(0, 5);
					ssd1306_WriteString(lcd_buf, Font_11x18, White);
					ssd1306_UpdateScreen(); //Update the LCD
				}
				break;
				case E_OTA_FAIL:
				{
					APP_LOGD("E_OTA_FAIL");
					ssd1306_Fill(Black);
					char lcd_buf[20];
					sprintf(lcd_buf, "firmware ota ..."); //convert temperature to string
					ssd1306_SetCursor(0, 5);
					ssd1306_WriteString(lcd_buf, Font_11x18, White);
					ssd1306_UpdateScreen(); //Update the LCD
				}
				break;
				case E_OTA_DONE:
				{
					APP_LOGD("E_OTA_DONE");
					ssd1306_Fill(Black);
					char lcd_buf[20];
					sprintf(lcd_buf, "device rebot..."); //convert temperature to string
					ssd1306_SetCursor(0, 5);
					ssd1306_WriteString(lcd_buf, Font_11x18, White);
					ssd1306_UpdateScreen(); //Update the LCD
				}
				break;
				default:
					break;
				}
			}
		}

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/