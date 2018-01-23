//led.h
#ifndef _LED_H_
#define _LED_H_

#ifdef __cplusplus
extern "C" {
#endif

//void led_setup(void);
//void set_all_leds(unsigned char led_state);
void led_control(unsigned char color, unsigned char led_mode, unsigned char led_power);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_LED_H_
