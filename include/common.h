#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

// Universal structure for all boards
typedef struct struct_message {
  char msg[32];
  int counter;
} struct_message;

// Broadcast Address (Sends to everyone)
static uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#endif
