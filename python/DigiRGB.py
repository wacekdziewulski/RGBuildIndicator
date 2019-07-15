#!/usr/bin/python
#
# Assumes 'UsbStreamDemo1.pde' is loaded on Arduino and 
# LEDs are present on pins 11, 12 and 13.
#

import usb # 1.0 not 0.4
import sys
import time
from arduino.usbdevice import ArduinoUsbDevice

sys.path.append("..")

if __name__ == "__main__":

    try:
        theDevice = ArduinoUsbDevice(idVendor=0x16c0, idProduct=0x05df)
    except Exception as ex:
        print("DigiRGB Error: {}".format(ex))
        exit(1)

    color_list = sys.argv

    theDevice.write(ord("f"))

    if color_list[1] == 0:
        theDevice.write(0)
    else:
        theDevice.write(int(color_list[1]))

    if color_list[2] == 0:
        theDevice.write(0)
    else:
        theDevice.write(int(color_list[2]))

    if color_list[3] == 0:
        theDevice.write(0)
    else:
        theDevice.write(int(color_list[3]))
