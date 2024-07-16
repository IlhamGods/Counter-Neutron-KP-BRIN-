#include <Wire.h>
#include <RTClib.h>

const int clearPin = 17;  // Pin untuk mereset counter 74HC590 V
const int rckPin = 16;    // Pin untuk mengunci data ke register 74HC590 V
const int sckPin = 18;    // Pin untuk menggeser data 74HC590

const int qPins[8] = {32, 33, 25, 26, 27, 14, 12, 13};  // Q0 hingga Q7

RTC_DS3231 rtc;

char t[32];

void setup() {
  Serial.begin(115200);

  pinMode(clearPin, OUTPUT);
  pinMode(rckPin, OUTPUT);
  pinMode(sckPin, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(qPins[i], INPUT);
  }

  digitalWrite(clearPin, LOW);  // Reset counter 74HC590
  delay(10);
  digitalWrite(clearPin, HIGH);

  Wire.begin(21, 22);  // Inisialisasi I2C pada GPIO21 (SDA) dan GPIO22 (SCL) untuk ESP32

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(2024, 7, 16, 11, 0, 0));  // Set waktu awal jika RTC kehilangan daya
  }
}

void loop() {
  digitalWrite(rckPin, LOW);  // Latch the count into the register
  delay(1);  // Short delay
  digitalWrite(rckPin, HIGH);

  // Read the count from the register
  int count = 0;
  for (int i = 0; i < 8; i++) {
    count |= (digitalRead(qPins[i]) << i);
  }

  DateTime now = rtc.now();  // Mendapatkan waktu dari RTC

  Serial.print("Count: ");
  Serial.print(count);
  Serial.print(" | Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

  delay(1000);  // Menunggu 1 detik sebelum membaca lagi
}
