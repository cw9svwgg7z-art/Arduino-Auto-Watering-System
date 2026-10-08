/*
  SISTEM PENYIRAMAN OTOMATIS TANPA RTC
  Untuk tanaman tomat dan cabai
  
  Hardware:
  - Arduino Nano
  - Relay Module 5V (1 channel)
  - Pompa Diafragma 5V
  - Nozzle 0.8mm
  
  Catatan:
  - Tanpa RTC (lebih mudah, no library tambahan)
  - Menggunakan millis() untuk timing
  - Penyiraman hanya 10 detik per jadwal
  - Arduino harus nyala terus-menerus agar jadwal akurat
  
  PENTING: Jika Arduino mati/restart, waktu akan reset!
*/

const int relayPump = 2;   // Pin relay pompa (D2)

// Jadwal penyiraman (dalam milidetik sejak startup)
// 1 jam = 3600000 ms
// 1 menit = 60000 ms

unsigned long jadwalMulanorWaktu[] = {
  6 * 3600000 + 30 * 60000,   // 06:30 = 23400000 ms
  12 * 3600000 + 0 * 60000,   // 12:00 = 43200000 ms
  18 * 3600000 + 30 * 60000   // 18:30 = 66600000 ms
};

const int jumlahJadwal = 3;
const unsigned long durasiSiram = 10000;  // 10 detik = 10000 ms

bool sedangMenyiram = false;
int indeksJadwalAktif = -1;
unsigned long waktuMulaiSiram = 0;
bool sudahTerjadwal[3] = {false, false, false};  // Flag mencegah trigger berulang

unsigned long waktuStartup = 0;

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println("=== SISTEM PENYIRAMAN OTOMATIS TANPA RTC ===");
  Serial.println("Arduino Nano - Versi Sederhana");
  Serial.println();

  // Setup relay
  pinMode(relayPump, OUTPUT);
  digitalWrite(relayPump, HIGH);  // Relay off (aktif LOW)
  Serial.println("Relay siap di pin D2");
  Serial.println();

  // Catat waktu startup
  waktuStartup = millis();

  Serial.println("Jadwal penyiraman (hanya 10 detik):");
  Serial.println();
  
  Serial.println("  1. 06:30 - 10 detik (pagi)");
  Serial.println("  2. 12:00 - 10 detik (siang)");
  Serial.println("  3. 18:30 - 10 detik (sore)");
  
  Serial.println();
  Serial.println("SISTEM SIAP - Menunggu jadwal...");
  Serial.println();
  Serial.println("Catatan: Arduino harus tetap menyala agar jadwal akurat!");
  Serial.println();
}

void nyalakanPompa() {
  digitalWrite(relayPump, LOW);  // Relay aktif LOW
  sedangMenyiram = true;
  waktuMulaiSiram = millis();

  Serial.print("[");
  tampilkanWaktuElapsed();
  Serial.print("] POMPA ON - 10 detik");
  Serial.println();
}

void matikanPompa() {
  digitalWrite(relayPump, HIGH);  // Relay mati
  sedangMenyiram = false;
  indeksJadwalAktif = -1;

  Serial.print("[");
  tampilkanWaktuElapsed();
  Serial.println("] POMPA OFF");
}

void tampilkanWaktuElapsed() {
  unsigned long elapsed = millis() - waktuStartup;
  
  unsigned long jam = (elapsed / 3600000) % 24;
  unsigned long menit = (elapsed / 60000) % 60;
  unsigned long detik = (elapsed / 1000) % 60;

  if (jam < 10) Serial.print("0");
  Serial.print(jam);
  Serial.print(":");
  if (menit < 10) Serial.print("0");
  Serial.print(menit);
  Serial.print(":");
  if (detik < 10) Serial.print("0");
  Serial.print(detik);
}

void cekJadwal() {
  unsigned long elapsedMs = millis() - waktuStartup;

  // Jika pompa sedang menyiram, cek apakah 10 detik sudah terlampaui
  if (sedangMenyiram) {
    unsigned long elapsedSiram = millis() - waktuMulaiSiram;
    
    if (elapsedSiram >= durasiSiram) {
      matikanPompa();
    }
    return;
  }

  // Cek setiap jadwal
  for (int i = 0; i < jumlahJadwal; i++) {
    // Jika waktu elapsed sudah melampaui jadwal
    if (elapsedMs >= jadwalMulanorWaktu[i] && elapsedMs < jadwalMulanorWaktu[i] + 1000) {
      
      // Dan belum pernah di-trigger hari ini
      if (!sudahTerjadwal[i]) {
        indeksJadwalAktif = i;
        nyalakanPompa();
        sudahTerjadwal[i] = true;  // Mark sebagai sudah dikerjakan
      }
    }

    // Reset flag jika sudah melewati 24 jam (reset sistem)
    if (elapsedMs >= 86400000) {  // 24 jam = 86400000 ms
      for (int j = 0; j < jumlahJadwal; j++) {
        sudahTerjadwal[j] = false;  // Reset flag
      }
      Serial.println();
      Serial.println("=== 24 JAM TELAH BERLALU - SISTEM RESET ===");
      Serial.println();
      waktuStartup = millis();  // Reset waktu startup
    }
  }
}

void loop() {
  // Tampilkan waktu setiap 30 detik
  static unsigned long waktuTampilTerakhir = 0;
  if (millis() - waktuTampilTerakhir >= 30000) {
    Serial.print("Waktu elapsed: ");
    tampilkanWaktuElapsed();
    if (sedangMenyiram) {
      Serial.print(" - POMPA MENYIRAM");
    }
    Serial.println();
    waktuTampilTerakhir = millis();
  }

  // Cek jadwal setiap 100 ms
  cekJadwal();

  delay(100);
}
