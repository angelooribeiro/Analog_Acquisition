/*
 * CFile1.c
 *
 * Created: 22/02/2016 22:13:04
 *  Author: angel
 */ 

#include "parse.h"

extern volatile char parse_index;
extern volatile char received_comand[];					//command to be executed
extern volatile unsigned char flag_sampling_cycle;			//flag to determinate if is a sampling cycle

extern volatile unsigned char digital_flag = 0;

void parse(void)
{
	char temp = str_read();
	switch(temp)
	{
		//request for read
		case 'r':
		case 'R':
			parse_read();
			break;
		
		//request for memory acess
		case 'm':
		case 'M':
			parse_memory();
			break;
		
		//request for printing the version
		case'v':
		case 'V':
			temp = str_read();
			if( (temp != 'e') && (temp != 'E') )
				serial_error();
			if( (temp =='e') || (temp =='E') )
			{
				temp  = str_read();
				if( (temp != 'r') && (temp != 'R') )
					serial_error();
				if( (temp == 'r') || (temp == 'R') )
				{
					temp = str_read();
					if(temp == '\0')
					{
						serial_writestr(version);
						save_command();				//save the last command executed with success
					}
					if(temp != '\0')
						serial_error();
				}
			}
			break;
			
										/*########################	GUIA 2	##############################*/
	/*#################################################             PL2                    #############################################################*/
	/*##################################################################################################################################################*/
		/*define channel to be sampled*/
		case 'a':
		case 'A':
		temp = str_read();
			if( (temp == 'c') || (temp =='C') )
			{
				set_analog_in();
			}	
			if( (temp != 'c') && (temp !='C') )
				serial_error();
			break;
		
		/*stop the sampling cycle operation*/
		case '.':
			temp = str_read();
			if(temp != '\0')		/*if is the end of the command*/
				serial_error();
			if(temp == '\0')
			{	
				if(flag_sampling_cycle == 0)
					serial_writestr("Sampling cycle is not running\n");
				
				if(flag_sampling_cycle)						/*if is running*/
				{
					TCCR1B &= ~0x07;						/*switch off the time*/
					TIMSK1 &= ~(1 << OCIE1B);
					flag_sampling_cycle = 0;				//disable the sampling cycle
				}
			}
		break;
		
		case 'b':
		case 'B':
			parse_buffer();
		break;
		
		case 's':
		case 'S':
			parse_s();
		break; /*end s case*/
		
		/*filter case*/	
		case 'f':
		case 'F':
			parse_f();
		break; /*end f case*/
		
		/*linear case*/	
		case 'l':
		case 'L':
			parse_l();
		break;	/*end Linear case*/
		
		//if nothing checks
		default:
			serial_error();
		break;
	}
	
}
