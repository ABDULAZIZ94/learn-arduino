/*
  ReadAnalogVoltage

  Reads an analog input on pin 0, converts it to voltage, and prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input on analog pin 0:
  int valueA0 = analogRead(A0);
  int valueA1 = analogRead(A1);
  int valueA2 = analogRead(A2);
  int valueA3 = analogRead(A3);
  int valueA4 = analogRead(A4);
  int valueA5 = analogRead(A5);
  
  // print out the value you read:
  Serial.println(valueA0);
  Serial.println(valueA1);
  Serial.println(valueA2);
  Serial.println(valueA3);
  Serial.println(valueA4);
  Serial.println(valueA5);
  
}
