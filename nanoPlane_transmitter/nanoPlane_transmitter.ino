
#include <PCD8544.h>
#include<ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
StaticJsonDocument<200> root;
PCD8544 lcd;
int x=0;//left/right
int y=0;//up/down
int t=0;//throttle

int interv = 50;
unsigned long lastTime=0;

char* input="0";
int ss=10;
int loraReset=9;
int dio0=8;

void setup() {
  //Serialbegin(9600);
  lcd.begin(84, 48);
  //Serialprintln("LCD on?");
  LoRa.setPins(ss, loraReset, dio0);
  //Serialprintln("LoRa 433Mhz Transmitter");

  if (!LoRa.begin(433E6)) {
    //Serialprintln("Starting LoRa failed!");
    lcd.setCursor(0,1);
    lcd.print("LoRa init Failed");
    
    while (1);
  }
 
}

void loop() {
 // lcd.clear();
  
  x=analogRead(A0);
  y=analogRead(A1);
  t=analogRead(A2);
  //Serialprintln(x);
//convert to degrees
  x=map(x,1024,0,0,180);
  y=map(y,1024,0,0,180);
  t=map(t,820,0,100,0);
  //add throttle
  //pitch/yaw?

  
 
    lcd.setCursor(0,1);
    lcd.print("x:");
    lcd.print(x);

    
    
    lcd.print(" y:");
    lcd.print(y);
    lcd.setCursor(0,2);
    lcd.print("throttle: ");
    lcd.print(t);
 /* 
  int packetSize = LoRa.parsePacket();
  //Serialprint(packetSize);
  if (packetSize) {
   gotPacket(packetSize);
  }
*/
 unsigned long currentTime = millis(); 
  if(currentTime - lastTime > interv) 
  {
     lastTime = currentTime; 
     makeSendJson(x,y,t);
     lcd.clear();
  }

}

void makeSendJson(int x_in,int y_in,int t_in)
{  
  root["x"] = x_in;
  root["y"] = y_in;
  root["t"] = t_in;
  
  // send packet
  String jsonP;
  serializeJson(root,jsonP);
  root.clear();
  
    LoRa.beginPacket();
    LoRa.print(jsonP);
    
    LoRa.endPacket(); 
    delay(150);//wait for SPI to do its thing, maybe
    
  
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
     inputBuff[0]='\0';
    //Serialprintln(inputBuff);
    //lcd.setCursor(0,4);
    //lcd.print(LoRa.packetRssi());
    //lcd.print("  Db");  
}
  
