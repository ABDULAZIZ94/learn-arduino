int in1 = 8;
int in2 = 9;
int in3 = 10;
int in4 = 2;

//the following are all ~PWM capable ports 
int enable1 = 3;
int enable2 = 11;
int rc_channel4 = 6;
int rc_channel5 = 5;
//int switchD = 5;

void setup() {
  pinMode(rc_channel4, INPUT);
  pinMode(rc_channel5, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  int pwm = 0;
  int rc4 = pulseIn(rc_channel4, HIGH, 25000);
  int rc5 = pulseIn(rc_channel5, HIGH, 25000);
  
  

  
  if(rc4==0){
      Serial.println(" no signal");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      analogWrite(enable1, 0);
  }
  else if(rc4 > 1630){ //right stick
      pwm = map(rc4, 1530, 1800, 0, 100); //map our speed to 0-255 range
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(enable1, pwm);
      Serial.print(" right stick speed: ");
      Serial.println(pwm);
  }
  else if(rc4 < 1360){
      pwm = map(rc4, 1360, 1000, 0, 100); //map our speed to 0-255 range
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(enable1, pwm);
      Serial.print(" left stick speed: ");
      Serial.println(pwm);
  }else {
      Serial.println(" stick centered");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      analogWrite(enable1, 0);
  }

   if(rc5==0){
      Serial.println(" no signal");
      digitalWrite(in3, LOW);
      digitalWrite(in3, LOW);
      analogWrite(enable2, 0);
  }
  else if(rc5 > 1630){ //right stick
      pwm = map(rc5, 1530, 1800, 0, 100); //map our speed to 0-255 range
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enable2, pwm);
      Serial.print(" right stick speed: ");
      Serial.println(pwm);
  }
  else if(rc5 < 1360){
      pwm = map(rc5, 1360, 1000, 0, 100); //map our speed to 0-255 range
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enable2, pwm);
      Serial.print(" left stick speed: ");
      Serial.println(pwm);
  }else{
      Serial.println(" stick centered");
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      analogWrite(enable2, 0);
  }
  
  delay(10);
}
