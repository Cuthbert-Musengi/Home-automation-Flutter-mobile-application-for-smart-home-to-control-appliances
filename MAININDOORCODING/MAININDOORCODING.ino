#include <Blynk.h>
#include <DNSServer.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <DHT_U.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

void gassensor();


#define Buzzer 10
#define MQ2 A0
#define flame D0
#define trig D5
#define echo D6
#define PIR D3
#define relay1 D7
#define relay2 D8
#define BLYNK_PRINT Serial

LiquidCrystal_I2C lcd(0*27, 16, 2);

char auth[] = "qL9kBaEzLYCp84h6NvOsbnEokaaNLduA";
char ssid[] = "netstar";
char pass[] = "00000000";

DHT dht(D4, DHT11);
BlynkTimer timer;
bool pirbutton = 0;

 BLYNK_WRITE(V0) {
  pirbutton = param.asInt();
 }

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(Buzzer, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(flame, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  timer.setInterval(100L, gassensor);
  timer.setInterval(100L, flamesensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L,pirsensor);
  timer.setInterval(100L, ultrasonic);
}

void gassensor(){
  int value = analogRead(MQ2);
  Serial.println(value);
  value = map(value, 0, 1024, 0, 100);
  if(value <= 55){
    digitalWrite(Buzzer, LOW);
  }else if(value > 55{
    Blynk.notify("Warning! Gas Leak detected!!!");
    digitalWrite(Buzzer, HIGH);
  }
  Blynk.virtualWrite(V1, value);
  lcd.setCursor(0, 0);
  lcd.print("G:");
  lcd.print(" ");
  lcd.print(value);
}

void DHT11sensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if(isnan(h) || isnan(t)){
    Serial.println("Failed to read from DHT Sensor!");
    return;
  }
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, h);

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(t);
  
  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(h);
}

void pirsensor(){
  bool value = digitalRead(PIR);
  if(pirbutton == 1){
    if(value == 0){
      digitalWrite(Buzzer, LOW);  
    }else if(value == 1){
      Blynk.notify("Warning! Please check your ouside security");
      digitalWrite(Buzzer, HIGH);
    }
  }
}

void ultrasonic(){
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;
  Blynk.virtualWrite(V4, cm);
  lcd.setCursor(9, 1);
  lcd.print("W");
  lcd.print(cm);
  lcd.print(" ");
}

BLYNK_WRITE(V5){
  bool RelayOne = param.asInt();
  if(RelayOne == 1){
    digitalWrite(relay1, LOW);
  }else{
    digitalWrite(relay1, HIGH);
  }
}

BLYNK_WRITE(V6){
  bool RelayTwo = param.asInt();
  if(RelayTwo == 1){
    digitalWrite(relay2, LOW);
  }else{
    digitalWrite(relay2, HIGH);
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
