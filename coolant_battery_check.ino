
#include <NewPing.h> 
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <dht.h>
dht DHT;
#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 300
#define D1 30
#define D2 20
#define D3 10


// NewPing setup of pins and maximum distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
const int rs = 39, en = 41, d4 = 25, d5 = 33, d6 = 29, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//pin lay out
int water = A0;
int thermo = A12;
int red = 2;
int green = 3;
int motorPin = 5;
int receiver = 10; // Signal Pin of IR receiver to Arduino Digital Pin 11
int buzzer = 49;
//end pin
/* GLobal VAR */
int motorspeed =0;
int passcode =1234; // access code for security check
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

void setup() {
   Serial.begin(9600); 
   lcd.begin(16, 2);
   pinMode(red,OUTPUT);
   pinMode(green,OUTPUT);
   pinMode(motorPin, OUTPUT);
   //digitalWrite(motorPin,100);
   irrecv.enableIRIn(); // Start the receiver
   pinMode(buzzer,OUTPUT);
//   digitalWrite(motorPin,HIGH);
//   digitalWrite(red,HIGH);
//   digitalWrite(green,HIGH);
}
void unauthorized(){
  digitalWrite(buzzer,HIGH);
  delay(3000);//wait for 3s
  digitalWrite(buzzer,LOW);
  delay(1000);//wait for 1ms
}

void authorized(){
  digitalWrite(buzzer,HIGH);
  delay(1000);//wait for 3s
  digitalWrite(buzzer,LOW);
  delay(500);//wait for 1ms
}
void loop() {
  /* Initial State */
  
  //Security Check
  if (Serial.available()){
    int pass = Serial.parseInt();
    if (pass != passcode) unauthorized();
    else authorized();
  }
  int level = analogRead(water); // water level
  int distance = sonar.ping_cm(); //distance
  int chk = DHT.read11(thermo);
  int temp = DHT.temperature; //temp in C
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
  Serial.print(distance);
  Serial.print(",");
  Serial.print(level);
  Serial.print(",");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(motorspeed);
  Serial.println();
  /*printng to LCD screen)*/
  String str_T = String (temp);
  String str_W;
  lcd.setCursor(0, 0);
  lcd.print("Temp:"); lcd.print(str_T); lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Water:");
  if (level >= 400) {
    str_W = "HIGH";
    lcd.print(str_W);
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(motorPin,LOW);
  }
  if (level <=100) {
    str_W = "LOW";
    lcd.print(str_W);
//    digitalWrite(red,LOW);
//    digitalWrite(green,LOW);
//    digitalWrite(motorPin,LOW);
  }
  delay(1000);
}

/*-----( Function )-----*/
void translateIR() // takes action based on IR code received
{
  switch(results.value)
  {
  case 0xFFA25D: //OFF - turn off system
    analogWrite(motorPin, 0); 
    digitalWrite(red,0);  digitalWrite(green,0);
    break;
  case 0xFFE01F: //down
    if (motorspeed >= 0){
      if ( (motorspeed - 10 <= 0)) motorspeed =0;
      else motorspeed -= 10;
    }
    analogWrite(motorPin, motorspeed);
//    Serial.println(motorspeed);
    break;
  case 0xFF906F: //up
    if (motorspeed <= 255) {
      if ( (motorspeed + 10) >= 255) motorspeed=255;
      else motorspeed += 10;
    }
    analogWrite(motorPin, motorspeed);
//    Serial.println(motorspeed);
    break;
  case 0xFF6897: digitalWrite(red,0);  digitalWrite(green,0); break; //0
  case 0xFF30CF: digitalWrite(red,10);  digitalWrite(green,10); break; //1
  case 0xFF18E7: digitalWrite(red,255);  digitalWrite(green,255);    break; //2  

  default: ;
  }// End Case
  delay(200); // Do not get immediate repeat

} //END translateIR
