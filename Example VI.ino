#include <DHT.h>
#include <Vegemite.h>

DHT dht(A1, DHT22);
Vegemite v;         //WIFI 링크를 사용하여 브로커와 통신

uint32_t StartTime;
uint32_t DataCaptureDelay;

uint32_t getStartTime() {
  return millis() + DataCaptureDelay;
}

void setup() {
  Serial.begin(250000);

  dht.begin();

  DataCaptureDelay = 2000;
  StartTime = getStartTime();
}

// 2000ms 간격으로 온도, 습도 저장 및 브로커로 통신
void loop() {
  if (millis() > StartTime) {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (!isnan(humidity) && !isnan(temperature)) {
      v.send("temperature", temperature);
      v.send("humidity", humidity);
    }
    StartTime = getStartTime();
  }
}