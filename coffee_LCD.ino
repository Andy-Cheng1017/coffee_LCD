#include <ESP32Encoder.h>
//#include <Encoder.h>
//#include <RGBLed.h>
#include <Adafruit_NeoPixel.h>
#include <BluetoothSerial.h>
//#include <EEPROM.h>

#define D1 32
#define D2 12
#define D3 13
#define D4 14
#define D5 25
#define D7 27
#define D8 26
#define Encoder_1 15
#define Encoder_2 2
#define Encoder_Button 4
#define WS2812B_Pin 21
#define NUMPIXELS 20
#define R_Pin 5
#define G_Pin 18
#define B_Pin 19
#define Sleep_Pin 33

//Encoder myEnc(Encoder_1, Encoder_2);
//RGBLed led(R_Pin, G_Pin, B_Pin, 1);
Adafruit_NeoPixel pixels(NUMPIXELS, WS2812B_Pin, NEO_GRB + NEO_KHZ800);
BluetoothSerial BT;
ESP32Encoder encoder;

void setup() {
  delay(5000);
  Serial2.begin(115200);
  Serial.begin(115200);
  pinMode(D1, INPUT_PULLDOWN);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT_PULLDOWN);
  pinMode(D4, OUTPUT);
  pinMode(D5, INPUT_PULLDOWN);
  pinMode(D7, INPUT_PULLUP);
  pinMode(D8, OUTPUT);
  pinMode(Sleep_Pin, INPUT_PULLUP);
  pinMode(Encoder_Button, INPUT_PULLUP);


  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(R_Pin, 0);
  ledcAttachPin(G_Pin, 1);
  ledcAttachPin(B_Pin, 2);
  ledcWrite(0, 255);
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  pixels.begin();
  BT.begin("Coffee");
  //  EEPROM.begin(4096);
  //  delay(10000);

  ESP32Encoder::useInternalWeakPullResistors = UP;
  encoder.attachSingleEdge(Encoder_1, Encoder_2);
  encoder.clearCount();
  digitalWrite(D2, 0);
  ledcWrite(0, 90);
  ledcWrite(1, 40);
  ledcWrite(2, 0);
  //    Serial.println(Brightness);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color( 5 , 5 , 5 ));
  }
  pixels.show();
}
int Sleep = 1;
long t;
long t1;
int flip = 1;
int LastCount;
int LcdCommend;
int Brightness;
//String a;
int a[12];
void loop() {
  if (LastCount != encoder.getCount()) {
    LcdCommend = encoder.getCount() > LastCount ? 0x01 : 0x02;
    LastCount = encoder.getCount();
    Serial.print(LcdCommend);
    Serial2.print(LcdCommend);

  }
  //  Serial.println("ok");
  if (!digitalRead(Encoder_Button)) {
    delay(10);
    while (!digitalRead(Encoder_Button));
    Serial.print('3');
    Serial2.print('3');
    delay(10);
  }
  if (digitalRead(Sleep_Pin)) {
    delay(10);
    while (digitalRead(Sleep_Pin));
    delay(10);
    if (Sleep == 1) {
      Serial.print('4');
      Serial2.print('4');
      Sleep *= -1;
    }
    else if (Sleep == -1) {
      Serial.print('4');
      Serial2.print('5');
      Sleep *= -1;
    }
    //    Serial.print('4');
    //    Serial2.print('4');
    //    while (Sleep) {
    //      delay(10);
    //      Sleep = !digitalRead(Sleep_Pin);
    //      while (digitalRead(Sleep_Pin));
    //      delay(10);
    //    }
    //    Serial.print('5');
    //    Serial2.print('5');
    //    delay(10);
  }
  if (Sleep == -1) {
    ledcWrite(0, 256);
    ledcWrite(1, 256);
    ledcWrite(2, 256);
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color( 0 , 0 , 0 ));
    }
    pixels.show();
  }
  else if(Sleep == 1){
    ledcWrite(0, 90);
    ledcWrite(1, 40);
    ledcWrite(2, 0);
    //    Serial.println(Brightness);
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color( 5 , 5 , 5 ));
    }
    pixels.show();
  }
  if (BT.available()) {
    //藍牙端接受資料發送到電腦
    Serial2.write(BT.read());
  }
  if (Serial2.available()) {
    delay(10);
    for (int i = 0; i < 12; i++) {
      a[i] = Serial2.read();
    }
    if ((a[0] > 0 && a[0] <= 10 ) || (a[4] > 0 && a[4] <= 10 )) {
      a[0] *= 20;
      a[4] *= 50;
      //
      for (int i = 0; i < 12; i++) {
        Serial.println(a[i]);
      }
      //    Serial.print(c);
      //    Serial.flush();                       //erase any data on serial port after operation
      //    BT.write(Serial2.read());
    }
    else {
      for (int i = 0; i < 12; i++) {
        a[i] = 0;
      }
    }
  }
  if (millis() - t1 > 100) {
    t1 = millis();
    if (a[0] >= 1) {
      digitalWrite(D4, 1);
      Serial.println(a[0]);
      a[0]--;
    }
    else {
      digitalWrite(D4, 0);
    }
    if (a[4] >= 1) {
      Serial.println(a[4]);
      digitalWrite(D8, 1);
      a[4]--;
    }
    else {
      digitalWrite(D8, 0);
    }

  }
}

//}
//    Serial1.write(Serial.read());         //Push it through port 1 to Device B
//  if (Serial.available()) {
//    Serial2.write(Serial.read);         //Push it through port 2 to Device A
//    Serial2.write(0xff);
//    Serial2.write(0xff);
//    Serial2.write(0xff);
//    delay(10);
//  }
//    Serial.flush();                       //erase any data on serial port after operation
