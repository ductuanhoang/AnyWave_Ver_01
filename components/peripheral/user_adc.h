/*
 * user_adc.h
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

#ifndef MAIN_USER_DRIVER_PERIPHERAL_USER_ADC_H_
#define MAIN_USER_DRIVER_PERIPHERAL_USER_ADC_H_

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
void user_adc_init(void);

int user_adc_get_value(void);

#endif /* MAIN_USER_DRIVER_PERIPHERAL_USER_ADC_H_ */
