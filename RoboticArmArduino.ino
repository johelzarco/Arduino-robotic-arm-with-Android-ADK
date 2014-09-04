#include <Servo.h>
#include "variant.h"
#include <stdio.h>
#include <adk.h>

// Accessory descriptor. It's how Arduino identifies itself to Android.
char applicationName[] = "RoboticArm"; // the app on your phone
char accessoryName[] = "Arduino Due"; // your Arduino board
char companyName[] = "AppUniversity";
  
// 
char versionNumber[] = "1.0";
char serialNumber[] = "1";
char url[] = "http://bio-robotics.org/androidArduino/RoboticArm.apk";
  
USBHost Usb;
ADK adk(&Usb, companyName, applicationName, accessoryName,versionNumber,url,serialNumber);
  
// Pin 13 has an LED connected on most Arduino boards.
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
    mainOne.attach(8);  // attaches the servo on pin 9 to the servo object 
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
    //int difZ = 0;
    
    uint8_t buf[RCVSIZE];
    uint32_t nbread = 0;
    // Apagar motor
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
             
            if(s == "open"){
              digitalWrite(led, HIGH);
              PinzaOpen();
            }else if(s == "close"){
              digitalWrite(led, LOW);
              PinzaClose();
            }else if(s.startsWith(mano)){//mano
              Serial.println(s);
              gX = s;// copia de la string que viene del android
              gX.replace('m', '0');// reemplazamos el caracter diferenciador
              giroX = gX.toInt();// convertimos a entero
              Serial.println(giroX);
              delay(30);
              manoUno.write(giroX);
            }else if(s.startsWith(mun)){//muneca
              Serial.println(s);
              gX = s;
              gX.replace('n', '0');
              giroX = gX.toInt();
              Serial.println(giroX);
              delay(30);
              manoDos.write(giroX);
            }else if(s.startsWith(ante)){//antebrazo
              Serial.println(s);
              gX = s;
              gX.replace('a', '0');
              giroX = gX.toInt();
              Serial.println(giroX);
              delay(30);
              anteOne.write(giroX);
              anteTwo.write(giroX);
            }else if(s.startsWith(bra)){//brazo principal
              Serial.println(s);
              gX = s;
              gX.replace('b', '0');
              giroX = gX.toInt();
              Serial.println(giroX);
              delay(30);
              mainOne.write(giroX);
              mainTwo.write(giroX);
            }else if(s == "left"){//brazo completo con el motorreductor, la cadena empieza con "z"
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
