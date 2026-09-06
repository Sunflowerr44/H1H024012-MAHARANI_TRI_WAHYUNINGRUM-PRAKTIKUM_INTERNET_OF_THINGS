# Source Code - Percobaan 1A: 

Folder ini berisi kode program untuk 

## Penjelasan Logika Program

Kode pada Percobaan 1A Akuisisi Data Sensor Berikut adalah alur logikanya:
1. Sensor mengambil data dari lingkungan
2. Mikrokontroler mengecek pembacaan suhu
3. Monitor menampilkan pesan "gagal membaca suhu" jika data tidak terbaca, atau monitor menampilkan suhu jika pembacaan berhasil.

## Konfigurasi Pin
| Komponen | Pin ESP32 | Modus | Keterangan |
|---|---|---|---|
| Pin SDA DHT | Pin 6 | `INPUT` | Dihubungkan langsung ke ESP32 | 
| Pin VCC DHT | Pin 3V3 | - | Dihubungkan langsung ke 3V3 ESP |
| Pin GND DHT | Pin GND | - | Dihubungkan langsung ke ESP32 |

## Source Code
```cpp
#include <DHT.h>

#define DHTPIN 4        // pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT11   // tipe sensor yang digunakan

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin(); // inisialisasi sensor DHT22
  Serial.println("Memulai akuisisi data sensor DHT22...");
}

void loop() {
  // Membaca data kelembaban dan suhu
  float kelembaban = dht.readHumidity();
  float suhu = dht.readTemperature();

  // Periksa apakah pembacaan berhasil
  if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT22!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C, Kelembaban: ");
    Serial.print(kelembaban);
    Serial.println(" %");
  }

  delay(2000); // jeda pembacaan setiap 2 detik
}
```