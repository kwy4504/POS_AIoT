#include <DHT.h>
#include <AsyncTimer.h>
#include <Vegemite.h>

DHT dht(A1, DHT22);
AsyncTimer t;       //시간 간격 설정을 위한 라이브러리 정의 t
Vegemite v;         //WIFI 링크를 사용하여 브로커와 통신

void setup() {
  Serial.begin(250000);

  dht.begin();

  // 2000ms 간격으로 온도, 습도 저장 및 브로커로 통신
  t.setInterval([](){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (!isnan(humidity) && !isnan(temperature)) {
      v.send("temperature", temperature);
      v.send("humidity", humidity);
    }

  }, 2000);
}

void loop() {
  t.handle();
}