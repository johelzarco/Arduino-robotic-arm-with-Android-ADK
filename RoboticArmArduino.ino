// Arduino code to control a robotic arm of six degrees of freedom made of 7 servo motors and one gearmotor.
// Commands to control the robotic arm will be received through USB from an Android App that implements the Android Open Accessory Protocol (https://github.com/johelzarco/Android-RoboticArm)
// It has been successfully tested with an Arduino Due but it should work with Arduino Mega ADK or any arduino with an USB Host Shield (http://www.circuitsathome.com/)
// This code uses the USB Host library developed by Oleg Mazurov.
// You can view this project in more detail in http://www.bio-robotics.org
//
// Project developed at the BioRobotics Laboratory UNAM http://biorobotics.fi-p.unam.mx/

#include <Servo.h>
#include "variant.h"
#include <stdio.h>
#include <adk.h>

// Accessory descriptor. It's how Arduino identifies itself to Android.
char applicationName[] = "RoboticArm"; // the app on your phone
char accessoryName[] = "Arduino Due"; // your Arduino board
char companyName[] = "AppUniversity";
  
char versionNumber[] = "1.0";
char serialNumber[] = "1";
// The arduino will look for your app in this URL in case it's not already installed in your device
char url[] = "http://bio-robotics.org/androidArduino/RoboticArm.apk";
  
USBHost Usb;
ADK adk(&Usb, companyName, applicationName, accessoryName,versionNumber,url,serialNumber);
 
int led = 13;
Servo mainOne;                  
Servo mainTwo;

Servo anteOne;
Servo anteTwo;

Servo manoUno;
Servo manoDos;


Servo pinza;
int pos = 0;
int i = 0; 

int motorA = 6;
int motorB = 7;
int valorZ = 50;
  
void setup() {
    Serial.begin(9600);
    cpu_irq_enable();
    pinza.attach(2);
    manoUno.attach(3);
    manoDos.attach(4);
    anteOne.attach(11);
    anteTwo.attach(12);
    mainOne.attach(8); 
    mainTwo.attach(9);
    
    pinMode(led, OUTPUT);
    pinMode(motorA, OUTPUT);
    pinMode(motorB, OUTPUT);
}
  
#define RCVSIZE 128
  
void loop() {
    
    char helloworld[] = "Hello World!\r\n";
    String mano = "m";//letra para diferenciar mano
    String mun = "n"; // muneca
    String ante = "a";
    String bra = "b";
    String gX = "";
    String giZ = "z";
    int giroX = 0;
    
    uint8_t buf[RCVSIZE];
    uint32_t nbread = 0;
    // 
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    
    Usb.Task();
      
    if (adk.isReady()){
       
        adk.read(&nbread, RCVSIZE, buf);
        if (nbread > 0){
            adk.write(nbread, buf);
             
            //Convert nbread to String
            String s = "";
            for (uint32_t i = 0; i < nbread; ++i) {
              s += (char)buf[i];
            }
            // Depending on the String it receives from the Android device it will move consequently.
            // For controlling the gripper it will receive open or close and
            // for the the rest of the servos it will read one string that contains the coordinate to move and one letter to 
            // identify which servo should spin.
             
            if(s == "open"){
              digitalWrite(led, HIGH);
              PinzaOpen();
            }else if(s == "close"){
              digitalWrite(led, LOW);
              PinzaClose();
            }else if(s.startsWith(mano)){//hand
              Serial.println(s);
              gX = s;// copia de la string que viene del android
              gX.replace('m', '0');// reemplazamos el caracter diferenciador
              giroX = gX.toInt();// convertimos a entero
              Serial.println(giroX);
              delay(30);
              manoUno.write(giroX);
            }else if(s.startsWith(mun)){//wrist
              Serial.println(s);
              gX = s;
              gX.replace('n', '0');
              giroX = gX.toInt();
              Serial.println(giroX);
              delay(30);
              manoDos.write(giroX);
            }else if(s.startsWith(ante)){//forearm
              Serial.println(s);
              gX = s;
              gX.replace('a', '0');
              giroX = gX.toInt();
              Serial.println(giroX);
              delay(30);
              anteOne.write(giroX);
              anteTwo.write(giroX);
            }else if(s.startsWith(bra)){//main arm
              Serial.println(s);
              gX = s;
              gX.replace('b', '0');
              giroX = gX.toInt();
              Serial.println(giroX);
              delay(30);
              mainOne.write(giroX);
              mainTwo.write(giroX);
            }else if(s == "left"){//gearmotor that rotates the whole arm in the Z axis
              Serial.println(s);
              GiroIzq();
            }else if(s == "right"){
              GiroDer();
            }
            
            
        }
          
    }
      
}

void brazo (){
 // int i = 0;
   i = i + 1;
int posMain = 0;

  for(posMain = 120; posMain>=40; posMain-=1)     
  {                                
    mainOne.write(posMain);              
    mainTwo.write(posMain);
    delay(20);                       
  }
  delay(300);  
  for(posMain = 40; posMain <= 120; posMain += 1) 
  {                                  
    mainOne.write(posMain);    
    mainTwo.write(posMain);
    delay(20);                       
  }
  digitalWrite(led, HIGH); 
  delay(300); 
}

void antebrazo(){
int posB = 0;
for(posB = 20; posB <= 70; posB += 1) 
  {                                  
    anteOne.write(posB);
    anteTwo.write(posB);
    delay(20);                       
  }
  digitalWrite(led, HIGH); 
  delay(300);
  
  for(posB = 70; posB>=20; posB-=1)     
  {                                
    anteOne.write(posB);
    anteTwo.write(posB);
    delay(20);                       
  }
  delay(300);
}

void ManoC(){
 int p = 0;
    for(p = 0; p <= 180; p += 1)  {                                  
      manoUno.write(p);
      manoDos.write(p);
      pinza.write(p);      
      delay(15);                       
    } 
  for(p = 180; p >= 0; p -= 1){                                
      manoUno.write(p);
      manoDos.write(p);
      pinza.write(p);            
      delay(15);                       
    } 
 }

void PinzaOpen (){
int pos = 0;
    for(pos = 0; pos <= 150; pos += 1)  {                                  
    pinza.write(pos);              
    delay(10);                       
  }  
}

void PinzaClose (){
int pos = 0;
  for(pos = 150; pos>=0; pos-=1){                                
    pinza.write(pos);              
    delay(10);                       
  }  
}

void GiroIzq(){
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, HIGH);
  delay(100);
}

void GiroDer(){
  digitalWrite(motorA, HIGH);
  digitalWrite(motorB, LOW);
  delay(100);
}
