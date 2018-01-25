/*
 * Alexandru Gaal:
 * ColorBolt SW for RGB flashlight with variable brightness and blink frequency
 * 2017.12.12
 */

// Include & Defines section
#include "config.h"
#include "led.h"
#include "soft_pwm.h"

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
  //setup digital pins for the switch input
  switch_setup();
  
  //Make first analog read to initialize old_pot since initial POT position is unknown
  pot_value = analogRead(POT1);
  old_pot = pot_value;

  //led_setup();
  //init soft pwm with current pot value
  soft_pwm_init(pot_value/4);
  //setup timer for softpwm
  cli();  //disable all interrupts
  timer_setup();
  sei();  //enable all interrupts
  
  //start serial port
  Serial.begin(9600);
  Serial.println("Setup ready!");
  
  pinMode(13,OUTPUT);
  Serial.println("Interrupt initialized!");
}

void loop() 
{
  //Serial.println(millis());
  //local variable declarations
  unsigned char sw1_read = 1;
  unsigned char sw2_read = 0;
  //static is necessary to retain the value of local variable after exiting the function
  static unsigned char old_sw1 = 1;
  static unsigned char old_sw2 = 1;;
  static unsigned char sw1_state = 1;
  static unsigned char sw2_state = 1;
  static unsigned char temp_mode = 0;
  static unsigned char mode_select = DEFAULT_MODE;
  static unsigned char color_select = DEFAULT_COLOR;
  static unsigned char old_mode = 0;
  static unsigned char old_color = color_select;
  static unsigned long blink_timestamp = 0;
  static unsigned long sw1_debounce = 0;
  static unsigned long sw2_debounce = 0;
    
  //Reading of switch (digital) values
  //Switch1 handling: reading, debouncing, setting mode if necessary
  sw1_read = digitalRead(SWITCH1);
  //if switch press or noise record timestamp
  if(old_sw1 != sw1_read)
  {
    sw1_debounce = millis();
  }
  //if time expired check if switch state realy changed, if yes update state
  if ((millis() - sw1_debounce) > DEBOUNCE_DELAY)
  {
    if(sw1_read != sw1_state)
    {
      sw1_state = sw1_read;
      if(!sw1_state)
      {
        mode_select = (mode_select + 1) % NR_OF_MODES;  //toggle between 0 - NR_OF_MODES
      }     
    }
  }
  old_sw1 = sw1_read;
  
  //Switch2 handling: reading, debouncing, setting color if necessary
  sw2_read = digitalRead(SWITCH2);
  if(old_sw2 != sw2_read)
  {
    sw2_debounce = millis();
  }
  if ((millis() - sw2_debounce) > DEBOUNCE_DELAY)
  {
    if(sw2_read != sw2_state)
    {
      sw2_state = sw2_read;
      if(!sw2_state)
      {
        color_select = (color_select + 1) % NR_OF_COLORS; //toggle between 0 - NR_OF_COLORS
        if(color_select == 0) { color_select = 1; }  //ignore color = 0, we have mode = 0 to turn off LEDs
      }     
    }
  }
  old_sw2 = sw2_read;

  
  //Reading of potentiometer (analog) value
  pot_value = analogRead(POT1);
  if(pot_value > 1016)  //why??? 
  {
    pot_value = 1016;
  }
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
        led_control(color_select, 0, 0);
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
          led_control(color_select, temp_mode, BLINK_BRIGHTNESS);
          temp_mode ^= 1;
        }
      break;

      default:
        //nothing to do
      break;
    }
  }
}

TIMER1_INTERRUPT();

