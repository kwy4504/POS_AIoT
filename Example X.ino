#include <DHT.h>
#include <AsyncTimer.h>
#include <Vegemite.h>
#include <SoftPWM.h>

auto SOILMOIST_PIN = A6;
auto DHT22_PIN = A1;
auto FAN_PIN = A3;
auto PUMP_PIN = 16;
auto LAMP_PIN = 17;

SOFTPWM_DEFINE_CHANNEL(FAN_PIN);
DHT dht(DHT22_PIN, DHT22);
AsyncTimer t;
Vegemite v;

bool currentPumpWorking = false;

void setup(){
  Serial.begin(250000);
  SoftPWM.begin(490);
  dht.begin();

  pinMode(SOILMOIST_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);

  v.requestSubscription("config-auto");
  v.requestSubscription("config-light");
  v.requestSubscription("config-fan");
  v.requestSubscription("pump-water");

  t.setInterval([](){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    if(!isnan(humidity) && !isnan(temperature)){
      v.send("temperature", temperature);
      v.send("humidity", humidity);
    }

    int soilMoist = map(analogRead(SOILMOIST_PIN), 0, 1023, 100, 0);
    v.send("soilmoist", soilMoist);
  }, 1000);


  t.setInterval([](){
///////////////////////////// Auto Mode OFF 일때 AutoConf == 0 ///////////////////////////////
    int AutoConf = int(v.recv("config-auto"));
    if (AutoConf == 0) {
      int pumpWater = int(v.recv("pump-water"));
      int lightConf = int(v.recv("config-light"));
      int fanSpeed = int(v.recv("config-fan"));

      if(pumpWater ==1 && !currentPumpWorking){
        currentPumpWorking = true;
        v.send("pump-water", 0);
        digitalWrite(PUMP_PIN, HIGH);
        t.setTimeout([](){
          digitalWrite(PUMP_PIN, LOW);
          currentPumpWorking = false;
        },5000); 
      }
    

//////////////////////////////// fanSpeed 조절 part ////////////////////////////////////
    if(fanSpeed == 1) {SoftPWM.set(65);}
    if(fanSpeed == 2) {SoftPWM.set(100);} 
    if(fanSpeed == 0) {SoftPWM.set(0);}

    digitalWrite(LAMP_PIN, lightConf ==1 ? HIGH: LOW);
  }

///////////////////////////// Auto Mode ON 일때 AutoConf == 1 ///////////////////////////////
  if (AutoConf == 1) {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if(!isnan(humidity) && !isnan(temperature)){
      v.send("temperature", temperature);
      v.send("humidity", humidity);
    }
    int soilMoist = map(analogRead(SOILMOIST_PIN), 0, 1023, 100, 0);
    v.send("soilmoist", soilMoist);
    
    if(temperature < 25) {
      digitalWrite(LAMP_PIN, HIGH);
    }
    if(temperature >= 25) {
      digitalWrite(LAMP_PIN, LOW);
    }
    if(humidity < 60) {
      digitalWrite(PUMP_PIN, HIGH);
    }
    if(humidity >= 60) {
      digitalWrite(PUMP_PIN, LOW);
    }
    if(humidity < 75) {
      SoftPWM.set(0);
    }
    if(humidity >= 75) {
      SoftPWM.set(100);
    }
  }

  }, 500);
}

void loop(){
  v.subscribe();
  t.handle();
}

