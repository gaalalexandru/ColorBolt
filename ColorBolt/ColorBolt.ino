/*
 * Alexandru Gaal:
 * ColorBolt SW for RGB flashlight with variable brightness and blink frequency
 * 2017.12.12
 */

// Include & Defines secion
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
#define DEFAULT_COLOR (7)  //white
#define MODE_OFF  (0)
#define MODE_ON (1)
#define MODE_BLINK (2)
#define POT_FILTER_LIMIT (10)
#define DEBOUNCE_DELAY (100)

//Global variable declarations
int old_pot = 0;  //needs to be signed due to filtering
int pot_value = 0;  //needs to be signed due to filtering

//Function declarations, static functions are visible only in this file
static void switch_setup(void)
{
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);
}

void setup() 
{
  //setup digital pins
  led_setup();
  switch_setup();
  
  //Make first analog read to initialize old_pot since initial POT position is unknown
  pot_value = analogRead(POT1);
  old_pot = pot_value;

  //start serial port
  Serial.begin(9600);
  Serial.println("Setup ready!");
}

void loop() 
{
  //local variable declarations
  unsigned char sw1_read = 0;
  unsigned char sw2_read = 0;
  //static is necessary to retain the value of local variable after exiting the function
  static unsigned char temp_mode = 0;
  static unsigned char mode_select = 0;
  static unsigned char color_select = DEFAULT_COLOR;
  static unsigned char old_mode = mode_select;
  static unsigned char old_color = color_select;
  static unsigned long blink_timestamp = 0;
    
  //reading of switch (digital) and potentiometer (analog) values
  sw1_read = digitalRead(SWITCH1);
  delay(DEBOUNCE_DELAY);
  if(!sw1_read)
  {
    mode_select = (mode_select + 1) % NR_OF_MODES;  //toggle between 0 - NR_OF_MODES
  }
  
  sw2_read = digitalRead(SWITCH2);
  delay(DEBOUNCE_DELAY);
  if(!sw2_read)
  {
    color_select = (color_select + 1) % NR_OF_COLORS; //toggle between 0 - NR_OF_COLORS
    if(color_select == 0) { color_select = 1; }  //ignore color = 0, we have mode = 0 to turn off LEDs
  }

  pot_value = analogRead(POT1);
  
  if(//Start LED processing if one of the following conditions are TRUE:
     //1. If mode or color changes, it's necesary for new LED processing
     (old_mode != mode_select)||(old_color != color_select)||
     //2. If mode is set to blink, processing is necessary to toggle the LED state
     (mode_select == MODE_BLINK)||
     //3. If POT setting value changes, processing is necessary to adjust brightness or toggle frequency
     (old_pot < (pot_value-POT_FILTER_LIMIT))||(old_pot > (pot_value+POT_FILTER_LIMIT))
     )
  {
    Serial.print("Enter control loop with mode: "); Serial.print(mode_select); 
    Serial.print(" , color: "); Serial.print(color_select);
    Serial.print(" , power: "); Serial.println(pot_value);
    old_mode = mode_select;
    old_color = color_select;
    old_pot = pot_value;
    
    switch(mode_select)
    {
      case(MODE_OFF):
        //power value is not relevant
        led_control(color_select, 0, 255);
      break;
  
      case(MODE_ON):
        led_control(color_select, 1, (pot_value / 4));
      break;
  
      case(MODE_BLINK):
        //power value is to max while blinking
        //If blink delay time expired do the following: 
        //1. record timestamp used for next toggle
        //2. run led control
        //3. toggle temp_mode (ON / OFF)
        if((millis() - blink_timestamp) >= pot_value)
        {
          blink_timestamp = millis();
          led_control(color_select, temp_mode, 255);
          temp_mode ^= 1;
        }
      break;

      default:
        //nothing to do
      break;
    }
  }
}
