# Data Pengamatan Modul 3: Protokol Komunikasi IoT

## Percobaan 3A: Komunikasi Data Menggunakan HTTP

### Tabel 1. Hasil Pengiriman Data melalui HTTP POST

| No. | Waktu Pengiriman (s) | Data JSON yang Dikirim | HTTP Response Code | Response Body | Status Pengiriman |
| :---: | :---: | :---: | :---: | :---: | :---: |
| 1 | 0 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 2 | 10 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 3 | 20 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 4 | 30 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 5 | 40 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 6 | 50 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 7 | 60 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 8 | 70 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 9 | 80 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |
| 10 | 90 | `{"suhu":28.5,"kelembaban":65}` | `200` | `data:"{\"suhu\":28.5,\"kelembaban\":65}"` | Berhasil |

**Keterangan:**
- **Data JSON yang Dikirim:** misalnya `{"suhu":28.5,"kelembaban":65.0}`
- **HTTP Response Code:** misalnya `200`
- **Response Body:** salin bagian response yang menunjukkan data diterima server.
- **Status Pengiriman:** berdasarkan hasil pengamatan, berhasil atau gagal.

---

## Percobaan 2B: Komunikasi MQTT

### Tabel 2. Hasil Komunikasi Data melalui MQTT

| No. | Waktu Pengiriman (s) | Status Broker | Topic | Data JSON yang Dipublish | Data Diterima Subscriber | Status Pengiriman |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 1 | 0 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 2 | 10 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 3 | 20 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 4 | 30 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 5 | 40 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 6 | 50 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 7 | 60 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 8 | 70 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 9 | 80 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
| 10 | 90 | Terhubung | `unsoed/tk245004/nim10/sensor` | `{"suhu":28.5,"kelembaban":65}` | `{"suhu":28.5,"kelembaban":65}` | Berhasil |
