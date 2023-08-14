#pragma once
#include "GameConfigs.h"

class Player {
private:
  uint8_t position;
  uint16_t balance;

  char* name;
public:
  Player()
    : position(0), balance(gc::PlayerInitBalance) {}

  void move(uint8_t steps) {
    position += steps;
    if (position >= gc::FieldSize) {
      position %= gc::FieldSize;
      balance += 2000;
    }
  }

  uint8_t getPos() {
    return position;
  }

  void setName(uint8_t idx) {
    name = gc::PlayerNames[idx];
  }

  char *getName() {
    return name;
  }
};