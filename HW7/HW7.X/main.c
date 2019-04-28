//main.c
//HW7
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>  //for sprintf
#include "myPIC32.h"     //a buncha #pragmas
#include "ili9341.h"
#include "imu.h"


int main() {

  __builtin_disable_interrupts();
     __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

      // 0 data RAM access wait states
      BMXCONbits.BMXWSDRM = 0x0;

      // enable multi vector interrupts
      INTCONbits.MVEC = 0x1;

      // disable JTAG to get pins back
      DDPCONbits.JTAGEN = 0;

      TRISAbits.TRISA4 = 0; //User LED
      TRISBbits.TRISB4 = 1; //USER button
      LATAbits.LATA4 = 1; //start LED high
      
      SPI1_init();
      LCD_init();
      LCD_clearScreen(ILI9341_BLACK);
      LCD_print("Initializing IMU",50,120,ILI9341_MAGENTA,ILI9341_BLACK);
      initIMU();
      LCD_clearScreen(ILI9341_BLACK);
      
      _CP0_SET_COUNT(0);    
      __builtin_enable_interrupts();

      
      char m[100];
      int length = 14;
      unsigned char data[length];
  while(1){

        //char k = getIMU(0x0F);
        i2c_read_multiple(IMUADDR, 0x20, data, length); 
        short int temp = (data[1]<<8)|data[0];
        short int x_gyr = (data[3]<<8)|data[2];
        short int y_gyr = (data[5]<<8)|data[4];
        short int z_gyr = (data[7]<<8)|data[6];
        short int x_acc = (data[9]<<8)|data[8];
        short int y_acc = (data[11]<<8)|data[10];
        short int z_acc = (data[13]<<8)|data[12];
        if (_CP0_GET_COUNT()>24000000/10){ //10 Hz
        LATAINV = 0b10000; //Heartbeat
        
        sprintf(m,"accel x,y,z %d, %d, %d  ",x_acc,y_acc,z_acc);
        _CP0_SET_COUNT(0);
        LCD_print(m,0,5,ILI9341_WHITE,ILI9341_BLACK);
        LCD_drawbar(100*x_acc/32767,100,5,120,160,ILI9341_CYAN,ILI9341_BLACK);//need to edit LCD_drawbar to go in directions besides right
        sprintf(m,"%d Hz",24000000/(_CP0_GET_COUNT())); //frame rate
        LCD_print(m,0,312,ILI9341_MAGENTA,ILI9341_BLACK);       
      }
      
  
    }
}
     