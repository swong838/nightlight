#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
/* LED info */
const int       
    NUMPIXELS = 1,
    PIXEL_PIN = 2,
    THERMAL_PIN = 3,
    LUX_PIN = 4,
    INTERVAL = 5000;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int readTemperature(){
    int t = 0;
    return t;
}
uint32_t getColorFromTemperature(t){
   return pixels.Color(red, green, blue); 
}

void applyLighting(uint32_t targetColor) {
    int i;
    for(i=0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, targetColor);
    }
    pixels.show();
}

void setup(){
    randomSeed(analogRead(0));
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // init neopixel library
    pixels.begin(); 
}
void loop(){
    int t = readTemperature();
    uint32_t newColor = getColorFromTemperature(t);
    applyLighting(newColor);
    delay(INTERVAL);
}