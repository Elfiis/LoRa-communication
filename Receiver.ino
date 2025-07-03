// —— Alıcı (Receiver) ——  
#include <SoftwareSerial.h>
SoftwareSerial FixSerial(10, 11); // LoRa RX = 10, TX = 11

#define M0_PIN 6
#define M1_PIN 7

// Ekranda okuduğun ve gönderici (sender) modüle atadığın adres:
#define SENDER_ADDR_HIGH 0x00  // ADDH
#define SENDER_ADDR_LOW  0x02  // ADDL
#define CHANNEL          0x12  // 18 decimal → 0x12 hex

// Aynı checksum fonksiyonu
unsigned int hesaplaChecksum(const String &data) {
  unsigned int sum = 0;
  for (size_t i = 0; i < data.length(); i++) sum += data[i];
  return sum;
}

void setup() {
  Serial.begin(115200);

  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);
  // Fixed receive modu
  digitalWrite(M0_PIN, HIGH);
  digitalWrite(M1_PIN, LOW);
  delay(150);               // <<< mod geçişini garantiye al

  FixSerial.begin(9600);
  delay(150);               // <<< buffer’da biriken “çöp” verileri atmak için

  // Gelen ilk saçma verileri at
  while (FixSerial.available()) FixSerial.read();

  Serial.println("📡 Alıcı Başladı. Veri bekleniyor...");
}

void loop() {
  if (!FixSerial.available()) return;

  // 1) Gelen JSON’u oku
  String payload = FixSerial.readStringUntil('\n');
  payload.trim();
  Serial.print("📥 Gelen payload: "); Serial.println(payload);

  // 2) “message” içeriğini ayrıştır
  const String keyMsg = "\"message\":\"";
  int mStart = payload.indexOf(keyMsg);
  if (mStart < 0) return;
  mStart += keyMsg.length();
  int mEnd = payload.indexOf("\"", mStart);
  String msg = payload.substring(mStart, mEnd);

  // 3) Gönderilen checksum’u çek
  const String keyCs = "\"checksum\":";
  int cStart = payload.indexOf(keyCs);
  if (cStart < 0) return;
  cStart += keyCs.length();
  int cEnd = payload.indexOf("}", cStart);
  unsigned int recvCs = payload.substring(cStart, cEnd).toInt();

  // 4) Hesapla ve kontrol et
  unsigned int calcCs = hesaplaChecksum(msg);
  bool ok = (recvCs == calcCs);
  Serial.println(ok ? "✅ Mesaj bütün." : "❌ Mesaj bozuk!");

  // 5) Geri bildirim: header + “alındı” veya “yanlış”
  FixSerial.write((byte)SENDER_ADDR_HIGH);
  FixSerial.write((byte)SENDER_ADDR_LOW);
  FixSerial.write((byte)CHANNEL);
  if (ok) {
    FixSerial.println("alındı");
    Serial.println("📣 'alındı' yollandı.");
  } else {
    FixSerial.println("yanlış");
    Serial.println("📣 'yanlış' yollandı.");
  }
}
