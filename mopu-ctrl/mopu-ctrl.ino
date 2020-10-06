#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  4

//map
#define RC_CH2  0
#define RC_CH4  1
#define RC_CH5  2
#define RC_CH6  3

#define RC_CH2_INPUT  A0
#define RC_CH4_INPUT  A1
#define RC_CH5_INPUT  A2
#define RC_CH6_INPUT  A3

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];
volatile int var_speed;

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
    analogWrite(10,var_speed);
  }else if(rc_values[RC_CH2] >1600){
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    analogWrite(10,var_speed);
  }else{
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    analogWrite(10,0);
  }
}
void calc_ch4() { 
  calc_input(RC_CH4, RC_CH4_INPUT); 
  if(rc_values[RC_CH4] < 1400){
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    analogWrite(11,var_speed);
  }else if(rc_values[RC_CH4] >1600){
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    analogWrite(11,var_speed);
  }else{
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    analogWrite(11,0);
  }
}
void calc_ch5() { 
  calc_input(RC_CH5, RC_CH5_INPUT);
  if(rc_values[RC_CH5] > 1500){
    digitalWrite(6, LOW);
  }else if(rc_values[RC_CH5] < 1500){
    digitalWrite(6, HIGH);
  }else{
    digitalWrite(6, LOW);
  }
}
void calc_ch6() { 
  calc_input(RC_CH6, RC_CH6_INPUT); 
  var_speed = map(rc_values[RC_CH6],1000,2000,0, 255);
  if(var_speed < 0 )
    var_speed = 1;
}
void setup() {
  //Serial.begin(SERIAL_PORT_SPEED);
  var_speed = 1;
  Serial.begin(9600);
  pinMode(RC_CH2_INPUT, INPUT);//set analog pin as input pin
  pinMode(RC_CH4_INPUT, INPUT);//set analog pin as input pin
  pinMode(RC_CH5_INPUT, INPUT);//set analog pin as input pin
  pinMode(RC_CH6_INPUT, INPUT);//set analog pin as input pin
    
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);//execute function when interrupted by signal from analog pin
  enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);//execute function when interrupted by signal from analog pin
  enableInterrupt(RC_CH5_INPUT, calc_ch5, CHANGE);//execute function when interrupted by signal from analog pin
  enableInterrupt(RC_CH6_INPUT, calc_ch6, CHANGE);//execute function when interrupted by signal from analog pin
}

void loop() {
  rc_read_values();

  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  Serial.print("CH4:"); Serial.print(rc_values[RC_CH4]); Serial.print("\t");
  Serial.print("CH5:"); Serial.print(rc_values[RC_CH5]); Serial.print("\t");
  Serial.print("CH6:"); Serial.println(rc_values[RC_CH6]);
  Serial.print("Var Speed:"); Serial.println(var_speed);
  
  //map(val,1000,2000,0, 255)
  delay(200);
}
