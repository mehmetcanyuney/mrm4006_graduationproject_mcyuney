//Writen by Mehmet Can Yüney

/* 
Simple CRC library for Modbus commmunication. 

Writen in C.

For any further questions please don't hesitate to contach with the owner.
Owner's Email : mehmetcanyny@gmail.com
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>

int calculateCRC(char* data, int lenght);
void divideCRC(int crc, uint8_t* crcLow, uint8_t* crcHigh);
void hexStringCreation(char* data, int lenght, char* result);
void setTextFormat(uint8_t modbusAdress, uint8_t function, int adress, int data, char* result);
char intToHexChar(uint8_t value);
int charToInt(char temp);
