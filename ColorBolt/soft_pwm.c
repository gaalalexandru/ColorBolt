#include "soft_pwm.h"
#include "config.h"
#include <avr/io.h>

volatile unsigned char pwm_width[SOFT_PWM_CHMAX];
volatile unsigned char pwm_width_buffer[SOFT_PWM_CHMAX];
const unsigned char pins[SOFT_PWM_CHMAX] = {4,5,6,7,8,9,10,11,12};

void timer_setup(void)
{
//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 50;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= /*(1 << CS12) | */(1 << CS11);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
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
  //Directly writing to controller register is much faster
  //than using pinMode and digitalWrite function
  //Set direction of pins
  //DDRx is the Data Direction Register for port x
  //Similar to pinMode function, but much faster
  //Arduino Pin Nr and ATMEGA328 Pin Nr mapping is:
  //Pin 4 => PD4 (Port D, Pin 4)
  //Pin 5 => PD5 (Port D, Pin 5)
  //Pin 6 => PD6
  //Pin 7 => PD7
  //Pin 8 => PB0 (Port B, Pin 0)
  //Pin 9 => PB1 (Port B, Pin 1)
  //Pin 10 => PB2
  //Pin 11 => PB3
  //Pin 12 => PB4
  DDRB |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);  //PB0 - 4
  DDRD |= (1 << 4) | (1 << 5)| (1 << 6)| (1 << 7);  //PD4 - 7
  //Set value of pins
  //PORTx is the Port / Pin state Register for port x
  //Similar to digitalWrite, but much faster
  PORTB |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);  //PB0 - 4
  PORTD |= (1 << 4) | (1 << 5)| (1 << 6)| (1 << 7);  //PD4 - 7
}

void soft_pwm_update(void)
{
  static unsigned char softcount = 0x00;
  softcount++;
  if (softcount == 0)
  //verbose code for speed, do not replace with for...
  //last element should equal SOFT_PWM_CHMAX - 1
  {
    pwm_width[0] = pwm_width_buffer[0]; 
    pwm_width[1] = pwm_width_buffer[1];
    pwm_width[2] = pwm_width_buffer[2];
    pwm_width[3] = pwm_width_buffer[3];
    pwm_width[4] = pwm_width_buffer[4];
    pwm_width[5] = pwm_width_buffer[5]; 
    pwm_width[6] = pwm_width_buffer[6];
    pwm_width[7] = pwm_width_buffer[7];
    pwm_width[8] = pwm_width_buffer[8];

    //if duty cycle width > 0 condition is evaluated as true = 1, led should be turned on
    //else condition is evaluated as false = 0, led should be turned off
    #if RGB_NEGATIVE_LOGIC
    (pwm_width[0] > 0) ? (PORTD &= ~(1 << 4)):(PORTD |= (1 << 4));
    (pwm_width[1] > 0) ? (PORTD &= ~(1 << 5)):(PORTD |= (1 << 5));
    (pwm_width[2] > 0) ? (PORTD &= ~(1 << 6)):(PORTD |= (1 << 6));
    (pwm_width[3] > 0) ? (PORTD &= ~(1 << 7)):(PORTD |= (1 << 7)); 
    (pwm_width[4] > 0) ? (PORTB &= ~(1 << 0)):(PORTB |= (1 << 0));
    (pwm_width[5] > 0) ? (PORTB &= ~(1 << 1)):(PORTB |= (1 << 1));
    (pwm_width[6] > 0) ? (PORTB &= ~(1 << 2)):(PORTB |= (1 << 2));
    (pwm_width[7] > 0) ? (PORTB &= ~(1 << 3)):(PORTB |= (1 << 3));
    (pwm_width[8] > 0) ? (PORTB &= ~(1 << 4)):(PORTB |= (1 << 4));
    #else
    (pwm_width[0] > 0) ? (PORTD |= (1 << 4)):(PORTD &= ~(1 << 4));
    (pwm_width[1] > 0) ? (PORTD |= (1 << 5)):(PORTD &= ~(1 << 5));
    (pwm_width[2] > 0) ? (PORTD |= (1 << 6)):(PORTD &= ~(1 << 6));
    (pwm_width[3] > 0) ? (PORTD |= (1 << 7)):(PORTD &= ~(1 << 7)); 
    (pwm_width[4] > 0) ? (PORTB |= (1 << 0)):(PORTB &= ~(1 << 0));
    (pwm_width[5] > 0) ? (PORTB |= (1 << 1)):(PORTB &= ~(1 << 1));
    (pwm_width[6] > 0) ? (PORTB |= (1 << 2)):(PORTB &= ~(1 << 2));
    (pwm_width[7] > 0) ? (PORTB |= (1 << 3)):(PORTB &= ~(1 << 3));
    (pwm_width[8] > 0) ? (PORTB |= (1 << 4)):(PORTB &= ~(1 << 4));
    #endif  //RGB_NEGATIVE_LOGIC
  }
  else
  {
    // clear port pin on pwm_width match
    #if RGB_NEGATIVE_LOGIC
    if (pwm_width[0] == softcount) { PORTD |= (1 << 4); }
    if (pwm_width[1] == softcount) { PORTD |= (1 << 5); }
    if (pwm_width[2] == softcount) { PORTD |= (1 << 6); }
    if (pwm_width[3] == softcount) { PORTD |= (1 << 7); }
    if (pwm_width[4] == softcount) { PORTB |= (1 << 0); }
    if (pwm_width[5] == softcount) { PORTB |= (1 << 1); }
    if (pwm_width[6] == softcount) { PORTB |= (1 << 2); }
    if (pwm_width[7] == softcount) { PORTB |= (1 << 3); }
    if (pwm_width[8] == softcount) { PORTB |= (1 << 4); }
    #else
    if (pwm_width[0] == softcount) { PORTD &= ~(1 << 4); }
    if (pwm_width[1] == softcount) { PORTD &= ~(1 << 5); }
    if (pwm_width[2] == softcount) { PORTD &= ~(1 << 6); }
    if (pwm_width[3] == softcount) { PORTD &= ~(1 << 7); }
    if (pwm_width[4] == softcount) { PORTB &= ~(1 << 0); }
    if (pwm_width[5] == softcount) { PORTB &= ~(1 << 1); }
    if (pwm_width[6] == softcount) { PORTB &= ~(1 << 2); }
    if (pwm_width[7] == softcount) { PORTB &= ~(1 << 3); }
    if (pwm_width[8] == softcount) { PORTB &= ~(1 << 4); }	
    #endif  //RGB_NEGATIVE_LOGIC
  }
}



