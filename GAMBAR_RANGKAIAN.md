# GAMBAR RANGKAIAN RELAY DAN POMPA DIAFRAGMA

## GAMBAR 1: Cara Kerja Relay (Saklar Elektronik)

### Kondisi RELAY OFF (Arduino D2 = HIGH)

```
                    RELAY CH1
                  ┌───────────┐
                  │           │
                  │ IN1       │
                  │  ▲        │
              D2 ─┤  │ HIGH   │
         (Arduino) │           │
                  │           │
                  └───┬───────┘
                      │
                     (Saklar Terbuka - Tidak Ada Arus)
                      │
                  ┌───┴─────────────────┐
                  │                     │
              NO ─┤ ╱ (Terbuka)    COM ─┤
                  │                     │
                  │                     │
                  └─────────────────────┘

Hasil: POMPA TIDAK MENYALA
```

### Kondisi RELAY ON (Arduino D2 = LOW)

```
                    RELAY CH1
                  ┌───────────┐
                  │           │
                  │ IN1       │
                  │  ▼        │
              D2 ─┤  │ LOW    │
         (Arduino) │           │
                  │           │
                  └───┬───────┘
                      │
                     (Saklar Tertutup - Ada Arus)
                      │
                  ┌───┴─────────────────┐
                  │                     │
              NO ─┤ ══ (Tertutup)  COM ─┤
                  │   ▼                 │
                  │  Arus Mengalir      │
                  │   ▼                 │
                  └─────────────────────┘

Hasil: POMPA MENYALA
```

---

## GAMBAR 2: Rangkaian Lengkap Sistem (Versi Sederhana)

```
╔═══════════════════════════════════════════════════════════════════════╗
║                     POWER SUPPLY 5V POMPA                            ║
║                                                                       ║
║  ┌──────────┐                                                        ║
║  │   +5V   ────────────────┬──────────────────────────────┐          ║
║  │ Merah   │                │                             │          ║
║  └──────────┘                │                             │          ║
║                              ▼                             ▼          ║
║                    ┌──────────────────┐              ┌──────────┐    ║
║                    │  RELAY CH1       │              │  POMPA  │    ║
║                    │                  │              │ 5V      │    ║
║   Arduino D2 ─────►│ IN1 (Input)      │              │         │    ║
║   (Kontrol)        │                  │              │ Merah + │    ║
║                    │ NO ───────────────┼──────────────┤         │    ║
║                    │ (Normally Open)  │              │ Hitam - │    ║
║                    │                  │              │         │    ║
║                    │ COM ──────────────┤──────┐      └────┬────┘    ║
║                    │ (Common)          │      │           │         ║
║                    └──────────────────┘      │           │         ║
║                                              │           │         ║
║  ┌──────────┐                                │           │         ║
║  │  GND    ├────────────────────────────────┴───────────┴──────┐   ║
║  │ Hitam   │                                                   │   ║
║  └──────────┘                                                   │   ║
║                                                                  │   ║
║  ┌────────────────────────────────────────────────────────────┴──┐ ║
║  │   GND (Ke Arduino Nano)  [SANGAT PENTING!]                  │ ║
║  └───────────────────────────────────────────────────────────────┘ ║
╚═══════════════════════════════════════════════════════════════════════╝

KETERANGAN WARNA KABEL:
🔴 Merah   = Positif (+)
⚫ Hitam   = Negatif/Ground (-)
🟡 Kuning  = Data/Kontrol
```

---

## GAMBAR 3: Detail Koneksi Relay

```
┌──────────────────────────────────────────────────────────────┐
│              RELAY MODULE 2 CHANNEL 5V                      │
│                                                              │
│  Baris Atas:                                                │
│  ┌─────┬─────┬─────┬─────┐                                  │
│  │ GND │ VCC │ IN1 │ In2 │                                  │
│  └─┬───┴─┬───┴─┬───┴─┬───┘                                  │
│    │     │     │     │                                      │
│    │     │     │     └──► D3 (Relay 2 - Jika pakai 2 relay)│
│    │     │     │                                            │
│    │     │     └────────► D2 (Relay 1 - Untuk Pompa)       │
│    │     │                                                  │
│    │     └────────────────► 5V Arduino                     │
│    │                                                        │
│    └────────────────────────► GND Arduino                  │
│                                                              │
│  Baris Bawah (Channel 1 - Untuk Pompa):                   │
│  ┌────────────────────────────────────────┐               │
│  │  COM  │  NO  │  NC   │ COM  │  NO  │ NC │              │
│  └────┬──┴──┬───┴────┬───┴──┬──┴──┬───┴────┘              │
│       │     │        │      │     │                       │
│       │     │        │      │     └─► Channel 2 (Tidak pakai)
│       │     │        │      │                             │
│       │     │        │      └────► Channel 2 COM (GND)   │
│       │     │        │                                    │
│       │     │        └─────────── Channel 1 NC (Tidak pakai)
│       │     │                                             │
│       │     └──────────────────── Channel 1 NO (ke +5V)  │
│       │                                                   │
│       └──────────────────────────── Channel 1 COM (ke Pompa+)
│                                                            │
└──────────────────────────────────────────────────────────────┘

PENJELASAN:
- COM = Common (titik pusat, akan terhubung ke pompa)
- NO = Normally Open (terbuka saat relay OFF, tertutup saat ON)
- NC = Normally Close (tertutup saat OFF, terbuka saat ON) - tidak pakai
```

---

## GAMBAR 4: Koneksi Pin Arduino Nano

```
                    ARDUINO NANO
     ┌────────────────────────────────────┐
     │                                    │
     │  D1        D13 D12 D11 D10 D9 D8  │
     │   ├─────────────────────────────┤  │
     │   │ ◯ ◯ ◯ ◯  ◯  ◯  ◯  ◯  ◯  ◯  ◯ │  │
     │   │                             │  │
     │   ├─ D0  D1  D2  D3  D4  D5 D6 D7 ◯ │ ← D2 ke Relay IN1
     │   │ ◯  ◯  ◯  ◯  ◯  ◯  ◯  ◯  ◯    │
     │   │                             │  │
     │   ├─ A0  A1  A2  A3  A4  A5  A6 A7 │
     │   │ ◯  ◯  ◯  ◯  ◯  ◯  ◯  ◯       │
     │   │                             │  │
     │   ├─ 5V  GND                    │  │
     │   │ ◯   ◯                       │  │
     │   └────────────────────────────┘  │
     │                                    │
     │   KONEKSI YANG DIPAKAI:           │
     │                                    │
     │   5V ──► Relay VCC                │
     │   5V ──► RTC VCC                  │
     │   GND ─► Relay GND                │
     │   GND ─► RTC GND                  │
     │   GND ─► Power Supply GND         │
     │   D2 ──► Relay IN1                │
     │   A4 ──► RTC SDA                  │
     │   A5 ──► RTC SCL                  │
     │                                    │
     └────────────────────────────────────┘
```

---

## GAMBAR 5: Skema Aliran Arus Saat RELAY ON

```
SAAT RELAY ON (Arduino D2 = LOW):

1. ARUS MASUK KE POMPA:

   Power Supply +5V (Merah)
            │
            ├──────────────────────┐
            │                      │
            ▼                      ▼
        [Relay NO]           [Relay VCC]
            │                      │
            │ (Arus mengalir)      │
            │                      │
            ▼                      │
        [Relay COM] ◄─────────────┘
            │
            ▼
        [Pompa +] (Merah)
            │
            │ POMPA MENYALA
            │ AIR KELUAR
            │
        [Pompa -] (Hitam)
            │
            ▼
        GND Power Supply


2. ARUS KEMBALI KE GROUND:

   GND Power Supply (Hitam)
            │
            ├────────────────┐
            │                │
            ▼                ▼
        [Relay GND]    [Arduino GND]
            │                │
            └────────┬───────┘
                     │
              (HARUS TERHUBUNG!)
```

---

## GAMBAR 6: Koneksi RTC DS3231 (Tambahan)

```
                   RTC DS3231
            ┌──────────────────┐
            │                  │
            │ SDA  SCL VCC GND │
            │  ◯   ◯   ◯   ◯  │
            │  │   │   │   │  │
            └──┼───┼───┼───┼──┘
               │   │   │   │
               │   │   │   └─────► GND Arduino
               │   │   │
               │   │   └───────► 5V Arduino
               │   │
               │   └──────────► A5 (SCL) Arduino
               │
               └──────────────► A4 (SDA) Arduino
```

---

## GAMBAR 7: Tampilan Fisik Relay Module 2 Channel

```
Tampak Depan:
┌─────────────────────────────────────┐
│  RELAY MODULE 2 CHANNEL 5V          │
│                                     │
│  [Relay1 LED] [Relay2 LED]         │
│  ◯             ◯                    │
│                                     │
│  ┌─────────────────────────────┐   │
│  │ GND │ VCC │ IN1 │ IN2       │   │
│  └─────────────────────────────┘   │
│                                     │
│  ┌────────┬────────┬────────┬────┐ │
│  │ COM NO NC │ COM NO NC │      │ │
│  │ ◯   ◯  ◯  │ ◯   ◯  ◯  │      │ │
│  │Channel 1  │Channel 2 │      │ │
│  └────────┴────────┴────────┴────┘ │
│                                     │
└─────────────────────────────────────┘

Pin Koneksi Atas:
GND ─► Hitam (Ground)
VCC ─► Merah (5V)
IN1 ─► Kuning (D2 Arduino) ◄── POMPA
In2 ─► Orange (D3 Arduino) ← Tidak pakai

Pin Koneksi Bawah (Channel 1):
COM ─► Ke Pompa (+) Merah
NO  ─► Ke +5V Power Supply Merah
NC  ─► Tidak pakai (Normally Close)
```

---

## GAMBAR 8: Wiring Fisik Lengkap (Top View)

```
                    ┌─────────────────────────────┐
                    │    ARDUINO NANO             │
                    │                             │
          ┌────────►│ D2 (Pin Digital)            │
          │         │                             │
          │         │ A4 (SDA)  A5 (SCL)         │
          │         │                             │
          │    ┌───►│ 5V                          │
          │    │    │                             │
          │    │ ┌─►│ GND                         │
          │    │ │  │                             │
          │    │ │  └─────────────────────────────┘
          │    │ │
          │    │ │
          │    │ │    ┌──────────────────────────────┐
          │    └─┼───►│ RTC DS3231                   │
          │      │    │ VCC  SDA  SCL  GND           │
          │      │    │  │    │    │    │            │
          │      │    └──┼────┼────┼────┼────────────┘
          │      │       │    │    │    │
          │      │      5V   A4   A5   GND
          │      │
          │      │
          │      │    ┌──────────────────────────────┐
          │      └───►│ RELAY MODULE 2 CH            │
          │           │ VCC  GND  IN1  IN2           │
          │           │  │    │    │    │            │
          │           └──┼────┼────┼────┼────────────┘
          │              │    │    │    │
          │              5V  GND   D2  (tidak pakai)
          │              │    │
          │              │    │
          │              │    │    ┌─────────────┐
          │              │    └───►│ GND         │
          │              │         │ Power Supply│
          │              │         │  POMPA      │
          │              │         └─────────────┘
          │              │
          └──────────────┤
                         │
         ┌───────────────┴──────────────────────┐
         │   POWER SUPPLY 5V POMPA              │
         │                                      │
         │  + (Merah) ───► RELAY NO ────┐      │
         │                             │      │
         │                         [Pompa]   │
         │                          │ │      │
         │  - (Hitam) ◄─────────────┴─┴──────┘
         │
         └──────────────────────────────────────
```

---

## GAMBAR 9: Step-by-Step Pemasangan

### STEP 1: Siapkan Komponen

```
Komponen yang dibutuhkan:
✓ Arduino Nano
✓ Relay Module 2 Channel 5V
✓ Power Supply 5V (untuk relay + Arduino)
✓ Power Supply 5V (terpisah untuk pompa)
✓ Pompa Diafragma 5V
✓ Kabel Jumper (Merah, Hitam, Kuning, Orange)
✓ RTC DS3231 (optional, jika ingin real time)
```

### STEP 2: Hubungkan Arduino ke Relay

```
Arduino Nano          Relay Module
┌─────────┐           ┌──────────────┐
│  5V  ◯──┼──────────►│ VCC          │
│         │           │              │
│  GND ◯──┼──────────►│ GND          │
│         │           │              │
│  D2  ◯──┼──────────►│ IN1          │
└─────────┘           └──────────────┘

Kabel:
- Merah 5V: Arduino 5V → Relay VCC
- Hitam GND: Arduino GND → Relay GND
- Kuning D2: Arduino D2 → Relay IN1
```

### STEP 3: Hubungkan Power Supply Pompa ke Relay

```
Power Supply Pompa      Relay Module
┌──────────────┐        ┌──────────────┐
│ +5V  ◯───────┼───────►│ NO           │
│              │        │ (Normally    │
│              │        │  Open)       │
│ GND  ◯───────┼───────►│ GND          │
└──────────────┘        │              │
                        │ COM          │
                        │ (Common)     │
                        └──────────────┘

Kabel:
- Merah +5V: Power Supply → Relay NO
- Hitam GND: Power Supply → Relay GND
```

### STEP 4: Hubungkan Pompa ke Relay COM

```
Pompa            Relay Module
┌──────┐         ┌──────────────┐
│  +  ─┼────────►│ COM          │
│      │ Merah   │ (Common)     │
│  -  ─┼────────►│ GND          │
└──────┘ Hitam   └──────────────┘

Kabel:
- Merah Pompa+ → Relay COM
- Hitam Pompa- → Relay GND / Power Supply GND
```

### STEP 5: Satukan Semua Ground

```
Arduino GND ────┐
                │
Power Supply ───┼──► GND (Titik Pusat)
   GND         │
                │
Relay GND ─────┘

SANGAT PENTING: Semua GND harus terhubung ke 1 titik!
```

### STEP 6: Test Sistem

```
1. Pastikan pompa sudah terpasang ke pipa + nozzle

2. Upload kode ke Arduino:
   digitalWrite(2, LOW);   // Relay ON
   delay(5000);            // 5 detik
   digitalWrite(2, HIGH);  // Relay OFF
   delay(5000);

3. Lihat:
   ✓ LED relay menyala saat pompa ON
   ✓ Pompa berbunyi saat ON
   ✓ Air/cairan keluar dari nozzle
   ✓ Tidak ada air saat pompa OFF
```

---

## GAMBAR 10: Troubleshooting Visual

### Problem 1: Pompa Tidak Menyala

```
Kemungkinan 1: COM-Pompa tidak terhubung
┌──────────────┐
│ RELAY MODULE │
│              │
│ NO ───┐ COM ─┤  ✗ (Tidak terhubung)
│       │      │
└───────┼──────┘
        │
        ✓ Ke +5V
        ✗ COM KOSONG

Solusi: Hubungkan COM → Pompa (+)
```

### Problem 2: Arduino Tidak Kontrol Relay

```
Kemungkinan 2: GND tidak terhubung
┌──────────────┐
│  Arduino   │  Relay
│  GND ◯  │  GND ◯
│  ✗ Tidak terhubung
│
└──► Solusi: Hubungkan Arduino GND → Relay GND

Kemungkinan 3: D2 tidak terhubung
┌──────────────┐
│  Arduino   │  Relay
│  D2  ◯  │  IN1 ◯
│  ✗ Tidak terhubung
│
└──► Solusi: Hubungkan D2 → IN1
```

### Problem 3: Nozzle Tidak Menyemprot

```
Kemungkinan: Tekanan pompa rendah

Cek:
┌─────────────────────────────────────┐
│ Pompa ON? ──► Dengarkan suara?      │
│  ✓ Ya → Lanjut ke step berikutnya   │
│  ✗ Tidak → Cek relay & kabel       │
│                                     │
│ Ada aliran air? ──► Lihat pipa?     │
│  ✓ Ya → Nozzle mungkin tersumbat   │
│  ✗ Tidak → Cek pompa & reservoir   │
│                                     │
│ Tekanan cukup? ──► Test tanpa       │
│  ✓ Ya → Nozzle 0.8mm cocok         │
│  ✗ Tidak → Cek debit pompa         │
└─────────────────────────────────────┘
```

---

## CHECKLIST PEMASANGAN AKHIR

```
┌─────────────────────────────────────────────────┐
│  CHECKLIST SEBELUM NYALAKAN                     │
├─────────────────────────────────────────────────┤
│ □ Pompa sudah terpasang & pipa terhubung       │
│ □ Nozzle sudah terpasang                        │
│ □ Reservoir berisi air/cairan                   │
│ □ Arduino 5V → Relay VCC (Merah)               │
│ □ Arduino GND → Relay GND (Hitam)              │
│ □ Arduino D2 → Relay IN1 (Kuning)              │
│ □ Power Supply +5V → Relay NO (Merah)          │
│ □ Power Supply GND → Relay GND (Hitam)         │
│ □ Relay COM → Pompa + (Merah)                  │
│ □ Pompa - → GND Power Supply (Hitam)           │
│ □ Power Supply GND → Arduino GND (Hitam)       │
│ □ RTC terhubung (jika pakai)                    │
│ □ Kode sudah diupload ke Arduino                │
│ □ Relay LED menyala saat D2 LOW                │
│ □ Pompa menyala saat Relay LED ON              │
│                                                   │
│  AMAN UNTUK DIGUNAKAN ✓                        │
└─────────────────────────────────────────────────┘
```

---

Sudah jelas dengan gambar-gambar di atas? 

Kalau ada yang belum paham, saya bisa jelaskan part tertentu lebih detail lagi!

Mau saya buatkan juga:
1. Versi dengan 2 tangki (lebih kompleks)?
2. Layout fisik semua komponen di box?
3. Kode Arduino yang sudah siap pakai?
