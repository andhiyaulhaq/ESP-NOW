#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "common.h"

struct_message myData;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void OnDataSent(uint8_t *mac_addr, uint8_t status) {
  Serial.print("[ESP8266 TX] Status: ");
  Serial.println(status == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Prepare data
    strcpy(myData.msg, "Hello from ESP8266");
    myData.counter = millis() / 1000;

    // Send data
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}
