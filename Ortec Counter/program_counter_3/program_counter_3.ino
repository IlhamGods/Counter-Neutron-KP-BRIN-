#include <SPI.h>
#include <Ethernet2.h>
#include <Wire.h>
#include <RTClib.h>

// Konfigurasi untuk modul W5500
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetClient client;

const char* telegramBotToken = "YOUR_TELEGRAM_BOT_TOKEN";
const char* chatId = "YOUR_CHAT_ID";

// Pin konfigurasi
const int clearPin = 17;  // Pin untuk mereset counter 74HC590
const int rckPin = 16;    // Pin untuk mengunci data ke register 74HC590
const int sckPin = 18;    // Pin untuk menggeser data 74HC590
const int qPins[8] = {32, 33, 25, 26, 27, 14, 12, 13};  // Q0 hingga Q7

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  // Mengatur pin untuk 74HC590
  pinMode(clearPin, OUTPUT);
  pinMode(rckPin, OUTPUT);3
  pinMode(sckPin, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(qPins[i], INPUT);
  }

  // Reset counter 74HC590
  digitalWrite(clearPin, LOW);
  delay(10);
  digitalWrite(clearPin, HIGH);

  // Inisialisasi I2C untuk RTC DS3231
  Wire.begin(21, 22);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Inisialisasi Ethernet
  Ethernet.begin(mac, ip);

  // Memberi waktu untuk menginisialisasi Ethernet
  delay(1000);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found. Sorry, can't run without hardware.");
    while (true) {
      delay(1);
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
}

void loop() {
  digitalWrite(rckPin, LOW);  // Latch the count into the register
  delay(1);  // Short delay
  digitalWrite(rckPin, HIGH);

  // Membaca hitungan dari register
  int count = 0;
  for (int i = 0; i < 8; i++) {
    count |= (digitalRead(qPins[i]) << i);
  }

  DateTime now = rtc.now();  // Mendapatkan waktu dari RTC

  char message[200];
  sprintf(message, "Count: %d | Time: %02d:%02d:%02d", count, now.hour(), now.minute(), now.second());
  Serial.println(message);

  sendTelegramMessage(message);

  delay(60000);  // Mengirim data setiap 1 menit
}

void sendTelegramMessage(const char* message) {
  if (client.connect("api.telegram.org", 80)) {
    String url = "/bot";
    url += telegramBotToken;
    url += "/sendMessage?chat_id=";
    url += chatId;
    url += "&text=";
    url += message;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.telegram.org\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    // Baca semua data dari server
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  } else {
    Serial.println("Connection to Telegram failed.");
  }

  client.stop();
}
