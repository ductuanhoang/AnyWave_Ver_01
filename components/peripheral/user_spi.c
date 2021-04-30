/*
 * spi.c
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
#include <driver/spi_master.h>
#include "user_spi.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18


//To speed up transfers, every SPI transfer sends a bunch of lines. This define specifies how many. More means more memory use,
//but less overhead for setting up / finishing transfers. Make sure 240 is dividable by this.
#define PARALLEL_LINES 16
/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/


/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/
static spi_device_handle_t spi;
/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void spi_pre_transfer_callback(spi_transaction_t *t)
{

}

static void spi_post_transfer_callback(spi_transaction_t *t)
{
//  APP_LOGI("call back transmit done");
}

void spi_gpio_set_cs( void )
{
	gpio_set_level(PIN_NUM_CS, 1);
}

void spi_gpio_clear_cs(void)
{
	gpio_set_level(PIN_NUM_CS, 0);
}

void spi_gpio_set_dc(void)
{
	gpio_set_level(PIN_NUM_DC_OLED, 1);
}

void spi_gpio_clear_dc(void)
{
	gpio_set_level(PIN_NUM_DC_OLED, 0);
}


void spi_gpio_clear_res(void)
{
	gpio_set_level(PIN_NUM_REST_OLED, 0);
}

void spi_gpio_set_res(void)
{
	gpio_set_level(PIN_NUM_REST_OLED, 1);
}

void spi_gpio_init(void)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_DC_OLED |GPIO_OUTPUT_PIN_REST_OLED ;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

//    gpio_set_level(PIN_NUM_CS, 1); /* On power*/
    gpio_set_level(PIN_NUM_DC_OLED, 1); /* On power*/
    gpio_set_level(PIN_NUM_REST_OLED, 1); /* On power*/
}

void spiInit( void )
{
	spi_gpio_init();

    APP_LOGD("spiInit call");
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .miso_io_num = -1, // unuse //PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = PARALLEL_LINES * 320 * 2 + 8
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1 * 1000 * 1000,         //Clock out at 10 MHz
        .mode = 0,                              //SPI mode 0
        .spics_io_num = PIN_NUM_CS,             //CS pin
        .queue_size = 20,                        //We want to be able to queue 7 transactions at a time
        .pre_cb = spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
        .post_cb = spi_post_transfer_callback,
    };

    //Initialize the SPI bus
    ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    assert(ret == ESP_OK);

    //Attach the LCD to the SPI bus
    ret = spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
    assert(ret == ESP_OK);

    APP_LOGD("spiInit oke ret = %d", ret);
}

uint8_t spiWriteVal(uint8_t value)
{
    esp_err_t ret;
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length = 8;                   //Command is 8 bits
    t.tx_buffer = &value;             //The data is the cmd itself
    t.user = (void*)0;              //D/C needs to be set to 0
//    ret = spi_device_polling_transmit(spi, &t); //Transmit!
    ret = spi_device_transmit(spi, &t);
    assert(ret == ESP_OK);          //Should have had no issues.

    return SPI_OK_CODE;
}


/***********************************************************************************************************************
* Static Functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/

