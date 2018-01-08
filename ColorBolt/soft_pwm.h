//soft_pwm.h
#ifndef _SPWM_H
#define _SPWM_H

#ifdef __cplusplus
extern "C" {
#endif

void soft_pwm_init(unsigned char init_value);
void soft_pwm_handler(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_SPWM_H
