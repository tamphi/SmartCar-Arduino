
#include <NewPing.h> 
#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 300
#define D1 30
#define D2 20
#define D3 10

// NewPing setup of pins and maximum distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
int green = 2;
int red =3;
int yellow =4;
int motorPin = 8;
 
void setup() {
   Serial.begin(9600);
   pinMode(red,OUTPUT);
   pinMode(green,OUTPUT);
   pinMode(yellow,OUTPUT);
   pinMode(motorPin,OUTPUT);
   digitalWrite(red,LOW);
   digitalWrite(green,LOW);  
   digitalWrite(motorPin,HIGH);

}
 
void loop() {
   delay(50);
   digitalWrite(motorPin,100);
   unsigned int distance = sonar.ping_cm();
   //output warning LED light based on distance of object 
   if ( distance <= 30) {
    pinMode(green,HIGH);
    digitalWrite(motorPin,175);
   }
   else{
    pinMode(green,LOW);
    digitalWrite(motorPin,HIGH);
   }
   if (distance <= 20){
    pinMode(red,HIGH);
    digitalWrite(motorPin,100);
   }
   else {
    pinMode(red,LOW);
   }
   if (distance <= 10) {
    pinMode(yellow,HIGH);
    digitalWrite(motorPin,LOW);
   }
   else {
    pinMode(yellow,LOW);
   }
   
   Serial.print(distance);
   Serial.println("cm");
}
