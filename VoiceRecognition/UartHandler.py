import serial
import time


class UartHandler:
    def __init__(self, baudrate):
        self.baudrate = baudrate
        self.ser = serial.Serial("/dev/ttyAMA0",baudrate=self.baudrate)

    def readReg(self, dataList):
        data_string = str(dataList[0]) + "a" + str(dataList[1])
        if len(data_string) == 3:
            data_string = data_string + "aaa"
        elif len(data_string) == 4:
            data_string = data_string + "aa"

        self.ser.write(data_string.encode())

        useless = self.ser.read(8)

        ret_data = int(self.ser.read(3))
        return ret_data

    def writeReg(self, dataList):
        data_string = str(dataList[0]) + "a" + str(dataList[1])
        if len(data_string) == 5:
            data_string = data_string + "a"
        elif len(data_string) == 4:
            data_string = data_string + "aa"
        elif len(data_string) == 3:
            data_string = data_string + "aaa"

        self.ser.write(data_string.encode())
