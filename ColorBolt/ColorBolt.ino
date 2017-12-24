/*
 * Alexandru Gaal:
 * ColorBolt SW for RGB flashlight with variable intensity and blink frequency
 * 2017.12.12
 */

 /*--------------------------Include & Defines secion--------------------------*/
#include <SoftwareSerial.h>

#include "led.h"

#define SERIAL_DEBUG (1)

#define SWITCH1 (3)
#define SWITCH2 (2)
#define POT1    (A7)


#define NR_OF_MODES  (3)
//0 = off, 1 = on, 2 = blink
#define NR_OF_COLORS  (8)
//0 = off, 1 = red, 2 = green
//3 = blue, 4 = yellow, 5 = pink
//6 = cyan, 7 = white

#define DEBOUNCE_DELAY (50)

int mode_select = 0;  //off
int color_select = 0; //none (1-9)
int old_mode = 0;
int old_color = 0;
int sw1_read = 0;
int sw2_read = 0;
int pot_value = 0;

void switch_setup(void)
{
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);
}

void setup() 
{
  led_setup();
  switch_setup();
  #if SERIAL_DEBUG
  Serial.begin(9600);
  Serial.println("Setup ready!");
  #endif //SERIAL_DEBUG
}

void loop() 
{
  sw1_read = digitalRead(SWITCH1);
  delay(100);
  if(!sw1_read) 
  {
    mode_select = (mode_select + 1) % NR_OF_MODES;
  }

  sw2_read = digitalRead(SWITCH2);
  delay(100);
  if(!sw2_read) 
  {
    color_select = (color_select + 1) % NR_OF_COLORS;
  }

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

  if(mode_select == 1)
  {
    //all leds off
    led_control(color_select, 1, 100);
  }
  else
  {
    led_control(color_select, 0, 100);
  }
}
