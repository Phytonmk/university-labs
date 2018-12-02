#include <ncurses.h>
#include <iostream>
#include "inputNumber/inputNumber.h"
#include <time.h>
#include <stdlib.h>

struct coords {
  int x, y;
};

int gameBoard[5][5];
struct coords cursor = {0, 0}; 
bool machineGoes = false;

void drawGrid(int width, int height) {
  for (int x = 1; x < width; x++) {
    for (int y = 0; y < height * 4 - 1; y++) {
      mvprintw(y, x * 4 - 1 , "|");
    }
  } // | | |
  for (int y = 1; y < height; y++) {
    for (int x = 0; x < width * 4; x++) {
      mvprintw(y * 4 - 1, x , "-");
    }
  }
  /*
  -
  -
  -
  */
}

void drawElements(int width, int height) {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int outputX = 1 + x * 4;
      int outputY = 1 + y * 4;
      mvprintw(outputY, outputX - 1, " ");
      mvprintw(outputY, outputX + 1, " ");
      switch(gameBoard[x][y]) {
        case 0:
          mvprintw(outputY, outputX, " ");
          break;
        case 1:
          mvprintw(outputY, outputX, "x");
          break;
        case 2:
          mvprintw(outputY, outputX, "o");
          break;
      }
    }
  }
  mvprintw(1 + cursor.y * 4, 1 + cursor.x * 4 - 1, ">");
  mvprintw(1 + cursor.y * 4, 1 + cursor.x * 4 + 1, "<");
}

bool isWin(int width, int height) {
  bool won = false;
  int winLineDiraction = 0, winLineX = 0, winLineY = 0;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (gameBoard[x][y] != 0) {
        if (y >= 1 && gameBoard[x][y] == gameBoard[x][y - 1] && y < height - 1 && gameBoard[x][y] == gameBoard[x][y + 1])
          winLineDiraction = 1;
        else if (y >= 1 && x >= 1 && gameBoard[x][y] == gameBoard[x - 1][y - 1] && y < height - 1 && x < width - 1 && gameBoard[x][y] == gameBoard[x + 1][y + 1])
          winLineDiraction = 2;
        else if (x >= 1 && gameBoard[x][y] == gameBoard[x - 1][y] && x < width - 1 && gameBoard[x][y] == gameBoard[x + 1][y])
          winLineDiraction = 3;
        else if (y < height - 1 && x >= 1 && gameBoard[x][y] == gameBoard[x - 1][y + 1] && y >= 1 && x < width - 1 && gameBoard[x][y] == gameBoard[x + 1][y - 1])
          winLineDiraction = 4;
        if (winLineDiraction != 0) {
          won = true;
          winLineX = 1 + x * 4;
          winLineY = 1 + y * 4;
          break;
        }
      }
    }
    if (won)
      break;
  }
  if (won) {
    drawElements(width, height);
    switch(winLineDiraction) {
      case 1:
        for (int i = 0; i < 3 * 3; i++)
          if (i % 2 != 0)
            mvprintw(winLineY - 4 + i, winLineX, "|");
        break;
      case 2:
        for (int i = 0; i < 3 * 3; i++)
          if (i % 2 != 0)
            mvprintw(winLineY - 4 + i, winLineX - 4 + i, "\\");
        break;
      case 3:
        for (int i = 0; i < 3 * 3; i++)
          if (i % 2 != 0)
            mvprintw(winLineY, winLineX - 4 + i, "-");
        break;
      case 4:
        for (int i = 0; i < 3 * 3; i++)
          if (i % 2 != 0)
            mvprintw(winLineY + 4 - i, winLineX - 4 + i, "/");
        break;
    }
    getch();
  }
  return won;
}

void machine(int width, int height) {
  if (machineGoes) {
    machineGoes = false;
    for (int i = 0; i < 1000; i++) {
      int maybeX = rand() % (width);
      int maybeY = rand() % (height);
      if (gameBoard[maybeX][maybeY] == 0) {
        gameBoard[maybeX][maybeY] = 2;
        break;
      }
    }
  }
}

bool gameTick(int width, int height) {
  drawGrid(width, height);
  drawElements(width, height);
  mvprintw(12, 0, "");
  if (isWin(width, height))
    return false;
  // machine(width, height);
  // if (isWin(width, height))
  //   return false;
  char firstInputChar = getch();
  if (firstInputChar == '\033') {
    getch();
    switch(getch()) {
      case 'A':
        if (cursor.y > 0)
          cursor.y--;
        break;
      case 'B':
        if (cursor.y < height - 1)
          cursor.y++;
        break;
      case 'C':
        if (cursor.x < width - 1)
          cursor.x++;
        break;
      case 'D':
        if (cursor.x > 0)
          cursor.x--;
        break;
    }
  } else if (firstInputChar == 'x' || firstInputChar == 'q') {
    return false;
  } else if (firstInputChar == ' ' && gameBoard[cursor.x][cursor.y] == 0) {
    gameBoard[cursor.x][cursor.y] = 1;
    machineGoes = true;
  }
  machine(width, height);
  if (isWin(width, height))
    return false;
  return true;
}

int main(int argc, char const *argv[]) {
  int width = 0, height = 0;
  for (int x = 0; x < width; x++)
    for (int y = 0; y < width; y++)
      gameBoard[x][y] = 0;
  srand(time(NULL));
  while (width < 2 || width > 5) {
    width = inputNumber("Input width (from 2 to 5) >");
  }
  while (height < 2 || height > 5) {
    height = inputNumber("Input height (from 2 to 5) >");
  }
  initscr();
  cbreak();
  noecho();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  timeout(60 * 60 * 1000);
  while (true) {
    if (!gameTick(width, height))
      break;
  }
  endwin();
  return 0;
}