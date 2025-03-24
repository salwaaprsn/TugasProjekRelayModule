#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Inisialisasi LCD dengan alamat I2C 0x27, ukuran 16x2
LiquidCrystal_I2C lcd(0x27, 16, 4);

// Definisi pin dan tipe sensor DHT
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Definisi pin relay
#define RELAY_PIN 9

// Karakter khusus untuk suhu dan kelembaban
byte suhu[8] = {
    B00100,
    B01010,
    B01010,
    B01110,
    B11111,
    B11111,
    B01110,
    B00000
};

byte kelembaban[8] = {
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
    B00000
};

void setup() {
    // Inisialisasi LCD
    lcd.init();
    lcd.backlight();

    // Inisialisasi relay
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); // Relay mati di awal (asumsi aktif LOW)

    // Membuat karakter khusus
    lcd.createChar(1, kelembaban);
    lcd.createChar(2, suhu);

    // Menampilkan pesan awal
    lcd.setCursor(3, 0);
    lcd.print("Kelompok 6");
    lcd.setCursor(0, 1);
    lcd.print("Arduino Project");

    // Inisialisasi sensor DHT
    dht.begin();

    delay(2000);
    lcd.clear();

    // Menampilkan label pada LCD
    lcd.setCursor(0, 0);
    lcd.write(2);
    lcd.print(" Suhu: ");
    lcd.setCursor(0, 1);
    lcd.write(1);
    lcd.print(" Lembab: ");
}

void loop() {
    // Membaca data dari sensor DHT
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

    // Menangani kesalahan pembacaan sensor
    if (isnan(h) || isnan(t) || isnan(f)) {
        lcd.setCursor(8, 0);
        lcd.print("Error ");
        lcd.setCursor(10, 1);
        lcd.print("Error ");
        return;
    }

    // Kontrol relay: jika suhu >= 26 derajat, matikan lampu (relay off)
    if (t >= 26.0) {
        digitalWrite(RELAY_PIN, HIGH); // relay mati (asumsi aktif LOW)
    } else {
        digitalWrite(RELAY_PIN, LOW); // relay nyala
    }

    // Menampilkan suhu dan kelembaban pada LCD
    lcd.setCursor(8, 0);
    lcd.print(t, 1);
    lcd.print((char)223);
    lcd.print("C ");

    lcd.setCursor(10, 1);
    lcd.print(h, 0);
    lcd.print("% ");

    delay(1000);
}