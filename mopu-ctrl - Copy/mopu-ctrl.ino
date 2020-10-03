#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  2

//map
#define RC_CH2  0
#define RC_CH3  1

#define RC_CH2_INPUT  A0
#define RC_CH3_INPUT  A1

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

//calculate pwm lenght
void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();//record when the pin start became HIGH
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);//how long was the pin HIGH
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch2() { 
  calc_input(RC_CH2, RC_CH2_INPUT);
  if(rc_values[RC_CH2] <1400){
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    analogWrite(10,255);
  }else if(rc_values[RC_CH2] >1600){
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    analogWrite(10,255);
  }else{
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    analogWrite(10,0);
  }
}
void calc_ch3() { 
  calc_input(RC_CH3, RC_CH3_INPUT); 
  if(rc_values[RC_CH3] < 1400){
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    analogWrite(11,255);
  }else if(rc_values[RC_CH3] >1600){
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    analogWrite(11,255);
  }else{
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    analogWrite(11,0);
  }
}

void setup() {
  //Serial.begin(SERIAL_PORT_SPEED);
  Serial.begin(9600);
  pinMode(RC_CH2_INPUT, INPUT);//set analog pin as input pin
  pinMode(RC_CH3_INPUT, INPUT);//set analog pin as input pin

//  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);//execute function when interrupted by signal from analog pin
  enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);//execute function when interrupted by signal from analog pin
}

void loop() {
  rc_read_values();

  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  Serial.print("CH3:"); Serial.println(rc_values[RC_CH3]);
  //map(val,1000,2000,0, 255)
  delay(200);
}
