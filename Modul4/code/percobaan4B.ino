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