/*
 * spi.h
 *
 *  Created on: Jan 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_USER_DRIVER_PERIPHERAL_USER_SPI_H_
#define MAIN_USER_DRIVER_PERIPHERAL_USER_SPI_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "../../Common.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define TX_SPI_DONE_BIT         (0x1)       // bit 0
#define TX_FIFO_EMPTY_BIT       (0x2)       // bit 1
#define TX_FIFO_NOT_FULL_BIT    (0x4)       // bit 2
#define TX_WORD_COMPLETE_BIT    (0x8)       // bit 3
#define TX_SPI_IDLE_BIT         (0x10)  // bit 4
#define TX_INTERRUPT_BIT        (0x80)  // bit 7

#define RX_FIFO_FULL_BIT        (0x10)  // bit 4
#define RX_FIFO_NOT_EMPTY       (0x20)  // bit 5
#define RX_BUFF_OVERRUN_BIT     (0x40)  // bit 6
#define RX_INTERRUPT            (0x80)  // bit 7

#define SPI_OK_CODE                 (0x0)
#define SPI_RX_EMPTY_CODE           (0x1)
#define SPI_TX_SEND_PROBLEM_CODE    (0x2)

#define SPI_MAX_TRY     (100)
#define NB_DRIVERS_MAX  (2)

#define SPI_MAX_RETRY   (5)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
void spiInit( void );

uint8_t spiWriteVal(uint8_t value);

void spi_gpio_set_cs( void );

void spi_gpio_clear_cs(void);

void spi_gpio_set_dc(void);
void spi_gpio_clear_dc(void);

void spi_gpio_set_res(void);
void spi_gpio_clear_res(void);

#endif /* MAIN_USER_DRIVER_PERIPHERAL_USER_SPI_H_ */
