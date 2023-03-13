// Library
#include <WiFi.h> 
#include <FirebaseESP32.h>  
#include "MQ7.h"

// Koneksi Firebase dan wifi
#define FIREBASE_HOST "https://iott-896b7-default-rtdb.asia-southeast1.firebasedatabase.app/" // URL RTDB Firebase
#define FIREBASE_AUTH "QzSYGvhA7NnIWlFLYlDuw4qlhRA6fyVc4ADqJX22"  // Token Firebase
#define WIFI_SSID "hjjn"  // Nama Wifi/Hospot
#define WIFI_PASSWORD "11111111"  // Password Wifi/Hotspot
int Buzzer = 26;

// Deklarasi setiap sensor
int mq7 = 34;
int PPM_mq7 = 0;
int input_mq2 = 34;  
int input_mq135 = 34;     


FirebaseData firebaseData;

void setup(){

  Serial.begin(115200);
  
  pinMode(input_mq135, INPUT);
  pinMode(Buzzer, OUTPUT);
  
  // Koneksi Wifi 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("terhubung dengan IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("-");
  Serial.println("Terhubung...");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

void loop() {

  // MQ 135
  int nilai = analogRead(input_mq135);
  int persen = (nilai * 0.004887)/4.84*100;
  int PPM_mq135 = (((nilai*0.004887)*100)*2);

  Serial.print("Nilai Input MQ-135: ");
  Serial.println(nilai);
  
  Serial.print("Konsentrasi CO2: ");
  Serial.print(persen);
  Serial.println(" %");

  Serial.print("Kadar CO2: ");
  PPM_mq135 = (PPM_mq135 / 11);
  Serial.print(PPM_mq135);
  Serial.println(" PPM");

  // MQ 7
   PPM_mq7 = analogRead(mq7);
   PPM_mq7 = (PPM_mq7 / 10);
   Serial.print("Kadar CO: "); 
   Serial.println(PPM_mq7);
   
   // MQ 2
    int output_mq2 = analogRead(input_mq2);  /*Analog value read function*/
    Serial.print("Gas Sensor: ");  
    Serial.print(output_mq2);   /*Read value printed*/
    Serial.print("\t");
    Serial.print("\t");
    if (output_mq2 > 900) {   
      digitalWrite(Buzzer, HIGH);
      delay(1000);
      digitalWrite(Buzzer, LOW);
      delay(1000);  
      Serial.println("Ada Gas");  
    } else {
        Serial.println("Tidak ada Gas");
        digitalWrite(Buzzer, LOW);
    }
    delay(1000);                
    
  // RTDB Firebase
 
  Firebase.setFloat(firebaseData, "/ (MQ-135) Kadar CO2 (PPM)", PPM_mq135);
  Firebase.setFloat(firebaseData, "/ (MQ-2) Kadar Gas (PPM)", output_mq2);
  Firebase.setFloat(firebaseData, "/ (MQ-7) Kadar CO (PPM)", PPM_mq7);
  delay(1000);

     
}


 
