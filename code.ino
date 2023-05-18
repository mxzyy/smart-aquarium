#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>

#define ServoPin 12 // D6
#define DS18Pin 2 // D4
#define RelayPin 14 // D5
#define Switch1Pin 13 // D7 Servo
#define Switch2Pin 15 // D8 Relay
#define ServoIn 16 // D0

BlynkTimer timer;
Servo MG90s;
OneWire oneWire(DS18Pin);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "fE-5JokC8_4jQufTxFZp3xiwk7RzlecC";
char ssid[] = "andaikan";
char pass[] = "12345678";

void sendTemp(){
  sensors.requestTemperatures();
  int t = sensors.getTempCByIndex(0);
  Blynk.virtualWrite(V0, t);  
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(RelayIn, INPUT);
  //pinMode(ServoIn, INPUT);
  pinMode(RelayPin, OUTPUT);  
  pinMode(Switch1Pin, OUTPUT);
  pinMode(Switch2Pin, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  MG90s.attach(ServoPin);
  MG90s.write(0);
  Blynk.begin(auth, ssid, pass, IPAddress(41, 216, 186, 178), 8080);
  timer.setInterval(1000L, sendTemp);
  lcd.setCursor(0,0);
  lcd.print("SMART");
  lcd.setCursor(0,1);
  lcd.print("AQUARIUM");
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
  sensors.requestTemperatures();
  int t = sensors.getTempCByIndex(0);
  Serial.print("Suhu : ");
  Serial.print(t);
  if (t <= 28){
    pinMode(RelayPin, OUTPUT);
    digitalWrite(RelayPin, HIGH);
  }else{
    pinMode(RelayPin, INPUT);
    digitalWrite(RelayPin, LOW);
  }

  lcd.setCursor(0, 0);
  lcd.print("SMART AQUARIUM");
  lcd.setCursor(0, 1);
  lcd.print("SUHU : ");
  lcd.print(t);
  lcd.print(" C");
  
  
  Blynk.run();
  timer.run();
}
