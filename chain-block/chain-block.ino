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
volatile bool up_need_neutral = false;
volatile bool down_need_neutral = false;

//to make signal not determine relay
volatile bool relay1 = false;
volatile bool relay2 = false;
volatile bool relay3 = false;
volatile bool relay4 = false;

//start time
volatile unsigned long up_start_time = 0;
volatile unsigned long up_current_time = 0;
volatile unsigned long up_elapsed_time = 0;
volatile unsigned long down_start_time = 0;
volatile unsigned long down_current_time = 0;
volatile unsigned long down_elapsed_time = 0;
volatile unsigned long neutral_start_time = 0;
volatile unsigned long neutral_current_time = 0;
volatile unsigned long neutral_elapsed_time = 0;

volatile unsigned long on_duration = 250;

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
  neutral_current_time = millis();
  neutral_elapsed_time = neutral_current_time - neutral_start_time;

      //calculate how long hoist switched on
    if( relay1==false && relay2==false ){
      up_start_time=millis();
      up_need_neutral = false;
      
    }else if( relay1==true && relay2==true ){
        up_current_time=millis();
        up_elapsed_time=up_current_time - up_start_time;
    }
  
    if( relay3==false && relay4==false ){
      down_start_time=millis();
      down_need_neutral = false;
    }else if( relay3==true && relay4==true ){
        down_current_time=millis();
        down_elapsed_time=down_current_time - down_start_time;
    }
  
  if(rc_values[RC_CH2] > 1700 && up_need_neutral==false && neutral_elapsed_time>2000){
      if(up_elapsed_time >= on_duration){
        relay1=false;
        relay2=false;
        up_need_neutral=true;
        return;
      }
      relay1=true;
      relay2=true;
      neutral_start_time = 0; 
  }else if(rc_values[RC_CH2] < 1200 && down_need_neutral==false && neutral_elapsed_time>2000){
      if(down_elapsed_time >= on_duration){
        relay3=false;
        relay4=false;
        down_need_neutral=true;
        return;
      }
      relay3=true;
      relay4=true;
      neutral_start_time=0;
  }else{
      relay1=false;
      relay2=false;
      relay3=false;
      relay4=false;
      up_elapsed_time=0;
      down_elapsed_time=0;
      if(neutral_start_time == 0){
        neutral_start_time=millis();
      }else{
         neutral_current_time = millis(); 
      }
      neutral_elapsed_time = neutral_current_time - neutral_start_time;
  }

}
void calc_ch5() { 
  calc_input(RC_CH5, RC_CH5_INPUT);
    if(rc_values[RC_CH5] > 1500){
      on_duration = 500;
    }else if(rc_values[RC_CH5] < 1500){
      on_duration = 250;
    }
  }

void setup() {
  //Serial.begin(SERIAL_PORT_SPEED);
  Serial.begin(9600);
  pinMode(RC_CH2_INPUT, INPUT);//set analog pin as input pin
  pinMode(RC_CH5_INPUT, INPUT);//set analog pin as input pin
                                          
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);//execute function when interrupted by signal from analog pin
//  enableInterrupt(RC_CH5_INPUT, calc_ch5, CHANGE);//execute function when interrupted by signal from analog pin
}

void loop() {
  rc_read_values();
  set_relay_state();
//  Serial.print("UpStart:");Serial.print(up_start_time);Serial.print("\t");
//  Serial.print("UpCurrent:");Serial.print(up_current_time);Serial.print("\t");
//  Serial.print("UpElapsed:");Serial.print(up_elapsed_time);Serial.print("\t");
//  Serial.print("DownStart:");Serial.print(down_start_time);Serial.print("\t");
//  Serial.print("DownCurrent:");Serial.print(down_current_time);Serial.print("\t");
//  Serial.print("DownElapsed:");Serial.print(down_elapsed_time);Serial.print("\t");
//  Serial.print("Up Need Neutral:");Serial.print(up_need_neutral);Serial.print("\t");
//  Serial.print("Down Need Neutral:");Serial.println(down_need_neutral);Serial.print("\t");
    Serial.print(neutral_start_time);Serial.print("\t"); 
    Serial.print(neutral_current_time);;Serial.print("\t");
    Serial.println(neutral_elapsed_time);
//  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
//  Serial.print("CH5:"); Serial.println(rc_values[RC_CH5]);
}
