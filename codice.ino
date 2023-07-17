/********************************************************
* Seconda Esperienza Secondo Periodo Sistemi Automatici 
* Bosi Mattia 5°N
* LA SCHEDA SCELTA PER QUESTO PROGETTO E' ARDUINO UNO
***********************************************************/
/***********************************************************
* PIN ANALOGICI CHE SI POSSONO UTILIZZARE COME DIGITALI
* A0 = 14 - A1 = 15 - A2 = 16 - A3 = 17 - A4= 18 - A5 = 19
*
************************************************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "DHT.h"

#include <LiquidCrystal.h>




#define DHTTYPE DHT11
#define DHTPIN 7
#define RELEVENTOLA 8
#define RELERISCALDATORE 12
#define AD590PIN A1
#define DIM 5
#define INTERRUTTORE 9
#define PULSUP 16
#define PULSDOWN 17

int TEMPMAX = 0;
int TEMPMIN = 0;
int modTempMax;
int modTempMin;
float temperatura[DIM];
float umidita[DIM];
float valoreAnalogico[DIM];
float tempIstDht;
float tempIstAd590;
float tempAd590;
float tempAd590Volt;
float valoreUmidita = 0;
float media = 0;
int val2 = 0;
int dim = 5;
int val;
int i;


LiquidCrystal lcd(13,11,6,5,4,3);
DHT dht(DHTPIN,DHTTYPE);


void setup() 
{
  Serial.begin(9600);
  
  //dichiarazione pin 
  
  pinMode(RELEVENTOLA, INPUT);
  pinMode(RELERISCALDATORE,INPUT);
  pinMode(DHTPIN,INPUT);
  pinMode(AD590PIN,INPUT);
  pinMode(INTERRUTTORE,INPUT);
  pinMode(PULSUP, INPUT);
  pinMode(PULSDOWN, INPUT);
  TEMPMAX = 24;
  TEMPMIN = 19;
  modTempMax=0;
  modTempMin=0;


 
  //INIZIALIZZAZIONE DISPLAY LCD
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  
}

void loop()
{

    int val = digitalRead(INTERRUTTORE); //LETTURA STATO INTERRUTTORE

    delay(1000);
   
  
            

  
    if(val == 0)  //MODALITA AUTOMATICA
    { 
      
      digitalWrite(RELERISCALDATORE, LOW);
      digitalWrite(RELEVENTOLA, LOW);
      lcd.print("MODALITA'");
      lcd.setCursor(0,1);
      lcd.print("AUTOMATICA");
      Serial.println("MODALITA' AUTOMATICA");

      delay(1000);
      lcd.clear();

      
      misuroTemperatura();
      misuroUmidita();
      calcoloMedia();

    }
    
    if(val == 1)  //MODALITA MANUALE
    {
      
      lcd.print("MODALITA'");
      lcd.setCursor(0,1);
      lcd.print("MANUALE");
      delay(1000);
      lcd.clear();
      
      modifTemp();
      
      
    }
   
    
}



float misuroTemperatura() //FUNZIONE CHE ACQUISISCE I VALORI DI TEMPERATURA
{
i=0;
val == digitalRead(INTERRUTTORE);
int conta = 5;
if(val==0)
{       while( conta > 0)
        {
                lcd.print("ACQUISIZIONE FRA");
                lcd.setCursor(0,1);
                
                lcd.print(conta);
                conta--;
                delay(1000);
                lcd.clear();
        
        }
       
       do
       {
          
          tempAd590Volt = analogRead(AD590PIN);
       
          tempAd590 = map(tempAd590Volt,0, 1023, 273, 353);
          tempIstAd590 = (tempAd590 - 293);
          
          tempIstDht = dht.readTemperature();
          //tempIstDht = 22;
         
          delay(50);
          temperatura[i] = (tempIstDht + tempIstAd590)/2;
           

          Serial.println(i);
 
          
          Serial.print("TEMP DHT = ");
          Serial.print(tempIstDht);
          Serial.println("°C");

          Serial.print("TEMP AD = ");
          Serial.print(tempIstAd590);
          Serial.println("°C");

          lcd.print("MISURAZIONE T");
          lcd.setCursor(0,1);
          lcd.print("IN CORSO...");
          
        delay(1000);        
        
        i++;
        lcd.clear();

      }while(i<DIM);
      for(i=0;i<DIM;i++)
      {
        Serial.print("TEMP ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.println(temperatura[i]);
        delay(200);
      }
   }
   if(val==1)
   {
        lcd.clear();
        delay(1000);
        return 1;
   }
  

  
}

float misuroUmidita()
{
  i=0;
  valoreUmidita=0;
  val == digitalRead(INTERRUTTORE);
    if(val == 0)
    {
        do
        {  
          umidita[i] = dht.readHumidity();
          
          valoreUmidita = valoreUmidita + umidita[i];
          Serial.println(i);
      
          Serial.print("UMIDITA' = ");
          Serial.println(umidita[i]);
        
          lcd.print("MISURAZIONE UM");
          lcd.setCursor(0,1);
          lcd.print("IN CORSO...");
          delay(1000);
          lcd.clear();
    
          i++;
        }while(i<DIM);
        
        valoreUmidita = valoreUmidita/DIM;

        Serial.print("MEDIA UMIDITA' = ");
        Serial.println(valoreUmidita);
        lcd.print("UMIDITA = ");
        lcd.print(valoreUmidita);
        delay(2000);
        lcd.clear();
    }
    if(val == 1)
    {
        lcd.clear();
        delay(1000);
        return 1;
    }
 
}

float calcoloMedia()
{
    digitalWrite(RELERISCALDATORE, LOW);
    digitalWrite(RELEVENTOLA, LOW);
    val == digitalRead(INTERRUTTORE);
    media = 0;
    if(val==0)
    {
        for(i=0;i<DIM;i++)
        {
          media = media + temperatura[i];
          delay(200);
        }
      
        media = media/DIM;
        
        lcd.print("MEDIA TEMP = ");
        lcd.print(media);
        Serial.print("MEDIA TEMP = ");
        Serial.print(media);
        Serial.println("°C");
      
        delay(1000);
        lcd.clear();
      
      if(media < TEMPMIN )
      {
         tempBassa();
      }
      else if (media > TEMPMAX)
      {
         tempAlta();
      }
      else
      {
          lcd.clear();
          lcd.print("TEMP OK...");
          
          Serial.println("TEMP OK...");
          
          delay(2000);
          lcd.clear();
      }
  

  }
  if(val ==1)
  {
    lcd.clear();
    delay(1000);
    return 1;
  }   
      
}
  

void tempBassa()
{
    lcd.clear();
    lcd.print("ATTIVO");
    lcd.setCursor(0,1);
    lcd.print("RISCALDATORE");

   
    Serial.println("ATTIVO RISCALDATORE");
    digitalWrite(RELERISCALDATORE, HIGH);
    
    delay(2000);
    
    lcd.clear();
}

void tempAlta()
{

    lcd.clear();
    lcd.print("ATTIVO");
    lcd.setCursor(0,1);
    lcd.print("VENTOLA");

    Serial.println("ATTIVO VENTOLA");
    digitalWrite(RELEVENTOLA, HIGH);
    delay(2000);
    
    lcd.clear();
  
}

int modifTemp()     //FUNZIONE CHE MODIFICA LA TEMP IN MODALITA MANUALE
{
  i=0;
  val == digitalRead(INTERRUTTORE);

  delay(500);
  
  if(digitalRead(INTERRUTTORE) == 0)
  {
    return 1;
  }
  else if(digitalRead(INTERRUTTORE) == 1)
  {
    if(digitalRead(PULSDOWN) == HIGH &&  digitalRead(PULSUP) == LOW ) //MODIFICA TEMP MAX
    { 
        do
        {
          modTempMax = modTempMax +1;
          delay(1000);
          lcd.print("TEMPMAX = ");
          lcd.print(modTempMax);
          delay(1000);
          lcd.clear(); 
          
        }while(digitalRead(PULSUP) == LOW);
        lcd.clear();
    }
  
    delay(1000);

     if(digitalRead(PULSDOWN) == LOW && digitalRead(PULSUP) == HIGH)  //MODIFICA TEMP MIN
    { 
        do
        {
          modTempMin = modTempMin +1;
          delay(1000);
          lcd.print("TEMPMIN = ");
          lcd.print(modTempMin);
          delay(1000);
          lcd.clear(); 

        }while(digitalRead(PULSDOWN) == LOW);
      lcd.clear();
    }
  
    delay(1000);
  
  
  if (digitalRead(PULSDOWN) == HIGH && digitalRead(PULSUP) == HIGH )
    {
      do
      {
        TEMPMIN = modTempMin;
        TEMPMAX = modTempMax;

        lcd.clear();
        lcd.print("TEMPMIN IMPOSTATA");
        lcd.setCursor(0,1);
        lcd.print(TEMPMIN);
        delay(2000);
        lcd.clear();

        lcd.print("TEMPMAX IMPOSTATA");
        lcd.setCursor(0,1);
        lcd.print(TEMPMAX);
        delay(2000);
        lcd.clear();
        
      }while(digitalRead(PULSDOWN) == HIGH && digitalRead(PULSUP) == HIGH && digitalRead(INTERRUTTORE) == 1 );
      
      lcd.clear();
    }
  }  

} 



      
