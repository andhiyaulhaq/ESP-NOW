#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

// Universal structure for all boards
typedef struct struct_message {
  char msg[32];
  uint32_t seq_num; // Used to track if packets are missing
} struct_message;

// Broadcast Address (Sends to everyone)
static uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#endif
