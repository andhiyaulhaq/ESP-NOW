#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"

struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("[ESP32 RX] Msg: ");
  Serial.print(myData.msg);
  Serial.print(" | Time: ");
  Serial.println(myData.counter);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  Serial.println("[ESP32] Receiver Listening...");
}

void loop() {
  yield(); // Allow background WiFi tasks to run
}
