#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"

// struct_message is defined in common.h
struct_message myData;

// [CHANGED] Updated callback signature to 'esp_now_recv_info_t' to access RSSI
void OnDataRecv(const esp_now_recv_info_t * info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  // [NEW] Extract RSSI (Signal Strength) from the info structure
  int rssi = info->rx_ctrl->rssi;

  // Serial.print("[ESP32 RX] Msg: ");
  // Serial.print(myData.msg);
  // Serial.print(" | Time: ");
  // Serial.print(myData.counter);

  // // [NEW] Print the signal strength
  // Serial.print(" | RSSI: ");
  // Serial.print(rssi);
  // Serial.println(" dBm");

	// --- TELEPLOT FORMATTING (Required for Graph) ---
  // Format: >Label:Value

  // Plot RSSI
  Serial.print(">RSSI:");
  Serial.println(rssi);

  // Plot Packet Count (Time) to see if it freezes
  Serial.print(">Time:");
  Serial.println(myData.counter);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // It tells the ESP32: "Whenever a message hits our antenna, stop what you are doing and run the function called OnDataRecv."
  // [NOTE] Removed the explicit cast (esp_now_recv_cb_t) because the new signature matches automatically
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("[ESP32] Receiver Listening...");
}

void loop() {
  yield(); // Allow background WiFi tasks to run
}
