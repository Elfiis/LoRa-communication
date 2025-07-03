// —— Verici (Transmitter) ——  
#include <SoftwareSerial.h>
SoftwareSerial FixSerial(10, 11); // LoRa RX = 10, TX = 11

#define M0 6
#define M1 7

unsigned long const TIMEOUT = 10000; // ms
bool confirmed = false;

// ASCII toplamı checksum fonksiyonu
unsigned int hesaplaChecksum(const String &data) {
  unsigned int sum = 0;
  for (size_t i = 0; i < data.length(); i++) sum += data[i];
  return sum;
}

void setup() {
  Serial.begin(115200);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  // Normal (transparent) mod
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  delay(150);               // <<< mod geçişini garantiye al

  FixSerial.begin(9600);
  delay(150);               // <<< karşı tarafta FixSerial.begin() bitsin diye bekle

  // Gelen ilk saçma verileri at
  while (FixSerial.available()) FixSerial.read();

  Serial.println("🚀 Gönderici Başladı.");
}

void loop() {
  if (confirmed) return;  // Onaylandıysa artık gönderme

  // 1) Mesaj + checksum + JSON hazırla
  String mesaj = "elif";            
  const uint8_t senderID = 1;       
  unsigned int cs = hesaplaChecksum(mesaj);
  String json = "{\"message\":\"" + mesaj +
                "\",\"senderID\":" + String(senderID) +
                ",\"checksum\":" + String(cs) + "}";

  // 2) Başlık + JSON gönder
  FixSerial.write((byte)0x00);
  FixSerial.write((byte)0x01);
  FixSerial.write((byte)0x12);
  FixSerial.println(json);
  Serial.print("📤 Gönderilen JSON: "); Serial.println(json);

  // 3) “alındı” veya “yanlış” bekle
  unsigned long start = millis();
  bool gotResp = false;
  while (millis() - start < TIMEOUT) {
    if (FixSerial.available()) {
      String resp = FixSerial.readStringUntil('\n');
      resp.trim();
      if (resp == "alındı") {
        Serial.println("✅ Alıcı’dan ALINDI geldi. Gönderim durduruldu.");
        confirmed = true;
        gotResp = true;
        break;
      }
      else if (resp == "yanlış") {
        Serial.println("⚠️ Alıcı’dan YANLIŞ geldi. Yeniden gönderiliyor...");
        gotResp = true;
        break;
      }
    }
  }
  if (!gotResp) {
    Serial.println("⏱️ Geri bildirim gelmedi, yeniden denenecek...");
  }

  delay(1000);
}