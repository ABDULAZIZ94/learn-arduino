#include <EnableInterrupt.h>
 
 #define outputA 6
 #define outputB 7
 #define button 5
  
 int counter = 0; 
 int aState;
 int aLastState;
 int button_pressed;

void check_btn(){
   button_pressed = digitalRead(5);
   Serial.print("Button:");
   Serial.println(button_pressed);
}
   
 void setup() { 
   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);
   pinMode (button,INPUT);
   
   Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA);   

   enableInterrupt(button, check_btn, CHANGE);
 } 
 void loop() { 
   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter += 100;
     } else {
       counter -= 100;
     }
     Serial.print("Position:");
     Serial.println(counter);
     Serial.print("\t");
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state

   button_pressed = digitalRead(5);
   Serial.print("Button:");
   Serial.println(button_pressed);
 }
