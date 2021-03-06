/*
 * heder.h
 *
 * Created: 23/02/2016 15:16:14
 *  Author: angel
 */ 


#ifndef HEDER_H_
#define HEDER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial.h"
#include "parse.h"
#include "parse_mmr.h"
#include "parse_read.h"
#include "number_conv.h" 
#include "adc.h"
#include "parse_buffer.h"
#include "parse_s.h"
#include "parse_f.h"
#include "parse_l.h"

extern char version[60];			//program version
extern volatile char received_command[30];							//command to be executed
extern volatile char last_command[30];								//stores the last command executed with success
extern volatile char parse_index;
extern char command_read();											//read the command to be executed 
extern void print_help();										//prints the help menu
extern void save_command();										//stores the last command executed with success
extern char str_read_last(void);

#endif /* HEDER_H_ */ 