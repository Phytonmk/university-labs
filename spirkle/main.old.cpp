#include <iostream>
#include <ncurses.h>
#include <string>

void outputNumber(int x, int y, int number) {
  std::string s = std::to_string(number);
  while (s.length() < 3) {
    s = " " + s;
  }
  char const *pchar = s.c_str();
  mvprintw(y, x, pchar);  
}

void printSpirkle(int width, int height) {
  std::string xDir = "right";
  std::string yDir = "bottom";
  std::string axis = "x";
  int x = 0, y = 0, i = 0;
  bool printInt = true;
  while (true) {
    printInt = true;
    if (axis == "x") {
      if (xDir == "right")
        x++;
      else
        x--;
      if (x > width || x < 0) {
        if (axis == "x") {
          axis = "y";
        } else {
          axis = "x";
        }
        if (yDir == "bottom")
          yDir = "top";
        else
          yDir = "bottom";
        printInt = false;
      }
    } else {
      if (yDir == "bottom")
        y++;
      else
        y--;
      if (y > height || y < 0) {
        if (axis == "x") {
          axis = "y";
        } else {
          axis = "x";
        }
        if (xDir == "left")
          xDir = "right";
        else
          xDir = "left";
        printInt = false;
      }
    }
    napms(300);
    if (printInt) {
      outputNumber(x, y, i);
      break;
    }
  }

}

int main() {
  // int width, height;
  // std::cout << "Input m: ";
  // std::cin >> width;
  // std::cout << "Input n: ";
  // std::cin >> height;
  // std::cout << "\n";
  // if (width > 8)
  //   width = 8;
  // if (width < 1)
  //   width = 1;
  // if (height > 8)
  //   height = 8;
  // if (height < 1)
  //   height = 1;


  initscr();
  cbreak(); 
  noecho();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  // outputNumber(10, 10, i);

  printSpirkle(8, 4);

  endwin();
  return 0;
}