#define USB_CFG_DEVICE_NAME     'D','i','g','i','B','l','i','n','k'
#define USB_CFG_DEVICE_NAME_LEN 9
#include <DigiUSB.h>

#define MODE_SET   115 // letter 's'
#define MODE_FADE  102 // letter 'f'
#define COMMAND_LEN   4
#define COLOR_MAX_VALUE 255

#define PIN_GREEN 0
#define PIN_RED   1
#define PIN_BLUE  2
#define SOFT_PWM_PIN 2

/* Fading variables */
bool fading = false;
const unsigned int totalFadingSteps  = 1000;
unsigned int currentFadingStep  = 0;
int PrevRed   = 0;
int PrevGreen = 0;
int PrevBlue  = 0;
/* End of fading variables */

/* LED color values */
unsigned short Red = 0;
unsigned short Green = 0;
unsigned short Blue = 0;

void setup() {
    DigiUSB.begin();
    pinMode(PIN_GREEN,OUTPUT);
    pinMode(PIN_RED,OUTPUT);
    pinMode(PIN_BLUE,OUTPUT);
}

void readColorsFromUSB() {
    byte in = 0;
    in = DigiUSB.read();
    Red = in;
    in = DigiUSB.read();
    Green = in;
    in = DigiUSB.read();
    Blue = in;
}

void ignoreColorsFromUSB() {
    DigiUSB.read();
    DigiUSB.read();
    DigiUSB.read();
}

boolean handleMode(unsigned short mode_) {
    if (mode_ == MODE_FADE) {
        PrevRed = Red;
        PrevGreen = Green;
        PrevBlue = Blue;
        fading = true;
        return true;
    }
    if (mode_ == MODE_SET) {
        return true;
    }

    return false;
}

/**
 * Sets the intensity (0-255) of a given color on the RGB LED
 * @param pin use PIN_RED, PIN_GREEN, PIN_BLUE
 * @param colorIntensity give a value 0 to 255
 */
void setColor(unsigned short pin, unsigned short colorIntensity) {
    if (pin == SOFT_PWM_PIN) {
        softPWM(pin, colorIntensity);
    }
    else {
        analogWrite(pin, colorIntensity);
    }
}

void softPWM(unsigned short pin, unsigned short colorIntensity) {
    if(colorIntensity == 0) {
        digitalWrite(pin, LOW);
        return;
    }

    else if(colorIntensity == COLOR_MAX_VALUE) {
        digitalWrite(pin, HIGH);
        return;
    }

    // On period  
    for (int x=0; x<colorIntensity; x++) {
        digitalWrite(pin, HIGH);
    }
    
    // Off period
    for (int x=0; x<(COLOR_MAX_VALUE-colorIntensity); x++) {
        digitalWrite(pin, LOW);
    }
}

unsigned short calculateFadingColor(int prevColor, int expectedColor) {
    return prevColor + ((float)(expectedColor - prevColor) * ((float)currentFadingStep / (float)totalFadingSteps));
}

void displayFadingLED() {
    if (currentFadingStep == totalFadingSteps) {
        currentFadingStep = 0;
        fading = false;
    }
    else {
        ++currentFadingStep;
        setColor(PIN_RED,   calculateFadingColor(PrevRed,   Red));
        setColor(PIN_GREEN, calculateFadingColor(PrevGreen, Green));
        setColor(PIN_BLUE,  calculateFadingColor(PrevBlue,  Blue));
        delay(2);
    }
}

void displaySolidLED() {
    setColor(PIN_RED, Red);
    setColor(PIN_GREEN, Green);
    setColor(PIN_BLUE, Blue);
}

void loop() {
    DigiUSB.refresh();

    if (DigiUSB.available() >= COMMAND_LEN) {
        byte in = 0;
        in = DigiUSB.read();
        if (handleMode(in)) {
            readColorsFromUSB();
        }
        else {
            ignoreColorsFromUSB();
        }
    }

    if (fading) {
        displayFadingLED();
    }
    else {
        displaySolidLED();
    }
}
