#pragma once

#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN A0
#define RST_PIN A1

#define RELAX_TIME 1000  // ms

class Rfid {
private:
  MFRC522 sensor;
  uint8_t id;
  unsigned long timeout;

  uint8_t hash(byte *buffer, byte *bufferSize) {
    int val = 0;
    for (byte i = 0; i < bufferSize; i++) {
      val += (buffer[i] * (i + 1)) % 256;
    }

    if (!val)
      return 1;

    return val % 256;
  }

public:
  Rfid()
    : sensor(SS_PIN, RST_PIN), timeout(0) {}

  void begin() {
    SPI.begin();        // Init SPI bus
    sensor.PCD_Init();  // Init MFRC522
  }

  uint8_t readCard() {
    if (millis() - timeout < RELAX_TIME)
      return 0;

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!sensor.PICC_IsNewCardPresent())
      return 0;

    // Verify if the NUID has been readed
    if (!sensor.PICC_ReadCardSerial())
      return 0;

    // Calculate hash
    id = hash(sensor.uid.uidByte, sensor.uid.size);
    timeout = millis();

    // Halt PICC
    sensor.PICC_HaltA();
    // Stop encryption on PCD
    sensor.PCD_StopCrypto1();

    return id;
  }
};

Rfid rfid;
