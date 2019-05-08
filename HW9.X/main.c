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
      
      
      int i;
      for (i=0;i<20;i++){
        LCD_drawPixel(100+i,120,ILI9341_WHITE);
        LCD_drawPixel(100,120+i,ILI9341_WHITE);
        LCD_drawPixel(100+i,140,ILI9341_WHITE);
        LCD_drawPixel(120,120+i,ILI9341_WHITE);
      }
      LCD_print("+",108,130,ILI9341_WHITE,ILI9341_BLACK);
      
      LCD_print("0",110,160,ILI9341_WHITE,ILI9341_BLACK);
      
      for (i=0;i<20;i++){
        LCD_drawPixel(100+i,180,ILI9341_WHITE);
        LCD_drawPixel(100,180+i,ILI9341_WHITE);
        LCD_drawPixel(100+i,200,ILI9341_WHITE);
        LCD_drawPixel(120,180+i,ILI9341_WHITE);
      }
      LCD_print("-",108,190,ILI9341_WHITE,ILI9341_BLACK);
      
      _CP0_SET_COUNT(0);    
      __builtin_enable_interrupts();

      
      char m[100];
      int length = 14;
      unsigned char data[length];
      int num=0;
  while(1){
        while (_CP0_GET_COUNT()<24000000/20){} //20 Hz
        i2c_read_multiple(IMUADDR, 0x20, data, length); 
        short int temp = (data[1]<<8)|data[0];
        short int x_gyr = (data[3]<<8)|data[2];
        short int y_gyr = (data[5]<<8)|data[4];
        short int z_gyr = (data[7]<<8)|data[6];
        short int x_acc = (data[9]<<8)|data[8];
        short int y_acc = (data[11]<<8)|data[10];
        short int z_acc = (data[13]<<8)|data[12];
        
        LATAINV = 0b10000; //Heartbeat
        
        sprintf(m,"accel x,y,z %d, %d, %d  ",x_acc,y_acc,z_acc);
        _CP0_SET_COUNT(0);
        LCD_print(m,0,5,ILI9341_WHITE,ILI9341_BLACK);
        //LCD_drawbar('h',100*x_acc/32767,100,5,120,160,ILI9341_CYAN,ILI9341_BLACK);
        //LCD_drawbar('v',100*y_acc/32767,100,5,120,160,ILI9341_CYAN,ILI9341_BLACK);
        
        unsigned short x, y; int z; 
        XPT2046_read(&x, &y, &z);
        sprintf(m,"touch x,y,z: %d, %d, %d    ",x,y,z);
        LCD_print(m,0,30,ILI9341_WHITE,ILI9341_BLACK);
        x = 240*x/32768;
        y = 320-320*y/32768;
        sprintf(m,"pixel touch x,y: %d, %d    ",x,y);
        LCD_print(m,0,55,ILI9341_WHITE,ILI9341_BLACK);  
        
        sprintf(m,"%d Hz",24000000/(_CP0_GET_COUNT())); //frame rate
        LCD_print(m,0,312,ILI9341_MAGENTA,ILI9341_BLACK);       
        
        
        if (z>-22000){
            if (((x<120) && x>100) && ((y<140) && (y>120))){
                num++;
                sprintf(m," %d   ",num);
                LCD_print(m,110,160,ILI9341_WHITE,ILI9341_BLACK);
            }
            
            if (((x<120) && x>100) && ((y<200) && (y>180))){
                num--;
                sprintf(m," %d   ",num);
                LCD_print(m,110,160,ILI9341_WHITE,ILI9341_BLACK);
            }
        }
      
  
    }
}
     