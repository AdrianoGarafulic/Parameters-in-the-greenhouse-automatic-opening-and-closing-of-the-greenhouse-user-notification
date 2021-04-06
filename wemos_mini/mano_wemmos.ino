#define BLYNK_PRINT Serial
//#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include<ESP8266WiFi.h>
#include<ESP8266mDNS.h>
#include<WiFiUdp.h>
#include<ArduinoOTA.h>
#include<BlynkSimpleEsp8266.h>
#include<DHT.h>
#include<math.h>  
#define ONE_WIRE_BUS 2
#define relej_gosi_svitlo 15

// You shouldgetAuth Token intheBlynk App.
// Go to the Project Settings (nuticon).
char auth[] ="E_eiaN2Ja6PcnHRvwgX6JbRlScF4qV8q";

// Your WiFi credentials.
// Set password to "" for opennetworks.
char ssid[] ="GAASI_WIFI-EXT-2";
char pass[] ="armando1";
bool stanje=HIGH,kalibracija=HIGH,kalibracija1=HIGH;
bool a=HIGH,a1=HIGH,a2=HIGH,n1=0,n2=0,n3=0,n4=0, notifikacije=HIGH, da_ili_ne_gosi_svitlo=HIGH,n5t1=0, n1t1 = 0, n2t1 = 0, n3t1=0, n4t1=0;
const int Grijanje1 = 4;
const int Grijanje2 = 5;


int temp1=22,temp2=21,stupanj_grijanja,voc,co2,temp_visoka_t1, temp_previsoka_t1, temp_niska_t1, temp_preniska_t1;
float temp_offset=0.5,h,t;
int b=0,brojac_baseline  = 0,c=0,temp_gasi_svitlo=45;//brojaci
float temp_senzor1,temp_senzor2;

OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0x2F, 0xE7, 0x75, 0xD0, 0x1, 0x3C, 0xEE };
DeviceAddress sensor2 = { 0x28, 0xC2, 0x7E, 0x75, 0xD0, 0x1, 0x3C, 0xC9 };



BLYNK_CONNECTED() {
  // Requestthelateststatefromthe server
   Blynk.syncVirtual(V3);
   Blynk.syncVirtual(V1);
   Blynk.syncVirtual(V2);
   Blynk.syncVirtual(V4);
   Blynk.syncVirtual(V9);
   Blynk.syncVirtual(V8);
   Blynk.syncVirtual(V11); 
   Blynk.syncVirtual(V14); 
   Blynk.syncVirtual(V15); 
   Blynk.syncVirtual(V16); 
   Blynk.syncVirtual(V20); 
   Blynk.syncVirtual(V21); 
   Blynk.syncVirtual(V26); 
   Blynk.syncVirtual(V33); 
   Blynk.syncVirtual(V34); 
   Blynk.syncVirtual(V35); 
   Blynk.syncVirtual(V36); 
   Blynk.syncVirtual(V37); 
   Blynk.syncVirtual(V38); 
   Blynk.syncVirtual(V39);      

  // Alternatively, youcouldoverride server stateusing:
  //Blynk.virtualWrite(V2, ledState);
}
BLYNK_WRITE(V3) {//botun auto/manual grijanje


  stanje=param.asInt();
  }
 
BLYNK_WRITE(V2) {//reset botun
  if((param.asInt()==1))
  {
    ESP.restart();
  }

 }

BLYNK_WRITE(V1)
 {
if((param.asInt()==1)&&(stanje==LOW))
  {
     digitalWrite(Grijanje1,HIGH);
     a=HIGH;
  }
  if ((param.asInt()==0)&&(stanje==LOW))
  {
     digitalWrite(Grijanje1,LOW);
     a=LOW;
  }
 }
BLYNK_WRITE(V8)
 {
if((param.asInt()==1)&&(stanje==LOW))
  {
     digitalWrite(Grijanje2,HIGH);
     a1=HIGH;
  }
  if ((param.asInt()==0)&&(stanje==LOW))
  {
     digitalWrite(Grijanje2,LOW);
     a1=LOW;
  }

  
  
 }
  BLYNK_WRITE(V4)
 {
  temp1=param.asInt();
 }
  BLYNK_WRITE(V9)
 {

  temp2=param.asInt();
 }
  BLYNK_WRITE(V11)
 {
  temp_offset=param.asFloat();
 }
  
 BLYNK_WRITE(V33)
{
  temp_gasi_svitlo = param.asInt();
}
BLYNK_WRITE(V34)
{
  da_ili_ne_gosi_svitlo = param.asInt();
}
BLYNK_WRITE(V35)
{
  temp_visoka_t1=param.asInt();
}

BLYNK_WRITE(V36)
{
  temp_previsoka_t1=param.asInt();
}
BLYNK_WRITE(V37)
{
  temp_niska_t1=param.asInt();
}

BLYNK_WRITE(V38)
{
  temp_preniska_t1=param.asInt();
}
 BLYNK_WRITE(V39)
{
 notifikacije=param.asInt();
}

BlynkTimer timer;

// ThisfunctionsendsArduino'sup time everysecond to Virtual Pin (5).
// In theapp, Widget'sreadingfrequencyshouldbe set to PUSH. Thismeans
// thatyoudefine how often to send data to Blynk App.
void sendSensor()
{


Blynk.virtualWrite(V31,temp_senzor1);
Blynk.virtualWrite(V32,temp_senzor2);

 
  // You cansendanyvalue at any time.
  // Pleasedon'tsend more that 10 values per second.

  Blynk.virtualWrite(V1, a);
  Blynk.virtualWrite(V8, a1);
  Blynk.virtualWrite(V7,stupanj_grijanja);
  Blynk.virtualWrite(V10,WiFi.RSSI()); 
  Blynk.virtualWrite(V19,temp_offset);
  int callFunction2 =timer.setTimeout(1000, sendSensor2);
}

void sendSensor2() 
{

 
  Blynk.virtualWrite(V1, a);
  Blynk.virtualWrite(V8, a1);
  Blynk.virtualWrite(V7,stupanj_grijanja);
  int callFunction3 =timer.setTimeout(1000, sendSensor3);
   // your sensor2 codehere  
   // THREE
}
void sendSensor3() 
{
  Blynk.virtualWrite(V1, a);
  Blynk.virtualWrite(V8, a1);
  Blynk.virtualWrite(V7,stupanj_grijanja);
  // your sensor2 codehere  
   // THREE
}




void setup()
{
  // Debugconsole
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);   
  pinMode(Grijanje1,OUTPUT);
  pinMode(Grijanje2,OUTPUT);
  pinMode(relej_gosi_svitlo,OUTPUT);
  digitalWrite(Grijanje1,HIGH);
  digitalWrite(Grijanje2,HIGH);
  digitalWrite(relej_gosi_svitlo,HIGH);
  


  // You canalsospecify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
   Blynk.config(auth);
   Blynk.connect();

  // Setup a function to becalledeverysecond
  timer.setInterval(3000L,sendSensor);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress,unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress/ (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ",error);
    if (error== OTA_AUTH_ERROR) Serial.println("AuthFailed");
    else if (error== OTA_BEGIN_ERROR) Serial.println("BeginFailed");
    else if (error== OTA_CONNECT_ERROR) Serial.println("ConnectFailed");
    else if (error== OTA_RECEIVE_ERROR) Serial.println("ReceiveFailed");
    else if (error== OTA_END_ERROR) Serial.println("EndFailed");
  });
  ArduinoOTA.setHostname("Wemos temperatura");
  ArduinoOTA.begin();

}

void loop()
{
ArduinoOTA.handle();

sensors.requestTemperatures();
temp_senzor1=sensors.getTempC(sensor1);
temp_senzor2=sensors.getTempC(sensor2);

     if((digitalRead(Grijanje1)==HIGH)&&(digitalRead(Grijanje2)==HIGH))
      stupanj_grijanja=0;
     if((digitalRead(Grijanje1)==HIGH)&&(digitalRead(Grijanje2)==LOW))
      stupanj_grijanja=1;
     if((digitalRead(Grijanje1)==LOW)&&(digitalRead(Grijanje2)==HIGH))
      stupanj_grijanja=2;
     if((digitalRead(Grijanje1)==LOW)&&(digitalRead(Grijanje2)==LOW))
       stupanj_grijanja=3;

  
    if (temp1>temp_senzor1 )
       {
        if(stanje==HIGH)
          {//AKO JE NA AUTOMATIKU
           digitalWrite(Grijanje1,LOW);
          }
        }
     else
       {
        if((stanje==HIGH)&&(temp1+temp_offset)<temp_senzor1)
         {
        digitalWrite(Grijanje1,HIGH);
         }
       }



      if (temp2>temp_senzor1)
      {
        if(stanje==HIGH)
        {//AKO JE NA AUTOMATIKU
          digitalWrite(Grijanje2,LOW);
        }
      }
      else
      {      
        if((stanje==HIGH)&&(temp2+temp_offset)<temp_senzor1)
        {
        digitalWrite(Grijanje2,HIGH);
        }
      }

      if (stanje==LOW)
      { 
        digitalWrite(Grijanje1,a);
        digitalWrite(Grijanje2,a1);       
      }
     if(notifikacije==HIGH)
      {
     if ((temp_senzor2<temp_visoka_t1))
         n1t1=0;
      if ((temp_senzor2<temp_previsoka_t1))
         n2t1=0;
      if ((temp_senzor2>temp_niska_t1))
         n3t1=0;
      if ((temp_senzor2>temp_preniska_t1))
         n4t1=0;         
      if ((temp_senzor2>temp_visoka_t1)&&(n1t1==0))
         {
          n1t1=1;
          Blynk.notify("Room temperatura visoka!!");
         } 
      
      if ((temp_senzor2>temp_previsoka_t1)&&(n2t1==0))
         {
          n2t1=1;
          Blynk.notify("Room temperatura previsoka !!!");
         }
      if ((temp_senzor2<temp_niska_t1)&&(n3t1==0))
         {
          n3t1=1;
          Blynk.notify("Room temperatura niska");
         } 
      
      if ((temp_senzor2<temp_preniska_t1)&&(n4t1==0))
         {
          n4t1=1;
          Blynk.notify("Room temperatura preniska !!!");
         }   
    }  
      
      
  if (da_ili_ne_gosi_svitlo == HIGH)
  {
    if ((temp_senzor2 < temp_gasi_svitlo - 5))
    {
      n5t1 = 0;
      digitalWrite(relej_gosi_svitlo, LOW);

    }
    if ((temp_senzor2 > temp_gasi_svitlo) && (n5t1 == 0))
    {
      n5t1 = 1;
      digitalWrite(relej_gosi_svitlo, HIGH);
      Blynk.notify("Ugašeno svitlo 2  !!!!!");
   }}

  
      Blynk.run();
      timer.run();
}
