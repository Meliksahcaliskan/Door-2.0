#include <SPI.h>            //NFC kütüphanesi
#include <RFID.h>           //NFC kütüphanesi
#include <Servo.h>          //Servo kütüphanesi


#define trigPin 4           //Mesafe sensörü port tanımlama
#define echoPin 2           //Mesafe sensörü port tanımlama

RFID lrt720(10, 9);         //NFC port tanımlama

int sure, mesafe;           //Mesafe ölçümü için gerekli değişkenler

Servo myservo;              //Servo tanımlama

int kart1[5] = {112, 212, 222, 33, 91};     //NFC 1. kart değerleri
int kart2[5] = {33, 66, 250, 35, 186};      //NFC 2. kart değerleri

int kapi = 0;               //Kapının durumu: 0 = kapalı , 1 = açık

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();            //NFC için 
  lrt720.init();          //NFC için  

  pinMode(trigPin, OUTPUT);     //Mesafe hoparlör tanımlama
  pinMode(echoPin, INPUT);      //Mesafe mikrofon tanımlama

  myservo.attach(9);            //Servo port atama

  Serial.println("Başladı");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (lrt720.isCard()) {        //Kart okumaya başlıyor


    if (lrt720.readCardSerial()) {            
      if (lrt720.serNum[0] == kart1[0]) {       //Eğer kartlardan herhangi biri okunursa hangi kartın okunduğunu söylüyor ve açıksa kapatıyor, kapalıysa açıyor
        Serial.println("1. kart okutuldu");
        if (kapi == 0) {
          Serial.println("kapı kapalı");
          myservo.write(0);                     //Kapıyı açıyor (duruma göre dönüş yönü ayarlanacak)
          delay(15);
        } else {
          Serial.println("kapı açık");
          myservo.write(90);                    //Kapıyı kapatıyor (duruma göre dönüş yönü ayarlanacak)
          delay(15);
        }
      } else if (lrt720.serNum[0] == kart2[0]) {
        Serial.println("2. kart okutuldu");
        if (kapi == 0) {
          Serial.println("kapı kapalı");
          myservo.write(0);
          delay(15);
        } else {
          Serial.println("kapı açık");
          myservo.write(90);
          delay(15);
        }
      }
      /*Serial.println("Kart Bulundu ID : ");    ==> Burada kart değerleri bastırılıyor ama ihtiyacımız yok
        Serial.print(lrt720.serNum[0]);
        Serial.print(" , ");
        Serial.print(lrt720.serNum[1]);
        Serial.print(" , ");
        Serial.print(lrt720.serNum[2]);
        Serial.print(" , ");
        Serial.print(lrt720.serNum[3]);
        Serial.print(" , ");
        Serial.print(lrt720.serNum[4]);
        Serial.println(" ");*/
    }
  }
  lrt720.halt();

  
  digitalWrite(trigPin, LOW);         //Mesafe sensörü için gerekli işlemler
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH);
  mesafe = (sure / 2) * 0.0343;

  
  Serial.print(mesafe);               //Mesafeyi bastırıyor
  Serial.println(" cm uzaklıkta");
  delay(500);

  if (mesafe < 6) {                   //Mesafeye göre kapının açık mı kapalı mı olduğunu söylüyor  (uzunluk veya aralık sonradan ayarlanabilir
    Serial.println("Kapı açık");
    kapi = 1;
  } else {
    Serial.println("Kapı kapalı");
    kapi = 0;
  }


                                      //Buraya serverdan gelen veriye göre aksiyon aldıracağın komutları yazabilirsin servo kodları yukarıda var zaten.



}
