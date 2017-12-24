#include "led.h"
//#include <SoftwareSerial.h>

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

void led_control(unsigned char color, unsigned char led_state, unsigned int led_power)
{
  if(led_state == 1)
  {
    switch(color)
    {
      case RED:
        set_all_leds(LED_OFF);
        digitalWrite(RED1,LED_ON); digitalWrite(RED2,LED_ON); digitalWrite(RED3,LED_ON);
      break;
      
      case GREEN:
        set_all_leds(LED_OFF);
        digitalWrite(GREEN1,LED_ON); digitalWrite(GREEN2,LED_ON); digitalWrite(GREEN3,LED_ON);
      break;
      
      case BLUE:
        set_all_leds(LED_OFF);
        digitalWrite(BLUE1,LED_ON); digitalWrite(BLUE2,LED_ON); digitalWrite(BLUE3,LED_ON);
      break;
  
      case YELLOW:
        set_all_leds(LED_OFF);
        digitalWrite(RED1,LED_ON); digitalWrite(RED2,LED_ON); digitalWrite(RED3,LED_ON);
        digitalWrite(GREEN1,LED_ON); digitalWrite(GREEN2,LED_ON); digitalWrite(GREEN3,LED_ON);
      break;
  
      case PINK:
        set_all_leds(LED_OFF);
        digitalWrite(RED1,LED_ON); digitalWrite(RED2,LED_ON); digitalWrite(RED3,LED_ON);
        digitalWrite(BLUE1,LED_ON); digitalWrite(BLUE2,LED_ON); digitalWrite(BLUE3,LED_ON);
      break;
  
      case CYAN:
        set_all_leds(LED_OFF);
        digitalWrite(GREEN1,LED_ON); digitalWrite(GREEN2,LED_ON); digitalWrite(GREEN3,LED_ON);
        digitalWrite(BLUE1,LED_ON); digitalWrite(BLUE2,LED_ON); digitalWrite(BLUE3,LED_ON);
      break;

      case WHITE:
        set_all_leds(LED_ON);
      break;
      
      default:
        set_all_leds(LED_OFF);
        //Serial.println("Undefined color!");
      break;
    }
  }
  else
  {
    set_all_leds(LED_OFF);
  }
}


