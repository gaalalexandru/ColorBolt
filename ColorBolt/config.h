//config.h
#ifndef _CFG_H_
#define _CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define OUTPUT_PIN (1)

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
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_BLUE 3
#define COLOR_YELLOW 4
#define COLOR_PINK 5
#define COLOR_CYAN 6
#define COLOR_WHITE 7

//LED control type
#define RGB_NEGATIVE_LOGIC  (1)
#if RGB_NEGATIVE_LOGIC
#define LED_ON  (0)
#define LED_OFF (1)
#else
#define LED_ON  (1)
#define LED_OFF (0)
#endif //RGB_NEGATIVE_LOGIC
#define ANALOG_LED_CONTROL (1)

//Soft pwm config
#define USE_SOFT_PWM (1)
//Number of pins that need software pwm
#define SOFT_PWM_CHMAX (4)
//This is the frequency of the sw PWM
//Frequency = 1/(2 * SOFT_PWM_INTERVAL)
#define SOFT_PWM_INTERVAL (50)  //micro seconds

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_CFG_H_
