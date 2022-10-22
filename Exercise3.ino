///////////////////청년 AI 빅데이터 아카데미 C4조 김우영, 전현지/////////////////////
/////////////////////////////AIOT_Exercise 3//////////////////////////////////////
/*************업로딩 에러시 터미널 열고 sudo chmod a+rw /dev/ttyUSB0***************/

//전처리문//
#include <U8g2lib.h>      // OLED 모듈
#include "DHT.h"          // DHT22
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define SOILHUMI A6
#define PUMP 16

uint32_t DataCaptureDelay = 3000;     // 3000ms 간격 설정
uint32_t DataCapture_ST = 0;

//변수 선언//
float Soilhumi; 

//쎄-텁 팡션//
void setup() {
  u8g2.begin();
  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
  
  DataCapture_ST = millis();
}

//뤂 팡션//
void loop() {
  //*****3000ms 간격으로 Temp, Humi 측정*********//
  if ((millis() - DataCapture_ST) > DataCaptureDelay) {

  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  OLEDdraw();
  DataCapture_ST = millis();
  }
  //토양 습도가 30 이하 일때 펌프 가동
  if (Soilhumi <= 30) {
    digitalWrite(PUMP, HIGH);
  }
  //토양 습도가 60 이상 일때 펌프 중지
  if (Soilhumi >= 60) {
    digitalWrite(PUMP, LOW);
  }

}

void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 15, "AIBD AIOT");

  //*******Soilhumidity 표시********//
  u8g2.drawStr(1, 36, "Solhumidity");
  u8g2.setCursor(85, 36); u8g2.print(Soilhumi);
  u8g2.drawStr(116, 36, "%");

  u8g2.sendBuffer();
}