/*
 * user_i2c.h
 *
 *  Created on: Apr 1, 2021
 *      Author: ductu
 */

#ifndef MAIN_USER_DRIVER_PERIPHERAL_USER_I2C_H_
#define MAIN_USER_DRIVER_PERIPHERAL_USER_I2C_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <stdbool.h>
#include "../../Common.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
/** *** I2C Begin ***
 * @brief  Config I2C bus and Install Driver
 * @param  sda_io_num    [GPIO number for SDA line]
 * @param  scl_io_num    [GPIO number for SCL line]
 * @param  sda_pullup_en [Enable internal pullup on SDA line]
 * @param  scl_pullup_en [Enable internal pullup on SCL line]
 * @param  clk_speed     [I2C clock frequency for master mode, (no higher than 1MHz for now), Default 100KHz]
 *                       @see "driver/i2c.h"
 * @return               - ESP_OK   Success
 *                       - ESP_ERR_INVALID_ARG Parameter error
 *                       - ESP_FAIL Driver install error
 */
esp_err_t begin(uint8_t i2c_num, gpio_num_t sda_io_num, gpio_num_t scl_io_num, uint32_t clk_speed);

/**
 * Stop I2C bus and unninstall driver
 */
esp_err_t close();

/**
 * Timeout read and write in milliseconds
 */
void setTimeout(uint32_t ms);


/**
 * *** WRITING interface ***
 * @brief  I2C commands for writing to a 8-bit slave device register.
 *         All of them returns standard esp_err_t codes. So it can be used
 *         with ESP_ERROR_CHECK();
 * @param  devAddr   [I2C slave device register]
 * @param  regAddr   [Register address to write to]
 * @param  bitNum    [Bit position number to write to (bit 7~0)]
 * @param  bitStart  [Start bit number when writing a bit-sequence (MSB)]
 * @param  data      [Value(s) to be write to the register]
 * @param  length    [Number of bytes to write (should be within the data buffer size)]
 *                   [writeBits() -> Number of bits after bitStart (including)]
 * @param  timeout   [Custom timeout for the particular call]
 * @return  - ESP_OK Success
 *          - ESP_ERR_INVALID_ARG Parameter error
 *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
 *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
 *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
 */
esp_err_t writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data, int32_t timeout);
esp_err_t writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data, int32_t timeout);
esp_err_t writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data, int32_t timeout);
esp_err_t writeBytes(uint8_t devAddr, uint8_t regAddr, size_t length, const uint8_t *data, int32_t timeout);

/**
 * *** READING interface ***
 * @breif  I2C commands for reading a 8-bit slave device register.
 *         All of them returns standard esp_err_t codes.So it can be used
 *         with ESP_ERROR_CHECK();
 * @param  devAddr   [I2C slave device register]
 * @param  regAddr   [Register address to read from]
 * @param  bitNum    [Bit position number to write to (bit 7~0)]
 * @param  bitStart  [Start bit number when writing a bit-sequence (MSB)]
 * @param  data      [Buffer to store the read value(s)]
 * @param  length    [Number of bytes to read (should be within the data buffer size)]
 * @param  timeout   [Custom timeout for the particular call]
 * @return  - ESP_OK Success
 *          - ESP_ERR_INVALID_ARG Parameter error
 *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
 *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
 *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.]
 */
esp_err_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, int32_t timeout);
esp_err_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, int32_t timeout);
esp_err_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, int32_t timeout);
esp_err_t readBytes(uint8_t devAddr, uint8_t regAddr, size_t length, uint8_t *data, int32_t timeout);

/**
 * @brief  Quick check to see if a slave device responds.
 * @param  devAddr   [I2C slave device register]
 * @param  timeout   [Custom timeout for the particular call]
 * @return  - ESP_OK Success
 *          - ESP_ERR_INVALID_ARG Parameter error
 *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
 *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
 *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.]
 */
esp_err_t testConnection(uint8_t devAddr, int32_t timeout);

/**
 * I2C scanner utility, prints out all device addresses found on this I2C bus.
 */
void scanner();

#endif /* MAIN_USER_DRIVER_PERIPHERAL_USER_I2C_H_ */
