/*
 * parse_f.c
 *
 * Created: 29/03/2016 19:50:34
 *  Author: angel
 */ 
#include "parse_f.h"


void parse_f()
{
	unsigned char temp =str_read();

	switch(temp)
	{
		/*filter wight*/
		case 'w':
		case 'W':
		set_filter_weight();
		break;
		
		/*Filter ON*/
		case 'n':
		case 'N':
			temp = str_read();
			if(temp == '\0')
			{
				flag_filter_on = 1;
				serial_writestr("Filter ON\n");	
			}
			if(temp != '\0')
				serial_error();
		break;
		
		/*Filter OFF*/
		case 'f':
		case 'F':
			temp = str_read();
			if(temp == '\0')
			{	
				flag_filter_on = 0;
				serial_writestr("Filter OFF\n");
			}
			if(temp != '\0')
				serial_error();
		break;
		
		default:
			serial_error();
		break;
	}
}


/*define the weight of the filter*/
void set_filter_weight(void)
{
	unsigned char control_digit1 = str_read();
	unsigned char temp_filter_weight = parse_number();
	unsigned char control_digit2 = str_read();
	
	if( (control_digit1 != 0x20) || (control_digit2 != '\0') || (temp_filter_weight < 0x00) || (temp_filter_weight > 0x09) )
		serial_error();
	
	if( (control_digit1 == 0x20) && (control_digit2 == '\0') && (temp_filter_weight >= 0x00) && (temp_filter_weight <= 0x09) )
	{
		filter_weight = temp_filter_weight;
		serial_writestr("Filter changed\n");
	}
}
