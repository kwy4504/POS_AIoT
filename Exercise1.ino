///////////////////청년 AI 빅데이터 아카데미 C4조 김우영, 전현지/////////////////////
/////////////////////////////AIOT_Exercise 3//////////////////////////////////////
/*************업로딩 에러시 터미널 열고 sudo chmod a+rw /dev/ttyUSB0***************/

//전처리문//
#include <U8g2lib.h>      // OLED 모듈
#include "DHT.h"          // DHT22
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN A1
#define DHTTYPE DHT22
#define SOILHUMI A6

DHT dht(DHTPIN, DHTTYPE);

uint32_t DataCaptureDelay = 3000;     // 3000ms 간격 설정
uint32_t DataCapture_ST = 0;

//변수 선언//
float Temp;     // 온도
float Humi;     // 습도
float Soilhumi; // 토양 습도

//쎄-텁 팡션//
void setup() {
  dht.begin();
  u8g2.begin();
  pinMode(SOILHUMI, INPUT);         // pin num = A6, mode = INPUT

  DataCapture_ST = millis();
}

//뤂 팡션//
void loop() {
  //*****3000ms 간격으로 Temp, Humi 측정*********//
  if ((millis() - DataCapture_ST) > DataCaptureDelay) {
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();

  //*****Humi, Temp, Soilhumi nan값 출력시*******//
  if (isnan(Humi) || isnan(Temp) || isnan(Soilhumi)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);    // 습도가 높으면 0, 낮으면 100
  OLEDdraw();
  DataCapture_ST = millis();
  }
}

void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 15, "AIBD AIOT");

  //*******Temp 표시*********//
  u8g2.drawStr(15, 36, "Temp.");
  u8g2.setCursor(85, 36);
  u8g2.print(Temp);
  u8g2.drawStr(114, 36, "\xb0");
  u8g2.drawStr(119, 36, "C");

  //*******Humidity 표시********//
  u8g2.drawStr(15, 47, "Humidity");
  u8g2.setCursor(85, 47); u8g2.print(Humi);
  u8g2.drawStr(116, 47, "%");

  //*******Soilhumidity 표시********//
  u8g2.drawStr(1, 58, "Solhumidity");
  u8g2.setCursor(85, 58); u8g2.print(Soilhumi);
  u8g2.drawStr(116, 58, "%");

  u8g2.sendBuffer();
}