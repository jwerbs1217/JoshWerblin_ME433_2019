#include "xpt2046.h"
#include "ili9341.h"


void XPT2046_read(unsigned short *x, unsigned short *y, unsigned int *z){
    int z1,z2;
    CS2 = 0;
    spi_io(0b10010001);//request to read y position
    *y = spi_io(0x00);
    *y = (*y<<8)|spi_io(0x00);
    CS2 = 1;
    
    CS2 = 0;
    spi_io(0b11010001);//request to read x position
    *x = spi_io(0x00);
    *x = (*x<<8)|spi_io(0x00);
    CS2 = 1;

    CS2 = 0;
    spi_io(0b10110001);//request to read Z1
    z1 = spi_io(0x00);
    z1 = (z1<<8)|spi_io(0x00);
    CS2 = 1;
    
    CS2 = 0;
    spi_io(0b11000001);//request to read Z2
    z2 = spi_io(0x00);
    z2 = (z2<<8)|spi_io(0x00);
    CS2 = 1;
    
    *z = z1-z2+4095;
}

void num2pix(unsigned short *x, unsigned short *y){
    
}