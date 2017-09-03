/*
 * parse_f.h
 *
 * Created: 29/03/2016 19:50:20
 *  Author: angel
 */ 


#ifndef PARSE_F_H_
#define PARSE_F_H_

#include "header.h"

volatile unsigned char flag_filter_on;
volatile unsigned char filter_weight;

void parse_f(void);
void set_filter_weight(void);



#endif /* PARSE_F_H_ */