# Pertemuan 4

## Tujuan Praktikum
1. Memahami konsep pertukaran data dua arah (*bidirectional*) pada sistem IoT
2. Memahami mekanisme *subscribe* dan proses deserialisasi data JSON pada ESP8266
3. Mengimplementasikan penerimaan perintah kendali melalui MQTT untuk menggerakkan aktuator secara *real-time*
4. Mengimplementasikan sistem IoT yang dapat mempublikasikan data sensor dan menerima perintah kendali secara bersamaan (*full duplex*)
5. Mampu menganalisis mekanisme pertukaran data IoT secara menyeluruh pada sistem yang saling terhubung

## Alat dan Bahan
* ESP8266 NodeMCU
* Sensor DHT11
* LED
* Kabel USB Data
* Laptop/PC: Terinstal Arduino IDE 
* Jaringan WiFi: Terkoneksi ke Internet
* Aplikasi Client MQTT: MQTT Explorer / HiveMQ WebSocket Client
* Broker MQTT Client

### Skematik Percobaan
Rangkaian terdiri dari ESP8266 yang terhubung ke LED indikator pada GPIO D2 (dengan resistor pembatas arus 220 Ohm) dan sensor suhu/DHT11.
+-------------------------------------------------+
|                  ESP8266 DevKit                 |
|                                                 |
|   3V3 / 5V  -------------------- VCC DHT11      |
|   GND       -------------------- GND DHT11 & LED|
|   Pin D2    ---- [Resistor] ---- (+) LED        |
|   Pin D4    -------------------- DATA DHT11     |
+-------------------------------------------------+

## Percobaan 4A
### Gambaran Umum
Percobaan 4A berfokus pada implementasi penerimaan data (*subscribe*) pada topik perintah MQTT[cite: 28, 50]. ESP8266 menerima data berformat JSON, melakukan deserialisasi data menggunakan pustaka `ArduinoJson`, lalu mengendalikan status LED (menyala/mati) berdasarkan isi perintah[cite: 28, 50]. Selain itu, dilakukan modifikasi kode untuk mendukung kontrol intensitas kecerahan LED menggunakan sinyal PWM (`analogWrite`)

### Kode Program
```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Konfigurasi WiFi
const char* ssid = "Going Seventeen";
const char* password = "infinixsmart6";

// Konfigurasi MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Topic MQTT
const char* topicPerintah = "unsoed/tk245004/KelompokIvenRani/perintah";

// Konfigurasi LED
const int ledPin = D2;

// Inisialisasi WiFi dan MQTT
WiFiClient espClient;
PubSubClient client(espClient);
// Fungsi callback untuk menerima pesan MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String pesan;
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }
  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(pesan);
  // Deserialisasi data JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, pesan);
  if (error) {
    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }
  const char* perintah = doc["perintah"];
  // Kontrol LED berdasarkan perintah
  if (String(perintah) == "ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("Aktuator: ON");
  }
  else if (String(perintah) == "OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("Aktuator: OFF");
  }
}

// Fungsi menghubungkan ESP8266 ke WiFi
void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi berhasil terhubung!");
}

// Fungsi menghubungkan ESP8266 ke broker MQTT
void hubungkanMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil terhubung!");
      // Subscribe ke topic perintah
      client.subscribe(topicPerintah);
      Serial.print("Subscribe ke topic: ");
      Serial.println(topicPerintah);
    }
    else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

// Fungsi setup
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

// Fungsi loop
void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }
  // Memproses pesan MQTT secara terus-menerus
  client.loop();
}

```
### Penjelasan Kode
* analogWriteRange(255);: Mengatur batas maksimum rentang sinyal Pulse Width Modulation (PWM) pada ESP8266 menjadi 255.   
* int intensitas = doc["intensitas"] | 255;: Mengambil nilai atribut "intensitas" dari payload JSON yang telah dideserialisasi. Apabila kunci tidak disertakan, nilai otomatis diatur ke default 255.   
* constrain(intensitas, 0, 255);: Memastikan nilai intensitas yang masuk tidak berada di luar batas aman 0 hingga 255.   
* analogWrite(ledPin, intensitas);: Menyuplai tegangan analog terpolarisasi sinyal PWM ke pin LED sehingga kecerahannya menyesuaikan nilai intensitas yang dikirim melalui MQTT

### Pertanyaan dan Jawaban Praktikum Percobaan 3A
1. Gambarkan diagram alur (flowchart) proses penerimaan dan pemrosesan pesan pada fungsi callback di atas!
2. Apa yang akan terjadi apabila pesan yang dipublikasikan bukan merupakan format JSON yang valid?
3. Jelaskan mengapa fungsi client.subscribe() dipanggil di dalam fungsi hubungkanMQTT(), bukan di dalam setup()!
4. Modifikasi program agar data JSON yang diterima juga memuat nilai intensitas untuk mengatur kecerahan LED menggunakan PWM.

Jawaban:
1. 
2. Proses parsing data pada fungsi deserializeJson() akan gagal dan mengembalikan kondisi error. Program akan mencetak log "Gagal parsing JSON" pada Serial Monitor, lalu menjalankan instruksi return. Akibatnya, pemrosesan pesan langsung dihentikan sehingga status LED tidak berubah dan tidak mengeksekusi perintah yang salah.
3. Fungsi client.subscribe() wajib dipanggil setelah koneksi jaringan TCP dan sesi MQTT ke broker berhasil terbentuk. Jika dipanggil di setup(), proses subscribe hanya berjalan sekali di awal dan akan gagal jika broker belum terhubung. Memanggilnya di dalam hubungkanMQTT() menjamin bahwa ESP8266 secara otomatis mendaftar ulang (re-subscribe) ke topik perintah setiap kali terjadi koneksi ulang (reconnect) pasca putus jaringan
4. 

## Percobaan 4B
### Gambaran Umum
Percobaan 4B mengimplementasikan sistem pertukaran data dua arah (full duplex). ESP8266 mempublikasikan data sensor suhu (menggunakan data dummy/sensor DHT) ke topik data secara berkala setiap 5 detik menggunakan mekanisme non-blocking millis(), sekaligus tetap responsif menerima perintah kendali aktuator (subscribe) melalui topik terpisah tanpa terhambat.

### Kode Program
```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Konfigurasi WiFi
const char* ssid = "Going Seventeen";
const char* password = "infinixsmart6";

// Konfigurasi MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Topic MQTT
const char* topicData = "unsoed/tk245004/KelompokIvenRani/data";
const char* topicPerintah = "unsoed/tk245004/KelompokIvenRani/perintah";

// Konfigurasi LED
const int ledPin = D2;

// Inisialisasi WiFi dan MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Konfigurasi interval publish
unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000;

// Data dummy pengganti sensor DHT11 (sensor rusak)
const float suhuDummy[] = {
  28.5, 28.7, 28.6, 28.9, 29.1,
  29.0, 28.8, 29.2, 29.3, 29.1
};
const int jumlahDummy = sizeof(suhuDummy) / sizeof(suhuDummy[0]);
int indeksDummy = 0;

// Fungsi callback untuk menerima pesan MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String pesan;
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }
  JsonDocument doc;
  if (deserializeJson(doc, pesan)) {
   return; // Abaikan jika parsing gagal
  }
  const char* perintah = doc["perintah"] | "";
  digitalWrite(ledPin, String(perintah) == "ON" ? HIGH : LOW);
  Serial.print("Perintah diterima -> Aktuator: ");
  Serial.println(perintah);
}

// Fungsi menghubungkan ESP8266 ke WiFi
void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi berhasil terhubung!");
}

// Fungsi menghubungkan ESP8266 ke broker MQTT
void hubungkanMQTT() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.subscribe(topicPerintah);
      Serial.println("Terhubung dan subscribe topic perintah");
    }
    else {
      delay(2000);
    }
  }
}

// Fungsi setup
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

// Fungsi loop
void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }
  client.loop(); // Memproses pesan masuk secara terus-menerus
  // Publish data secara berkala tanpa memblokir proses subscribe
  if (millis() - waktuTerakhirPublish > intervalPublish) {
    waktuTerakhirPublish = millis();
    float suhu = suhuDummy[indeksDummy]; // Mengambil data dummy
    indeksDummy = (indeksDummy + 1) % jumlahDummy;
    JsonDocument doc;
    doc["suhu"] = suhu;
    char buffer[128];
    serializeJson(doc, buffer);
    client.publish(topicData, buffer);
    Serial.print("Data terkirim: ");
    Serial.println(buffer);
  }
}
```

### Penjelasan Kode
* const char* topicBuzzer & buzzerPin = D5: Mendaftarkan topik MQTT baru khusus kontrol buzzer serta mendefinisikan GPIO D5 sebagai pin output aktuator kedua.
* strcmp(topic, topicPerintah) & strcmp(topic, topicBuzzer): Membandingkan nama topik penerima data pada callback secara case-sensitive untuk memisahkan logika kontrol LED dan Buzzer.
* client.subscribe(topicBuzzer): Mendaftarkan ESP8266 di dalam fungsi hubungkanMQTT() agar mendengarkan instruksi pada topik buzzer selain topik LED.
* if (millis() - waktuTerakhirPublish > intervalPublish): Logika non-blocking yang memeriksa selisih waktu. Pengiriman data suhu berjalan tiap 5000 ms tanpa memblokir eksekusi client.loop()

### Pertanyaan dan Jawaban Praktikum 3B
1. Mengapa penggunaan delay() yang lama sebaiknya dihindari pada program yang menggabungkan proses publish dan subscribe secara bersamaan?
2. Jelaskan cara kerja mekanisme non-blocking menggunakan fungsi millis() pada program di atas!
3. Apa yang akan terjadi apabila fungsi client.loop() jarang dipanggil (misalnya hanya sekali setiap 10 detik)?
4. Modifikasi program agar menambahkan satu topic perintah baru untuk mengendalikan aktuator kedua (misalnya buzzer).

Jawaban: 
1. Penggunaan delay() bersifat blocking (menghentikan seluruh eksekusi instruksi mikrokontroler). Jika delay() digunakan, fungsi client.loop() tidak dapat dipanggil selama jeda tersebut, sehingga pesan MQTT yang masuk tidak dapat diproses secara real-time, terjadi penundaan respons aktuator, dan risiko terputusnya koneksi dari broker MQTT akibat keep-alive timeout.
2. Fungsi millis() mengembalikan waktu dalam milidetik sejak board ESP8266 mulai menyala. Program menyimpan timestamp terakhir data dikirim (waktuTerakhirPublish) dan membandingkannya dengan waktu saat ini (millis() - waktuTerakhirPublish > intervalPublish). Jika selisihnya telah melebihi 5000 ms, data akan dikirim dan timestamp diperbarui. Mekanisme ini membuat program dapat mengukur interval waktu pengiriman tanpa menghentikan alur utama program.
3. Keterlambatan Eksekusi: Perintah kontrol aktuator tidak akan langsung direspon.   Hilang Sifat Real-time: Responsivitas sistem IoT menjadi sangat lambat.   Putus Koneksi Broker: Broker MQTT dapat menganggap ESP8266 terputus/mati karena tidak mengirimkan sinyal ping/keep-alive rutin.   Penumpukan Pesan/Loss: Paket data incoming dapat terlewat atau tidak terproses secara instan.
4. 
