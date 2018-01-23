#include "led.h"
#include "config.h"

extern volatile unsigned char pwm_width_buffer[SOFT_PWM_CHMAX];

//static (local) function definitions (are only used in this file)
static void set_led_red(unsigned char led_power)
{
  pwm_width_buffer[2] = 255 - led_power;  //red3
  pwm_width_buffer[5] = 255 - led_power;  //red2
  pwm_width_buffer[8] = 255 - led_power;  //red1
}

static void set_led_green(unsigned char led_power)
{
  pwm_width_buffer[1] = 255 - led_power;  //green3
  pwm_width_buffer[4] = 255 - led_power;  //green2
  pwm_width_buffer[7] = 255 - led_power;  //green1
}

static void set_led_blue(unsigned char led_power)
{
  pwm_width_buffer[0] = 255 - led_power;  //blue3
  pwm_width_buffer[3] = 255 - led_power;  //blue2
  pwm_width_buffer[6] = 255 - led_power;  //blue1
}

//external function definitions (used in other modules / files)
void led_setup(void){}
void set_all_leds(unsigned char led_state){}

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
    set_led_red(255);
    set_led_green(255);
    set_led_blue(255);
  }
  else
  {
    switch(color)
    {
      case COLOR_RED:
        set_led_red(led_power);
        set_led_green(255);
        set_led_blue(255);
      break;
      
      case COLOR_GREEN:
        set_led_red(255);
        set_led_green(led_power);
        set_led_blue(255);
      break;
      
      case COLOR_BLUE:
        set_led_red(255);
        set_led_green(255);
        set_led_blue(led_power);
      break;
  
      case COLOR_YELLOW:
        set_led_red(led_power);
        set_led_green(led_power);
        set_led_blue(255);        
      break;
  
      case COLOR_PINK:
        set_led_red(led_power);
        set_led_green(255);
        set_led_blue(led_power);
      break;
  
      case COLOR_CYAN:
        set_led_red(255);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;

      case COLOR_WHITE:
        set_led_red(led_power);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;
      
      default:
        set_led_red(255);
        set_led_green(255);
        set_led_blue(255);
      break;
    }
  }
}


