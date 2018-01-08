//soft_pwm.c

#include "soft_pwm.h"
#include "config.h"

unsigned char pwm_width[SOFT_PWM_CHMAX];
volatile unsigned char pwm_width_buffer[SOFT_PWM_CHMAX];
const unsigned char pins[SOFT_PWM_CHMAX] = {4,7,8,12}; //Blue3 , Blue2, Green2, Red2

// function to initialize the sw pwm pins
void soft_pwm_init(unsigned char init_value)
{
  unsigned char i, pwm;
  pwm = init_value;
  for (i = 0; i < SOFT_PWM_CHMAX; i++) // initialise all channels
  {
    pwm_width[i]  = pwm; // set default PWM values
    pwm_width_buffer[i] = pwm; // set default PWM values
  }
  pinMode(pins[0], OUTPUT_PIN);
  pinMode(pins[1], OUTPUT_PIN);
  pinMode(pins[2], OUTPUT_PIN);
  pinMode(pins[3], OUTPUT_PIN);
  pinMode(13, OUTPUT_PIN);
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
    digitalWrite(pins[0],!(pwm_width[0] > 0));
    digitalWrite(pins[1],!(pwm_width[1] > 0));
    digitalWrite(pins[2],!(pwm_width[2] > 0));
    //digitalWrite(pins[3],!(pwm_width[3] > 0));
    digitalWrite(pins[3],LED_ON);
    #else
    digitalWrite(pins[0],(pwm_width[0] > 0));
    digitalWrite(pins[1],(pwm_width[1] > 0));
    digitalWrite(pins[2],(pwm_width[2] > 0));
    digitalWrite(pins[3],(pwm_width[3] > 0)); 
    #endif  //RGB_NEGATIVE_LOGIC
  }
  /*else
  {*/
    // clear port pin on pwm_width match
    if (pwm_width[0] == softcount)
    {
      digitalWrite(pins[0],LED_OFF);
    }
    if (pwm_width[1] == softcount)
    {
      digitalWrite(pins[1],LED_OFF);
    }
    if (pwm_width[2] == softcount)
    {
      digitalWrite(pins[2],LED_OFF);
    }
    if (pwm_width[3] == softcount)
    {
      digitalWrite(pins[2],LED_OFF);
    }    
  //}
}

void soft_pwm_handler(void) 
{
  unsigned long currentMicros = micros();
  static unsigned long previousMicros = 0;
  //currentMicros = micros();
  // check to see if we need to increment our PWM counters yet
  if (currentMicros - previousMicros >= SOFT_PWM_INTERVAL)
  {
    soft_pwm_update();
    // reset the micros() tick counter.
    digitalWrite(13, !digitalRead(13));
    previousMicros = currentMicros;
  }
}
