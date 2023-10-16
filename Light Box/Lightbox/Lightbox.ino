#include <Adafruit_NeoPixel.h>

#define PotentiometerPin 3  //Analog
#define SwitchPin 5         //Digital
#define NeoPin 8            //Digital
#define NeoBrightness 255    //0-255
#define NumNeoPixels 8

Adafruit_NeoPixel strip(NumNeoPixels, NeoPin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(NeoBrightness);
  pinMode(SwitchPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int sel = map(analogRead(PotentiometerPin), 0, 1023, 1, 4);
  int Switch = digitalRead(SwitchPin);
  //Switch = 0;  //Switch override
  //Switch on or off?
  switch (Switch) {

    case 0:
      //Special mode selection
      switch (sel) {
        //Where is the potentiometer?
        case 1:
          //Off
          colorful(0, 0, 0);
          delay(1000);
          break;

        case 2:
          //Changing Colors
          colorful(rand() * 255, rand() * 255, rand() * 255);
          delay(2000);
          break;

        case 3:
          //Rainbow
          rainbow(100);
          break;

        case 4:
          //Strobe
          strobe(0);
          break;
      }
      break;

    case 1:
      //Solid color selection
      if (map(analogRead(PotentiometerPin), 0, 1023, 1, 5) == 1) {
        //White
        colorful(255, 255, 255);
        delay(1000);
      } else {
        //Rainbow of colors to select from
        long color = map(analogRead(PotentiometerPin), 204, 1023, 0, 62000);
        for (int i = 0; i < strip.numPixels(); i++) {
          int pixelHue = color + (65536L / strip.numPixels());
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
          strip.show();
        }
        delay(100);
      }
      break;
  }
}

//Sets the whole strip to a color
void colorful(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels() + 1; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
    strip.show();
  }
}

//Slowly shifts the whole strip through the rainbow
void rainbow(int DelayVal) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(DelayVal);
    //Break out when not selected anymore
    if (map(analogRead(PotentiometerPin), 0, 1023, 1, 4) == 3 && digitalRead(SwitchPin) == 0) {
    } else {
      firstPixelHue = 5 * 65535;
    }
  }
}

//Strobes the strip
void strobe(int Delay) {
  //if delay==0, then it cycles
  if (Delay == 0) {
    for (int progress = -100; progress < 101; progress++) {
      Delay = abs(progress);
      colorful(255, 255, 255);
      delay(Delay);
      colorful(0, 0, 0);
      delay(Delay);
      //Break out when not selected anymore
      if (map(analogRead(PotentiometerPin), 0, 1023, 1, 4) == 4 && digitalRead(SwitchPin) == 0) {
      } else {
        progress = 101;
      }
    }
  } else {
  }
  colorful(255, 255, 255);
  delay(Delay);
  colorful(0, 0, 0);
  delay(Delay);
}