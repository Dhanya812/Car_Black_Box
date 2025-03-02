#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char control_bit)
{
	CLCD_RS = control_bit;  //initially it could be data or instruction, here at first the data is stored in the register
	CLCD_PORT = byte;

	/* Should be atleast 200ns */
    //to give clock pulse enable on and off which is used to store the data to the register
	CLCD_EN = HI;
	CLCD_EN = LO;

	PORT_DIR = INPUT;   //TRISD7 is input as RD7 in busy pin
	CLCD_RW = HI;
	CLCD_RS = INSTRUCTION_COMMAND;  //here we change the RS value because at first the data is stored in register,
                                    //now it should process, therfore instruction commond should be pass so that it gives the instruction to process

	do
	{                      //here we have to give clock pulse initially because to process the first instruction
		CLCD_EN = HI;
		CLCD_EN = LO;
	} while (CLCD_BUSY);  //give the clock pulse until the instruction completely process

	CLCD_RW = LO;    //when we want to send next set of instruction we have to reset the R/W as 0
	PORT_DIR = OUTPUT;   //when we want to send next set of instruction we have to reset the TRISD7 as 0
}

void init_clcd()
{
	/* Set PortD as output port for CLCD data */
	TRISD = 0x00;
	/* Set PortC as output port for CLCD control */
	TRISC = TRISC & 0xF8;   //only 3 pins are used as output pins for control bit  xxxx x000

	CLCD_RW = LO;

	
     /* Startup Time for the CLCD controller */
    __delay_ms(30); //building delay - blocking delay
    
    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(4100);  //delay based on flowchart
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(100);    //delay based on flowchart
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(1);  //delay based on flowchart
    
    CURSOR_HOME;  //for these minimum 37 micro second delay should be give, but here we are going with polling method
    __delay_us(100);  //as it is polling method delay is not necessary ,we can even comment the delay
    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);
    CLEAR_DISP_SCREEN;
    __delay_us(500);
    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);  //Instruction passed 
	while (*data != '\0')   //here we will pass data address
	{
		clcd_write(*data++, DATA_COMMAND);
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)   //add is where the data should be print
{
	clcd_write(addr, INSTRUCTION_COMMAND);   //address is instruction,RS = 0
	clcd_write(data, DATA_COMMAND);    //data RS =1
}
