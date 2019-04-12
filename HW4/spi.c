#include "spi.h"

void initSPI1(){
  RPB15Rbits.RPB15R = 0b0011; //pin RB15 is SS1
  RPB13Rbits.RPB13R = 0b0011; //pin RB13 is SDO1
  SDI1Rbits.SDI1R = 0b0000; //pin RPA1 is SDI1

}
