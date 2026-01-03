#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"

struct_message myData;
uint32_t last_seq_num = 0;
int lost_packets = 0;

// [LEGACY CALLBACK] This signature works with the standard PlatformIO ESP32 package (Core v2.x)
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  // 1. Calculate Packet Loss (Signal Quality Indicator)
  // If the new number isn't (last + 1), we missed some!
  if (last_seq_num != 0 && myData.seq_num != last_seq_num + 1) {
    int diff = myData.seq_num - last_seq_num - 1;
    lost_packets += diff;
    Serial.print(" [!!! LOST ");
    Serial.print(diff);
    Serial.print(" PACKETS !!!] ");
  }
  last_seq_num = myData.seq_num;

  // 2. Format MAC Address manually
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  // 3. Print Status
  Serial.print("SRC: ");
  Serial.print(macStr);
  Serial.print(" | Seq: ");
  Serial.print(myData.seq_num);
  Serial.print(" | Total Lost: ");
  Serial.println(lost_packets);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // [LEGACY REGISTRATION]
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("--- Legacy Signal Checker (Loss Counter) Started ---");
}

void loop() {
  yield();
}
