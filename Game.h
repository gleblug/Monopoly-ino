#pragma once

#include "Player.h"

class Game {
private:
  uint8_t playersCount;
  uint8_t currentPlayer;

public:
  Game() = delete;

  Game(uint8_t pc)
    : playersCount(pc), currentPlayer(0) {}

  start() {
    screen.clear();

    Player players[playersCount];

    while (1) {
      Player &p = players[currentPlayer];
      p.setName(currentPlayer);

      auto steps = rollTheDice();
      p.move(steps);
      showTheCell(p.getPos());

      showPlayerMenu(p);

      if (currentPlayer == playersCount - 1) currentPlayer = 0;
      else ++currentPlayer;
    }
  }

  uint8_t rollTheDice() {
    auto dice1 = random(1, 6);
    auto dice2 = random(1, 6);
    uint8_t score = dice1 + dice2;
    bool isDouble = (dice1 == dice2);
    screen.showTheDice(score, isDouble);

    while (1)
      if (keys.getKey() == '*') break;

    if (isDouble) --currentPlayer; 
    
    return score;
  }

  void showTheCell(uint8_t pos) {
    auto text = gc::FieldCells[pos];
    screen.showTheCell(text);

    while (1)
      if (keys.getKey() == '*') break;
  }

  void showPlayerMenu(Player &p) {
    screen.clear();
    screen.setRoof(p.getName());
    while (1)
      if (keys.getKey() == '*') break;
  }
};