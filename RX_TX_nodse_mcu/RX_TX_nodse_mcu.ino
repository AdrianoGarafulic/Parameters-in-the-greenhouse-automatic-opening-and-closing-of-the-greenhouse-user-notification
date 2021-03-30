#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#define ONE_WIRE_BUS 2
#define relej_svitlo 12
#define RESET_PIN 14
#define RESET_PIN_SLAVE 13

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// #include <SimpleTimer.h>

char auth[] = "MfyPfotapG4rRVeeIMrlQD9Ox-PXX0oJ";

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid     = "GAASI_WIFI-EXT-2";
const char* pass = "armando1";
int temp_visoka_t1, temp_previsoka_t1, temp_niska_t1, temp_preniska_t1, temp_gasi_svitlo = 45, vlaga_visoka_1, vlaga_previsoka_1, vlaga_niska_1, vlaga_preniska_1, vlaga_visoka_2, vlaga_previsoka_2, vlaga_niska_2, vlaga_preniska_2;
bool notifikacije_t1, n1t1 = 0, n2t1 = 0, n3t1, n4t1, n5t1, notifikacije_t2, n1t2 = 0, n2t2 = 0, n3t2, n4t2, notifikacije_t5, n1t5 = 0, n2t5 = 0, n3t5, n4t5, da_ili_ne_gosi_svitlo;

BlynkTimer  timer;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors

int prva, druga, treca;
float temp;

BLYNK_CONNECTED()
{
  // Request the latest state from the server
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V6);
  Blynk.syncVirtual(V7);
  Blynk.syncVirtual(V8);
  Blynk.syncVirtual(V9);
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V15);
  Blynk.syncVirtual(V16);
  Blynk.syncVirtual(V17);
  Blynk.syncVirtual(V18);
  Blynk.syncVirtual(V19);
  Blynk.syncVirtual(V20);
  Blynk.syncVirtual(V21);
  Blynk.syncVirtual(V22);
  Blynk.syncVirtual(V23);
  Blynk.syncVirtual(V24);
  Blynk.syncVirtual(V25);
  Blynk.syncVirtual(V26);

}

BLYNK_WRITE(V6)
{
  temp_visoka_t1 = param.asInt();
}

BLYNK_WRITE(V7)
{
  temp_previsoka_t1 = param.asInt();
}
BLYNK_WRITE(V8)
{
  temp_niska_t1 = param.asInt();
}

BLYNK_WRITE(V9)
{
  temp_preniska_t1 = param.asInt();
}
BLYNK_WRITE(V10)
{
  notifikacije_t1 = param.asInt();
}
BLYNK_WRITE(V15)
{
  vlaga_visoka_1 = param.asInt();
}


BLYNK_WRITE(V16)
{
  vlaga_previsoka_1 = param.asInt();
}
BLYNK_WRITE(V17)
{
  vlaga_niska_1 = param.asInt();
}

BLYNK_WRITE(V18)
{
  vlaga_preniska_1 = param.asInt();
}
BLYNK_WRITE(V19)
{
  notifikacije_t2 = param.asInt();
}
BLYNK_WRITE(V20)
{
  vlaga_visoka_2 = param.asInt();
}


BLYNK_WRITE(V21)
{
  vlaga_previsoka_2 = param.asInt();
}
BLYNK_WRITE(V22)
{
  vlaga_niska_2 = param.asInt();
}

BLYNK_WRITE(V23)
{
  vlaga_preniska_2 = param.asInt();
}
BLYNK_WRITE(V24)
{
  notifikacije_t5 = param.asInt();
}
BLYNK_WRITE(V25)
{
  temp_gasi_svitlo = param.asInt();
}
BLYNK_WRITE(V26)
{
  da_ili_ne_gosi_svitlo = param.asInt();
}


BLYNK_WRITE(V1)
{ //reset botun
  if ((param.asInt() == 1))
  {
    digitalWrite(RESET_PIN, LOW);
  }
  else
    digitalWrite(RESET_PIN, HIGH);

}
BLYNK_WRITE(V27)
{ //reset SLAVE botun, resetiranje slave-a odnosno ocitavanje realtime vrijednosti
  if ((param.asInt() == 1))
  {
    digitalWrite(RESET_PIN_SLAVE, HIGH);
  }
  else
    digitalWrite(RESET_PIN_SLAVE, LOW);

}




void setup()
{
  digitalWrite(RESET_PIN, HIGH);
  digitalWrite(RESET_PIN_SLAVE, LOW);
  // Debug console
  Serial.begin(9600);
  sensors.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(relej_svitlo, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(RESET_PIN_SLAVE, OUTPUT);
  digitalWrite(relej_svitlo, LOW);

  timer.setInterval(1000L, sensorvalue1);
  timer.setInterval(1000L, sensorvalue2);
  timer.setInterval(1000L, sensorvalue3);
  timer.setInterval(1000L, tempvalue);


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
    myString = myString + rdata;
    // Serial.print(rdata);
    if ( rdata == '\n')
    {
      String l = getValue(myString, ',', 0);
      String m = getValue(myString, ',', 1);
      String n = getValue(myString, ',', 2);

      prva = l.toFloat();
      druga = m.toInt();
      treca = n.toInt();
      myString = "";
      // end new code
    }
  }

  if (notifikacije_t1 == HIGH)
  {
    if ((temp < temp_visoka_t1))
      n1t1 = 0;
    if ((temp < temp_previsoka_t1))
      n2t1 = 0;
    if ((temp > temp_niska_t1))
      n3t1 = 0;
    if ((temp > temp_preniska_t1))
      n4t1 = 0;


    if ((temp > temp_visoka_t1) && (n1t1 == 0))
    {
      n1t1 = 1;
      Blynk.notify("T1 temperatura visoka");
    }

    if ((temp > temp_previsoka_t1) && (n2t1 == 0))
    {
      n2t1 = 1;
      Blynk.notify("T1 temperatura previsoka !!!");
    }
    if ((temp < temp_niska_t1) && (n3t1 == 0))
    {
      n3t1 = 1;
      Blynk.notify("T1 temperatura niska");
    }

    if ((temp < temp_preniska_t1) && (n4t1 == 0))
    {
      n4t1 = 1;
      Blynk.notify("T1 temperatura preniska !!!");
    }
  }
  if (notifikacije_t2 == HIGH)
  {
    if ((prva < vlaga_visoka_1))
      n1t2 = 0;
    if ((prva < vlaga_previsoka_1))
      n2t2 = 0;
    if ((prva > vlaga_niska_1))
      n3t2 = 0;
    if ((prva > vlaga_preniska_1))
      n4t2 = 0;

    if ((prva > vlaga_visoka_1) && (n1t2 == 0))
    {
      n1t2 = 1;
      Blynk.notify("T2 Vlaga 1 visoka");
    }

    if ((prva > vlaga_previsoka_1) && (n2t2 == 0))
    {
      n2t2 = 1;
      Blynk.notify("Vlaga 1 previsoka !!!");
    }
    if ((prva < vlaga_niska_1) && (n3t2 == 0))
    {
      n3t2 = 1;
      Blynk.notify("Vlaga 1 niska");
    }

    if ((prva < vlaga_preniska_1) && (n4t2 == 0))
    {
      n4t2 = 1;
      Blynk.notify("Vlaga 1 preniska !!!");
    }
  }

  if (notifikacije_t5 == HIGH)
  {
    if ((druga < vlaga_visoka_2))
      n1t5 = 0;
    if ((druga < vlaga_previsoka_2))
      n2t5 = 0;
    if ((druga > vlaga_niska_2))
      n3t5 = 0;
    if ((druga > vlaga_preniska_2))
      n4t5 = 0;

    if ((druga > vlaga_visoka_2) && (n1t5 == 0))
    {
      n1t5 = 1;
      Blynk.notify("Vlaga 2 visoka");
    }

    if ((druga > vlaga_previsoka_2) && (n2t5 == 0))
    {
      n2t5 = 1;
      Blynk.notify("Vlaga 2 previsoka !!!");
    }
    if ((druga < vlaga_niska_2) && (n3t5 == 0))
    {
      n3t5 = 1;
      Blynk.notify("Vlaga 2 niska");
    }

    if ((druga < vlaga_preniska_2) && (n4t5 == 0))
    {
      n4t5 = 1;
      Blynk.notify("Vlaga 2 preniska !!!");
    }
  }

  if (da_ili_ne_gosi_svitlo == HIGH)
  {
    if ((temp < temp_gasi_svitlo - 5))
    {
      n5t1 = 0;
      digitalWrite(relej_svitlo, LOW);

    }
    if ((temp > temp_gasi_svitlo) && (n5t1 == 0))
    {
      n5t1 = 1;
      digitalWrite(relej_svitlo, HIGH);
      Blynk.notify("Ugašeno svitlo  !!!!!");
    }



  }

  Blynk.run();
  timer.run();

}

void tempvalue()
{
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, temp);
}

void sensorvalue1()
{
  int sdata = prva;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);
}
void sensorvalue2()
{
  int sdata = druga;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, sdata);
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
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
