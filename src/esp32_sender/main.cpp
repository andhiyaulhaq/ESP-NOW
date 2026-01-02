#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "common.h"

// struct_message is defined in common.h
struct_message myData;
esp_now_peer_info_t peerInfo;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("[ESP32 TX] Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

	// Sets the device to "Station Mode" (Wi-Fi Client). This turns on the Wi-Fi radio, which is required for ESP-NOW to function.
  WiFi.mode(WIFI_STA);

	// Starts the ESP-NOW protocol. If this fails, the code stops.
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

	// Tells the ESP32, "Hey, whenever you send a message, run the OnDataSent function so I know if it worked."
  esp_now_register_send_cb(OnDataSent);

  // Register Peer
	// memcpy() : Copies memory blocks; used here to copy the MAC address.
	// memcpy(dest, src, n)
	// Arg 1 (Destination): Where to paste the data (the empty array waiting to be filled).
	// Arg 2 (Source): Where to copy the data from (the existing array holding the information).
	// Arg 3 (Size): The number of bytes to copy (how much "stuff" to move).
	// broadcastAddress is defined in common.h
	// sizeof(peerInfo.peer_addr): Gets the size of the peer_addr array to ensure the correct number of bytes are copied.
  memcpy(peerInfo.peer_addr, broadcastAddress, sizeof(peerInfo.peer_addr));

  peerInfo.channel = 0; 			// Uses the current Wi-Fi channel.
  peerInfo.encrypt = false; 	// Disables encryption for simplicity.

	// Registers the peer device with ESP-NOW. If this fails, the code stops.
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
	// Checks if 2000 milliseconds (2 seconds) have passed since the last transmission
  if ((millis() - lastTime) > timerDelay) {
    // Prepare data

		// It copies the text "Hello from ESP32" into myData.msg
    strcpy(myData.msg, "Hello from ESP32");
    // It sets myData.counter to the number of seconds since the program started
		myData.counter = millis() / 1000;

    // Send data
		// Arg 1: Where to send it (the receiver's MAC address).
		// Arg 2: The data itself (cast to a byte array).
		// Arg 3: The size of the data.
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}
