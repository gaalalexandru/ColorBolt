//config.h
#ifndef _CFG_H_
#define _CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

//Pin mapping for switches and pot
#define SWITCH1 (3)
#define SWITCH2 (2)
#define POT1    (A7)

//Pin mapping for LEDs
#define RED1   (12)
#define GREEN1 (11)
#define BLUE1  (10)
#define RED2   (9)
#define GREEN2 (8)
#define BLUE2  (7)
#define RED3   (6)
#define GREEN3 (5)
#define BLUE3  (4)

//General mode configurations
//0 = off, 1 = on, 2 = blink
#define NR_OF_MODES  (3)
#define MODE_OFF     (0)
#define MODE_ON      (1)
#define MODE_BLINK   (2)
#define DEFAULT_MODE (MODE_ON)

//General color configurations
//0 = off, 1 = red, 2 = green
//3 = blue, 4 = yellow, 5 = pink
//6 = cyan, 7 = white
#define NR_OF_COLORS  (8)
#define COLOR_RED     (1)
#define COLOR_GREEN   (2)
#define COLOR_BLUE    (3)
#define COLOR_YELLOW  (4)
#define COLOR_PINK    (5)
#define COLOR_CYAN    (6)
#define COLOR_WHITE   (7)
#define DEFAULT_COLOR (COLOR_WHITE)

#define POT_FILTER_LIMIT (10)
#define DEBOUNCE_DELAY   (100)
#define MIN_BRIGHTNESS   (4)
#define BLINK_BRIGHTNESS (255)

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
#define SOFT_PWM_CHMAX (9)

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_CFG_H_
