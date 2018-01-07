//soft_pwm.c

#include "soft_pwm.h"
#define SOFT_PWM_COUNT (4)

// typedef for properties of each sw pwm pin
typedef struct pwmPins {
  int pin;
  int pwmValue;
  bool pinState;
  int pwmTickCount;
} pwmPin;

const int pinCount = 4;
const byte pins[SOFT_PWM_COUNT] = {4,7,8,12};

pwmPin myPWMpins[SOFT_PWM_COUNT];

// function to "setup" the sw pwm pin states
void setup_soft_pwm(void)
{
  unsigned char index = 0;
  for (index=0; index < SOFT_PWM_COUNT; index++) {
    myPWMpins[index].pin = pins[index];
 
    // mix it up a little bit
    // changes the starting pwmValue for odd and even
    myPWMpins[index].pwmValue = 50;
    
    myPWMpins[index].pinState = ON;
    myPWMpins[index].pwmTickCount = 0;
 
    // unlike analogWrite(), this is necessary
    pinMode(pins[index], OUTPUT);
  }
}

void handle_soft_pwm(void) {
  currentMicros = micros();
  // check to see if we need to increment our PWM counters yet
  if (currentMicros - previousMicros >= microInterval) {
    // Increment each pin's counter
    for (int index=0; index < pinCount; index++) {
    // each pin has its own tickCounter
      myPWMpins[index].pwmTickCount++;
 
    // determine if we're counting on or off time
      if (myPWMpins[index].pinState == ON) {
        // see if we hit the desired on percentage
        // not as precise as 255 or 1024, but easier to do math
        if (myPWMpins[index].pwmTickCount >= myPWMpins[index].pwmValue) 
        {
          myPWMpins[index].pinState = OFF;
        }
      } 
      else 
      {
        // if it isn't on, it is off
        if (myPWMpins[index].pwmTickCount >= pwmMax) 
        {
          myPWMpins[index].pinState = ON;
          myPWMpins[index].pwmTickCount = 0;
        }
      }
      // could probably use some bitwise optimization here, digitalWrite()
      // really slows things down after 10 pins.
      digitalWrite(myPWMpins[index].pin, !myPWMpins[index].pinState);
    }
    // reset the micros() tick counter.
    digitalWrite(13, !digitalRead(13));
    previousMicros = currentMicros;
  }
}
