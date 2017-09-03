/*
 * parse_l.c
 *
 * Created: 30/03/2016 18:43:38
 *  Author: angel
 */ 

#include "parse_l.h"


void parse_l()
{
	unsigned char temp = str_read();
	switch(temp)
	{
		/*Linear ON*/
		case 'n':
		case 'N':
			flag_linear_on = 1; /*signal for a linear output*/
			if(linear_was_on == 0)	/*if no data in the buffers*/
				read_linear_values();
			linear_was_on = 1;	/*declare that the data is in the buffers*/
		break;
			
		/*Linear OFF*/
		case 'f':
		case 'F':
			flag_linear_on = 0;	/*signal for a normal output*/
		break;
	}
}

unsigned int linear_value(unsigned int value)
 {
	 
 }

/*read the values from eeprom -> msb -> lsb*/
void read_linear_values(void)
{
	unsigned int temp_linear;
	unsigned int addr = 0;
	unsigned char index=0;
	char i = 0;
	/*read linear table*/
	do{
		if( ((i%2) != 0) )		//if makes an intenger 
		{
			EEAR = addr;					//addr to be redden
			EECR |= (1<<EERE);				//start reading, takes only one machine cycle
			temp_linear |= EEDR;			/*the lsb byte*/
			linear_table[index++] = temp_linear;
			addr++;							//inc the pointer to the next position of memory
		}
		
		if( ((i%2) == 0) )				//if not makes an integer
		{
			EEAR = addr;					//addr to be redden
			EECR |= (1<<EERE);				//start reading, takes only one machine cycle
			temp_linear = ( EEDR << 8);		/*the msb byte*/
			addr++;							//inc the pointer to the next position of memory
		}
	}while(linear_table[index] < 1023);
	
	index = 0;
}