#include "imu.h"
#include<xc.h>
#include "i2c_master_noint.h"

void initIMU(){
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    i2c_master_setup();
    setIMU(0x10,0b10000010); //CTRL1_XL: 1.66kHz sample rate, 2g sensitivity, 100Hz filter 
    setIMU(0x11,0b10001000); //CTRL2_G: 1.66kHz sample rate, 1000dps sensitivity
    setIMU(0x12,1<<2); //CTRL3_C: IF_INC=1, allowing multiple reads in a row
}

void setIMU(char pin, char level){
      i2c_master_start();
      i2c_master_send(IMUADDR << 1);
      i2c_master_send(pin); // the register to write to
      i2c_master_send(level); // the value to put in the register
      i2c_master_stop(); // make the stop bit
}

char getIMU(unsigned char reg){
    i2c_master_start(); // make the start bit
    i2c_master_send(IMUADDR << 1|0); // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(reg); // the register to read from
    i2c_master_restart(); // make the restart bit
    i2c_master_send(IMUADDR<<1|1); // write the address, shifted left by 1, or'ed with a 1 to indicate reading
    char r = i2c_master_recv(); // save the value returned
    i2c_master_ack(1); // make the ack so the slave knows we got it
    i2c_master_stop(); // make the stop bit
    return r;
}
