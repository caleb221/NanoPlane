#include <SPI.h>
#include <LoRa.h>
#include<Servo.h>
#include<ArduinoJson.h>

const int capacity = JSON_OBJECT_SIZE(3);//station has 14 objects, its the max amount
StaticJsonDocument<capacity> jb;

Servo servoR;
Servo servoL;
Servo engine;

//===================
//      PINS
int servoRight=3;
int servoLeft= 5;
int escControl=6;
int rLight=8;
int gLight= 7;

int interv =50;
int x=90;
int y=90;
int t;
char* input="0";
unsigned long lastTime=0;


void setup() {
  Serial.begin(9600);
  pinMode(gLight,OUTPUT);
  pinMode(rLight,OUTPUT);
  digitalWrite(rLight,HIGH);
  servoR.attach(servoRight);
  servoL.attach(servoLeft);
  
  engine.attach(escControl,1000,2000);
  
  //while (!Serial);

  Serial.println("LoRa Plane Reciever!");
  delay(100);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  LoRa.receive();
  steer(4,0);
  digitalWrite(rLight,LOW);
  digitalWrite(gLight,HIGH);
}

void loop() {
  
  
  int packetSize = LoRa.parsePacket();
 // Serial.println(packetSize);
  if (packetSize) {
   gotPacket(packetSize);
  }
  //Serial.print("v");

  

 unsigned long currentTime = millis(); 
  if((currentTime - lastTime) > interv) 
  {
   //  Serial.println(x);
     lastTime = currentTime;
     int x_offset= (x-90)*2;
     int y_offset =(y-90)*2;
   
     if(x_offset <=-10 && y>=80)
     {
      steer(0,abs(x_offset));
     }
     else if(x_offset>30 && y>= 80)
     {
      //why is this default      
      steer(1,x_offset);
     }
     else if(y<80 && x_offset >10)
     {
      steer(2,abs(y_offset)); 
     }
     else if(y>105)
     {
      steer(3,y_offset); 
     }
     else
     {
      steer(4,0);
     }
     engineThrottle(t);
  }


}

void engineThrottle(int throttle)
{
engine.write(throttle);
  //analogWrite(6,throttle/4);  
  
}

void steer(int upDown,int degree)
{ 
  int currentPos=90;
  int lPos=50;
  int rPos=130;
  /*
  Serial.println("  deg:  ");
  Serial.print(degree);
  Serial.print("   #:  ");
 Serial.print(upDown);
  */
  
  if(upDown ==0)
  {
    //left up, right down
    servoR.write(currentPos-degree);
    
  delay(10);
    servoL.write(currentPos-degree);
    
  delay(10);
    
  }
  else if(upDown ==1)
  {
    //left down, right up
    servoR.write(currentPos+degree);
    
  delay(10);
    servoL.write(currentPos+degree);
  
  delay(10);
  }
  else if(upDown ==2)
  {
    // both down
    
    servoR.write(currentPos+degree);
    
  delay(10);
    servoL.write(currentPos-degree);
  
  
  delay(10);
  }
  else if (upDown==3)
  {
    //both down
   
    servoR.write(currentPos-degree);
    
  delay(10);
    servoL.write(currentPos+degree);
 
  delay(10);
  }
  else if (upDown ==4)
  {
    //return to 90 degrees
     servoR.write(currentPos);
     
  delay(10);
     servoL.write(currentPos);
     delay(150);
     return;   
  }
  else
  {
    return;
  }
  delay(10);
  
}


void gotPacket(int packetSize)
{
   char inputBuff[packetSize];
    int i=0;
    
    // read packet
    while (LoRa.available()) {
      inputBuff[i]=(char)LoRa.read();
      i++;
    } 
    //Serial.println(inputBuff);
  
  DeserializationError err = deserializeJson(jb,inputBuff);
  if(err)
   {  
    Serial.println(inputBuff); 
    Serial.println("\n\nJSON error");
    jb.clear();
    inputBuff[0]='\0';
    LoRa.receive();     
    return;
   }
    
     x=jb["x"];
     y=jb["y"];
     t=jb["t"];
   // Serial.println(LoRa.packetRssi());

    inputBuff[0]='\0';
    jb.clear();
}
