/*
 * stm32_com.c
 *
 *  Created on: Jan 9, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "stm32_com.h"
#include "../../peripheral/user_uart.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define MAX_DATA_SEND 12
/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
#define HEADER_01 0xAA
#define HEADER_02 0x55
#define MESSAGE_LENGTH 7
/*
 * data header/power/mode/frequency/checksum
 * */
typedef union
{
	uint8_t sum_data[MAX_DATA_SEND];
	struct
	{
		uint8_t header_1;
		uint8_t header_2;
		uint8_t lenght_cmd;
		uint8_t command;
		uint8_t power;
		uint8_t mode;
		uint8_t uv_led;
		uint8_t error;
		uint16_t frequency;
		uint8_t checksum_01;
		uint8_t checksum_02;
	};
} AnyWay_Data_t;

AnyWay_Data_t AnyWay_Data;

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static void vsm_protocol_device_response(e_anywave_command command);
static void vsm_protocol_send(uint8_t *buf, uint8_t buf_len);

static void anywave_com_process_task(void *pvParameters);
static void anywave_com_process( void ); // call in loop

static e_esp32stm32_com esp32stm32_com = E_ESP_GET_STATE;

static bool have_message_comming = false;
/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/


/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/


void anywave_tx_process_task_creat(void)
{
	xTaskCreatePinnedToCore(anywave_com_process_task, "anywave_com", 2 * 1024, NULL, 5 | portPRIVILEGE_BIT, NULL, 1);
}

static void anywave_com_process_task(void *pvParameters)
{
	APP_LOGD("anywave_com_process_task call");
	uint32_t count_message = 0;
	while (1)
	{
		anywave_com_process();
		if( have_message_comming == false)
		{
			if( count_message % 10)
			{
				APP_LOGD("Connection error");
			}
			count_message++;
		}
		else
		{
			count_message = 0;
			have_message_comming = false;
		}
		APP_LOGD("Connection error 2");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void anywave_set_event(e_esp32stm32_com event)
{
	esp32stm32_com = event;
}
/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void anywave_com_process( void )
{
	switch (esp32stm32_com)
	{
		case E_ESP_NONE:
			break;
		case E_ESP_GET_STATE:
			vsm_protocol_device_response(E_COMMAND_GET_STATE);
			esp32stm32_com = E_ESP_SET_STATE;
			break;
		case E_ESP_SET_STATE:
			vsm_protocol_device_response(E_COMMAND_SET_STATE);
			esp32stm32_com = E_ESP_GET_STATE;
			break;
		case E_STM32_RESPONSE:
			break;
		case E_STM32_TIMEOUT:
			break;
		default:
			break;
	}
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
bool anywave_parser_message(uint8_t *message, uint16_t length)
{
	have_message_comming = true;
	bool status = false;
	AnyWay_Data_t _AnyWay_Data;
	if( length == MAX_DATA_SEND)
	{
		for (int i = 0; i < length; i++)
		{
			_AnyWay_Data.sum_data[i] = message[i];
			APP_LOGD("data = %x", _AnyWay_Data.sum_data[i]);
		}
		if( _AnyWay_Data.command == E_COMMAND_RESPONSE)
		{
			device_data.device_state.error = _AnyWay_Data.error; // get only error state from stm32
			APP_LOGD("_AnyWay_Data.error %d", _AnyWay_Data.error);
		}
		APP_LOGD("Command %d", _AnyWay_Data.command);
	}
	else
	{
		status = false;
		APP_LOGE("error length");
	}

	return status;
}

static void vsm_protocol_device_response(e_anywave_command command)
{
    uint8_t tmp_data[MESSAGE_LENGTH];
    uint16_t level_for_send = 0;

    tmp_data[0] = command;
    tmp_data[1] = device_data.device_state.power;
    tmp_data[2] = device_data.device_state.mode;
    tmp_data[3] = device_data.device_state.uv_led;

    if( device_data.device_state.mode == E_MODE_PWM)
    {
    	switch (device_data.device_state.level) {
			case E_INTENSITY_LOW:
				level_for_send = e_PWM_LEVEL_LOW;
				break;
			case E_INTENSITY_MID:
				level_for_send = e_PWM_LEVEL_MID;
				break;
			case E_INTENSITY_HIGH:
				level_for_send = e_PWM_LEVEL_HIGH;
				break;
			case E_INTENSITY_ULTRA:
				level_for_send = e_PWM_LEVEL_ULTRA;
				break;
			default:
				break;
		}
    }
    else if( device_data.device_state.mode == E_MODE_PWM_GCG)
    {
    	switch (device_data.device_state.level) {
			case E_INTENSITY_LOW:
				level_for_send = e_PWM_GCG_LEVEL_LOW;
				break;
			case E_INTENSITY_MID:
				level_for_send = e_PWM_GCG_LEVEL_MID;
				break;
			case E_INTENSITY_HIGH:
				level_for_send = e_PWM_GCG_LEVEL_HIGH;
				break;
			case E_INTENSITY_ULTRA:
				level_for_send = e_PWM_GCG_LEVEL_ULTRA;
				break;
			default:
				break;
		}
    }
    else if( device_data.device_state.mode == E_MODE_PIEZO)
    {
    	switch (device_data.device_state.level) {
			case E_INTENSITY_LOW:
				level_for_send = e_PIEZO_LEVEL_LOW;
				break;
			case E_INTENSITY_MID:
				level_for_send = e_PIEZO_LEVEL_MID;
				break;
			case E_INTENSITY_HIGH:
				level_for_send = e_PIEZO_LEVEL_HIGH;
				break;
			case E_INTENSITY_ULTRA:
				level_for_send = e_PIEZO_LEVEL_ULTRA;
				break;
			default:
				break;
		}
    }

	tmp_data[4] = level_for_send>> 8;
	tmp_data[5] = level_for_send & 0x00FF;

    tmp_data[6] = 0x00; // don't send error to slave just only get from slave

    vsm_protocol_send(tmp_data, MESSAGE_LENGTH);
}

/***********************************************************************************************************************
* Static Functions
***********************************************************************************************************************/

static void vsm_protocol_send(uint8_t *buf, uint8_t buf_len)
{
    uint8_t tmp_cksa = 0;
    uint8_t tmp_cksb = 0;
    uint8_t i = 0;

    AnyWay_Data.sum_data[0] = HEADER_01;
    AnyWay_Data.sum_data[1] = HEADER_02;
    AnyWay_Data.sum_data[2] = buf_len;

    tmp_cksa += buf_len;
    tmp_cksb += tmp_cksa;

    for (i = 0; i < buf_len; i++)
    {
        AnyWay_Data.sum_data[3 + i] = buf[i];
        tmp_cksa += buf[i];
        tmp_cksb += tmp_cksa;
    }

    AnyWay_Data.sum_data[3 + i] = tmp_cksa;
    AnyWay_Data.sum_data[4 + i] = tmp_cksb;

    UserUart_UartTX(AnyWay_Data.sum_data, MAX_DATA_SEND);
}
/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/

