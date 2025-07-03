#include <SoftwareSerial.h>
SoftwareSerial FixSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  FixSerial.begin(9600);
  delay(1000);
  Serial.println("📡 A2 Hazır - ACK alıcısı");
}

void loop() {
  if (FixSerial.available()) {
    String gelen = FixSerial.readStringUntil('\n');
    Serial.println("📥 A2 → Gelen mesaj: " + gelen);
  }
}
