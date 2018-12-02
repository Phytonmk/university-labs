#include <iostream>
#include <sstream>
#include <string>
#include <ncurses.h>

int getOuterCirclesOffset(int depth, int width, int height) {
  int result = 0;
  for (int i = 0; i < depth; i++) {
    int levelWidth = (width - i * 2) * 2;
    int levelHeight = (height - i * 2 - 2) * 2;
    if (levelWidth < 0)
      levelWidth = 0;
    if (levelHeight < 0)
      levelHeight = 0;
    result += levelWidth + levelHeight;
  }
  return result;
}

int getNumberPyPosition(int x, int y, int width, int height, bool clockwise, bool fromCenter) {
  int yDepth = y, xDepth = x;
  if (xDepth > (width - 1) / 2)
    xDepth = width - x - 1;
  if (yDepth > (height - 1) / 2)
    yDepth = height - y - 1;
  int depth = yDepth < xDepth ? yDepth : xDepth;
  int result = getOuterCirclesOffset(depth, width, height);
  int levelWidth = (width - depth * 2); 
  int levelHeight = (height - (depth) * 2);
  if (levelWidth < 0)
    levelWidth = 0;
  if (levelHeight < 0)
    levelHeight = 0;
  int levelX = x - depth;
  int levelY = y - depth;
  if (levelX < 0)
    levelX = 0;
  if (levelY < 0)
    levelY = 0;
  if (clockwise) {
    if (levelY == 0)
      result += levelX;
    else if (levelX == levelWidth - 1)
      result += levelWidth + levelY - 1;
    else if (levelY == levelHeight - 1)
      result += levelWidth + levelHeight - 2 + (levelWidth - levelX) - 1;
    else if (levelX == 0) {
      result += levelWidth * 2 + levelHeight - 2 + (levelHeight - 2 - levelY);
    }
  } else {
    if (levelX == 0)
      result += levelY;
    else if (levelY == levelHeight - 1)
      result += levelHeight + levelX - 1;
    else if (levelX == levelWidth - 1)
      result += levelWidth + levelHeight - 2 + (levelHeight - levelY) - 1;
    else if (levelY == 0) {
      result += levelHeight * 2 + levelWidth - 2 + (levelWidth - 2 - levelX);
    }
  }
  if (fromCenter) {
    return (abs(width * height) - 1 - result) + 1;
  }
  return result + 1;
}

int inputNumber(std::string placeholder) {
  std::cout << placeholder;
  std::string line;
  int number;
  while (std::getline(std::cin, line)) {
    std::stringstream ss(line);
    if (ss >> number) {
      if (ss.eof())
        return number;
    }
    std::cout << "Wrong input, try again: ";
  }
  return 0;
}

int main() {
  int width = inputNumber("Inut spirkle width (max 8): ");
  int height = inputNumber("Inut spirkle height (max 8): ");
  if (width > 8)
    width = 8;
  if (height > 8)
    height = 8;
  if (width < 0)
    width = 0;
  if (height < 0)
    height = 0;


  initscr();
  cbreak(); 
  noecho();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  mvprintw(4, 5, "Select spirkle type:");
  mvprintw(6, 7, "Clockwise to center");
  mvprintw(7, 7, "Clockwise from center");
  mvprintw(8, 7, "Anticlockwise to center");
  mvprintw(9, 7, "Anticlockwise from center");

  int selectedType = 0;

  while (true) {
    char key = getch();
    mvprintw(selectedType + 6, 5, " ");
    if (key == '\033') {
      getch();
      switch (getch()) {
        case 'A':
          if (selectedType > 0)
            selectedType--;
          break;
        case 'B':
          if (selectedType < 3)
            selectedType++;
          break;
      }
    } else if (key == '1' || key == '2' || key == '3' || key == '4') {
      switch (key) {
        case '1':
          selectedType = 0;
          break;
        case '2':
          selectedType = 1;
          break;
        case '3':
          selectedType = 2;
          break;
        case '4':
          selectedType = 3;
          break;
      }
    } else if (key == '\n' || key == ' ') {
      break;
    }
    mvprintw(selectedType + 6, 5, "*");
  }

  endwin();

  bool clockwise, fromCenter;

  switch (selectedType) {
    case 0:
      clockwise = true;
      fromCenter = false;
      break;
    case 1:
      clockwise = true;
      fromCenter = true;
      break;
    case 2:
      clockwise = false;
      fromCenter = false;
      break;
    case 3:
      clockwise = false;
      fromCenter = true;
      break;
  }

  printf("Selected type: %d\n\n", selectedType);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      printf("%3d", getNumberPyPosition(x, y, width, height, clockwise, fromCenter));
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}