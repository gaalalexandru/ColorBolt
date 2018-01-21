/*
 * Alexandru Gaal:
 * ColorBolt SW for RGB flashlight with variable brightness and blink frequency
 * 2017.12.12
 */

// Include & Defines section
#include "config.h"
#include "led.h"

//Global variable declarations
int old_pot = 0;  //needs to be signed due to filtering
int pot_value = 0;  //needs to be signed due to filtering
volatile unsigned char pwm_width[SOFT_PWM_CHMAX];
volatile unsigned char pwm_width_buffer[SOFT_PWM_CHMAX];
const unsigned char pins[SOFT_PWM_CHMAX] = {4,7,8,12}; //Blue3 , Blue2, Green2, Red2

//Function declarations, static functions are visible only in this file
static void switch_setup(void)
{
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);
}
void soft_pwm_init(unsigned char init_value)
{
  unsigned char i, pwm;
  pwm = init_value;
  for (i = 0; i < SOFT_PWM_CHMAX; i++) // initialise all channels
  {
    pwm_width[i]  = pwm; // set default PWM values
    pwm_width_buffer[i] = pwm; // set default PWM values
  }
  pinMode(pins[0], OUTPUT);
  pinMode(pins[1], OUTPUT);
  pinMode(pins[2], OUTPUT);
  pinMode(pins[3], OUTPUT);
  
  digitalWrite(pins[0],LED_OFF);
  digitalWrite(pins[1],LED_OFF);
  digitalWrite(pins[2],LED_OFF);
  digitalWrite(pins[3],LED_OFF);
}
static void soft_pwm_update(void)
{
  static unsigned char softcount = 0x00;
  //softcount = (softcount + 1) % 100;
  softcount++;
  if (softcount == 0)
  //verbose code for speed, do not replace with for...
  //last element should equal SOFT_PWM_CHMAX - 1
  {
    pwm_width[0] = pwm_width_buffer[0]; 
    pwm_width[1] = pwm_width_buffer[1];
    pwm_width[2] = pwm_width_buffer[2];
    pwm_width[3] = pwm_width_buffer[3];
    //if duty cycle width > 0 condition is evaluated as true = 1
    //else condition is evaluated as false = 0
    #if RGB_NEGATIVE_LOGIC
    //digitalWrite(pins[0],(pwm_width[0] == 255));
    //digitalWrite(pins[1],(pwm_width[1] == 255));
    //digitalWrite(pins[2],(pwm_width[2] == 255));
    //digitalWrite(pins[3],(pwm_width[3] == 255));
    PORTB &= ((1<<4)|(1<<0));
    PORTD &= ((1<<4)|(1<<7));
    //digitalWrite(pins[3],LED_ON);
    #else
    digitalWrite(pins[0],LED_ON);
    digitalWrite(pins[1],LED_ON);
    digitalWrite(pins[2],LED_ON);
    digitalWrite(pins[3],LED_ON); 
    #endif  //RGB_NEGATIVE_LOGIC
  }
  /*else
  {*/
    // clear port pin on pwm_width match
    if (pwm_width[0] == softcount)
    {
      //digitalWrite(4,LED_OFF);
      PORTD |= (1<<4);
    }
    if (pwm_width[1] == softcount)
    {
      //digitalWrite(7,LED_OFF);
      PORTD |= (1<<7);
    }
    if (pwm_width[2] == softcount)
    {
      //digitalWrite(8,LED_OFF);
      PORTB |= (1<<0);
    }
    if (pwm_width[3] == softcount)
    {
      //digitalWrite(12,LED_OFF);
      PORTB |= (1<<4);
    }    
  //}
}

void setup() 
{
  //setup digital pins
  led_setup();
  switch_setup();
  
  //Make first analog read to initialize old_pot since initial POT position is unknown
  pot_value = analogRead(POT1);
  old_pot = pot_value;

  //init soft pwm with current pot value
  soft_pwm_init(pot_value/4);

  //start serial port
  Serial.begin(9600);
  Serial.println("Setup ready!");
}

void loop() 
{
  //local variable declarations
  unsigned char sw1_read = 1;
  unsigned char sw2_read = 0;
  //static is necessary to retain the value of local variable after exiting the function
  static unsigned char old_sw1 = 1;
  static unsigned char old_sw2 = 1;;
  static unsigned char sw1_state = 1;
  static unsigned char sw2_state = 1;
  static unsigned char temp_mode = 0;
  static unsigned char mode_select = 0;
  static unsigned char color_select = DEFAULT_COLOR;
  static unsigned char old_mode = mode_select;
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
  if(pot_value > 1016) 
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
        led_control(color_select, 0, 254);
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
          led_control(color_select, temp_mode, 250);
          temp_mode ^= 1;
        }
      break;

      default:
        //nothing to do
      break;
    }
  }
  //soft_pwm_handler();
  unsigned long currentMicros = micros();
  static unsigned char status_led = 0;
  static unsigned long previousMicros = 0;
  //currentMicros = micros();
  // check to see if we need to increment our PWM counters yet
  /*
  if (currentMicros - previousMicros >= SOFT_PWM_INTERVAL)
  {
    
    // reset the micros() tick counter.
    digitalWrite(13, status_led);
    status_led ^= 1;
    previousMicros = currentMicros;
  }*/
  soft_pwm_update();
}
