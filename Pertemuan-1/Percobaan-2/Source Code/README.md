# Source Code - Percobaan 2A: Kendali Aktuator

Folder ini berisi kode program untuk kendali aktuator. Jenis aktuator yang digunakan yaitu relay module.

## Penjelasan Logika Program

Kode pada Percobaan 2 - Kendali Aktuator
Berikut adalah alur logikanya:

1. Sensor mengambil data dari lingkungan
2. Mikrokontroler mengecek pembacaan suhu
3. Monitor menampilkan pesan "gagal membaca suhu" jika data tidak terbaca, atau monitor menampilkan suhu jika pembacaan berhasil.
4. Pengecekan suhu dengan ambang batas yang ditetapkan. Jika > ambang batas, aktuator akan menyala, dan sebaliknya.

## Konfigurasi Pin
| Komponen | Pin Arduino | Modus | Keterangan |
|---|---|---|---|
| Pin SDA DHT | Pin 6 | `INPUT` | Dihubungkan langsung ke ESP32 | 
| Pin VCC DHT | Pin 3V3 | - | Dihubungkan langsung ke ESP |
| Pin GND DHT | Pin GND | - | Dihubungkan langsung ke ESP32 |
| Pin IN RELAY | Pin 26 | `OUTPUT` | Dihubungkan langsung ke ESP32 |
| Pin GND RELAY | Pin GND | - | Dihubungkan langsung ke ESP32 |
| Pin VCC RELAY | Pin 5V | - | Dihubungkan langsung ke ESP32 |


## Kode Program
```cpp
#include <DHT.h>
#define DHTPIN 4 // Pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22 // Tipe sensor yang digunakan
#define RELAYPIN 26 // Pin kendali relay

DHT dht(DHTPIN, DHTTYPE);

const float suhuThreshold = 30.0; // Ambang batas suhu (°C)

void setup() {
   Serial.begin(115200);
   dht.begin();
   pinMode(RELAYPIN, OUTPUT);
   digitalWrite(RELAYPIN, LOW); // Aktuator mati di awal
}
void loop() {
   // Membaca data suhu
   float suhu = dht.readTemperature();
   if (isnan(suhu)) { //Jika tidak bisa membaca data
      Serial.println("Gagal membaca data sensor!");
   } else {
      Serial.print("Suhu: ");
      Serial.print(suhu);
      Serial.print(" °C -> ");
      // Kendali aktuator berdasarkan hasil akuisisi data sensor
      if (suhu > suhuThreshold) { // Jika suhu yang dibaca lebih dari Threshold
         digitalWrite(RELAYPIN, HIGH); // Aktifkan relay
         Serial.println("Aktuator: ON");
      } else {
         digitalWrite(RELAYPIN, LOW); // Matikan relay
         Serial.println("Aktuator: OFF");
      } 
   }
   delay(2000); // Jeda pembacaan setiap 2 detik
}
```