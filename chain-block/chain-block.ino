#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  2

#define RC_CH2  0
#define RC_CH5  1

#define RC_CH2_INPUT  A0
#define RC_CH5_INPUT  A1

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];
volatile bool need_neutral = false;

//to make signal not determine
volatile bool relay1 = false;
volatile bool relay2 = false;
volatile bool relay3 = false;
volatile bool relay4 = false;

//start time
volatile unsigned long start_time = 0;
volatile unsigned long current_time = 0;
volatile unsigned long elapsed_time = 0;

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void set_relay_state(){  
  if(relay1){
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
  }else{
    pinMode(4, INPUT);
    digitalWrite(4, HIGH);
  }

  if(relay2){
    pinMode(5, OUTPUT);
    digitalWrite(5, LOW);
  }else{
    pinMode(5, INPUT);
    digitalWrite(5, HIGH);
  }
  
  if(relay3){
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);    
  }else{
    pinMode(6, INPUT);
    digitalWrite(6, HIGH);
  }
  
  if(relay4){
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);    
  }else{
    pinMode(7, INPUT);
    digitalWrite(7, HIGH);    
  }
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

  //calculate how long hoist switched on
  if(relay1==false && relay2 ==false){
    start_time=millis();
    need_neutral = false;
  }else if(relay1==true && relay2 ==true){
      current_time=millis();
      elapsed_time=current_time - start_time;
  }
  
  if(rc_values[RC_CH2] > 1700 && need_neutral==false){
      relay1=true;
      relay2=true; 
      if(elapsed_time >= 1500){
        relay1=false;
        relay2=false;
        need_neutral=true;
        return;
     }
  }else if(rc_values[RC_CH2] < 1300 ){
    relay3=true;
    relay4=true;
  }else{
    relay1=false;
    relay2=false;
    relay3=false;
    relay4=false;
    elapsed_time=0;
  }

}
void calc_ch5() { calc_input(RC_CH5, RC_CH5_INPUT); }

void setup() {
  //Serial.begin(SERIAL_PORT_SPEED);
  Serial.begin(9600);
  pinMode(RC_CH2_INPUT, INPUT);//set analog pin as input pin
  pinMode(RC_CH5_INPUT, INPUT);//set analog pin as input pin
                                          
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);//execute function when interrupted by signal from analog pin
  enableInterrupt(RC_CH5_INPUT, calc_ch5, CHANGE);//execute function when interrupted by signal from analog pin
}

void loop() {
  rc_read_values();
  set_relay_state();
  Serial.print("Start:");Serial.print(start_time);Serial.print("\t");
  Serial.print("Current:");Serial.print(current_time);Serial.print("\t");
  Serial.print("Elapsed:");Serial.print(elapsed_time);Serial.print("\t");
  Serial.print("Need Neutral:");Serial.println(need_neutral);Serial.print("\t");
//  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
//  Serial.print("CH5:"); Serial.println(rc_values[RC_CH5]);
}
