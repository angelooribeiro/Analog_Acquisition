/*
 * parse_buffer.h
 *
 * Created: 28/03/2016 14:41:54
 *  Author: angel
 */ 

#include "header.h"

#ifndef PARSE_BUFFER_H_
#define PARSE_BUFFER_H_

#define OLEN_ADC 1024
#define MASk_ADC (OLEN_ADC-1)
/*variables to manipulate the buffer*/
volatile int Adc_Buffer[OLEN_ADC];			/*stores the redden information from Adc*/
volatile unsigned int adc_buff_start;		//transmission buffer start index
volatile unsigned int adc_buff_end;			//transmission buffer end index
volatile unsigned int adc_buffer_size;			//size of the buffer
volatile unsigned char flag_cycle_complet;			//flag to declare that sampling cycle maskes one lap

void parse_buffer(void);
void set_buffer_size(void);		/*Redefines the size fo the buffer*/
void read_buffer_internal(void); /* Reads x bytes from buffer in internal units (adc vlaue)*/
void read_buffer_external(void);  /* Reads x bytes from buffer in external units (input value)*/
void read_buffer_eng_units(void); /* Reads x bytes from buffer in engineering units (real value)*/


#endif /* PARSE_BUFFER_H_ */