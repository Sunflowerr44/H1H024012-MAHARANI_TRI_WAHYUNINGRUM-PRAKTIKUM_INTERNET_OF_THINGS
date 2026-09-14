# Pertemuan 2 - Konfigurasi Jaringan

## Tujuan dan Penjelasan Singkat

Praktikum pada Pertemuan 2 ini berfokus pada implementasi konfigurasi jaringan nirkabel (WiFi) menggunakan mikrokontroler ESP8266:
* Memahami konsep dasar jaringan nirkabel pada perangkat IoT berbasis ESP32.
* Memahami perbedaan mode station (STA), Access Point (AP), dan AP+STA pada ESP32.
* Mengimplementasikan konfigurasi jaringan untuk menghubungkan ESP ke jaringan WiFi yang tersedia.
* Mengimplementasikan ESP sebagai AP mandiri yang dapat diakses oleh perangkat lain.
* Mampu membaca dan menganalisis parameer jaringan seperti IP address, MAC address, dan RSSI.

---

## Peralatan yang Diperlukan

* **ESP8266 NodeMCU** (1 buah)
* **Breadboard** (1 buah)
* **LED** (1 buah)
* **Kabel Jumper**
* **Kabel USB Micro / Type-B** (1 buah)
* **Laptop / PC dengan Arduino IDE** (Sudah terpasang ESP8266 Core Package)


## Percobaan 2A: Konfigurasi Mode Station (STA)

### Gambaran Umum
Percobaan 2A bertujuan untuk mengimplementasikan ESP8266 sebagai **Station (STA)**, yaitu bertindak sebagai perangkat klien yang mencari dan menghubungkan diri ke jaringan WiFi lokal yang telah tersedia. Program melakukan autentikasi menggunakan SSID dan password yang ditentukan. Ketika proses sambungan berhasil, sistem memperoleh konfigurasi jaringan dinamis dari server DHCP lokal, menampilkan alamat IP, MAC address, serta nilai RSSI ke Serial Monitor, dan menyalakan LED indikator pada pin GPIO 2 sebagai umpan balik visual. Status koneksi kemudian dipantau secara periodik setiap 5 detik. Apabila koneksi terputus, sistem akan menginisiasi mekanisme *auto-reconnect* secara otomatis.

### Skematik Percobaan

```text
[ ESP8266 GPIO 2 ] ------------------------- [ Anoda (+) LED ]
[ ESP8266 GND ]    ------------------------- [ Katoda (-) LED ]
```

### Kode Program
```cpp
#include <ESP8266WiFi.h>

const char* ssid = "myminetae";
const char* password = "12345678";

const int ledPin = 2; // LED indikator status koneksi

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Set mode WiFi menjadi Station
  WiFi.mode(WIFI_STA);

  // Memulai koneksi ke jaringan WiFi
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  digitalWrite(ledPin, HIGH); // nyalakan LED sebagai indikator
}

void loop() {

  // Mengecek apakah koneksi WiFi terputus
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi terputus!");
    Serial.println("Mencoba menghubungkan kembali...");

    digitalWrite(ledPin, LOW);

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi berhasil terhubung kembali!");

    digitalWrite(ledPin, HIGH);
  }

  // Menampilkan informasi koneksi setiap 5 detik
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
  } else {
    Serial.println("Status: Terputus");
  }

  delay(5000);
}
```

### Penjelasan Kode
* ```const char* ssid = "..."; & const char* password = "...";``` : Mendeklarasikan variabel konstanta string yang menyimpan SSID (nama AP) dan kata sandi jaringan WiFi target.
* ```const int ledPin = 2;``` : Menetapkan nomor pin GPIO 2 (LED internal/eksternal) sebagai indikator visual status koneksi.
* ```pinMode(ledPin, OUTPUT);``` : Mengonfigurasi GPIO 2 sebagai output digital.
* ```digitalWrite(ledPin, LOW);``` : Memastikan LED dalam kondisi mati di awal program.
* ```WiFi.mode(WIFI_STA);``` : Menyetel modul radio WiFi agar beroperasi secara khusus sebagai klien (Station).
* ```while (WiFi.status() != WL_CONNECTED) { ... }``` : Perulangan penahan yang mencetak titik . tiap 500 ms selama status belum terhubung (WL_CONNECTED).
* ```WiFi.localIP()``` : Mengambil alamat IP dinamis yang dialokasikan oleh DHCP server router ke ESP8266.
* ```WiFi.macAddress()``` : Membaca alamat fisik unik (MAC Address) dari antarmuka network ESP8266.
* ```WiFi.RSSI()``` : Mengukur daya sinyal radio WiFi yang diterima (Received Signal Strength Indicator) dalam dBm.
* ```digitalWrite(ledPin, HIGH);``` : Mengirim sinyal logika HIGH ke GPIO 2 untuk menyalakan LED tanda berhasil terkoneksi.
* ```if (WiFi.status() != WL_CONNECTED)``` : Mendeteksi apakah status jaringan terputus di tengah jalan. Jika ya, mematikan LED, memutuskan sesi (```WiFi.disconnect()```), dan memicu sambungan ulang (```WiFi.begin()```).

### Pertanyaan Praktikum Percobaan 2A
1. Gambarkan diagram alur (flowchart) proses koneksi ESP8266 ke jaringan WiFi pada program diatas!
2. Apa fungsi dari perintah WiFi.mode(WIFI_STA) pada program tersebut?
3. Jelaskan apa yang terjadi apabila SSID atau password yang dimasukkan salah!
4. Modifikasi program agar ESP32 mencoba menghubungkan ulang (reconnect) secara otomatis apabila koneksi WiFi terputus, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md!

### Jawaban Percobaan 2A
1. 
2. Berfungsi untuk mengonfigurasi chip WiFi agar beroperasi secara khusus dalam mode STA. Pada mode STA, perangkat bertindak sebagai client yang akan terhubung ke Access Point eksternal yang sudah ada untuk mendapat akses ke jaringan lokal atau internet dan mendapatkan alokasi IP Address.
3. Jika SSID salah, perangkat tidak menemukan SSID target. Perangkat akan terus mencoba mencari SSID yang tidak tersedia sehingga proses koneksi gagal dan tertahan pada siklus perulangan. Jika Password salah, Perangkat tidak dapat menyelesaikan proses autentikasi WPA2/WPA. kode akan stuck pada while (WiFi.status() != WL_CONNECTED) sambil terus-menerus mencetak titik di serial monitor. 
4. kode modifikasi agar ESP8266 mencoba reconnect otomatis saat terputus.
```cpp
#include <ESP8266WiFi.h>

const char* ssid = "myminetae";
const char* password = "";

const int ledPin = 2; // LED indikator status koneksi

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Set mode WiFi menjadi Station
  WiFi.mode(WIFI_STA);

  // Memulai koneksi ke jaringan WiFi
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  digitalWrite(ledPin, HIGH);
}

void loop() {

  // Mengecek apakah koneksi WiFi terputus
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi terputus!");
    Serial.println("Mencoba menghubungkan kembali...");

    digitalWrite(ledPin, LOW);

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi berhasil terhubung kembali!");

    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    digitalWrite(ledPin, HIGH);
  }

  delay(1000);
}
```
Penjelasan Program: 
Penambahan kondisi ```if (WiFi.status() != WL_CONNECTED)``` pada fungsi ```loop()``` berfungsi sebagai pengawas (watchdog) koneksi. Ketika jaringan terputus, ```WiFi disconnect()``` dipanggil untuk membersihkan sesi lama sebelum ```WiFi.begin()``` memicu pemulihan sambungan ulang tanpa perlu mematikan/mereset mikrokontroler (reboot).

## Percobaan 2B : Konfigurasi AP
### Gambaran Umum
Percobaan 2B mengonfigurasi ESP8266 sebagai penyedia jaringan mandiri (SoftAP / Soft Access Point). ESP8266 memancarkan sinyal WiFi dengan SSID dan password tersendiri sehingga perangkat lain (seperti smartphone atau laptop) dapat terhubung secara langsung tanpa memerlukan router eksternal. Selain itu, diperkenalkan juga implementasi Mode Ganda (AP+STA), yang memungkinkan ESP8266 terhubung ke internet via WiFi utama sekaligus menyediakan Access Point lokal secara simultan.

### Kode Program
```cpp
#include <ESP8266WiFi.h>

const char* ap_ssid = "ESP8266-PraktikumIoT";
const char* ap_password = "12345678"; // minimal 8 karakter

void setup() {
  Serial.begin(115200);

  // Set mode WiFi menjadi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");
  Serial.print("SSID : ");
  Serial.println(ap_ssid);
  Serial.print("IP Address : ");
  Serial.println(apIP);
}

void loop() {
  // Menampilkan jumlah perangkat yang terhubung setiap 5 detik
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}
```

### Pertanyaan Praktikum 2B
1. Mengapa alamat IP default Access Point pada ESP32 umumnya bernilai 192.168.4.1?
2. Apa perbedaan mendasar antara mode Station dan mode Access Point pada ESP32?
3. Jelaskan risiko keamanan apabila password Access Point tidak diberikan atau terlalu sederhana!
4. Modifikasi program agar ESP32 berjalan pada mode AP+STA
(terhubung ke WiFi rumah sekaligus menyediakan Access Point), dan berikan penjelasan di setiap baris kode nya dalam bentuk README.md!

### Jawaban Praktikum Percobaan 2B
1. Alamat 192.168.4.1 merupakan alamat IP standar yang dikonfigurasi secara hardcode pada firmware ESP8266 untuk mode SoftAP. Alamat ini dipilih agar tidak bentrok dengan subnet router rumah/kantor yang umumnya alokasi IP 192.168.0.x atau 192.168.1.x
2. Tabel perbedaan kedua mode : 
**Tabel 2.5. Perbedaan STA dan AP**

| Parameter | Mode STA | Mode AP |
| :--- | :--- | :--- |
| **Peran** | Client | Host/Penyedia jaringan |
| **Koneksi** | Membutuhkan router eksternal untuk terhubung | Tidak membutuhkan router eksternal |
| **Alokasi IP** | Menerima alokasi IP dari DHCP server router | Membagikan alokasi IP ke client yang terhubung |
| **Akses Internet** | Dapat mengakses internet jika router terhubung ke internet | Hanya jaringan lokal |
3. * Pihak luar dapat terhubung ke jaringan AP tanpa autentikasi
* Lalu lintas data yang terkirim ke client dapat disadap
* Penyerang dapat memenuhi alokasi batas maksimum client sehingga perangkat tidak dapat terhubung
4. kode
```cpp
#include <ESP8266WiFi.h>

const char* ssid = "vivo";
const char* password = "12345678";

const char* ap_ssid = "ESP8266-PraktikumIoT";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);

  // Set mode WiFi menjadi AP + Station
  WiFi.mode(WIFI_AP_STA);

  // Menghubungkan ESP8266 ke WiFi utama
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");
  Serial.print("IP Station : ");
  Serial.println(WiFi.localIP());

  // Membuat Access Point
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");
  Serial.print("SSID : ");
  Serial.println(ap_ssid);
  Serial.print("IP Access Point : ");
  Serial.println(apIP);
}

void loop() {
  // Menampilkan jumlah perangkat yang terhubung setiap 5 detik
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}
```
Penjelasan Program : 
* ```WiFi.mode(WIFI_AP_STA);``` : Mengaktifkan dua antarmuka radio sekaligus.
* ```WiFi.begin(ssid, password);``` : Menghubungkan antarmuka STA ke router eksternal.
* ```WiFi.softAP(ap_ssid, ap_password);``` : Memancarkan jaringan SoftAP lokal mandiri secara bersamaan.
