//
float sensor_state;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor_state = analogRead(A0);
  Serial.println(sensor_state);
}










 
