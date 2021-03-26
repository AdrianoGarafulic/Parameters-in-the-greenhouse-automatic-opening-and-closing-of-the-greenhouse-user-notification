#define BLYNK_PRINT Serial
  #include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
// #include <SimpleTimer.h>
 
char auth[] = "Dc7JizofxNe9-PqsrEXqRPDfQS85tozB ";
 
// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid     = "GAASI_WIFI-EXT-2";
const char* pass = "armando1";
 
BlynkTimer  timer;
 
String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors
 
float prva,druga,treca;
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
  
}
 
 
 
void setup()
{
  // Debug console
  Serial.begin(9600);
 
  Blynk.begin(auth, ssid, pass);
 
     timer.setInterval(1000L,sensorvalue1); 
     timer.setInterval(1000L,sensorvalue2);
     timer.setInterval(1000L,sensorvalue3); 

 
}
 
void loop()
{

  
   if (Serial.available() == 0 ) 
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
   
  if (Serial.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
   // Serial.print(rdata);
    if( rdata == '\n')
    {
   //  Serial.println(myString); 
  // Serial.println("fahad");
// new code


String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
String n = getValue(myString, ',', 2); 

 
 
 prva = l.toFloat();
druga = m.toInt();
treca = n.toInt();

 
  myString = "";
// end new code
    }
  
/*else
 {
 String l ="0" ;
String m = "0";
String n = "0"; 
String k = "0";
String j = "0"; 
String o = "0"; 
String p = "0";  
 prva = 0;
druga = 0;
treca = 0;
cetvrta=0;
peta=0;
sesta=0;
sedma=0;
 }*/ 
  }
}
 
void sensorvalue1()
{
float sdata = prva;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);
   //Serial.print("CH4   ");
   //Serial.println(sdata);
 
}
void sensorvalue2()
{
int sdata = druga;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, sdata);
  //Serial.print("CO   ");
    // Serial.println(sdata);
 
}

void sensorvalue3()
{
int sdata = treca ;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, sdata);
  //Serial.print("LPG   ");
    // Serial.println(sdata);
 
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
