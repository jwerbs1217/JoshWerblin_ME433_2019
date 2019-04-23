
#ifndef IO_EXPANDER_H    
#define IO_EXPANDER_H
void initExpander();
void setExpander(char, char);
char getExpander();
#define IOADDR 0b0100000 //change the address of your expander here

#endif