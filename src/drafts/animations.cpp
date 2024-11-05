//goofy functionality, but animations work


#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT; // Bluetooth serial instance
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* SCL=*/ 9, /* SDA=*/ 8);

bool btConnected = false; // Bluetooth connection state
unsigned long lastUpdate = 0;
int dots = 0;
static const unsigned char image_download_bits[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xff,0x19,0xc0,0x38,0x00,0xe7,0xff,0x0c,0xe0,0x00,0x63,0x18,0x70,0x00,0x00,0xc0,0x80,0x19,0xc0,0x38,0x80,0x67,0x00,0x06,0xe0,0x01,0x63,0x18,0x1c,0x00,0x00,0x60,0x00,0x1b,0xc0,0x7c,0xc0,0x67,0x00,0x06,0xe0,0x03,0x63,0x18,0x06,0x00,0x00,0x60,0x00,0x18,0x60,0x6c,0xe0,0x36,0x00,0x06,0x60,0x87,0x61,0x98,0x03,0x00,0x00,0x60,0x00,0xf8,0x7f,0xcc,0x60,0xf3,0x3f,0x06,0x30,0x8e,0x61,0xfc,0x00,0x00,0x00,0x60,0x00,0xf8,0x7f,0xcc,0x30,0xf3,0x3f,0x06,0x30,0x8c,0x31,0xfc,0x00,0x00,0x00,0x30,0x00,0x0c,0x60,0x8c,0x19,0x33,0x00,0x06,0x30,0x98,0x31,0x8c,0x01,0x00,0x00,0x30,0x80,0x0d,0x60,0x86,0x0d,0x33,0x00,0x03,0x30,0xb0,0x31,0x0c,0x07,0x00,0x00,0x70,0xc0,0x0c,0x30,0x06,0x07,0x1b,0x00,0x03,0x30,0xf0,0x31,0x0c,0x0e,0x00,0x00,0xe0,0xff,0x0c,0x30,0x06,0x03,0xf9,0x3f,0xff,0x13,0xe0,0x30,0x06,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe3,0x00,0xf1,0x3f,0x0c,0x30,0xfe,0xe3,0xff,0xfc,0x1f,0xe3,0x7f,0xfc,0x07,0x00,0xe3,0x81,0x39,0x70,0x0c,0x30,0x07,0x07,0x0e,0x0c,0x18,0x73,0x00,0x1e,0x0c,0x00,0xb3,0x81,0x19,0xc0,0x06,0xb0,0x01,0x06,0x06,0x06,0x98,0x31,0x00,0x06,0x0c,0x80,0x31,0x83,0x19,0xc0,0x06,0xb0,0x01,0x00,0x06,0x06,0x98,0x31,0x00,0x06,0x00,0x80,0x31,0x86,0x19,0xc0,0x06,0x98,0xff,0x00,0x06,0x06,0x9c,0xf1,0x3f,0xfe,0x03,0x80,0x31,0x8e,0x19,0x60,0x06,0x18,0xff,0x03,0x06,0xfe,0x8f,0xf1,0x3f,0xfc,0x0f,0x80,0x31,0x9c,0x18,0x60,0x06,0x18,0x00,0x03,0x06,0x06,0x8c,0x19,0x00,0x00,0x0c,0x80,0x19,0xf8,0x0c,0x60,0x06,0xd8,0x00,0x03,0x03,0x03,0x8c,0x19,0x00,0x03,0x0c,0x80,0x18,0xf0,0x0c,0x30,0x06,0xcc,0x81,0x03,0x03,0x03,0xcc,0x18,0x00,0x03,0x06,0xc0,0x18,0xe0,0xfc,0x1f,0xfe,0x87,0xff,0x01,0x03,0x03,0xcc,0xf8,0x3f,0xff,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const unsigned char bt_disconnected[] = {0x40,0x00,0xc1,0x00,0x42,0x01,0x44,0x02,0x48,0x04,0x10,0x04,0x20,0x02,0x40,0x00,0xa0,0x00,0x50,0x01,0x48,0x02,0x44,0x04,0x40,0x0a,0x40,0x11,0x80,0x20,0x00,0x00};

void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  
  // Show the logo (Paste your logo code from LOPAKA assets here)
  u8g2.drawXBM(0, -1, 128, 64, image_download_bits);
  u8g2.sendBuffer();
  delay(2000); // Hold logo for 2 seconds
  
  // Initialize Bluetooth
  SerialBT.begin("SmartPen");
  SerialBT.register_callback(btCallback); // Set callback for BT events
  
  // Show "Searching..." with animation
  showConnectionStatus("Searching", false);
}

void loop() {
  if (btConnected) {
    showConnectionStatus("Connected", true);
    u8g2.clearBuffer();
    u8g2.drawStr(0, 20, "Waiting for text...");
    u8g2.sendBuffer();
    delay(1000);
  } else {
    animateLoading("Searching");
  }
}

// Bluetooth callback
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    btConnected = true;
  } else if (event == ESP_SPP_CLOSE_EVT) {
    btConnected = false;
  }
}

// Show Bluetooth status and loading animation
void showConnectionStatus(const char* text, bool connected) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 15, text);
  
  // Draw Bluetooth icon (paste Bluetooth icon assets here from LOPAKA)
  if (connected) {
    u8g2.drawXBM(114, 1, 14, 16, bt_connected);
  } else {
    u8g2.drawXBM(114, 1, 14, 16, bt_disconnected);
  }
  u8g2.sendBuffer();
}

void animateLoading(const char* message) {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastUpdate > 500) { // Update every 500ms
    lastUpdate = currentMillis;
    dots = (dots + 1) % 4; // Rotate dots

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 15, message);

    // Add animated dots
    for (int i = 0; i < dots; i++) {
      u8g2.drawStr(60 + (i * 4), 15, ".");
    }

    // Draw Bluetooth icon based on connection status
    if (btConnected) {
      u8g2.drawXBM(114, 1, 14, 16, bt_connected);
    } else {
      u8g2.drawXBM(114, 1, 14, 16, bt_disconnected);
    }

    u8g2.sendBuffer();
  }
}