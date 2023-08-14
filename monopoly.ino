#include "Keys.h"
#include "Rfid.h"
#include "Screen.h"
#include "Game.h"

void setup() {
  Serial.begin(9600);
  rfid.begin();
  screen.begin();

  pinMode(A7, INPUT);
  randomSeed(micros() * analogRead(A7));
}

void loop() {
  screen.setRoof("Monopoly", bm::money_13x13);

  uint8_t menuSize = 3;
  char *menuItems[menuSize] = { "Новая игра", "Продолжить", "Настройки" };
  screen.setMenu(menuItems, menuSize);

  while (1) {
    auto key = keys.getKey();
    auto enterIdx = screen.update(key);

    if (enterIdx != -1) {
      switch (enterIdx) {
        case 0:
          uint8_t playersCount = inputPlayersCount();
          play(playersCount);
          break;
      }
      break;
    }
  }
}

uint8_t inputPlayersCount() {
  screen.setNumberInput();
  int8_t result = 0;

  while (1) {
    result = screen.update(keys.getKey());
    if (result > 1)
      break;
  }

  return result;
}

void play(uint8_t playersCount) {
  Game game(playersCount);
  game.start();
  // Serial.print("Play ");
  // Serial.println(playersCount);
}


// int id = rfid.readCard();
// char key = keys.getKey();

// if (id)
//   Serial.println(id);
// if (key == 'A')
// {
//   screen.printMenu();
// } else
//   return;

// delay(20);