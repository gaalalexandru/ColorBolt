/*
 * Alexandru Gaal:
 * ColorBolt SW for RGB flashlight with variable intensity and blink frequency
 * 2017.12.12
 */

 /*--------------------------Include & Defines secion--------------------------*/
 #include <SoftwareSerial.h>

#define SERIAL_DEBUG (1)

#define RGB_NEGATIVE_LOGIC  (1)

#if RGB_NEGATIVE_LOGIC
#define LED_ON  (0)
#define LED_OFF (1)
#else
#define LED_ON  (1)
#define LED_OFF (0)
#endif RGB_NEGATIVE_LOGIC

#define RED1    (12)
#define GREEN1  (11)
#define BLUE1   (10)
#define RED2    (9)
#define GREEN2  (8)
#define BLUE2   (7)
#define RED3    (6)
#define GREEN3  (5)
#define BLUE3   (4)
#define SWITCH1 (3)
#define SWITCH2 (2)

#define DEBOUNCE_DELAY (50)

int sw1_state = HIGH;
int sw2_state = HIGH;
int sw1_last_state = HIGH;
int sw2_last_state = HIGH;
int all_led_state = 1;

unsigned long sw1_press_time = 0;
unsigned long sw2_press_time = 0;

void led_setup(void)
{
  pinMode(RED1, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(BLUE1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(BLUE2, OUTPUT);
  pinMode(RED3, OUTPUT);
  pinMode(GREEN3, OUTPUT);
  pinMode(BLUE3, OUTPUT);
  digitalWrite(RED1, LED_OFF);
  digitalWrite(GREEN1, LED_OFF);
  digitalWrite(BLUE1, LED_OFF);
  digitalWrite(RED2, LED_OFF);
  digitalWrite(GREEN2, LED_OFF);
  digitalWrite(BLUE2, LED_OFF);
  digitalWrite(RED3, LED_OFF);
  digitalWrite(GREEN3, LED_OFF);
  digitalWrite(BLUE3, LED_OFF);
}

void set_all_leds(int led_state)
{
  digitalWrite(RED1, led_state);
  digitalWrite(GREEN1, led_state);
  digitalWrite(BLUE1, led_state);
  digitalWrite(RED2, led_state);
  digitalWrite(GREEN2, led_state);
  digitalWrite(BLUE2, led_state);
  digitalWrite(RED3, led_state);
  digitalWrite(GREEN3, led_state);
  digitalWrite(BLUE3, led_state);
}

void switch_setup(void)
{
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);
}

void setup() 
{
  // put your setup code here, to run once:
  led_setup();
  switch_setup();
  #if SERIAL_DEBUG
  Serial.begin(9600);
  Serial.println("Setup ready!");
  #endif //SERIAL_DEBUG
}

void loop() {
  sw1_state = digitalRead(SWITCH1);
  if(sw1_state != sw1_last_state)
  {
    sw1_press_time = millis();
  }
  if((millis() - sw1_press_time) > DEBOUNCE_DELAY)
  {
    if(sw1_state != sw1_last_state)
    {
      sw1_last_state = sw1_state;
    }
  }

  
  //sw2_state = digitalRead(SWITCH2);
  
  if(!sw1_state)
  {
    all_led_state ^= 1;

  }
  if(!sw2_state)
  {
  }
  set_all_leds(all_led_state);  
  // put your main code here, to run repeatedly
}
