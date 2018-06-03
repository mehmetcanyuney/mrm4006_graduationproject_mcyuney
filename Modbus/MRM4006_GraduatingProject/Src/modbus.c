#include "modbus.h"

//Creating modbus data (include crc calculation) that will be transmited by modbus protocol (Modbus RTU)
void createModBusData(uint8_t modbusAdress, uint8_t function, int adress, int data, uint8_t* result){
	char hex[12],hexRes[6];
	uint8_t crcL,crcH;
	int crc;
	
	setTextFormat(modbusAdress, function, adress, data, hex);
	hexStringCreation(hex, 12, hexRes);
	crc = calculateCRC(hexRes, 6);
	divideCRC(crc, &crcL, &crcH);
	result[7] = crcH;
	result[6] = crcL;
	
	result[0] = modbusAdress;
	result[1] = function;
	if(adress >= 256){
		int temp1 = (adress & 0xFF00) >> 8;
		int temp2 = adress & 0x00FF;
		result[2] = temp1;
		result[3] = temp2;
	}
	else{
		result[2] = 0;
		result[3] = adress;
	}
	if(data >= 256){
		int temp1 = (data & 0xFF00) >> 8;
		int temp2 = data & 0x00FF;
		result[4] = temp1;
		result[5] = temp2;
	}
	else{
		result[4] = 0;
		result[5] = data;
	}
	
}

//Writes single register in modbus device, data given with modbusData, responseData sent by modbus device.
void writeSingleRegister(UART_HandleTypeDef *huart,uint8_t* modbusData, uint8_t* responseData){
        HAL_GPIO_TogglePin(Modbus_Control_GPIO_Port,Modbus_Control_Pin);
        HAL_UART_Transmit(huart, (uint8_t*)modbusData, 8, 1000);
        HAL_Delay(3);
        HAL_GPIO_TogglePin(Modbus_Control_GPIO_Port,Modbus_Control_Pin);
        HAL_UART_Receive_IT(huart, (uint8_t*)responseData, 8);
        HAL_Delay(3);
}

//Reads single register from modbus device, requested register given with modbusData, response data contains the data that wanted to read.
void readSingleRegister(UART_HandleTypeDef *huart, uint8_t* modbusData, uint8_t* responseData){
        HAL_GPIO_TogglePin(Modbus_Control_GPIO_Port,Modbus_Control_Pin);
        HAL_UART_Transmit(huart, (uint8_t*)modbusData, 8, 1000);
        HAL_Delay(3);
        HAL_GPIO_TogglePin(Modbus_Control_GPIO_Port,Modbus_Control_Pin);
        HAL_UART_Receive_IT(huart, (uint8_t*)responseData, 7);
        HAL_Delay(3);
}