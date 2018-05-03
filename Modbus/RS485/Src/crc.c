#include "crc.h"

int calculateCRC(char* data, int lenght){
	unsigned int crc = 0xFFFF;
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
