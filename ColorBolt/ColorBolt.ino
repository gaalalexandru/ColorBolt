/*
 * Alexandru Gaal:
 * ColorBolt SW for RGB flashlight with variable intensity and blink frequency
 * 2017.12.12
 */

 /*--------------------------Include & Defines secion--------------------------*/
#include <SoftwareSerial.h>

#include "led.h"

//Pin mapping for switches and pot
#define SWITCH1 (3)
#define SWITCH2 (2)
#define POT1    (A7)

//General configurations
#define NR_OF_MODES  (3)
//0 = off, 1 = on, 2 = blink
#define NR_OF_COLORS  (8)
//0 = off, 1 = red, 2 = green
//3 = blue, 4 = yellow, 5 = pink
//6 = cyan, 7 = white
#define MODE_OFF  (0)
#define MODE_ON (1)
#define MODE_BLINK (2)

#define DEBOUNCE_DELAY (100)

unsigned char mode_select = 0;
unsigned char color_select = 1;
unsigned char old_mode = mode_select;
unsigned char old_color = color_select;
unsigned char sw1_read = 0;
unsigned char sw2_read = 0;
unsigned int pot_value = 0;

void switch_setup(void)
{
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);
}

void setup() 
{
  led_setup();
  switch_setup();
  //analogReadResolution(10);  //10 bits, values: 0-1023
  Serial.begin(9600);
  Serial.println("Setup ready!");
}

void loop() 
{
  static unsigned char temp_mode = 0;
  
  sw1_read = digitalRead(SWITCH1);
  delay(DEBOUNCE_DELAY);
  if(!sw1_read)
  {
    mode_select = (mode_select + 1) % NR_OF_MODES;
  }

  sw2_read = digitalRead(SWITCH2);
  delay(DEBOUNCE_DELAY);
  if(!sw2_read)
  {
    color_select = (color_select + 1) % NR_OF_COLORS;
  }

  pot_value = analogRead(POT1);

  if(old_mode != mode_select)
  {
    Serial.print("Mode: "); Serial.println(mode_select);
    old_mode = mode_select;
  }

  if(old_color != color_select)
  {
    Serial.print("Color: "); Serial.println(color_select);
    old_color = color_select;
  }

  if((old_mode != mode_select)||(old_color != color_select))
  {
    switch(mode_select)
    {
      case(MODE_OFF):
        led_control(color_select, 0, 127);
      break;
  
      case(MODE_ON):
        led_control(color_select, 1, 127/*(pot_value / 4)*/);
      break;
  
      case(MODE_BLINK):
        led_control(color_select, temp_mode, 127);
        temp_mode ^= 1;
        delay(pot_value);
      break;      
    }
    old_mode = mode_select;
    old_color = color_select;
  }
}
