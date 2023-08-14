#pragma once

#include <GyverOLED.h>
#include "Bitmaps.h"


enum class cnd {
  menu,
  cube,
  input,
  other
};

class Screen {
private:
  GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

  char *menuText[16];

  uint8_t menuSize;
  uint8_t menuPointer;
  uint8_t screenPointer;

  uint8_t inputValue;

  cnd condition;

  uint8_t menuEnter() {
    return menuPointer;
  }
  void menuBack() {}

  void menuUp() {
    if (menuPointer > 0)
      if ((menuPointer == 1) || (menuPointer == menuSize - 1)) {
        --menuPointer;
        --screenPointer;
      } else {
        --menuPointer;
      }
  }
  void menuDown() {
    if (menuPointer < menuSize - 1)
      if ((menuPointer == 0) || (menuPointer == menuSize - 2)) {
        ++menuPointer;
        ++screenPointer;
      } else {
        ++menuPointer;
      }
  }

  void menuRight() {}
  void menuLeft() {}

  void updateMenu() {
    oled.clear(0, 13, 128, 64);

    for (uint8_t i = 0; i < 3; ++i) {
      oled.setScale(i == screenPointer ? 2 : 1);
      oled.setCursorXY(0, 16 + 16 * i + (i == screenPointer ? 0 : 4));
      oled.print(menuText[menuPointer + i - screenPointer]);
    }
  }

public:
  Screen()
    : oled(), condition(cnd::other), inputValue(0) {}

  void begin() {
    oled.init();
    oled.setContrast(255);

    delay(20);
    oled.clear();
    oled.update();
    // oled.autoPrintln(true);
  }

  void clear() {
    oled.clear();
    oled.update();
    oled.home();
  }

  void update() {
    oled.update();
  }

  int8_t update(char key) {
    if (key && (condition == cnd::menu)) {
      switch (key) {
        case 'A':
          return menuEnter();
          break;
        case 'B':
          menuBack();
          break;
        case 'C':
          menuUp();
          break;
        case 'D':
          menuDown();
          break;
        case '*':
          menuLeft();
          break;
        case '#':
          menuRight();
          break;
      }
      updateMenu();
    } else if (key && (condition == cnd::input)) {
      switch (key) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          inputValue = (uint8_t)key - 48;
          setNumberInput();
          break;
        case 'A':
          if (inputValue)
            return inputValue;
          break;
        case 'B':
          inputValue = 0;
          break;
      }
    }

    update();
    return -1;
  }

  void setRoof(char *text, const uint8_t *bitmap_13x13 = bm::money_13x13) {
    oled.clear(0, 0, 128, 13);
    oled.drawBitmap(0, 0, bitmap_13x13, 13, 13);

    oled.setScale(1);
    oled.setCursorXY(18, 3);
    oled.print(text);
    update();
  }

  void setMenu(const char *text_array[], const uint8_t size) {
    oled.clear(0, 13, 128, 64);
    condition = cnd::menu;

    menuSize = size;
    menuPointer = 0;
    screenPointer = 0;

    for (uint8_t i = 0; i < menuSize; ++i) {
      menuText[i] = text_array[i];
    }

    updateMenu();
  }

  void setNumberInput() {
    oled.clear(0, 13, 128, 64);
    condition = cnd::input;

    oled.setScale(2);
    oled.setCursor(0, 2);
    oled.print("Сколько");
    oled.setCursor(0, 4);
    oled.print("игроков?");

    oled.setScale(3);
    oled.setCursor(106, 3);
    oled.print(inputValue);
  }

  void showTheDice(uint8_t score, bool isDouble) {
    char *text = (isDouble ? "Дубль!" : "Дубля нет.");

    clear();

    oled.setScale(2);
    oled.println(text);

    oled.setCursor(75, 3);
    oled.setScale(4);
    oled.println(score);

    oled.drawBitmap(0, 15, bm::dice_70x51, 70, 51);

    oled.update();
  }

  void showTheCell(char *cellName) {
    clear();
    oled.setCursor(0, 2);
    oled.setScale(2);
    oled.autoPrintln(true);
    oled.print(cellName);
    Serial.println(cellName);
    oled.autoPrintln(false);
    update();
  }
};

Screen screen;