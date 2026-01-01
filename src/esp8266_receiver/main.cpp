#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "common.h"

struct_message myData;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("[ESP8266 RX] Msg: ");
  Serial.print(myData.msg);
  Serial.print(" | Time: ");
  Serial.println(myData.counter);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("[ESP8266] Receiver Listening...");
}

void loop() {
  yield(); // Allow background WiFi tasks to run
}
