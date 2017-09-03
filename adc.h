/*
 * adc.h
 *
 * Created: 07/03/2016 16:33:06
 *  Author: Nuno
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "header.h"

extern volatile unsigned int* linear_table;	/*redden value*/
volatile unsigned char volatile last_sample;			//to store the last resutl from the filter

extern void set_analog_in(void);
void config_timer1(void);			//generate a interrupt to adc
void adc_configuration(void);		//default configurations of adc
void data_adc(void);			//routine for be used in the interrupt
void isr_timer1(void);
void filter_function(void);
void linear_function(void);
#endif /* ADC_H_ */