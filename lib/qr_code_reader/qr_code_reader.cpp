#include "qr_code_reader.h"
#include <ESP32QRCodeReader.h>

static ESP32QRCodeReader qr_reader(CAMERA_MODEL_WROVER_KIT);

void setupQRCodeReader() {
    qr_reader.setup();
    qr_reader.beginOnCore(1);
}

void startQRCodeScanning() {
    struct QRCodeData qrCodeData;
    if (qr_reader.receiveQrCode(&qrCodeData, 2000)) {  // Increase timeout as needed
        if (qrCodeData.valid) {
            Serial.print("QR Code Payload: ");
            Serial.println((const char *)qrCodeData.payload);
        } else {
            Serial.println("Invalid QR Code");
        }
    } else {
        Serial.println("No QR Code found.");
    }
}
