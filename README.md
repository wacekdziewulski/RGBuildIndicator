# Digispark RGB Build indicator

As a software developer, how ofter do You find Yourself not tracking the build progress as it takes quite a long time? I'd rather read something online than keep track of how my gradle / gcc build is doing. My aim was to provide myself with a tiny device that could tell me how my build is doing without having to minimize the browser or other application I had on top. That's how the Digispark-based Build Indicator was born. I took the USB-enabled ATTiny board, soldered an RGB LED to it and finally mashed-up some code to allow me to see the status with a glimpse of an eye. If the build fails - the LED turns Red, passes - Green. During the process the LED shines Blue, which indicates to me that it's still running. Great!

## Build

## Programming

## Installation

The solution consists of two subprojects. The first one is the Digispark code, which has to be uploaded to the indicator. That's the 'digispark' folder and Arduino .ino project file.

The second part is a Python script that is used to send commands to the Digispark using a usb library.

### Dependencies

The python project part requires python-usb to run.
On Ubuntu that would be:
* sudo apt install python-usb

### Running DigiUSB.py

After plugging in the device, it should show up in `dmesg`:

    $> dmesg
    [ 8775.844819] usb 1-1: new low-speed USB device number 7 using xhci_hcd
    [ 8776.001392] usb 1-1: New USB device found, idVendor=16c0, idProduct=05df
    [ 8776.001398] usb 1-1: New USB device strings: Mfr=1, Product=2, SerialNumber=0
    [ 8776.001402] usb 1-1: Product: DigiUSB
    [ 8776.001406] usb 1-1: Manufacturer: digistump.com
    [ 8776.004983] hid-generic 0003:16C0:05DF.0004: hiddev0,hidraw2: USB HID v1.01 Device [digistump.com DigiUSB] on usb-0000:00:14.0-1/input0

If it does, we're on the right track. Let's check that it actually works:

    $> sudo ./DigiUSB.py 0 255 0

If the LED lit up green it works! Now, we don't want to be running that through sudo each and every time. To allow a regular user to access the USB subsystem for the Digispark device only, we have to add some udev rules.

    $> sudo echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="05df", MODE="0777"' >> /lib/udev/rules.d/60-digispark.rules

This will write the access rules for the Digispark device only - we can see that, because both idVendor and idProduct were provided as parameters and they're the same as seen in dmesg. The good news is that every Digispark has the same IDs pertaining to it only, so we're not really making any security flaw here.

Then we need to refresh the udev rules:

    $> sudo udevadm control --reload-rules

Finally we need to replug Digispark. Take it out of the USB port and put it back. Now run the DigiUSB.py script again this time without sudo:

    $> ./DigiUSB.py 0 0 255

If it turned blue this time, it works without sudo. Congratulations!
