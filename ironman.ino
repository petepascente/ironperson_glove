#include <Adafruit_NeoPixel.h>

#define PIN 0
const int pixel_length = 6; //variable for number of LEDs in the neopixel strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixel_length, PIN, NEO_GRB + NEO_KHZ800); //define neopixel strip

int a0_val = 0; //variable that updates from analog reading pin A0

int a0_min = 0; //minimum a0 cut off value
int a0_max = 4095; //maximum a0 cut off value

void setup() {
  analogReadResolution(12); //set analog read resolution of the microcontroller to 12 bits
  strip.begin(); // start the neopixel strip
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600); //start serial communication
}

int clamp(int valmin, int valmax, int val) {
  if (val < valmin) return valmin;
  else if (val > valmax) return valmax;
  return val;
}

void loop() {
  a0_val = (a0_val + analogRead(A0)) / 2; //we analog read a0, add it to itself, and average it to smooth the signal (slightly)
  //Serial.println(a0_val); //print a0_val to serial
  int R = 3;
  int G = 5;
  int B = 5;
  for (uint16_t i = 0; i < strip.numPixels(); i++) { //iterate through all the LEDs in the neopixel strip
    strip.setPixelColor(i, R, G, B);
  }
  Serial.println(a0_val);
  a0_val = clamp(1450, 1560, a0_val);
  float v = map(a0_val, 1450, 1560, 5, 100);
  HSVtoRGB(187, 41, v, &R, &G, &B);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, R, G, B);
  }
  strip.show(); //update the strip colors
  delay(20); //20ms delay
}

/* keeping this for reference
  // int led_count = map(a0_val, a0_min, a0_max, 0, pixel_length); //maps a0_val between min and max to the number of LEDs to light up
  // int step_size = (a0_max - a0_min)/pixel_length; //find the a0 value interval between each LED to light
  // int R = 0;
  // int G = 0;
  // int B = 0;
  // for (uint16_t i = 0; i < strip.numPixels(); i++) { //iterate through all the LEDs in the neopixel strip
  //   if (i < led_count) { //light LEDs white
  //     HSVtoRGB(0, 0, 25, &R, &G, &B);
  //     strip.setPixelColor(i, R, G, B);
  //   } else if (i == led_count) { //change the hue of the last LED based on the remainder value
  //     float h = map(a0_val - led_count*step_size, 0, step_size, 0, 360);
  //     HSVtoRGB(h, 100, 25, &R, &G, &B);
  //     strip.setPixelColor(i, R, G, B);
  //   } else { //turn off the remainding LEDs
  //     strip.setPixelColor(i, 0, 0, 0);
  //   }
  // }
*/


// HSV to RGB by hdznrrd
// h: 0 to 360
// s: 0 to 100
// v: 0 to 100

void HSVtoRGB(float h, float s, float v, int *r, int *g, int *b) {
  int i;
  float f, p, q, t;

  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));

  s /= 100;
  v /= 100;

  if (s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v * 255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch (i) {
    case 0:
      *r = round(255 * v);
      *g = round(255 * t);
      *b = round(255 * p);
      break;
    case 1:
      *r = round(255 * q);
      *g = round(255 * v);
      *b = round(255 * p);
      break;
    case 2:
      *r = round(255 * p);
      *g = round(255 * v);
      *b = round(255 * t);
      break;
    case 3:
      *r = round(255 * p);
      *g = round(255 * q);
      *b = round(255 * v);
      break;
    case 4:
      *r = round(255 * t);
      *g = round(255 * p);
      *b = round(255 * v);
      break;
    default: // case 5:
      *r = round(255 * v);
      *g = round(255 * p);
      *b = round(255 * q);
  }
}
