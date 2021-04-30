/*
 * plan_task.c
 *
 *  Created on: Jan 7, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "mqtt_task.h"
#include "plan_task.h"
#include "../../Common.h"
#include "wifi_task.h"
#include "ota_task.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static void PlantControl_Task(void *pvParameters);

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void plan_task(void)
{
	wifi_task(NULL);
	mqtt_task_start();
	xTaskCreatePinnedToCore(PlantControl_Task, "plant_task", 6 * 1024, NULL, 2 | portPRIVILEGE_BIT, NULL, 1);
}

bool mqtt_start_first_time = false;
/***********************************************************************************************************************
* Static Functions
***********************************************************************************************************************/
static void PlantControl_Task(void *pvParameters)
{
	wifi_connected = false;
	// wifi_task(NULL);
	// mqtt_task_start();
	while (1)
	{
		if (device_data.ota_status == E_OTA_START)
		{
			device_data.ota_status = E_OTA_RUNNING;
			ota_task();
		}
		else if (device_data.ota_status == E_OTA_DONE)
		{
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			device_data.ota_status = E_OTA_NONE;
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			esp_restart();
		}
		else if (device_data.ota_status == E_OTA_FAIL)
		{
			vTaskDelay(1000);
			device_data.ota_status = E_OTA_NONE;
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/
