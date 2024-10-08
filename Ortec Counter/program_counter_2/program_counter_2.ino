#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


#define SERVER_PORT 8000
const char* ssid = "WIFI@MODAL DONG;
const char* password = "JanganRemasBijiKu";


AsyncWebServer server(80);
int i;
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {27, 0, false};
int AA;
int BB;
int CC;
int DD;
uint32_t EE;
uint32_t Hasil;
String Hasil2;
char index_html;
String Hasil3;

void IRAM_ATTR isr() {
  button1.numberKeyPresses += 1;
  button1.pressed = true;
}
void IRAM_ATTR isr2() {
  AA = 0;
  BB = 0;
  CC = 0;
  DD = 0;
  EE = 0;
  button1.numberKeyPresses = 0;
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  { Serial.print("->");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi Successfully Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("NodeMCU as a Server Role Started");
  for (i = 0; i < 25; i++)
  { digitalWrite(2, HIGH);
    delay(40);
    digitalWrite(2, LOW);
    delay(40);
  }
  pinMode(26, INPUT);
  pinMode(25, INPUT);
  pinMode(33, INPUT);
  pinMode(32, INPUT);
  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
  attachInterrupt(34, isr2, FALLING);
  server.on("/cacah", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", Hasil2);
  });
}
 

void loop() {
  if (button1.pressed) {
    button1.pressed = false;
  }
  int A = digitalRead(26);
  int B = digitalRead(25);
  int C = digitalRead(33);
  int D = digitalRead(32);

  if (A == HIGH) {
    AA = 1;
  } else {
    AA = 0;
  }
  if (B == HIGH) {
    BB = 2;
  } else {
    BB = 0;
  }
  if (C == HIGH) {
    CC = 4;
  } else {
    CC = 0;
  }
  if (D == HIGH) {
    DD = 8;
  } else {
    DD = 0;
  }
  EE = button1.numberKeyPresses * 10;
  Hasil = AA + BB + CC + DD + EE;
  Hasil2 = String(Hasil);
  
  }
