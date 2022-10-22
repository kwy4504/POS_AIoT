#define PUMP 16

void setup() {
  // put your setup code here, to run once:
  pinMode(PUMP, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PUMP, HIGH);
  delay(2000);
  digitalWrite(PUMP, LOW);
  delay(2000);
}
