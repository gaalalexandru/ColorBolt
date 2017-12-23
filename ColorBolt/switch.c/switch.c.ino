//switch.c
#define DEBOUNCE_MS (50)

byte get_switch_value(byte switch_pin)
{
  static byte old_state = HIGH; //store the last state, HIGH due to pullup, static to keep it's value
  static byte new_state; //store new valid state
  static unsigned long reading_time = 0;
  byte return_value;
  byte reading =  digitalRead(switch_pin); //read value from HW
  static byte start_debounce = 0;
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

