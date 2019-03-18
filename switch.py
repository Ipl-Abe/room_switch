import sys
import serial
from time import sleep

def main():
    ser = serial.Serial('/dev/ttyACM0',9600)
    args = sys.argv
    try:
        if args[1] == 'on':
            ser.write(b'w')
        if args[1] == 'off':
            ser.write(b'q')
        sleep(0.5)
        ser.write(b'f')
        sleep(25)
        ser.write(b'b')
        sleep(5)
        ser.write(b's')
    except KeyboardInterrupt:
        ser.write(b's')
        sys.exit()

if __name__ == '__main__':
    main()
