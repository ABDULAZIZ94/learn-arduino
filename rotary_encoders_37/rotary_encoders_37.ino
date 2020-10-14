int aPin = 6;
int bPin = 7;
int buttonPin = 5;

int a_val;
int b_val;
int butoon_pressed;

void setup ()
{
  Serial.begin(9600);
  pinMode (aPin, INPUT);
  pinMode (bPin, INPUT);
  pinMode (buttonPin, INPUT);
}
void loop ()
{
  a_val = digitalRead(6);
  Serial.print("A:");Serial.print(a_val);Serial.print("\t");
  
  b_val = digitalRead(7);
  Serial.print("B:");Serial.print(b_val);Serial.print("\t");
  
  butoon_pressed = digitalRead(5);
  Serial.print("Button:");Serial.println(butoon_pressed);
}
