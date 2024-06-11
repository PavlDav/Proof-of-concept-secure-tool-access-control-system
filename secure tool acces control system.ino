
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
  #include "rgb_lcd.h"  
  rgb_lcd lcd; 

float RFIDbadge;
float badge;
unsigned int compteur_temps=0;
char personne = 'o';
char outil= 'z';
char mode= 'x';
//RFID VALEURS:
//1:Employé valide Robert: 303407763
//2:Non valide: 272074531
//3:Outil Tournevis: 301081683
//4: Outil scie defectueuse: 226992019
//5: 2550819795
//6: 2551818211



#define PIN_LED_ROUGE 5
#define PIN_LED_VERTE 4
#define PIN_LED_JAUNE 7
int bouton_prise = 31; // Port d'entrée numérique
int bouton_remise= 33; // Port d'entrée numérique
int bouton_balance = 35; // Port d'entrée numérique
int bouton_reset = 41; // Port d'entrée numérique
int bouton_ouverture = 45; // Port d'entrée numérique

Servo myservo;


MFRC522 rfid(53,6); //(CS, RST) //Testé avec rfid(53,6) sur MEGA
#define RFID(val) (uint32_t(rfid.uid.uidByte[val])<<(val*8))
#define RFID_CODE RFID(0)+RFID(1)+RFID(2)+RFID(3)

const byte monUID[4] = {60, 203, 205, 33};
bool valid=true;


#include "HX711.h"
HX711 capteur;
int DAT = 3; //=DT
int CLK = 2;
long valeur;

// lecteur MP3 sur UART2
#include "DFRobotDFPlayerMini.h" 
#define mySoftwareSerial Serial2 // Serial2 = prise UART2 sur un shield Grove 
DFRobotDFPlayerMini myDFPlayer;



//////////SETUP



void setup()
{ 
  
  Serial.begin(115200); delay(500); Serial.println(F("Serial:ok"));
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.setTimeOut(500); // Définit un temps de time out sur la communication série à 500 ms
  myDFPlayer.volume(20);                                // Volume de 0~30
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);          // Utilisation de la carte SD

  SPI.begin();
  rfid.PCD_Init();
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(0);  

  capteur.begin(DAT,CLK);
  Serial.begin(9600);
  Serial.println("HX711");

  pinMode(bouton_prise, INPUT); // Initialiser le port d'entrée numérique en mode entrée
  pinMode(bouton_remise, INPUT);
  pinMode(bouton_balance, INPUT);
  pinMode(bouton_reset, INPUT);
  pinMode(bouton_ouverture, INPUT);


  pinMode(PIN_LED_ROUGE, OUTPUT);
  pinMode(PIN_LED_VERTE, OUTPUT);
  pinMode(PIN_LED_JAUNE, OUTPUT);

  lcd.begin(16, 2);     
  lcd.setCursor(0, 0); 
  lcd.print("EPF Projet outil BTP:)");
  delay(1000);
  lcd.clear();
}



////////////////////LOOP//////////////////////////////////////////////////////////////////////////////////////////////////////////





void loop()
{
    unsigned long tempsDebut = millis();  

  if (personne == 'o'){
    lcd.setRGB(255, 255, 255);
    lcd.setCursor(0, 0);
    lcd.print("Merci de");
    lcd.setCursor(0, 1);
    lcd.print("vous identifier");
  }
  else{
    compteur_temps++;
  }
  
  badge = (lecteurRFID());
  

  int etat_prise = digitalRead(bouton_prise);
  int etat_remise = digitalRead(bouton_remise); 
  int etat_balance = digitalRead(bouton_balance); 
  int etat_reset = digitalRead(bouton_reset); 
  int etat_ouverture = digitalRead(bouton_ouverture); 

  if (badge == 303407763){
    RFIDbadge = 303407763;
    
      lcd.clear();
      personne = 'r';
      digitalWrite(PIN_LED_VERTE, HIGH);
      myservo.write(90);
      myDFPlayer.play(1);
      lcd.setRGB(0,255,0);
      lcd.setCursor(0, 0);
      lcd.print("Porte ouverte.");
      lcd.setCursor(0, 1);
      lcd.print("Bonjour.");
      delay(4000);

      lcd.clear();
  
      digitalWrite(PIN_LED_VERTE, LOW);
      myservo.write(0);
      lcd.print("Porte fermee.");
      delay(1000);
      lcd.setRGB(255, 255, 255);
      lcd.clear();      
      }

if (rfid.PICC_IsNewCardPresent()&& personne=='r'){

    
      lcd.clear();
      personne = 'o';
      digitalWrite(PIN_LED_VERTE, HIGH);
      myservo.write(90);
      myDFPlayer.play(1);
      lcd.setRGB(0,255,0);
      lcd.setCursor(0, 0);
      lcd.print("Porte ouverte.");
      lcd.setCursor(0, 1);
      lcd.print("Au revoir.");
      delay(4000);

      lcd.clear();
  
      digitalWrite(PIN_LED_VERTE, LOW);
      myservo.write(0);
      lcd.print("Porte fermee.");
      delay(1000);
      lcd.setRGB(255, 255, 255);
      lcd.clear();      
      }

      
    else if  (badge == 272074531){
    RFIDbadge = 272074531;
    
      lcd.clear();
      myDFPlayer.play(2);
      digitalWrite(PIN_LED_ROUGE, HIGH);
      lcd.setRGB(255,0,0);
      lcd.print("Badge non valide.");

      delay(4000);
      digitalWrite(PIN_LED_ROUGE, LOW);
      lcd.clear();
      lcd.setRGB(255, 255, 255);
      }



      
        else if  (mode=='p'){
          if (badge == 301081683){
    RFIDbadge = 301081683;
    
      lcd.clear();
      outil='t';
      myDFPlayer.play(1);
      digitalWrite(PIN_LED_VERTE, HIGH);
      lcd.setRGB(0,255,0);
      lcd.print("Tournevis pris");

      delay(4000);
      digitalWrite(PIN_LED_VERTE, LOW);
      lcd.clear();
      lcd.setRGB(255, 255, 255);
      }
        }




       else if  (badge == 2551818211){
    RFIDbadge =2551818211;
    
      lcd.clear();
      outil='s';
      myDFPlayer.play(3);
      lcd.setRGB(255,255,0);
      digitalWrite(PIN_LED_JAUNE, HIGH);

      lcd.print("Scie !Probleme!");

      delay(4000);
      digitalWrite(PIN_LED_JAUNE, LOW);
      lcd.clear();
      lcd.setRGB(255, 255, 255);
      }




   else if (personne == 'r'){
    if (mode=='x'){
    lcd.clear();
    lcd.setRGB(0, 0, 255);
    lcd.setCursor(0, 0);
    lcd.print("Veuillez choisir");
    lcd.setCursor(0, 1);
    lcd.print("le mode");
    }
    if (etat_prise == HIGH) {
      mode='p';
       lcd.clear();
       lcd.setRGB(255, 255, 255);
       lcd.setCursor(0, 0);
      lcd.print("Mode prise");
      lcd.setCursor(0, 1);
      lcd.print("actif");
          delay(4000);
      }
    if (etat_balance == HIGH) {
      mode='b';

       lcd.clear();
       lcd.setRGB(255, 255, 255);
       lcd.setCursor(0, 0);
      lcd.print("Mode peser");
      lcd.setCursor(0, 1);
      lcd.print("actif");
          delay(4000);
              lcd.clear();
       lcd.setRGB(255, 255, 255);
       lcd.setCursor(0, 0);
      lcd.print("Merci de poser");
      lcd.setCursor(0, 1);
      lcd.print("un objet");
          delay(2000);
      lcd.clear();
      while(millis() - tempsDebut < 15000){
      valeur = capteur.read_average(1);
       lcd.setCursor(0, 0);
      lcd.print("Poids:");
      lcd.setCursor(0, 1);
      lcd.print(valeur);
      delay(1000);
      lcd.clear();

      }
        if (valeur >= 34000 && valeur <= 36000) {
       lcd.setCursor(0, 0);
    lcd.print("Ecouteurs");
              lcd.setCursor(0, 1);
                  lcd.print("reconnus");
              myDFPlayer.play(1);
      lcd.setRGB(0,255,0);
    delay(4000);
    lcd.clear();
  } else if(valeur >= 31000 && valeur <= 33000){
    lcd.setCursor(0, 0);
    lcd.print("Ecouteurs");
          lcd.setCursor(0, 1);
              lcd.print("!probleme!");
          myDFPlayer.play(3);
      lcd.setRGB(255,255,0);
          delay(4000);
    lcd.clear();
  }
      mode='x';
}
          
      
          if (etat_remise == HIGH) {
            mode='r';
       lcd.clear();
       lcd.setRGB(255, 255, 255);
       lcd.setCursor(0, 0);
      lcd.print("Mode remise");
      lcd.setCursor(0, 1);
      lcd.print("actif");
          delay(4000);
      }
       if (etat_reset == HIGH) {
        mode='x';
       lcd.clear();
       lcd.setRGB(255, 255, 255);
       lcd.setCursor(0, 0);
      lcd.print("Reinitialisation");
      lcd.setCursor(0, 1);
      lcd.print("en cours");
          delay(4000);
      return;
       }
   
  
   if (etat_ouverture ==HIGH ){
          lcd.clear();
           personne='o';
      digitalWrite(PIN_LED_VERTE, HIGH);
      myservo.write(90);
      myDFPlayer.play(1);
    lcd.setCursor(0, 0);
    lcd.print("Ouverture");
    lcd.setCursor(0, 1);
    lcd.print("d'urgence");
      lcd.setRGB(0,255,0);
      delay(4000);

      lcd.clear();
  
      digitalWrite(PIN_LED_VERTE, LOW);
      myservo.write(0);
      lcd.print("Porte fermee.");
      delay(1000);
      lcd.setRGB(255, 255, 255);
      lcd.clear();      

  }

}

}



//////////////FONCTIONS///////////////

long lecteurRFID() {
  if ((rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())) {
    uint32_t code = RFID_CODE;
    return code;
  }  
    // fin if
  else{
    return 0;
  }
}

