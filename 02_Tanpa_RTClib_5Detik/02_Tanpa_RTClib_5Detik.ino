/*
  SISTEM PENYIRAMAN OTOMATIS TANPA RTClib
  Durasi penyiraman: 5 detik
  Jadwal: menggunakan millis() Arduino internal
  
  Hardware:
  - Arduino Nano
  - Relay Module 5V (1 channel)
  - Pompa Diafragma 5V
  - Nozzle 0.8mm
  - Katup 2 arah 8mm (opsional)
  
  CATATAN:
  Sistem ini menggunakan timer internal Arduino (millis())
  Cocok untuk sistem yang selalu ON (tidak dimatikan)
  Jika power mati, timer akan reset
  
  Jadwal penyiraman setiap 6 jam:
  - Durasi: 5 detik
  - Interval: 6 jam (21600000 milliseconds)
*/

const int relayPump = 2;   // Pin relay pompa (D2)

unsigned long intervalMenyiram = 6 * 60 * 60 * 1000;  // 6 jam dalam milliseconds
unsigned long durasiMenyiram = 5 * 1000;              // 5 detik dalam milliseconds
unsigned long waktuTerakhirMenyiram = 0;
bool pompaNyala = false;

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println("=================================");
  Serial.println("SISTEM PENYIRAMAN OTOMATIS");
  Serial.println("Tanpa RTC - Durasi 5 detik");
  Serial.println("=================================");
  Serial.println();

  pinMode(relayPump, OUTPUT);
  digitalWrite(relayPump, HIGH);  // Relay off (aktif LOW)

  Serial.println("Relay siap di pin D2");
  Serial.println();
  Serial.println("Konfigurasi:");
  Serial.print("  Interval penyiraman: ");
  Serial.print(intervalMenyiram / 1000 / 60 / 60);
  Serial.println(" jam");
  Serial.print("  Durasi penyiraman: ");
  Serial.print(durasiMenyiram / 1000);
  Serial.println(" detik");
  Serial.println();

  waktuTerakhirMenyiram = millis();

  Serial.println("=== SISTEM SIAP ===");
  Serial.println();
  Serial.println("Sistem akan mulai menyiram dalam 6 jam pertama...");
  Serial.println();
}

void nyalakanPompa() {
  digitalWrite(relayPump, LOW);  // Relay aktif LOW
  pompaNyala = true;
  
  unsigned long menit = (millis() / 1000) / 60;
  unsigned long detik = (millis() / 1000) % 60;
  unsigned long jam = menit / 60;
  menit = menit % 60;

  Serial.print("[");
  if (jam < 10) Serial.print("0");
  Serial.print(jam);
  Serial.print(":");
  if (menit < 10) Serial.print("0");
  Serial.print(menit);
  Serial.print(":");
  if (detik < 10) Serial.print("0");
  Serial.print(detik);
  Serial.println("] POMPA ON - 5 detik");
}

void matikanPompa() {
  digitalWrite(relayPump, HIGH);  // Relay mati
  pompaNyala = false;
  waktuTerakhirMenyiram = millis();  // Reset timer

  unsigned long menit = (millis() / 1000) / 60;
  unsigned long detik = (millis() / 1000) % 60;
  unsigned long jam = menit / 60;
  menit = menit % 60;

  Serial.print("[");
  if (jam < 10) Serial.print("0");
  Serial.print(jam);
  Serial.print(":");
  if (menit < 10) Serial.print("0");
  Serial.print(menit);
  Serial.print(":");
  if (detik < 10) Serial.print("0");
  Serial.print(detik);
  Serial.println("] POMPA OFF");
}

void tampilkanStatus() {
  unsigned long totalMenitSejakPowerOn = millis() / 1000 / 60;
  unsigned long menitSekarang = totalMenitSejakPowerOn % 60;
  unsigned long jamSekarang = totalMenitSejakPowerOn / 60;

  Serial.print("Waktu: ");
  if (jamSekarang < 10) Serial.print("0");
  Serial.print(jamSekarang);
  Serial.print(":");
  if (menitSekarang < 10) Serial.print("0");
  Serial.print(menitSekarang);

  Serial.print(" | Status: ");
  if (pompaNyala) {
    Serial.println("POMPA MENYIRAM");
  } else {
    Serial.println("POMPA OFF");
  }
}

void loop() {
  unsigned long now = millis();

  // Jika pompa sedang menyiram
  if (pompaNyala) {
    unsigned long elapsedSinceStart = now - waktuTerakhirMenyiram;
    
    // Jika durasi sudah habis (5 detik), matikan pompa
    if (elapsedSinceStart >= durasiMenyiram) {
      matikanPompa();
    }
  }
  // Jika pompa tidak menyiram
  else {
    unsigned long elapsedSinceLastWatering = now - waktuTerakhirMenyiram;
    
    // Jika interval sudah terpenuhi (6 jam), nyalakan pompa
    if (elapsedSinceLastWatering >= intervalMenyiram) {
      nyalakanPompa();
    }
  }

  // Tampilkan status setiap 30 detik
  static unsigned long waktuTampilTerakhir = 0;
  if (millis() - waktuTampilTerakhir >= 30000) {
    tampilkanStatus();
    waktuTampilTerakhir = millis();
  }

  delay(1000);
}
