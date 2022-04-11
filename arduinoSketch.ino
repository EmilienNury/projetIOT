#include <NfcAdapter.h>
#include <PN532/PN532/PN532.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <TimeLib.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#if 0 // use SPI
    #include <SPI.h>
    #include <PN532/PN532_SPI/PN532_SPI.h>
    PN532_SPI pn532spi(SPI, 9);
    NfcAdapter nfc = NfcAdapter(pn532spi);
#elif 1 // use hardware serial

    #include <PN532/PN532_HSU/PN532_HSU.h>
    PN532_HSU pn532hsu(Serial1);
    NfcAdapter nfc(pn532hsu);
#elif 0  // use software serial

    #include <PN532/PN532_SWHSU/PN532_SWHSU.h>
    #include "SoftwareSerial.h"
    SoftwareSerial SWSerial(2, 3);
    PN532_SWHSU pn532swhsu(SWSerial);
    NfcAdapter nfc(pn532swhsu);
#else //use I2C

    #include <Wire.h>
    #include <PN532/PN532_I2C/PN532_I2C.h>
    PN532_I2C pn532_i2c(Wire);
    NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif

#define WIFI_SSID "S10"
#define WIFI_PASSWORD "pirpir01"

// Insert Firebase project API Key
#define API_KEY "AIzaSyC0rO3XTGbYBo3uMymErxRppkCyQoyCJ-A"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://iotnfc-6409c-default-rtdb.europe-west1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

const int speaker = 26;//set speaker to pin 8

int pirPin = 34;                 // PIR Out pin 
int pirStat = 0;  

bool IsAlarmActivated = true; //TODO swap
bool IsAlarmOn = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);




int playing = 0;
void tone(byte pin, int freq) {
  ledcSetup(0, 2000, 8); // setup beeper
  ledcAttachPin(pin, 0); // attach beeper
  ledcWriteTone(0, freq); // play tone
  playing = pin; // store pin
}
void noTone() {
  tone(playing, 0);
}

void setup() {
  SERIAL.begin(115200);
  SERIAL.println("NDEF Reader");
  nfc.begin();
  pinMode(speaker, OUTPUT);//define the speaker as an output
  pinMode(pirPin, INPUT);
  Serial.print(WIFI_SSID);
   Serial.print(WIFI_PASSWORD);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  timeClient.begin();
  timeClient.setTimeOffset(7200);


}

void loop() {
  if (IsAlarmActivated){
    pirStat = digitalRead(pirPin); 
    if (pirStat == HIGH) {   // if motion detected
      IsAlarmOn = true;
      if(Firebase.ready() && signupOK){
        String date = getDate();
        if(Firebase.RTDB.setString(&fbdo, "Alarm/"+date,date)){
           Serial.println("PASSED");
           Serial.println("PATH: " + fbdo.dataPath());
           Serial.println("TYPE: " + fbdo.dataType());
        }else{
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }
      }
    
    } else {
      //IsAlarmOn = false; //TODO appuit sur un bouton
    }
  }
  if (IsAlarmOn) {
    tone(speaker, 800);
    delay(200);
    tone(speaker, 600);
    //delay(200);
  } else {
    noTone();
    digitalWrite(speaker, LOW);// if else don't set off alarm
  }
  if (nfc.tagPresent()) {
        NfcTag tag = nfc.read();
        tag.print();
        IsAlarmActivated = false;
        IsAlarmOn = false;
        Firebase.RTDB.setString(&fbdo, "Entry/" + getDate(),tag.getUidString());
    }
    //delay(5000);
}

String getDate(){
  timeClient.update();
  auto epoch = timeClient.getEpochTime();
  String date = "";
  date = date + hour(epoch) + ":" + minute(epoch) + ":" + second(epoch) + " " + day(epoch) + "-" + month(epoch) + "-" + year(epoch);
  return date;
}
