#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
/* LED info */
const int       
    NUMPIXELS = 1,
    PIXEL_PIN = 1,
    THERMAL_PIN = 2,
    THERMAL_DATA_PIN = 1,
    LUX_PIN = 4,
    LUX_DATA_PIN = 2,
    INTERVAL = 250,
    SENSOR_SKEW = 14,
    OPTIMAL_C = 28;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int getTemperature(){
    // from https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
    int temp_C = 22,
        raw = analogRead(THERMAL_DATA_PIN),
        voltage = (raw) * (5000/1024);
    temp_C = ((voltage - 500) / 10) + SENSOR_SKEW;
    return temp_C;
}
uint32_t getColorFromTemperature(){
    float temp_C = getTemperature(),
          t = (temp_C - OPTIMAL_C) * 10;
    int 
        r = constrain((128 - t) * 2, 0, 255),
        g = constrain((128 - abs(t)) * 2, 0, 255),
        b = constrain((128 + t) * 2, 0, 255);

   return pixels.Color(r, g, b); 
}

int getBrightness(){
    int input = analogRead(LUX_DATA_PIN) - 128;
    input = constrain(input, 64, 255);
    return input;
}

void applyLighting(uint32_t targetColor, int brightness) {
    int i;
    for(i=0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, targetColor);
    }
    pixels.setBrightness(brightness);
    pixels.show();
}

void setup(){
    pinMode(PIXEL_PIN, OUTPUT);
    pinMode(THERMAL_PIN, INPUT);
    //pinMode(LUX_PIN, INPUT);
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // init neopixel library
    pixels.begin();

}
void loop(){
    uint32_t color = getColorFromTemperature();
    applyLighting(color, getBrightness());
    delay(INTERVAL);
}