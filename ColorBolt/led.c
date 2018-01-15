#include "led.h"
#include "config.h"

extern volatile unsigned char pwm_width_buffer[SOFT_PWM_CHMAX];

//static (local) function definitions (are only used in this file)
static void set_led_red(unsigned char led_power)
{
  #if ANALOG_LED_CONTROL
    #if USE_SOFT_PWM
      pwm_width_buffer[3] = 255 - led_power;
    #else
      analogWrite(RED1,led_power);
    #endif  //USE_SOFT_PWM
    analogWrite(RED2,led_power);
    analogWrite(RED3,led_power);
  #else
    digitalWrite(RED1,LED_ON); digitalWrite(RED2,LED_ON); digitalWrite(RED3,LED_ON);
  #endif //ANALOG_LED_CONTROL
}

static void set_led_green(unsigned char led_power)
{
  #if ANALOG_LED_CONTROL
    analogWrite(GREEN1,led_power);
    #if USE_SOFT_PWM
      pwm_width_buffer[2] = 255 - led_power;
    #else  
      analogWrite(GREEN2,led_power);
    #endif  //USE_SOFT_PWM
    analogWrite(GREEN3,led_power);
  #else
    digitalWrite(GREEN1,LED_ON); digitalWrite(GREEN2,LED_ON); digitalWrite(GREEN3,LED_ON);
  #endif //ANALOG_LED_CONTROL
}

static void set_led_blue(unsigned char led_power)
{
  #if ANALOG_LED_CONTROL
    analogWrite(BLUE1,led_power);
    #if USE_SOFT_PWM
      pwm_width_buffer[1] = 255 - led_power;
      pwm_width_buffer[0] = 255 - led_power;
    #else       
      analogWrite(BLUE2,led_power); 
      analogWrite(BLUE3,led_power);
    #endif  //USE_SOFT_PWM
  #else
    digitalWrite(BLUE1,LED_ON); digitalWrite(BLUE2,LED_ON); digitalWrite(BLUE3,LED_ON);
  #endif //ANALOG_LED_CONTROL
}

//external function definitions (used in other modules / files)
void led_setup(void)
{
  unsigned char index = 0;
  for(index = BLUE3; index <= RED1; index ++)
  {
    pinMode(index,1);
  }  
  set_all_leds(LED_OFF);
}

void set_all_leds(unsigned char led_state)
{
  /*
  unsigned char index = 0;
  for(index = BLUE3; index <= RED1; index ++)
  {
    digitalWrite(index,led_state);
  }
  */
  if(led_state == LED_OFF)
  {
    set_led_red(255);
    set_led_green(255);
    set_led_blue(255);
  }
}

void led_control(unsigned char color, unsigned char led_mode, unsigned char led_power)
//Paremeters for led control:
//1. color = the selected color 1 - 7
//2. led_mode = the state for all leds, ON / OFF
//3. led_power = the 8 bit duty cycle for PWM output signal
{
  //ensure that we have a minimum brightness of ~5% duty cycle
  if(led_power < 6)
  {
    led_power = 6;
  }
  //reverse led_power value if we use negative logic for common anode LEDs
  #if RGB_NEGATIVE_LOGIC
  led_power = 255 - led_power;
  #endif
  
  if(led_mode == 0)
  {
    set_all_leds(LED_OFF);
  }
  else
  {
    switch(color)
    {
      case COLOR_RED:
        //set_all_leds(LED_OFF);
        set_led_red(led_power);
        set_led_green(255);
        set_led_blue(255);
      break;
      
      case COLOR_GREEN:
        //set_all_leds(LED_OFF);
        set_led_red(255);
        set_led_green(led_power);
        set_led_blue(255);
      break;
      
      case COLOR_BLUE:
        //set_all_leds(LED_OFF);
        set_led_red(255);
        set_led_green(255);
        set_led_blue(led_power);
      break;
  
      case COLOR_YELLOW:
        //set_all_leds(LED_OFF);
        set_led_red(led_power);
        set_led_green(led_power);
        set_led_blue(255);        
      break;
  
      case COLOR_PINK:
        //set_all_leds(LED_OFF);
        set_led_red(led_power);
        set_led_green(255);
        set_led_blue(led_power);
      break;
  
      case COLOR_CYAN:
        //set_all_leds(LED_OFF);
        set_led_red(255);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;

      case COLOR_WHITE:
        //set_all_leds(LED_ON);
        set_led_red(led_power);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;
      
      default:
        //set_all_leds(LED_OFF);
        set_led_red(255);
        set_led_green(255);
        set_led_blue(255);
      break;
    }
  }
}


