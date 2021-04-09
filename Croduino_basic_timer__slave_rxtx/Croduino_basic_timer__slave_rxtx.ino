#include <SoftwareSerial.h>
SoftwareSerial nodemcu(0,1);
#define senzor_vlage_1 A0
#define senzor_vlage_2 A1
#define senzor_svitla A2
#define Reset_pin 4
#define napajanje_senzora_vlage1 3
#define napajanje_senzora_vlage2 5
#define Reset_pin_sa_master A3

unsigned long oldTime=0;
int vlaga1,vlaga2,svitlo;
bool b;
int a;
int sdata1 = 0; // sensor1 data
int sdata2 = 0; // sensor2 data
int sdata3 = 0; // sensor3 data
String cdata; // complete data, consisting of sensors values
unsigned long brojac=0;
void setup()

{
  digitalWrite(Reset_pin, HIGH);//prvo postavljam rst pin na high da ne dođe odma do reseta arduina
  delay(200); 
  Serial.begin(9600);
  nodemcu.begin(9600);
  pinMode(senzor_vlage_1, INPUT);
  pinMode(senzor_vlage_2, INPUT);
  pinMode(senzor_svitla, INPUT);
  pinMode(Reset_pin, OUTPUT);
  pinMode(Reset_pin_sa_master, INPUT);
  pinMode(napajanje_senzora_vlage1, OUTPUT); 
  pinMode(napajanje_senzora_vlage2, OUTPUT); 
  digitalWrite(napajanje_senzora_vlage1, HIGH); 
  digitalWrite(napajanje_senzora_vlage2, HIGH); 
}

void loop()

{
     unsigned long currentTime = millis();
          
  if (currentTime - oldTime < (3000))//Paljenje napajanja senzora
       {  
          a=0;
          b=0;
       }
  if (currentTime - oldTime < (5000)&&(millis() - oldTime > (3000))&&(b==0))//Očitavanje sa senzora
       { 
         vlaga1 =analogRead(senzor_vlage_1);  
         vlaga2 =analogRead(senzor_vlage_2);
         svitlo =analogRead(senzor_svitla);
         b=1;
       }    
  if (currentTime - oldTime < (7000)&&(millis() - oldTime > (6000)))////Gašenje napajanja senzora
       { 
        digitalWrite( napajanje_senzora_vlage1, LOW);
        digitalWrite( napajanje_senzora_vlage2, LOW);
       }
  
  if (currentTime - oldTime < (10000)&&(millis() - oldTime > (7000))&&(a<1))//slanje vrijednosti, dva puta šalje jer serial zna zaštekat
       { 
        //Serial.println(mois);
        sdata1=vlaga1;
        sdata2=vlaga2;
        sdata3=svitlo;
        cdata = cdata + sdata1+","+sdata2+","+sdata3; // comma will be used a delimeter
        Serial.println(cdata); 
        nodemcu.println(cdata);
        //delay(2000); 
        //cdata = ""; //  ovo bi trebalo za očistit variablu za ponovno primanje podaci, al ne triba pošto se arduino resetira
        a++;
        
       }    
 

   if (currentTime - oldTime >= (900000))//reset svaku 10 min 900000
       {
       // Serial.println("reset");
       //  delay(1000);
         digitalWrite(Reset_pin, LOW);
        }        

   if(analogRead(Reset_pin_sa_master)>300) //ZA OČITAVANJE VRIJEDNOSTI IZMEĐU INTERVALA SLANJA
       {
        digitalWrite(Reset_pin, LOW);  
       }
}
