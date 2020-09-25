#include <EnableInterrupt.h>
double channel;
int ch_val;
void setup(){
   pinMode(2, INPUT);//configure as input or output
   attachInterrupt(2, 0, RISING);
   Serial.begin(9600);
}
void loop(){
  //channel = pulseIn(2, HIGH);//record duration of the pin was high for a period of time
  ch_val = digitalRead(2);
  Serial.println(ch_val);
}
