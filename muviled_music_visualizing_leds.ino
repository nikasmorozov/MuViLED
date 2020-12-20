/*
MuViLED - Music Visualizing LEDS (by Nikas M.)
num_leds - the total number of LEDs you are going to use.
pin - pin to control the LED strip
*/

#include <FastLED.h>

#define num_leds 118
#define pin 9

CRGB leds[num_leds];

void setup()
{
//  Safe startup delay
  delay(3000);
//  Serial.begin(9600);
  FastLED.addLeds<WS2812B, pin, GRB>(leds, num_leds);
}

void loop()
{
  // Potentiometer that limits the voltage range from the audio input and so changes the sensitivity of the audio input.
  int sensitivity = (analogRead(A2));
  sensitivity = map(sensitivity, 0, 1023, 1000, 260);

  // Audio input from MAX9414 or similar. Carrier DC in my case gives an analogRead of around 240 so I reduce this value in map function to measure the input from zero.
  int audioIn = analogRead(A0);
  
  audioIn = (map(audioIn, 250, sensitivity, 0, 255));
  

  // Pot controlling brightness
  int brightness = analogRead(A3);
  brightness = map(brightness, 0, 1023, 0, 255);

  // Pot controlling FastLED HSV shift.
  int userHue = analogRead(A4);
  userHue = map(userHue, 0, 1023, 0, 255);

  // Pot controlling delay at which LEDs are updated.
  int userDelay = analogRead(A5);
  userDelay = map(userDelay, 0, 1023, 10, 60);

  if (audioIn > 10)
  {
    int y = audioIn + userHue;

    // Here the audio input is limited to 0 - 255 range which shifts the FastLED HSV value according to audio input and HUE shift.
    if (y > 255)
    {
      y = y % 255;
    }
//    Serial.println(y);
    leds[(num_leds / 2)] = CHSV(y, 255, y * 2);

   
    
  } else {
    leds[(num_leds / 2)] = CRGB::Black;
  }
   // Comment the line below to disable the blur and make the visualizer more reactive.
    //    blur1d(leds, num_leds, 5);

    for (int z = num_leds; z > (num_leds / 2); z--)
    {
      leds[z] = leds[z - 1];
    };
    for (int z = 0; z < (num_leds / 2); z++)
    {
      leds[z] = leds[z + 1];
    }
  FastLED.show();
    delay(userDelay);
}
