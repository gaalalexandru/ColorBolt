//soft_pwm.h
#ifndef _SWPWM_H_
#define _SWPWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER1_INTERRUPT(); ISR(TIMER1_COMPA_vect) { soft_pwm_update(); digitalWrite(13,!digitalRead(13)); }

//
//void set_all_leds(unsigned char led_state);
//void led_control(unsigned char color, unsigned char led_mode, unsigned char led_power);
void timer_setup(void);
void soft_pwm_init(unsigned char init_value);
void soft_pwm_update(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_SWPWM_H_
