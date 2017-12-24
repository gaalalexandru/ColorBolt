//switch.c
#define DEBOUNCE_MS (50)

unsigned char get_switch_value(unsigned char switch_pin)
{
  static unsigned char old_state = 1; //store the last state, HIGH due to pullup, static to keep it's value
  static unsigned char new_state; //store new valid state
  static unsigned long reading_time = 0;
  unsigned char return_value;
  unsigned char reading =  digitalRead(switch_pin); //read value from HW
  static unsigned char start_debounce = 0;
  if(old_state != reading) 
  {
    reading_time = millis();//record press time
    start_debounce = 1;
  }
  if(start_debounce)
  {
    if(millis() > (reading_time + DEBOUNCE_MS))
    {
      if(reading != new_state)
      {
        new_state = reading;
        if(!new_state) //if it's LOW (pressed)
        {
          return_value = 1;
        }
        else
        {
          return_value = 0;
        }
        start_debounce = 0;
      }
    }
  }
}


