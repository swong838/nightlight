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

int getTemperature(){
    int temp_C = 20;
    int raw = analogRead(THERMAL_PIN);
    // from https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
    int voltage = (raw) * (5000/1024);
    temp_C = (voltage - 500) / 10;
    return temp_C;
}
uint32_t getColorFromTemperature(temp_C){
/*
this.clip - need function to clip range from 0-255
    t = (temp - 22) * 10;
    r = this.clip((128 - t) * 2);
    g = this.clip((128 - Math.abs(t)) * 2);
    b = this.clip((128 + t) * 2);
*/

   return pixels.Color(255,255,255); 
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
    pinMode(THERMAL_PIN, INPUT);
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // init neopixel library
    pixels.begin(); 
}
void loop(){
    int temp_C = getTemperature();
    uint32_t newColor = getColorFromTemperature(temp_C);
    applyLighting(newColor);
    delay(INTERVAL);
}