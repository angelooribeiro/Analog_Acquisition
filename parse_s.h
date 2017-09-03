/*
 * parse_s.h
 *
 * Created: 29/03/2016 09:55:43
 *  Author: angel
 */ 


#ifndef PARSE_S_H_
#define PARSE_S_H_

#include "header.h"

volatile unsigned char flag_sampling_cycle;			//flag to determinate if is a sampling cycle
volatile unsigned char flag_sampling_run;			//flag to determinate if is a sampling cycle
volatile unsigned char secounds_dec;					//when is a sample period with seconds
volatile unsigned char old_secounds_dec;				//when is a sample period with seconds 


void parse_s(void);		//parse for the letter s
void set_sampling_period(void);	//defines the sampling period of the operation


#endif /* PARSE_S_H_ */