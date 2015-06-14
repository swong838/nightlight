#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
/* LED info */
const int       
    NUMPIXELS = 1,
    PIXEL_PIN = 3,
    THERMAL_PIN = 1,
    LUX_PIN = 4,
    INTERVAL = 100,
    SENSOR_SKEW = 14,
    MAX_C = 35,
    MIN_C = 15;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int getTemperature(){
    // from https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
    int temp_C = 22,
        raw = analogRead(THERMAL_PIN),
        voltage = (raw) * (5000/1024);
    temp_C = ((voltage - 500) / 10) + SENSOR_SKEW;
    return temp_C;
}
uint32_t getColorFromTemperature(){
    float temp_C = getTemperature(),
          t = (temp_C - 25) * 10;
    int 
        r = constrain((128 - t) * 2, 0, 255),
        g = constrain((128 - abs(t)) * 2, 0, 255),
        b = constrain((128 + t) * 2, 0, 255);
   return pixels.Color(r, g, b); 
}

int getBrightness(){
    int input = analogRead(LUX_PIN) - 128;
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
    randomSeed(analogRead(0));
    pinMode(PIXEL_PIN, OUTPUT);
    pinMode(THERMAL_PIN, INPUT);
    pinMode(LUX_PIN, INPUT);
#if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // init neopixel library
    pixels.begin();
    Serial.begin(9600);
}
void loop(){
    uint32_t color = getColorFromTemperature();
    int brightness = getBrightness();
    applyLighting(color, brightness);
    Serial.print("Temperature = ");
    Serial.println(getTemperature());
    Serial.print("Light = ");
    Serial.println(getBrightness());
    delay(INTERVAL);
}