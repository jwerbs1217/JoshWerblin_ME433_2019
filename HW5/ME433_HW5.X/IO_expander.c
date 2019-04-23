#include "IO_expander.h"
#include<xc.h>   

void initExpander(){
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    i2c_master_setup();
}

void setExpander(char pin, char level){
      i2c_master_start();
      i2c_master_send(IOADDR << 1);
      i2c_master_send(pin); // the register to write to
      i2c_master_send(level); // the value to put in the register
      i2c_master_stop(); // make the stop bit
}

char getExpander(){
    i2c_master_start(); // make the start bit
    i2c_master_send(IOADDR << 1|0); // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(9); // the register to read from
    i2c_master_restart(); // make the restart bit
    i2c_master_send(IOADDR<<1|1); // write the address, shifted left by 1, or'ed with a 1 to indicate reading
    char r = i2c_master_recv(); // save the value returned
    i2c_master_ack(1); // make the ack so the slave knows we got it
    i2c_master_stop(); // make the stop bit
    return r;
}