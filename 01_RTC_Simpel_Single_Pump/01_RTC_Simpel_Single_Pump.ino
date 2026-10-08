/*
  SISTEM PENYIRAMAN OTOMATIS DENGAN RTC DS3231
  Untuk tanaman tomat dan cabai
  
  Hardware:
  - Arduino Nano
  - RTC DS3231
  - Relay Module 5V (1 atau 2 channel)
  - Pompa Diafragma 5V
  - Nozzle 0.8mm
  - Katup 2 arah 8mm (opsional)
  
  Jadwal penyiraman otomatis setiap hari:
  - 06:30 - 3 menit (pagi)
  - 12:00 - 2 menit (siang)
  - 18:30 - 3 menit (sore)
*/

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const int relayPump = 2;   // Pin relay pompa (D2)

struct Jadwal {
  int jam;
  int menit;
  int durasiDetik;
};

// Jadwal penyiraman untuk tomat dan cabai
Jadwal jadwalPenyiram[] = {
  {6, 30, 180},    // 06:30 - 3 menit (pagi - pemberian air pagi)
  {12, 0, 120},    // 12:00 - 2 menit (siang - pemberian nutrisi/pestisida)
  {18, 30, 180}    // 18:30 - 3 menit (sore - pemberian air sore)
};

const int jumlahJadwal = 3;

bool sedangMenyiram = false;
int indeksJadwalAktif = -1;
unsigned long waktuMulaiSiram = 0;

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println("=== SISTEM PENYIRAMAN OTOMATIS ===");
  Serial.println("Arduino Nano + RTC DS3231");
  Serial.println();

  // Setup relay
  pinMode(relayPump, OUTPUT);
  digitalWrite(relayPump, HIGH);  // Relay off (aktif LOW)
  Serial.println("Relay siap di pin D2");
  Serial.println();

  // Setup I2C dan RTC
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("ERROR: RTC DS3231 tidak ditemukan!");
    Serial.println("Periksa koneksi I2C (SDA-A4, SCL-A5)");
    while (1) {
      delay(1000);
      Serial.println("Menunggu RTC...");
    }
  }

  Serial.println("RTC DS3231 terdeteksi");

  // Cek jika RTC belum pernah diset (kompilasi pertama kali)
  if (rtc.lostPower()) {
    Serial.println();
    Serial.println("PERINGATAN: RTC kehilangan daya!");
    Serial.println("Silakan set waktu menggunakan kode SetTime dibawah:");
    Serial.println();
    Serial.println("  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));");
    Serial.println();
    Serial.println("Uncomment baris di atas, upload, lalu comment kembali.");
    Serial.println();
  }

  // Tampilkan waktu RTC saat startup
  DateTime now = rtc.now();
  Serial.print("Waktu RTC sekarang: ");
  tampilkanWaktu(now);
  Serial.println();

  Serial.println("Jadwal penyiraman:");
  for (int i = 0; i < jumlahJadwal; i++) {
    Serial.print("  ");
    Serial.print(i + 1);
    Serial.print(". ");
    if (jadwalPenyiram[i].jam < 10) Serial.print("0");
    Serial.print(jadwalPenyiram[i].jam);
    Serial.print(":");
    if (jadwalPenyiram[i].menit < 10) Serial.print("0");
    Serial.print(jadwalPenyiram[i].menit);
    Serial.print(" - ");
    Serial.print(jadwalPenyiram[i].durasiDetik);
    Serial.println(" detik");
  }
  Serial.println();
  Serial.println("=== SISTEM SIAP ===");
  Serial.println();
}

void tampilkanWaktu(DateTime dt) {
  Serial.print(dt.day());
  Serial.print("/");
  Serial.print(dt.month());
  Serial.print("/");
  Serial.print(dt.year());
  Serial.print(" ");
  if (dt.hour() < 10) Serial.print("0");
  Serial.print(dt.hour());
  Serial.print(":");
  if (dt.minute() < 10) Serial.print("0");
  Serial.print(dt.minute());
  Serial.print(":");
  if (dt.second() < 10) Serial.print("0");
  Serial.print(dt.second());
}

void nyalakanPompa() {
  digitalWrite(relayPump, LOW);  // Relay aktif LOW
  sedangMenyiram = true;
  waktuMulaiSiram = millis();

  Serial.print("[");
  DateTime now = rtc.now();
  tampilkanWaktu(now);
  Serial.print("] POMPA ON - Durasi: ");
  Serial.print(jadwalPenyiram[indeksJadwalAktif].durasiDetik);
  Serial.println(" detik");
}

void matikanPompa() {
  digitalWrite(relayPump, HIGH);  // Relay mati
  sedangMenyiram = false;
  indeksJadwalAktif = -1;

  Serial.print("[");
  DateTime now = rtc.now();
  tampilkanWaktu(now);
  Serial.println("] POMPA OFF");
}

bool waktusiamSama(DateTime now, Jadwal j) {
  return (now.hour() == j.jam && now.minute() == j.menit);
}

void cekJadwal() {
  DateTime now = rtc.now();

  // Jika pompa sedang menyiram, cek apakah durasi sudah habis
  if (sedangMenyiram) {
    unsigned long elapsedSeconds = (millis() - waktuMulaiSiram) / 1000;
    
    if (elapsedSeconds >= jadwalPenyiram[indeksJadwalAktif].durasiDetik) {
      matikanPompa();
    }
    return;
  }

  // Cek jadwal baru
  for (int i = 0; i < jumlahJadwal; i++) {
    if (waktusiamSama(now, jadwalPenyiram[i]) && !sedangMenyiram) {
      indeksJadwalAktif = i;
      nyalakanPompa();
      return;
    }
  }
}

void loop() {
  DateTime now = rtc.now();

  // Tampilkan waktu setiap 10 detik
  static unsigned long waktuTampilTerakhir = 0;
  if (millis() - waktuTampilTerakhir >= 10000) {
    Serial.print("Waktu: ");
    tampilkanWaktu(now);
    if (sedangMenyiram) {
      Serial.print(" - POMPA MENYIRAM");
    }
    Serial.println();
    waktuTampilTerakhir = millis();
  }

  // Cek jadwal setiap detik
  cekJadwal();

  delay(1000);
}
