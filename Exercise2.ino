///////////////////청년 AI 빅데이터 아카데미 C4조 김우영, 전현지/////////////////////
/////////////////////////////AIOT_Exercise 2//////////////////////////////////////
/*************업로딩 에러시 터미널 열고 sudo chmod a+rw /dev/ttyUSB0***************/

//전처리문//
#include <U8g2lib.h>      // OLED 모듈
#include <SoftPWM.h>
#include "DHT.h"          // DHT22
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN A1
#define DHTTYPE DHT22
SOFTPWM_DEFINE_CHANNEL(A3);

DHT dht(DHTPIN, DHTTYPE);

uint32_t DataCaptureDelay = 3000;     // 3000ms 간격 설정
uint32_t DataCapture_ST = 0;

//변수 선언//
float Temp;     // 온도
float Humi;     // 습도
int PWM;        // 모터 수치

//쎄-텁 팡션//
void setup() {
  dht.begin();
  u8g2.begin();
  SoftPWM.begin(490);

  DataCapture_ST = millis();
}

//뤂 팡션//
void loop() {
  if ((millis() - DataCapture_ST) > DataCaptureDelay) {
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();

  if (isnan(Humi) || isnan(Temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  OLEDdraw();
  DataCapture_ST = millis();
  }
  // 25 < 온도 < 30 일때 PWM 65
  if ( (25.0 < Temp) && (Temp < 30.0)) {
    PWM = 65;
    SoftPWM.set(PWM);
  }
  // 30 <= 온도 일때 PWM 100  
  else if ( 30.0 <= Temp) {
    PWM = 100;
    SoftPWM.set(PWM);
  }
  // 온도 <= 25 일때 PWM 0
  else {
    PWM = 0;
    SoftPWM.set(PWM);
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

  //*******PWM 표시********//
  u8g2.drawStr(15, 58, "PMW");
  u8g2.setCursor(100, 58); u8g2.print(PWM);

  u8g2.sendBuffer();
}