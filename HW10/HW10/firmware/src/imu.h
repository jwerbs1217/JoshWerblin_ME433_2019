#ifndef IMU_H
#define IMU_H

void initIMU();
void setIMU(char pin, char level);
char getIMU(unsigned char reg);
#define IMUADDR 0b1101011

#endif