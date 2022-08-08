#include<Servo.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int inputPin = 5; //  PIR sensor
int pirState = LOW; 
int val = 0; 
int counter = 0;
int currentState = 0;
int previousState = 0;

Servo Myservo;
int pos=0;
int ledpin = 13; 
int touchpin = 9; 
int value ;        

void setup ()
{
  
    Serial.begin(9600);
     //lcd.begin();
    lcd.init();
    lcd.backlight();
  pinMode(inputPin, INPUT);
   lcd.begin(16, 2);
   lcd.setCursor(0, 0);
   lcd.print("INSERT COIN");
   lcd.setCursor(0, 1);
   lcd.print("PICK UP PAD");
  
   
   
   pinMode (touchpin, INPUT) ; 
  pinMode (ledpin, OUTPUT) ; 
  Myservo.attach(3);
}
void loop ()
{
 val = digitalRead(inputPin); // read input value
if (val == HIGH) { // check if the input is HIGH
if (pirState == LOW) {
// we have just turned on
currentState = 1;
 pirState = HIGH;
delay(1000);
}
} else {
//digitalWrite(ledPin, LOW); // turn LED OFF
if (pirState == HIGH){
// we have just turned of
currentState = 0;
// We only want to print on the output change, not state
pirState = LOW;
}
}
if(currentState != previousState){
if(currentState == 1){
counter = counter + 1;
 //lcd.clear();
 //lcd.setCursor(4,0);
// lcd.print("counter");
 //lcd.setCursor(4,1);
 //lcd.print(counter);
Serial.println(counter);
}
}

  value = digitalRead (touchpin) ; 
  if (value == HIGH)      
  {
    digitalWrite (ledpin, HIGH); 
    Myservo.write(180);
    delay(2000); 
  }
  else        
    digitalWrite (ledpin, LOW);  
    Myservo.write(0);
  }
