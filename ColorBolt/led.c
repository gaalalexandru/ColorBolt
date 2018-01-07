#include "led.h"
#include <math.h>

//Pin mapping for LEDs
#define RED1    (12)
#define GREEN1  (11)
#define BLUE1   (10)
#define RED2    (9)
#define GREEN2  (8)
#define BLUE2   (7)
#define RED3    (6)
#define GREEN3  (5)
#define BLUE3   (4)

//Color codes
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define PINK 5
#define CYAN 6
#define WHITE 7

//LED control type
#define RGB_NEGATIVE_LOGIC  (1)
#if RGB_NEGATIVE_LOGIC
#define LED_ON  (0)
#define LED_OFF (1)
#else
#define LED_ON  (1)
#define LED_OFF (0)
#endif //RGB_NEGATIVE_LOGIC
#define ANALOG_CONTROL (1)

//static (local) function definitions (are only used in this file)
static void set_led_red(unsigned char led_power)
{
  #if ANALOG_CONTROL
  analogWrite(RED1,led_power); analogWrite(RED2,led_power); analogWrite(RED3,led_power);
  #else
  digitalWrite(RED1,LED_ON); digitalWrite(RED2,LED_ON); digitalWrite(RED3,LED_ON);
  #endif //ANALOG_CONTROL
}

static void set_led_green(unsigned char led_power)
{
  #if ANALOG_CONTROL
  analogWrite(GREEN1,led_power); analogWrite(GREEN2,led_power); analogWrite(GREEN3,led_power);
  #else
  digitalWrite(GREEN1,LED_ON); digitalWrite(GREEN2,LED_ON); digitalWrite(GREEN3,LED_ON);
  #endif //ANALOG_CONTROL
}

static void set_led_blue(unsigned char led_power)
{
  #if ANALOG_CONTROL
  analogWrite(BLUE1,led_power); analogWrite(BLUE2,led_power); analogWrite(BLUE3,led_power);
  #else
  digitalWrite(BLUE1,LED_ON); digitalWrite(BLUE2,LED_ON); digitalWrite(BLUE3,LED_ON);
  #endif //ANALOG_CONTROL
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
  unsigned char index = 0;
  for(index = BLUE3; index <= RED1; index ++)
  {
    digitalWrite(index,led_state);
  }
}

void led_control(unsigned char color, unsigned char led_mode, unsigned char led_power)
//Paremeters for led control:
//1. color = the selected color 1 - 7
//2. led_mode = the state for all leds, ON / OFF
//3. led_power = the 8 bit duty cycle for PWM output signal
{
  #if RGB_NEGATIVE_LOGIC
  led_power = 255 - led_power;
  //led_power = 127;
  #endif
  
  if(led_mode == 0)
  {
    set_all_leds(LED_OFF);
  }
  else
  {
    switch(color)
    {
      case RED:
        set_all_leds(LED_OFF);
        //Serial.println("red");
        set_led_red(led_power);
      break;
      
      case GREEN:
        set_all_leds(LED_OFF);
        //Serial.println("green");
        set_led_green(led_power);
      break;
      
      case BLUE:
        set_all_leds(LED_OFF);
        set_led_blue(led_power);
      break;
  
      case YELLOW:
        set_all_leds(LED_OFF);
        set_led_red(led_power);
        set_led_green(led_power);
      break;
  
      case PINK:
        set_all_leds(LED_OFF);
        set_led_red(led_power);
        set_led_blue(led_power);
      break;
  
      case CYAN:
        set_all_leds(LED_OFF);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;

      case WHITE:
        set_all_leds(LED_ON);
        set_led_red(led_power);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;
      
      default:
        //set_all_leds(LED_OFF);
        //Serial.println("Undefined color!");
      break;
    }
  }
}


