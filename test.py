import serial
ser = serial.Serial('COM9', 9600, timeout=5)

while 1:
    print(ser.readline())
