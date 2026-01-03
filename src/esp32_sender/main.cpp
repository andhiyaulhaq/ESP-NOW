#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"

struct_message myData;
esp_now_peer_info_t peerInfo;

unsigned long lastTime = 0;
unsigned long timerDelay = 200; // Fast updates

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Optional: Uncomment to debug sender side
  Serial.print("[TX] Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Register Peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Prepare data
    strcpy(myData.msg, "Ping");
    myData.seq_num++; // Increment sequence number for the receiver to track

    // Send data
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}
