/*
 * message_parser.c
 *
 *  Created on: Apr 11, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "message_parser.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define BLE_HEADER_01 0xAA
#define BLE_HEADER_02 0x55
#define MAX_DATA_SEND 11
#define MESSAGE_BLE_LENGTH 11

static void message_protocol_cmd_handler(void);
static void message_protocol_parser_cmd(void);
static void message_protocol_send(uint8_t *buf, uint8_t buf_len);

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
typedef enum
{
	E_COMMAND_APP_CONTROL = 0x01,
	E_COMMAND_APP_READ = 0x02,
} e_command_type;

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static uint8_t payload_data[64];
static uint8_t state = 0;
static uint8_t cksA = 0;
static uint8_t cksB = 0;
static uint8_t r_cksA = 0;
static uint8_t r_cksB = 0;
static uint8_t length = 0;
static uint8_t length_message = 0;
static uint8_t length_count = 0;

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
void message_parser_hanlder(uint8_t *message, uint16_t message_length)
{
	APP_LOGD("message ble = ");
	for (int i = 0; i < message_length; i++)
	{
		printf("%x ", message[i]);
	}
	printf("\r\n");
	APP_LOGD("state = %d", state);
	for (int i = 0; i < message_length; i++)
	{
		switch (state)
		{
		case 0:
			/* code */
			if (message[i] == BLE_HEADER_01)
			{
				state = 1;
			}
			break;
		case 1:
			if (message[i] == BLE_HEADER_02)
			{
				state = 2;
			}
			break;
		case 2:
		{

			length = message[i];
			length_count = 0;
			state = 3;
			break;
		}
		case 3:
		{
			if (length_count < length)
			{
				payload_data[length_count] = message[i];
				cksA += message[i];
				length_count++;
				if (length_count == length)
				{
					cksB = cksA + cksA;
					state = 4;
				}
			}
			break;
		}
		case 4:
		{
			r_cksA = message[i];
			state = 5;
			break;
		}
		case 5:
		{
			r_cksB = message[i];
			if (cksA == r_cksA && cksB == r_cksB)
			{
				// process
				message_protocol_cmd_handler();
			}
			APP_LOGD("r_cksA = %x", r_cksA);
			APP_LOGD("r_cksB = %x", r_cksB);

			APP_LOGD("cksA = %x", cksA);
			APP_LOGD("cksB = %x", cksB);
			cksA = 0;
			cksB = 0;
			r_cksB = 0;
			r_cksA = 0;
			length_count = 0;
			length = 0;
			length_message = 0;
			state = 0;
			break;
		}
		default:
			break;
		}
	}
	// reset all
	cksA = 0;
	cksB = 0;
	r_cksB = 0;
	r_cksA = 0;
	length_count = 0;
	length = 0;
	length_message = 0;
	state = 0;
	memset(payload_data, 0x00, sizeof(payload_data));
}
/***********************************************************************************************************************
* Function Name:  message_protocol_cmd_handler
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void message_protocol_cmd_handler(void)
{
	switch (payload_data[0])
	{
	case 0x01:
		message_protocol_parser_cmd();
		break;
	default:
		break;
	}
}

void message_protocol_device_response(void)
{
	//    uint8_t buffer_send[] = "tuan\r\n";
	//    uart_trasmit(buffer_send, strlen((const char *)buffer_send));

	uint8_t tmp_data[MESSAGE_BLE_LENGTH];

	//    tmp_data[0] = E_STM32_RESPONSE;
	//    tmp_data[1] = AnyWay_Data.power;
	//    tmp_data[2] = AnyWay_Data.mode;
	//    tmp_data[3] = AnyWay_Data.frequency >> 8;
	//    tmp_data[4] = AnyWay_Data.frequency & 0x00FF;
	//    tmp_data[5] = AnyWay_Data.error;

	message_protocol_send(tmp_data, MESSAGE_BLE_LENGTH);
}

static void message_protocol_parser_cmd(void)
{
	APP_LOGD("message_protocol_parser_cmd");
	switch (payload_data[1]) // payload power
	{
	case E_POWER_OFF:
		device_data.device_state.power = E_POWER_OFF;
		break;
	case E_POWER_ON:
		device_data.device_state.power = E_POWER_ON;
		break;
	default:
		device_data.device_state.power = E_POWER_OFF;
		break;
	}

	switch (payload_data[2]) // payload Mode
	{
	case E_MODE_PWM:
		device_data.device_state.mode = E_MODE_PWM;
		break;
	case E_MODE_PWM_GCG:
		device_data.device_state.mode = E_MODE_PWM_GCG;
		break;
	case E_MODE_PIEZO:
		device_data.device_state.mode = E_MODE_PIEZO;
		break;
	default:
		device_data.device_state.mode = E_MODE_PWM;
		break;
	}

	switch (payload_data[3]) // payload level
	{
	case E_INTENSITY_LOW:
		device_data.device_state.level = E_INTENSITY_LOW;
		/* code */
		break;
	case E_INTENSITY_MID:
		device_data.device_state.level = E_INTENSITY_MID;
		break;
	case E_INTENSITY_HIGH:
		device_data.device_state.level = E_INTENSITY_HIGH;
		break;
	case E_INTENSITY_ULTRA:
		device_data.device_state.level = E_INTENSITY_ULTRA;
		break;
	default:
		device_data.device_state.level = E_INTENSITY_LOW;
		break;
	}

	switch (payload_data[4]) // uv_led
	{
	case E_UV_OFF:
		device_data.device_state.uv_led = E_UV_OFF;
		/* code */
		break;
	case E_UV_ON:
		device_data.device_state.uv_led = E_UV_ON;
		break;
	default:
		device_data.device_state.uv_led = E_UV_OFF;
		break;
	}

	APP_LOGD("device_data.device_state.power = %d", device_data.device_state.power);
	APP_LOGD("device_data.device_state.mode = %d", device_data.device_state.mode);
	APP_LOGD("device_data.device_state.level = %d", device_data.device_state.level);
	APP_LOGD("device_data.device_state.uv_led = %d", device_data.device_state.uv_led);
}

static void message_protocol_send(uint8_t *buf, uint8_t buf_len)
{
	uint8_t tmp_cksa = 0;
	uint8_t tmp_cksb = 0;
	uint8_t i = 0;

	//    AnyWay_Data.sum_data[0] = BLE_HEADER_01;
	//    AnyWay_Data.sum_data[1] = BLE_HEADER_02;
	//    AnyWay_Data.sum_data[2] = buf_len;
	//
	//    tmp_cksa += buf_len;
	//    tmp_cksb += tmp_cksa;
	//
	//    for (i = 0; i < buf_len; i++)
	//    {
	//        AnyWay_Data.sum_data[3 + i] = buf[i];
	//        tmp_cksa += buf[i];
	//        tmp_cksb += tmp_cksa;
	//    }
	//
	//    AnyWay_Data.sum_data[3 + i] = tmp_cksa;
	//    AnyWay_Data.sum_data[4 + i] = tmp_cksb;
	//
	//    // HAL_UART_Transmit(&huart1, AnyWay_Data.sum_data, buf_len + 5, 100);
	//    uart_trasmit(AnyWay_Data.sum_data, buf_len + 5);
}

/***********************************************************************************************************************
* Static Functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/
