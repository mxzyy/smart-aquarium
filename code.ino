#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>

#define ServoPin 12 // D6
#define DS18Pin 2 // D4
#define RelayPin 14 // D5
#define RelayIn 15 // D8
#define ServoIn 16 // D0

BlynkTimer timer;
Servo MG90s;
OneWire oneWire(DS18Pin);
DallasTemperature sensors(&oneWire);

char auth[] = "fE-5JokC8_4jQufTxFZp3xiwk7RzlecC";
char ssid[] = "OPPO A15";
char pass[] = "pecellele";

void sendTemp(){
  sensors.requestTemperatures();
  int t = sensors.getTempCByIndex(0);
  Blynk.virtualWrite(V0, t);  
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(RelayIn, INPUT);
  //pinMode(ServoIn, INPUT);
  //pinMode(RelayPin, OUTPUT);  
  Serial.begin(96000);
  MG90s.attach(ServoPin);
  MG90s.write(0);
  Blynk.begin(auth, ssid, pass, IPAddress(41, 216, 186, 178), 8080);
  timer.setInterval(1000L, sendTemp);
}

BLYNK_WRITE(V1){
  int pinValue = param.asInt();

  if (pinValue == 1){
    MG90s.write(180);
    delay(800);
    Blynk.virtualWrite(V1, LOW);
    MG90s.write(0);
  }else{
    MG90s.write(0);
  }
}

BLYNK_WRITE(V2){
  int pinValue = param.asInt();

  if (pinValue == 1){
    pinMode(RelayPin, OUTPUT);
    digitalWrite(RelayPin, HIGH);
  }else{
    pinMode(RelayPin, INPUT);
    digitalWrite(RelayPin, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  bool switchStat = digitalRead(RelayIn);
  if (switchStat == LOW){
    digitalWrite(RelayPin, HIGH);
  }else{
    digitalWrite(RelayPin, LOW);
  }
  Blynk.run();
  timer.run();
}
