/*
 * parse_l.h
 *
 * Created: 30/03/2016 18:43:21
 *  Author: angel
 */ 


#ifndef PARSE_L_H_
#define PARSE_L_H_

#include "header.h"

volatile unsigned char flag_linear_on;		/*indicates if linear must be used*/
volatile unsigned char linear_was_on;		/*indicates if the buffers have the values*/
volatile unsigned int* linear_table;	/*redden value*/

void parse_l(void);									/*parse the l information*/
void read_linear_values(void);						/*read the values from eeprom to the linear vectors*/
unsigned int linear_value(unsigned int value);		/*make the linearization*/

#endif /* PARSE_L_H_ */