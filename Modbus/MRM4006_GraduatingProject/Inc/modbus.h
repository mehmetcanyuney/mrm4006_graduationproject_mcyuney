//Writen by Mehmet Can Yüney

/* 
Simple CRC library for Modbus commmunication. 

Writen in C.

For any further questions please don't hesitate to contact with the owner.
Owner's Email : mehmetcanyny@gmail.com
*/

#include "crc.h"
#include "stm32f4xx_hal.h"
#include "main.h"

void createModBusData(uint8_t modbusAdress, uint8_t function, int adress, int data, uint8_t* result);
void writeSingleRegister(UART_HandleTypeDef *huart,uint8_t* modbusData, uint8_t* responseData);
void readSingleRegister(UART_HandleTypeDef *huart, uint8_t* modbusData, uint8_t* responseData);
