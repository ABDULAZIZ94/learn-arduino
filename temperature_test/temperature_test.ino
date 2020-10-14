float temp;
int tempPin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  temp = analogRead(tempPin);
  // read analog volt from sensor and save to variable
  temp = temp ;
  //convert the analog volt to its temperature equavalent
  Serial.print(temp);//display temperature value
  Serial.println();
  delay(1000);
  
}
