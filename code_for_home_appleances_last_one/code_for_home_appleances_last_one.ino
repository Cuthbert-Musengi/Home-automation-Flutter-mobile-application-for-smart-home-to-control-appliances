#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "farihome-79556-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "z0AxHleb5x4evWL22wFKEfjeKjzPZn4eJ9BM7xZ5"
#define WIFI_SSID "netstar"
#define WIFI_PASSWORD "00000000"

void setup() { 
  Serial.begin(9600);
  // Initialize the output variables as outputs
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

   // Set outputs relay active LOW
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

bool fan, micro, washen,  coffee;
void loop(){
  
  coffee = Firebase.getBool("Coffee Machine/Switch");
  micro = Firebase.getBool("Washing Machine/Switch");
  fan = Firebase.getBool("Gyser/Switch");
  washen = Firebase.getBool("Cooling Fan/Switch");

  Serial.println("Coffee Machine");
  if(coffee==true){
    digitalWrite(5,HIGH);
  }
  if(coffee==false){
    digitalWrite(5,LOW);
  }

  Serial.println("Washing Machine");
  if(micro==true){
    digitalWrite(4,HIGH);
  }
  if(micro==false){
    digitalWrite(4,LOW);
  }

  Serial.println("Gyser");
  if(washen==true){
    digitalWrite(0,HIGH);
  }
  if(washen==false){
    digitalWrite(0,LOW);
  }

  Serial.println("Cooling Fan");
  if(fan==true){
    digitalWrite(2,HIGH);
  }
  if(fan==false){
    digitalWrite(2,LOW);
  }
}
