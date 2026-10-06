# Pertemuan 3 - Protokol Komunikasi

## Tujuan dan Penjelasan Singkat
### Tujuan
1. Memahami konsep dasar protokol komunikasi pada sistem IoT (Internet of Things)
2. Memahami karakteristik dan perbedaan antara protokol HTTP dan MQTT dalam konteks IoT.
3. Mengimplementasikan pengiriman data dari mikroprosesor/mikrokontroller (ESP32 / ESP8266) ke server menggunakan protokol HTTP dengan metode POST dan format data JSON.
4. Mengimplementasikan pertukaran data dari ESP32/ESP8266 ke broker MQTT menggunakan pola publish-subscribe dengan format JSON.
5. Mampu menganalisis kelebihan dan kekurangan masing-masing protokol untuk berbagai skenario aplikasi IoT.

### Penjelasan Singkat
Protokol komunikasi mengatur tata cara pertukaran data antar-perangkat pada jaringan. Pada praktikum ini, dipelajari dua protokol utama IoT:
* HTTP (Hypertext Transfer Protocol): Berbasis model request-response dan stateless. Cocok untuk pengiriman data periodik/transaksional.
* MQTT (Message Queuing Telemetry Transport): Protokol lightweight berbasis publish-subscribe melalui perantara broker. Menggunakan persistent connection yang hemat energi dan bandwidth, cocok untuk komunikasi real-time/continuous.
* JSON (JavaScript Object Notation): Format standar pertukaran data ringan berbasis teks (key-value) yang diproses menggunakan pustaka ArduinoJson.

## Peralatan yang Diperlukan
* Board Microcontroller: ESP32 DevKit / ESP8266 NodeMCU (1 buah)
* Kabel USB Data: Micro-USB / USB-C (1 buah)
* Laptop/PC: Terinstal Arduino IDE (dengan Core ESP32/ESP8266, pustaka ArduinoJson versi 7.x, dan PubSubClient)
* Jaringan WiFi: Terkoneksi ke Internet
* Aplikasi Client MQTT: MQTT Explorer / HiveMQ WebSocket Client
* Server & Broker Endpoint

### Skematik Percobaan
Rangkaian praktikum menggunakan modul ESP32/ESP8266 yang dihubungkan langsung ke Laptop/PC via kabel komunikasi USB Data untuk catu daya serta komunikasi Serial Monitor.

+-------------------+             +-----------------------+
|  Laptop / PC      |  USB Data   |  ESP32 / ESP8266      |
|  (Arduino IDE /   |============>|  Dev Board            |
|   Serial Monitor) |             |                       |
+-------------------+             +-----------------------+
                                              |
                                              v (WiFi 2.4 GHz)
                                  +-----------------------+
                                  | Router / Hotspot WiFi |
                                  +-----------------------+
                                              |
                                              v (Internet)
                                 /-------------------------\
                                ( HTTP Server & MQTT Broker )
                                 \-------------------------/


## Percobaan 3A
### Gambaran Umum
Percobaan 3A bertujuan untuk mengimplementasikan pengiriman data telemetry sensor secara berkala (simulasi data suhu dan kelembaban) dari mikrokontroller ke server penguji httpbin.org menggunakan protokol HTTP dengan metode POST dan payload berformat JSON.

### Kode Program
```cpp
#include <WiFi.h> 
#include <HTTPClient.h>       // Library untuk protokol HTTP Client pada ESP32
#include <WiFiClientSecure.h> // Library untuk koneksi secure (HTTPS) pada ESP32
#include <ArduinoJson.h>      // Library untuk membuat dan memproses data format JSON

const char* ssid = "poco";
const char* password = "9876543210";
const char* serverUrl = "https://httpbin.org/post"; // URL server tujuan pengiriman HTTP POST

WiFiClientSecure wifiClient; // Membuat objek TLS/SSL client untuk koneksi HTTPS

void setup() {
  Serial.begin(115200);

  // Menonaktifkan validasi sertifikat SSL agar dapat terkoneksi ke HTTPS tanpa verifikasi CA
  wifiClient.setInsecure(); 
  
  WiFi.begin(ssid, password); // Memulai proses koneksi WiFi dengan SSID dan password
  Serial.print("Menghubungkan ke WiFi");
  
  // Looping menunggu status WiFi hingga terhubung
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");
}

void loop() {
  // Mengecek apakah status ESP32 masih terhubung ke WiFi
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http; // Membuat objek HTTPClient untuk memproses request HTTP
    
    // Inisialisasi koneksi HTTP/HTTPS menggunakan client secure dan URL tujuan
    http.begin(wifiClient, serverUrl); 
    
    // Menambahkan header request untuk memberitahu server format payload adalah JSON
    http.addHeader("Content-Type", "application/json"); 

    // Membuat objek data sensor dalam format JSON
    JsonDocument doc; 
    doc["suhu"] = 28.5;
    doc["kelembaban"] = 65.0;
    doc["waktu"] = millis();

    // Deklarasi variabel string penampung data JSON
    String requestBody;
    serializeJson(doc, requestBody); // Mengonversi objek JSON ke dalam bentuk string

    Serial.print("Mengirim data: ");
    Serial.println(requestBody);

    // Mengirim data melalui HTTP POST dan menyimpan response code
    int httpResponseCode = http.POST(requestBody); 

    if (httpResponseCode > 0) { // Mengecek jika ada respon balasan positif dari server
      Serial.print("Kode Response HTTP: ");
      Serial.println(httpResponseCode);
      Serial.println("Isi Response:");
      Serial.println(http.getString()); // Mengambil dan menampilkan body balasan dari server
    } else {
      Serial.print("Pengiriman gagal, kode error: ");
      Serial.println(httpResponseCode); // Mencetak kode eror internal library HTTPClient
    }

    http.end(); // Menutup koneksi HTTP untuk membebaskan resource memory
  }

  delay(10000);
}
```
### Penjelasan Kode
* WiFiClientSecure wifiClient; & wifiClient.setInsecure();: Membuka akses transmisi data terenkripsi HTTPS ke server tanpa harus memverifikasi SSL certificate root CA secara manual.
* HTTPClient http; & http.begin(wifiClient, serverUrl): Menyiapkan sesi klien protokol HTTP berbasis alamat URL target (httpbin.org/post).
* http.addHeader("Content-Type", "application/json"): Menyisipkan entri header HTTP request agar server dapat langsung mempassing isi payload sebagai dokumen struktur JSON.
* JsonDocument doc;: Menginisialisasi dokumen JSON internal (menggunakan pustaka ArduinoJson v7).
* doc["waktu"] = millis();: Menyisipkan parameter tambahan berupa waktu operasional perangkat dalam satuan milidetik (ms) sejak boot up.
* serializeJson(doc, requestBody): Mengubah pasangan data key-value dari objek JSON menjadi string terstruktur.
* http.POST(requestBody): Mengeksekusi transmisi data ke server menggunakan protokol HTTP method POST.
* http.end(): Menghentikan sesi HTTP POST untuk membebaskan ruang memori RAM perangkat.

### Pertanyaan dan Jawaban Praktikum Percobaan 3A
1. Gambarkan diagram alur (flowchart) proses pengiriman data melalui HTTP POST pada program di atas!
2. Apa fungsi dari perintah http.addHeader("Content-Type", "application/json") pada program tersebut?
3. Jelaskan arti dari kode response HTTP 200 dan sebutkan salah satu contoh kode response HTTP lain beserta artinya!
4. Modifikasi program agar ESP32 dapat mengirimkan data tambahan berupa waktu (dalam milidetik sejak dinyalakan menggunakan millis()) ke dalam JSON yang dikirim, dan berikan penjelasan di setiap baris kode yang ditambahkan.

Jawaban:
1. 
2. Perintah ini berfungsi untuk menambahkan meta-information pada HTTP Request Header. Header ini memberitahu server penerima bahwa isi pesan (payload body) dikirimkan menggunakan format JSON. Tanpa header ini, server dapat salah mengartikan format payload sebagai teks biasa (plain text) atau formulir web biasa (x-www-form-urlencoded), sehingga parsing data di tingkat server bisa mengalami error.
3. HTTP 200 (OK): Berarti permintaan (request) yang dikirimkan oleh klien telah berhasil diterima, dipahami, dan diproses oleh server tanpa kendala.
Contoh lain :
HTTP 404 (Not Found): Berarti server tidak dapat menemukan sumber daya atau URL endpoint yang diminta oleh klien.
HTTP 400 (Bad Request): Berarti permintaan klien gagal diproses karena kesalahan sintaksis data (misal sintaks JSON rusak/cacat).
4. 


## Percobaan 3B
### Gambaran Umum
Percobaan 3B bertujuan untuk mengimplementasikan transmisi data secara terstruktur dan efisien berbasis protokol MQTT. ESP32/ESP8266 dikondisikan sebagai Publisher yang mempublikasikan data telemetry sensor dalam bentuk paket JSON ke sebuah Topic tertentu di Broker Publik (broker.hivemq.com), yang kemudian dipantau secara langsung (real-time) oleh perangkat Subscriber via MQTT Explorer.

### Kode Program
```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "poco";
const char* password = "9876543210";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "unsoed/tk245004/kelompokAnda/sensor";

WiFiClient espClient;
PubSubClient client(espClient);

void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi berhasil terhubung!");
}

void hubungkanMQTT() {
  // Looping hingga koneksi ke broker MQTT berhasil
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    // Membuat Client ID acak agar tidak bentrok dengan pengguna lain
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil terhubung!");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort); // Mengatur konfigurasi alamat broker dan port
}

void loop() {
  // Cek apakah status MQTT masih terhubung
  if (!client.connected()) {
    hubungkanMQTT();
  }
  client.loop(); // Menjaga koneksi tetap hidup (keep-alive) & memproses antrean pesan

  // Membuat data sensor dalam format JSON
  JsonDocument doc;
  doc["suhu"] = 28.5;
  doc["kelembaban"] = 65.0;

  char buffer[128];
  serializeJson(doc, buffer); // Mengonversi dokumen JSON ke karakter array (buffer)

  // Mempublikasikan data ke topic MQTT target
  client.publish(mqttTopic, buffer);
  
  Serial.print("Data terkirim ke topic ");
  Serial.print(mqttTopic);
  Serial.print(": ");
  Serial.println(buffer);

  delay(5000);
}
```

### Penjelasan Kode
* PubSubClient client(espClient);: Mendeklarasikan instance MQTT client berbasis transport socket TCP/IP.
* mqttServer & mqttPort: Menentukan server perantara/broker MQTT (broker.hivemq.com) pada port standard non-encrypted 1883.
* mqttTopic: Saluran publikasi data unik (channel) agar data telemetry tidak bercampur dengan pengiriman milik perangkat lain.
* client.connect(clientId.c_str()): Menginisialisasi jabat tangan (handshake) koneksi TCP ke MQTT Broker menggunakan nama Client ID yang dibangkitkan secara acak.
* client.loop(): Instruksi terpenting untuk mempertahankan koneksi keep-alive (ping/pong paket) serta memproses antrean pesan masuk maupun keluar.
* serializeJson(doc, buffer): Mengubah objek format JSON menjadi array karakter (null-terminated char array) agar siap diproses oleh fungsi client.publish().
* client.publish(mqttTopic, buffer): Mengirimkan payload pesan berformat JSON ke topik yang dituju di broker.

### Pertanyaan dan Jawaban Praktikum 3B
1. Apa fungsi dari topic pada protokol MQTT, dan mengapa topic yang digunakan perlu dibuat unik?
2. Jelaskan fungsi dari perintah client.loop() yang dipanggil pada setiap iterasi loop()!
3. Apa yang akan terjadi apabila koneksi ke broker MQTT terputus di tengah program berjalan?

Jawaban:
1. Topic bertindak sebagai kriteria pengalamatan atau pengelompokan (routing channel) pesan dalam arsitektur Publish-Subscribe. Publisher mengirimkan data ke topic tertentu, dan Subscriber hanya akan menerima data dari topic yang didaftarkannya. Topic perlu dibuat unik karena pada broker publik (seperti HiveMQ), semua pengguna berbagi ruang broker yang sama. Jika nama topic terlalu umum (seperti "sensor"), data yang dikirim akan saling tertimpa atau bercampur (data collision) dengan data milik orang lain.
2. Menjaga koneksi socket TCP dengan broker agar tidak terputus (keep-alive ping), memproses paket data pesan masuk jika node ESP8266 bertindak sebagai subscriber, dan mengelola antrean (buffer) pengiriman data internal dari pustaka PubSubClient.
3. Pemanggilan client.publish() akan gagal (mengembalikan nilai false). Kondisi pengecekan if (!client.connected()) di dalam rutin utama loop() akan bernilai true, sehingga sistem akan memanggil kembali fungsi hubungkanMQTT() untuk melakukan percobaaan ulang koneksi (reconnection mechanism) secara berkala setiap 2 detik hingga perangkat terhubung kembali ke broker.
