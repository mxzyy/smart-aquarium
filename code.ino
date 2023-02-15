#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>

#define ServoPin 12 // D6
#define DS18Pin 2 //D4

BlynkTimer timer;
Servo MG90s;
OneWire oneWire(DS18Pin);
DallasTemperature sensors(&oneWire);

char auth[] = "fE-5JokC8_4jQufTxFZp3xiwk7RzlecC";
char ssid[] = "OPPO A15";
char pass[] = "pecellele";

void sendTemp(){
  sensors.requestTemperatures();
  int t = sensors.getTempFByIndex(0);
  Blynk.virtualWrite(V0, t);  
}

void setup() {
  // put your setup code here, to run once:
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



void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}
