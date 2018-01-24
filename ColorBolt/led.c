#include "led.h"
#include "config.h"

extern volatile unsigned char pwm_width_buffer[SOFT_PWM_CHMAX];

//static (local) function definitions (are only used in this file)
static void set_led_red(unsigned char led_power)
{
  pwm_width_buffer[2] = led_power;  //red3
  pwm_width_buffer[5] = led_power;  //red2
  pwm_width_buffer[8] = led_power;  //red1
}

static void set_led_green(unsigned char led_power)
{
  pwm_width_buffer[1] = led_power;  //green3
  pwm_width_buffer[4] = led_power;  //green2
  pwm_width_buffer[7] = led_power;  //green1
}

static void set_led_blue(unsigned char led_power)
{
  pwm_width_buffer[0] = led_power;  //blue3
  pwm_width_buffer[3] = led_power;  //blue2
  pwm_width_buffer[6] = led_power;  //blue1
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
  if(led_power < MIN_BRIGHTNESS)
  {
    led_power = MIN_BRIGHTNESS;
  }
  //reverse led_power value if we use negative logic for common anode LEDs
  #if RGB_NEGATIVE_LOGIC
  //led_power = 255 - led_power;
  #endif
  
  if(led_mode == 0)
  {
    set_led_red(0);
    set_led_green(0);
    set_led_blue(0);
  }
  else
  {
    switch(color)
    {
      case COLOR_RED:
        set_led_red(led_power);
        set_led_green(0);
        set_led_blue(0);
      break;
      
      case COLOR_GREEN:
        set_led_red(0);
        set_led_green(led_power);
        set_led_blue(0);
      break;
      
      case COLOR_BLUE:
        set_led_red(0);
        set_led_green(0);
        set_led_blue(led_power);
      break;
  
      case COLOR_YELLOW:
        set_led_red(led_power);
        set_led_green(led_power);
        set_led_blue(0);        
      break;
  
      case COLOR_PINK:
        set_led_red(led_power);
        set_led_green(0);
        set_led_blue(led_power);
      break;
  
      case COLOR_CYAN:
        set_led_red(0);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;

      case COLOR_WHITE:
        set_led_red(led_power);
        set_led_green(led_power);
        set_led_blue(led_power);
      break;
      
      default:
        set_led_red(0);
        set_led_green(0);
        set_led_blue(0);
      break;
    }
  }
}


