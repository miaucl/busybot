/*
  Keyboard Message test
  For the Adafruit Neo Trinkey
  Sends a text string when a one touchpad is pressed, opens up Notepad if the other is pressed
*/

#include <Adafruit_NeoPixel.h>
#include "Adafruit_FreeTouch.h"
#include "Keyboard.h"
#include "Mouse.h"

// Create the neopixel strip with the built in definitions NUM_NEOPIXEL and PIN_NEOPIXEL
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Create the two touch pads on pins 1 and 2:
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch qt_2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

// Mode and color
enum Mode {
  Move_Mode, // GREEN
  Click_Mode, // RED 
  Key_Mode, // BLUE
  F5_Mode // PURPLE
};
int mode = Move_Mode;
int mode_color[4] = {0x00FF00, 0xFF0000, 0x0000FF, 0xFF00FF};
char key = 'a'; // Change to key of your preference

// Action
#define ACTION_INTERVAL 60000 //ms
long last_action = 0;

// Brightness (out of 255)
#define BRIGHTNESS 20

// Touch threshold
#define TOUCH_THRESHOLD 500

void setup() 
{
  Serial.begin(9600);
  //while (!Serial) delay(10);
  
  strip.begin(); // start pixels
  strip.setBrightness(BRIGHTNESS); // not too bright!
  strip.show(); // Initialize all pixels to 'off'

  if (! qt_1.begin())  
    Serial.println("Failed to begin qt on pin 1");
  if (! qt_2.begin())  
    Serial.println("Failed to begin qt on pin 2");
    
  // initialize control over the keyboard and mous:
  Keyboard.begin();
  Mouse.begin();
}

void loop() 
{
  
  // measure the captouches
  uint16_t touch1 = qt_1.measure();
  uint16_t touch2 = qt_2.measure();
  // whether we think they are being touched
  bool Touch1State, Touch2State;

  // print out the touch readings
  // Serial.print("QT 1: "); Serial.print(touch1);
  // Serial.print("\t\tQT 2: "); Serial.println(touch2);

  // If a pad is touched, rotate to next mode
  if (touch1 > TOUCH_THRESHOLD || touch2 > TOUCH_THRESHOLD) 
  {
    mode++;
    if (mode > F5_Mode) mode = Move_Mode;
    strip.fill(mode_color[mode]);
    strip.show();
    delay(500);
  }
  else                    
  {
    strip.fill(0x000000);
    strip.show();
  }
  
  if (last_action + ACTION_INTERVAL < millis())
  {
    last_action = millis();
    strip.fill(mode_color[mode]);
    strip.show();

    switch (mode)
    {
      case Move_Mode:
        Mouse.move(10, 10, 0);
        delay(10);
        Mouse.move(-10, -10, 0);
      break;
      case Click_Mode:
        Mouse.click();
      break;
      case Key_Mode:
        Keyboard.print(key);
      break;
      case F5_Mode:
        Keyboard.write(KEY_F5);
      break;
    }
    delay(200);
  } 
  
  delay(10);  // and pause a moment
}