/*
 * ota_task.c
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
#include "ota_task.h"
#include "string.h"

// #include "nvs.h"
// #include "nvs_flash.h"
#include "../../Common.h"
#include "../../main.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"

#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"

/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

#define OTA_URL_SIZE 256

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
#define WEB_URL "https://raw.githubusercontent.com/ductuanhoang/updatefw/main/versionfw.txt"

// extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
// extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");

const char *server_cert_pem_start =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFBjCCBK2gAwIBAgIQDovzdw2S0Zbwu2H5PEFmvjAKBggqhkjOPQQDAjBnMQsw\n"
    "CQYDVQQGEwJVUzEXMBUGA1UEChMORGlnaUNlcnQsIEluYy4xPzA9BgNVBAMTNkRp\n"
    "Z2lDZXJ0IEhpZ2ggQXNzdXJhbmNlIFRMUyBIeWJyaWQgRUNDIFNIQTI1NiAyMDIw\n"
    "IENBMTAeFw0yMTAzMjUwMDAwMDBaFw0yMjAzMzAyMzU5NTlaMGYxCzAJBgNVBAYT\n"
    "AlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4gRnJhbmNpc2Nv\n"
    "MRUwEwYDVQQKEwxHaXRIdWIsIEluYy4xEzARBgNVBAMTCmdpdGh1Yi5jb20wWTAT\n"
    "BgcqhkjOPQIBBggqhkjOPQMBBwNCAASt9vd1sdNJVApdEHG93CUGSyIcoiNOn6H+\n"
    "udCMvTm8DCPHz5GmkFrYRasDE77BI3q5xMidR/aW4Ll2a1A2ZvcNo4IDOjCCAzYw\n"
    "HwYDVR0jBBgwFoAUUGGmoNI1xBEqII0fD6xC8M0pz0swHQYDVR0OBBYEFCexfp+7\n"
    "JplQ2PPDU1v+MRawux5yMCUGA1UdEQQeMByCCmdpdGh1Yi5jb22CDnd3dy5naXRo\n"
    "dWIuY29tMA4GA1UdDwEB/wQEAwIHgDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYB\n"
    "BQUHAwIwgbEGA1UdHwSBqTCBpjBRoE+gTYZLaHR0cDovL2NybDMuZGlnaWNlcnQu\n"
    "Y29tL0RpZ2lDZXJ0SGlnaEFzc3VyYW5jZVRMU0h5YnJpZEVDQ1NIQTI1NjIwMjBD\n"
    "QTEuY3JsMFGgT6BNhktodHRwOi8vY3JsNC5kaWdpY2VydC5jb20vRGlnaUNlcnRI\n"
    "aWdoQXNzdXJhbmNlVExTSHlicmlkRUNDU0hBMjU2MjAyMENBMS5jcmwwPgYDVR0g\n"
    "BDcwNTAzBgZngQwBAgIwKTAnBggrBgEFBQcCARYbaHR0cDovL3d3dy5kaWdpY2Vy\n"
    "dC5jb20vQ1BTMIGSBggrBgEFBQcBAQSBhTCBgjAkBggrBgEFBQcwAYYYaHR0cDov\n"
    "L29jc3AuZGlnaWNlcnQuY29tMFoGCCsGAQUFBzAChk5odHRwOi8vY2FjZXJ0cy5k\n"
    "aWdpY2VydC5jb20vRGlnaUNlcnRIaWdoQXNzdXJhbmNlVExTSHlicmlkRUNDU0hB\n"
    "MjU2MjAyMENBMS5jcnQwDAYDVR0TAQH/BAIwADCCAQUGCisGAQQB1nkCBAIEgfYE\n"
    "gfMA8QB2ACl5vvCeOTkh8FZzn2Old+W+V32cYAr4+U1dJlwlXceEAAABeGq/vRoA\n"
    "AAQDAEcwRQIhAJ7miER//DRFnDJNn6uUhgau3WMt4vVfY5dGigulOdjXAiBIVCfR\n"
    "xjK1v4F31+sVaKzyyO7JAa0fzDQM7skQckSYWQB3ACJFRQdZVSRWlj+hL/H3bYbg\n"
    "IyZjrcBLf13Gg1xu4g8CAAABeGq/vTkAAAQDAEgwRgIhAJgAEkoJQRivBlwo7x67\n"
    "3oVsf1ip096WshZqmRCuL/JpAiEA3cX4rb3waLDLq4C48NSoUmcw56PwO/m2uwnQ\n"
    "prb+yh0wCgYIKoZIzj0EAwIDRwAwRAIgK+Kv7G+/KkWkNZg3PcQFp866Z7G6soxo\n"
    "a4etSZ+SRlYCIBSiXS20Wc+yjD111nPzvQUCfsP4+DKZ3K+2GKsERD6d\n"
    "-----END CERTIFICATE-----\n";
/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static const char *TAG = "user_ota";
static void simple_ota_example_task(void *pvParameter);

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/
static char rcv_buffer[500];
static int length_buffer = 0;
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        strncpy(rcv_buffer, (char *)evt->data, evt->data_len);
        length_buffer = evt->data_len;
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}
/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void ota_task(void)
{
    xTaskCreatePinnedToCore(simple_ota_example_task, "ota_example_task", 8192, NULL, 5 | portPRIVILEGE_BIT, NULL, 1);
}

/***********************************************************************************************************************
* Static Functions
***********************************************************************************************************************/
static void simple_ota_example_task(void *pvParameter)
{
    printf("check version firmware...\n");

    esp_http_client_config_t config = {
        .url = WEB_URL,
        .event_handler = _http_event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    //download the json file
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        cJSON *json = cJSON_ParseWithLength(rcv_buffer, length_buffer);
        printf("rcv_buffer = %s\n", rcv_buffer);
        if (json == NULL)
        {
            printf("file download not json format, aborting ...\n");
        }
        else
        {
            cJSON *version = cJSON_GetObjectItemCaseSensitive(json, "version");
            cJSON *file = cJSON_GetObjectItemCaseSensitive(json, "file");

            if (!cJSON_IsNumber(version))
                printf("unable read version of firmware, aborting ...\n");
            else
            {
                double new_version = version->valuedouble;
                if (new_version > FIRMWARE_VERSION)
                {
                    printf("new version firmware need update now\n");
                    if (cJSON_IsString(file) && file->valuestring != NULL)
                    {
                        device_data.ota_status = E_OTA_RUNNING;
                        ESP_LOGI(TAG, "Starting OTA .....");

                        esp_http_client_config_t ota_config = {
                            .url = file->valuestring,
                            .cert_pem = NULL,
                            // .timeout_ms = 60000,
                            // .event_handler = _http_event_handler,
                            // .port = 80,
                            // .use_global_ca_store = true,
                            // .skip_cert_common_name_check = true,
                        };

                        esp_err_t ret = esp_https_ota(&ota_config);
                        if (ret == ESP_OK)
                        {
                            ESP_LOGI(TAG, "OTA done");
                            device_data.ota_status = E_OTA_DONE;
                            // esp_restart();
                        }
                        else
                        {
                            device_data.ota_status = E_OTA_FAIL;
                            ESP_LOGE(TAG, "Firmware upgrade failed");
                        }
                    }
                    else
                    {
                        printf("unable file name for update ...\n");
                        device_data.ota_status = E_OTA_FAIL;
                    }
                }
            }
        }
        length_buffer = 0;
        memset(rcv_buffer, 0x00, sizeof(rcv_buffer));
    }
    else
    {
        length_buffer = 0;
        memset(rcv_buffer, 0x00, sizeof(rcv_buffer));
        device_data.ota_status = E_OTA_FAIL;
        printf("unable download file , aborting update firmware.... \n");
    }

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/
