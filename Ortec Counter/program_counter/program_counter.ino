#include <WiFi.h>
#define SERVER_PORT 8000
const char* ssid = "WIFI@ITERA";
const char* password = "JanganRemasBijiKu";
WiFiServer server(SERVER_PORT);
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
}

void loop() {
  if (button1.pressed) {
    button1.pressed = false;
  }
  //Detach Interrupt after 1 Minute
  //static uint32_t lastMillis = 0;
  //if (millis() - lastMillis > 3000) {
  //; lastMillis = millis();
  //  detachInterrupt(button1.PIN);
  //Serial.println("Interrupt Detached!");
  //Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);

  WiFiClient client = server.available();
  if (client)
  { Serial.println("Hi...New Client");
    for (i = 0; i < 2; i++)
    { digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
      delay(50);
    }
    while (1)
    { while (client.available())
      { digitalWrite(2, HIGH);
        delay(20);
        digitalWrite(2, LOW);
        delay(20);
        uint8_t data = client.read();
        Serial.write(data);
        switch (data)
        { case 'a':
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
            Serial.println(Hasil);
            client.println(Hasil);
            break;
        }
        if (server.hasClient())
        {
          return;
        }
      }
    }

  }
}
