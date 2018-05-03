CRC16Modbus: function() {
        var crc = 0xFFFF;
        var str = this.CleanedString;
        for (var pos = 0; pos < str.length; pos++) {
            crc ^= str.charCodeAt(pos);
            for (var i = 8; i !== 0; i--) {
                if ((crc & 0x0001) !== 0) {
                    crc >>= 1;
                    crc ^= 0xA001;
                } else
                    crc >>= 1;
            }
        }
        return crc;
    }