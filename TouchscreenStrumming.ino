#include <RunningMedian.h>
RunningMedian samples = RunningMedian(11);
RunningMedian dysamples = RunningMedian(11);
bool UPSTRUM, DOWNSTRUM, RELEASE;
#include "Adafruit_TSC2007.h"
#include<Keyboard.h>
#define NUM_KEYS 5 // Number of buttons
#define KEY_1 'z'     // Macros go here; characters use quotation marks, modifiers use the ASCII table in the website below
#define KEY_2 'x'     // Change them here for your convenience
#define KEY_3 '1'
#define KEY_4 '2'
#define KEY_5 '3'
#define KEY_6 '4'
#define KEY_7 '5'
#include<Bounce2.h>
const int key[NUM_KEYS] = {4, 5, 6, 7, 8};
int i = 0;
//blue wire: sda, pin 2 on pro micro
//yellow wire: scl, pin 3 on pro micro
Bounce button[NUM_KEYS];
Adafruit_TSC2007 touch;

void setup() {
  Serial.begin(115200);

  if (!touch.begin()) {
    Serial.println("Couldn't find touch controller");
    while (1) delay(10);
  }
  Serial.println("Found touch controller");
  // Start debouncing the switches
  for (i = 0; i < NUM_KEYS; i++) {
    button[i] = Bounce();
    button[i].attach(key[i], INPUT_PULLUP);
    button[i].interval(5);
  }

  // Start your keyboard functionality
  Keyboard.begin();
  delay(1000);
}

void loop() {
  //touchscreen swipes to keyboard output
  uint16_t x, y, z1, z2;
  long mn1, m, dy, dm;
  long timestamp = millis();
  touch.read_touch(&x, &y, &z1, &z2);
  //Serial.print("Touch point: (");
  //Serial.print(x); Serial.print(", ");
  //Serial.print(y); Serial.print(" ");
  //Serial.print(z1); Serial.println(" / ");
  //Serial.print(z2); Serial.println(")");

  //raw touchscreen output is too noisy, so use a running median
  if (samples.getCount() > 0)
  {
    mn1 = samples.getMedian();
    //Serial.print(mn1); Serial.print("; ");
  }
  samples.add(y);
  m = samples.getMedian();
  //Serial.print(m); Serial.print(" ");

  //take the running median of the difference of the median coordinates
  if (samples.getCount() > 0)
  {
    dy = (m - mn1) * 100;
    dysamples.add(dy);
    dm = dysamples.getMedian();
    if (abs(dm) < 3000)
    {
      dm = 0;
    }
    //Serial.println(dm);
    if (dm > 0)
    {
      UPSTRUM = true;
      if (DOWNSTRUM or RELEASE)
      {
        //activate upstrum
        Keyboard.release(KEY_2);
        Keyboard.press(KEY_1);
        timestamp = millis();
        Serial.println("UPSTRUM PRESS");
      }
      DOWNSTRUM = false;
      RELEASE = false;
    }
    else if (dm < 0)
    {
      DOWNSTRUM = true;
      if (UPSTRUM or RELEASE)
      {
        //activate downstrum
        Keyboard.release(KEY_1);
        Keyboard.press(KEY_2);
        timestamp = millis();
        Serial.println("DOWNSTRUM PRESS");
      }
      UPSTRUM = false;
      RELEASE = false;
    }
    else //release all keys
    {
      if (not RELEASE)
      {
        Keyboard.release(KEY_1);
        Keyboard.release(KEY_2);
        Serial.println("RELEASE");
      }
      //Serial.println("RELEASE");
      UPSTRUM = false;
      DOWNSTRUM = false;
      RELEASE = true;
    }
  }

  //actual buttons
  for ( i = 0; i < NUM_KEYS; i++) {
    // Update the button states
    button[i].update();
  }

  if (button[0].fell()) {
    // Press and hold the buttons you want pressed
    Keyboard.press(KEY_3);
  }
  if (button[0].rose()) {
    // Release the same buttons you just pressed
    Keyboard.release(KEY_3);
  }
  if (button[1].fell()) {
    // Press and hold the buttons you want pressed
    Keyboard.press(KEY_4);
  }
  if (button[1].rose()) {
    Keyboard.release(KEY_4);
  }
  if (button[2].fell()) {
    // Press and hold the buttons you want pressed
    Keyboard.press(KEY_5);
  }
  if (button[2].rose()) {
    Keyboard.release(KEY_5);
  }
  if (button[3].fell()) {
    // Press and hold the buttons you want pressed
    Keyboard.press(KEY_6);
  }
  if (button[3].rose()) {
    Keyboard.release(KEY_6);
  }
  if (button[4].fell()) {
    // Press and hold the buttons you want pressed
    Keyboard.press(KEY_7);
  }
  if (button[4].rose()) {
    Keyboard.release(KEY_7);
  }


}
