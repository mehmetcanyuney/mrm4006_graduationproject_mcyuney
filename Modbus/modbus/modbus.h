//Writen by Mehmet Can Yüney

/* 
Simple CRC library for Modbus commmunication. 

Writen in C.

For any further questions please don't hesitate to contach with the owner.
Owner's Email : mehmetcanyny@gmail.com
*/

#include "crc.h"

void createModBusData(uint8_t modbusAdress, uint8_t function, int adress, int data, uint8_t* result);
