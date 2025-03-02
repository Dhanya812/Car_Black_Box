#include "i2c.h"
#include "external_eeprom.h"

void write_external_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_E);
	i2c_write(address);   //address of location where we want to store the data.
	i2c_write(data);
	i2c_stop();
    for(unsigned int i=3000;i--;);
    
}

unsigned char read_external_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_E);   //from which address location we want to read th data amongst 64 address locations.
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_E);
	data = i2c_read();
	i2c_stop();

	return data;
}