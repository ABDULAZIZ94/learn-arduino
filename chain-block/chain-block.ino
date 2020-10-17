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
volatile bool need_neutral;

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
  if(rc_values[RC_CH2] > 1600 ){
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    delay(10);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    need_neutral = true;
  }else if(rc_values[RC_CH2] < 1400){
    need_neutral = true;
  }else{
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    need_neutral = false;
    delay(10);
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

  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  Serial.print("CH5:"); Serial.println(rc_values[RC_CH5]);

  delay(200);
}
