int LedPinA = 5;
int ButtonPinA = 7;
int buttonStateA = 0;
int brightness = 0;
void setup ()
{
  Serial.begin(9600);
  pinMode (LedPinA, OUTPUT);
  pinMode (ButtonPinA, INPUT);
}
void loop ()
{
  buttonStateA = digitalRead (ButtonPinA);
  Serial.print(buttonStateA);
  if(buttonStateA == 1){
    analogWrite (LedPinA, 0);
  }else if(buttonStateA == 0){
    analogWrite (LedPinA, 255);
  }
  
  delay (100);
}
