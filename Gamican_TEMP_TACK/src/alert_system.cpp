#include <Arduino.h>
#include "alert_system.h"

void alert(float _temperature_, int T_min, int T_max, uint8_t buzzerPin){
  if(_temperature_ < T_min || _temperature_ > T_max){
    digitalWrite(buzzerPin, HIGH);
  }
  else{
    digitalWrite(buzzerPin, LOW);
  }
}
