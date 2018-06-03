#include "crc.h"

//Calculate CRC as a int
int calculateCRC(char* data, int lenght){
	int crc = 0xFFFF;
	int pos,i;
		
	for(pos = 0; pos < lenght; pos++){
		crc ^= (int)data[pos];
		for(i  = 8; i > 0; i--){
			if((crc & 0x0001) != 0){
				crc >>= 1;
				crc ^= 0xA001;
			}
			else{
				crc >>= 1;
			}
		}
	}
	return crc;
}

//Divides CRC into two seperated uint8_t
void divideCRC(int crc, uint8_t* crcLow, uint8_t* crcHigh){
	int temp;
	temp = (crc & 0xff00) >> 8;
	*crcLow = (uint8_t)crc;
	*crcHigh = (uint8_t)temp;
}

//Creates a hex string that contains the all modbus data
void hexStringCreation(char* data, int lenght, char* result){
	int i = 0, n = 0, temp;
	for(i = 0; i < lenght; i += 2){
		temp = 0;  	//if else ile a-b-c-d-e-f nin ayrilmasi gerek harfi çeviremiyor
		if(data[i] == 'A' || data[i] == 'B' || data[i] == 'C' || data[i] == 'D' || data[i] == 'E' || data[i] == 'F'){
			temp += charToInt(data[i]) * 16;
		}
		else{
			temp += (data[i] - '0') * 16;
		}
		if(data[i+1] == 'A' || data[i+1] == 'B' || data[i+1] == 'C' || data[i+1] == 'D' || data[i+1] == 'E' || data[i+1] == 'F'){
			temp += charToInt(data[i+1]);
		}
		else{
			temp += data[i + 1] - '0';
		}
		result[n] = (char)temp;
		n++;
	}
}

//Creates a string that contains hex format of modbus data
void setTextFormat(uint8_t modbusAdress, uint8_t function, int adress, int data, char* result){
	// For ModBus Adress
	uint8_t modadrL,modadrH;
	modadrL = modbusAdress & 0xF;
	modadrH = (modbusAdress & 0xF0) >> 4;
	result[0] = intToHexChar(modadrH);
	result[1] = intToHexChar(modadrL);
	
	// For Function
	uint8_t functionL,functionH;
	functionL = function & 0xF;
	functionH = (function & 0xF0) >> 4;
	result[2] = intToHexChar(functionH);
	result[3] = intToHexChar(functionL);
	
	//For Adress of Register
	uint8_t adressL,adressH,adressSL,adressSH; //second low and second high reprecents (xx00) x values
	adressL = adress & 0xF;
	adressH = (adress & 0xF0) >> 4;
	adressSL = (adress & 0xF00) >> 8;
	adressSH = (adress & 0xF000) >> 12;
	result[4] = intToHexChar(adressSH);
	result[5] = intToHexChar(adressSL);
	result[6] = intToHexChar(adressH);
	result[7] = intToHexChar(adressL);
	
	//For Data
	uint8_t dataL,dataH,dataSL,dataSH; //second low and second high reprecents (xx00) x values
	dataL = data & 0xF;
	dataH = (data & 0xF0) >> 4;
	dataSL = (data & 0xF00) >> 8;
	dataSH = (data & 0xF000) >> 12;
	result[8] = intToHexChar(dataSH);
	result[9] = intToHexChar(dataSL);
	result[10] = intToHexChar(dataH);
	result[11] = intToHexChar(dataL);
}

//Simple function for int to char
char intToHexChar(uint8_t value){
	if(value <= 9){
		return value + '0';
	}
	else if(value == 10){
		return 'A';
	}
	else if(value == 11){
		return 'B';
	}
	else if(value == 12){
		return 'C';
	}
	else if(value == 13){
		return 'D';
	}
	else if(value == 14){
		return 'E';
	}
	else if(value == 15){
		return 'F';
	}
}

//Simple function for hex char to int
int charToInt(char temp){
	if(temp == 'A'){
		return 10;
	}
	else if(temp == 'B'){
		return 11;
	}
	else if(temp == 'C'){
		return 12;
	}
	else if(temp == 'D'){
		return 13;
	}
	else if(temp == 'E'){
		return 14;
	}
	else if(temp == 'F'){
		return 15;
	}
}
