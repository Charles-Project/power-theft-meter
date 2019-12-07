#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
#include <Sim800l.h>
#include <SoftwareSerial.h> //is necesary for the library!! 
Sim800l Sim800l;  //to declare the library

char* text;
char* text1;
char* number;
char* number0;
char* number1;
bool error;

LiquidCrystal lcd(3, 8, 4, 5, 6, 7);
const int relay=12;
const int led=13;
const int sensorPin = A0;

float val=0; 
int vala=0;
long P=0;
float Amperage = 0;
float Amperage1 = 0;
long P1=0;
// new variables
int hw=0;
int hw1=0;


#define acs712 A2
#define acs712_1 A4

float var_current;
float var_current1;
int var_volt;

long lastSample = 0;
long sampleSum = 0;
long sampleCount = 0;
float vpc = 4.8828125;// this is in millivolt

long lastSample1 = 0;
long sampleSum1 = 0;
long sampleCount1 = 0;
float vpc1 = 4.8828125;// this is in millivolt


void setup() {
  // put your setup code here, to run once:
lcd.begin(16,2);
pinMode(led, OUTPUT);
pinMode(relay, OUTPUT);
digitalWrite(relay,LOW);

digitalWrite(led,HIGH);
delay(2000);
digitalWrite(led,LOW);

lcd.clear();
lcd.print("A POWER THEFT");
lcd.setCursor(0,1);
lcd.print("ALERT SYSTEM");
delay(2000);

lcd.clear();
lcd.print("A Final Year");
lcd.setCursor(0,1);
lcd.print("Project");
delay(2000);

 Serial.begin(9600); // only for debug the results . 
 Serial.println("DEBUGGING ALERT");

 Sim800l.begin(); // initializate the library.
  
 number="+2348038516291";//change to a valid number.
 number0="+2347039799624";
 text="Testing The module";
 text1="Power Theft !!! Alert, Alert, Alert !!!";

  error=Sim800l.sendSms(number,text);
  delay(3000);
    error=Sim800l.sendSms(number0,text);
  delay(2000);
     /*
     error=Sim800l.sendSms(number0,text);
     delay(3000);
      
     error=Sim800l.sendSms(number1,text);
     delay(3000);
      */
     
lcd.clear();
lcd.print("All SMS Sent");
delay(2000);

lcd.clear();
lcd.print("Checking ...");
lcd.setCursor(0,1);
lcd.print("For Power Theft");
delay(2000);
lcd.clear();
lcd.print("Power Supplied");
lcd.setCursor(0,1);
lcd.print("To House");
delay(2000);
digitalWrite(relay,HIGH);
delay(3000);
lcd.clear();
lcd.print("V=   ,I=   ,  ");
lcd.setCursor(0,1);
lcd.print("PW =    P=");
}

void loop() {
  // put your main code here, to run repeatedly:
//int f=0;

//while (f==0){
var_volt = voltageS();
var_current = currentS();
P = var_volt * var_current;

var_current1 = currentS1();
P1 = var_volt * var_current1;

int we=P;
int wf=P1;



// value reading
lcd.setCursor(2,0);
lcd.print(var_volt);
Serial.print("Voltage: ");
Serial.println(var_volt);
lcd.setCursor(8,0);
lcd.print(var_current, 1);
Serial.print("Current: ");
Serial.println(var_current);
//--curent 1
lcd.setCursor(12,0);
lcd.print(var_current1, 1);
Serial.print("Current: ");
Serial.println(var_current1);
//--


/*lcd.setCursor(4,1);
lcd.print(P);

Serial.print("Power : ");
Serial.println(P);
//--power1
lcd.setCursor(10,1);
lcd.print(P1);*/

if(we>=100)
  {
  
  lcd.setCursor(4,1);
  lcd.print(P);
 Serial.print("Power : ");
Serial.println(P);
  }

 else if((we<100) && (we >=10)){
  lcd.setCursor(6,1);
  lcd.print(" ");
  lcd.setCursor(4,1);
  lcd.print(P);
  Serial.println(P);
  }

  else if (we<10){
  lcd.setCursor(4,1);
  lcd.print(P);
   Serial.print("Power : ");
Serial.println(P);}
  

  else{
    
  }
  /////////////////

  if(wf>=100)
  {
  
  lcd.setCursor(10,1);
  lcd.print(P1);
 Serial.print("Power : ");
Serial.println(P1);
  }

 else if((wf<100) && (wf >=10)){
  lcd.setCursor(11,1);
  lcd.print(" ");
  lcd.setCursor(10,1);
  lcd.print(P1);
  Serial.println(P1);
  }

  else if (wf<10){
  lcd.setCursor(10,1);
  lcd.print(P1);
   Serial.print("Power : ");
Serial.println(P1);
}
  

  else{
    
  }
  



/////

if (P1>20){

  
  lcd.clear();
  lcd.print("Power Theft");
  lcd.setCursor(0,1);
  lcd.print("Detected ...");
  delay(2000);

  digitalWrite(relay,LOW);
  delay(6000);

  error=Sim800l.sendSms(number,text1);
  delay(3000);
      
   error=Sim800l.sendSms(number0,text1);
   delay(3000);
      
    


lcd.clear();
lcd.print("V=   ,I=   ,  ");
lcd.setCursor(0,1);
lcd.print("PW =    P=");
delay(2000);

digitalWrite(relay,HIGH);
delay(3000);
}
///..................................
delay(1000);
}






/////////////// voltage function/////////////////////
int voltageS(){
 //for(int i = 0; i < 10; i++){
 val = analogRead(sensorPin);
 val = (val*260.0)/1023.0;
 vala = val;

//val = vala/10; 
//vala = val;
return vala;
}
///////////////// current function1 ////////////////////

float currentS(){
  int x=0;
  while (x==0){

    if (millis() > lastSample + 1)
{
  sampleSum += sq(analogRead(acs712)-512);
  sampleCount++;
  lastSample = millis();
  //Serial.println("Good");
}

if (sampleCount == 1000){
  // Averaging Stuff
  float mean = sampleSum / sampleCount;
  Serial.println(mean);
  float value = sqrt(mean);
  Serial.println(value);
  float mv = value * vpc;
  Serial.println(mv);
  Amperage = mv/66;
  Serial.println(Amperage);
  
  
  Serial.println("The final RMS values is : " + String(value));
  Serial.println("The final RMS Voltage is : " + String(value*vpc));
  Serial.println("The final RMS Amperage is : " + String(Amperage));
  Serial.println("The final Poer is : " + String(Amperage*165));
  Serial.println("The final Poer is : " + String(Amperage*220));
  sampleSum = 0;
  sampleCount = 0;
  x=1;
  
} 
  }
  return Amperage;
}

////////////////////////////////////''''''--------------
//-----------------end of current function1-------------

//////////////// current function1 ////////////////////

float currentS1(){
  int r=0;
  while (r==0){//x same as r

    if (millis() > lastSample1 + 1)
{
  sampleSum1 += sq(analogRead(acs712_1)-512);
  sampleCount1++;
  lastSample1 = millis();
  //Serial.println("Good");
}

if (sampleCount1 == 1000){
  // Averaging Stuffs
  float mean1 = sampleSum1 / sampleCount1;
  Serial.println(mean1);
  float value1 = sqrt(mean1);
  Serial.println(value1);
  float mv1 = value1*vpc1;
  Serial.println(mv1);
  Amperage1 = mv1/66;
  Serial.println(Amperage1);
  
  Serial.println("The final RMS values is : " + String(value1));
  Serial.println("The final RMS Voltage is : " + String(value1*vpc1));
  Serial.println("The final RMS Amperage is : " + String(Amperage1));
  Serial.println("The final Poer is : " + String(Amperage1*165));
  Serial.println("The final Poer is : " + String(Amperage1*220));
  sampleSum1 = 0;
  sampleCount1 = 0;
  r=1;
  }
  }
  return Amperage1;
}





