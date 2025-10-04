# ❄️ Smart Mini Freezer Monitoring & Control System  

Proyek ini adalah **sistem monitoring dan pengendalian mini freezer berbasis Arduino & ESP32** yang terhubung ke **Blynk IoT**. Alat ini mampu menjaga suhu freezer tetap stabil, memberikan alarm saat kondisi tidak normal, serta memungkinkan pemantauan jarak jauh melalui aplikasi Blynk.
Sistem ini merupakan Smart Mini Freezer Monitoring & Control berbasis Arduino dan ESP32 yang dirancang untuk memantau serta mengendalikan suhu freezer secara otomatis maupun melalui simulasi. Arduino berfungsi membaca suhu dari sensor DS18B20, menampilkan informasi pada LCD 16x2, serta mengendalikan kipas dan buzzer berdasarkan ambang batas suhu yang ditentukan. Data kemudian dikirim dalam format JSON melalui komunikasi serial ke ESP32, yang selanjutnya meneruskan informasi tersebut ke platform Blynk IoT agar dapat dipantau secara real-time melalui smartphone.

---

## ⚙️ Fitur Utama
- 🔥 **Monitoring suhu real-time** dengan sensor DS18B20.  
- ❄️ **Kontrol kipas otomatis**:  
  - ON jika suhu > 8 °C.  
  - OFF jika ≤ 8 °C.  
- 🔊 **Alarm buzzer** dengan pola berkedip saat suhu melewati ambang batas.  
- 🖥️ **LCD 16x2 I2C** menampilkan mode, suhu, dan status kipas.  
- 🔘 **Tombol kontrol**:  
  - Tekan singkat: menambah/mengurangi suhu (hanya di **mode Simulasi**).  
  - Tekan lama (5 detik): mengganti mode **AUTO ↔ SIMULASI**.  
- 📡 **Komunikasi Serial (JSON)** Arduino ↔ ESP32.  
- 🌐 **Integrasi Blynk IoT**:  
  - `V0` → Gauge/Value Display suhu.  
  - `V1` → Label mode (AUTO/SIMULASI).  
  - `V2` → LED Widget kipas (ON/OFF).  
  - `V3` → LCD Widget menampilkan suhu & mode secara lengkap.  

---

## 🛠️ Hardware
- Arduino UNO / Nano / kompatibel  
- ESP32  
- Sensor suhu **DS18B20**  
- **LCD 16x2 I2C**  
- **Buzzer**  
- **Fan/Relay driver**  
- Tombol (2 untuk INC/DEC, 1 untuk Mode Switch)  
- Kabel jumper  

---

## 📐 Wiring Pin
### Arduino
- DS18B20 -> Pin 4
- BTN_INC -> Pin 7
- BTN_DEC -> Pin 6
- FAN -> Pin 9
- BUZZER -> Pin 5
- TX (JSON) -> Pin 8 (ke ESP32 RX)
### ESP32
- RX -> Pin 18 (menerima data dari Arduino)
- TX -> Pin 19 (opsional, untuk balasan)
- WiFi -> SSID/PASS sesuai konfigurasi

---

## 🚀 Cara Kerja
1. **Arduino** membaca suhu dari sensor DS18B20.  
2. Data suhu ditampilkan pada LCD bersama status kipas & mode.  
3. Jika suhu melewati ambang batas, kipas aktif & buzzer berbunyi.  
4. Data dikirim via **Serial JSON** ke ESP32 dengan format:  
   ```json
   {"temp":7.5,"mode":"AUTO","fan":1}
5. ESP32 menerima JSON, memprosesnya, lalu mengirimkan ke Blynk IoT.
6. Pengguna dapat memantau freezer secara real-time melalui aplikasi Blynk di smartphone.

---

## 📸 Tampilan LCD
- Baris 1: Mode operasi (AUTO / SIMULASI).
- Baris 2: Suhu (°C) & status kipas (ON/OFF).

---

## ✨ Lisensi
Proyek ini bersifat open-source. Silakan digunakan, dimodifikasi, dan dikembangkan lebih lanjut.  

---

## Contacs us : 
* [Frendi RoboTech](https://www.instagram.com/frendi.co/)
* [Whatsapp : +6287888227410](https://wa.me/+6287888227410)
* [Email    : frendirobotech@gmail.com](https://mail.google.com/mail/u/0/?view=cm&tf=1&fs=1&to=frendirobotech@gmail.com) atau [Email    : frendix45@gmail.com](https://mail.google.com/mail/u/0/?view=cm&tf=1&fs=1&to=frendix45@gmail.com)

---

## 👨‍💻 Author
Dikembangkan oleh: [Reog Robotic & Robotech Electronics]  
Lisensi: Open Source (MIT)
