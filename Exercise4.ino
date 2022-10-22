///////////////////청년 AI 빅데이터 아카데미 C4조 김우영, 전현지/////////////////////
/////////////////////////////AIOT_Exercise 4//////////////////////////////////////
/*************업로딩 에러시 터미널 열고 sudo chmod a+rw /dev/ttyUSB0***************/

//전처리문//
#include <U8g2lib.h>
#include <SoftPWM.h>
#include "DHT.h"
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN A1
#define DHTTYPE DHT22
SOFTPWM_DEFINE_CHANNEL(A3);
#define SOILHUMI A6
#define PUMP 16
#define LAMP 17

DHT dht(DHTPIN, DHTTYPE);

uint32_t DataCaptureDelay1 = 2000;
uint32_t DataCaptureDelay2 = 3000;
uint32_t DataCapture_ST1 = 0;
uint32_t DataCapture_ST2 = 0;

uint32_t TimeCompare;
uint32_t LED_StartTime = 0;

int Hour = 0;
int Minute = 0;
int Second = 10;
uint32_t TimeSum = (uint32_t) ((Hour * 60 + Minute) * 60 + Second) * 1000;

//변수 선언//
float Temp;     // 온도
float Humi;     // 습도
float Soilhumi; // 토양 습도

char* Fan;        // FAN 동작 상태
char* Pump;        // PUMP 동작 상태
char* Led;        // LED 동작 상태

//쎄-텁 팡션//
void setup() {
  dht.begin();
  u8g2.begin();
  SoftPWM.begin(490);
  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LAMP, OUTPUT);

  LED_StartTime = millis();

  DataCapture_ST1 = millis();
  DataCapture_ST2 = millis();
}

//뤂 팡션//
void loop() {

  ///////////////                온/습도 2초                  //////////////
  if ((millis() - DataCapture_ST1) > DataCaptureDelay1) {
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();

  if (isnan(Humi) || isnan(Temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  ///////////// 29 <= 온도 일때 PWM 100 ON
  if ( 29 <= Temp) {
    Fan = "ON";
    SoftPWM.set(100);
  }
  ///////////// 20 < 온도 < 29 일때 PWM 65 ON
  else if (( 20 < Temp ) && ( Temp < 29 )) {
    Fan = "ON";
    SoftPWM.set(65);
  }
  //////////// 온도 <= 20 일때 PWM 0 OFF
  else {
    Fan = "OFF";
    SoftPWM.set(0);
  }
  OLEDdraw();
  DataCapture_ST1 = millis();
  }

  ////////////////         토양 습도 3초마다 측정           ///////////////////////////
  if ((millis() - DataCapture_ST2) > DataCaptureDelay2) {
    Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);

  if (isnan(Soilhumi)) {
    Serial.println(F("Failed to read from Soilhumi sensor!"));
    return;
  }
  
  ///////          토양 습도가 30~60 일때 펌프 가동, 이외 중지          ///////////////
  if ((30 <= Soilhumi) && (Soilhumi <= 60)) {
    digitalWrite(PUMP, HIGH);
    Pump = "ON";
  } else {
    digitalWrite(PUMP, LOW);
    Pump = "OFF";
  }
  
  OLEDdraw();
  DataCapture_ST2 = millis();
  }

  ////////////            LED 자동 ON, OFF 가동 주기 10초           //////////////////
  TimeCompare = (millis() - LED_StartTime) / TimeSum;
  if (TimeCompare % 2 == 0) {
    digitalWrite(LAMP, HIGH);
    Led = "ON";
  } else {
    digitalWrite(LAMP, LOW);
    Led = "OFF";
  }
}

void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);

  //*******Temp, Humidity, Soilhumidity 표시*********//
  u8g2.drawStr(4, 12, "Temp | Hum | S.Hum");
  u8g2.setCursor(1, 28);
  u8g2.print(Temp);
  u8g2.drawStr(28, 28, "\xb0");
  u8g2.drawStr(32, 28, "C");
  u8g2.setCursor(47, 28);
  u8g2.print(Humi);
  u8g2.drawStr(67, 28, "%");
  u8g2.setCursor(87, 28);
  u8g2.print(Soilhumi);
  u8g2.drawStr(107, 28, "%");

  u8g2.drawStr(1, 40, "-------------------------------------");
  //*******FAN, PUMP, LED(시간 간격) 표시*********//
  u8g2.drawStr(4, 51, "FAN | PUMP | LED(s)");
  u8g2.setCursor(5, 63);
  u8g2.print(Fan);
  u8g2.setCursor(45, 63);
  u8g2.print(Pump);
  u8g2.setCursor(85, 63);
  u8g2.print(Led);
  u8g2.setCursor(115, 63);
  u8g2.print(TimeSum / 1000);

  u8g2.sendBuffer();
}