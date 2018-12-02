#include <ncurses.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

struct coords {
  int x, y;
};
struct gameMap {
  int width, height;
};

int foodAmount = 5;

int keyboardCatcher(std::string &dir, bool &exitRequired, bool &dirChangeLocked) {
  while (true) {
    char firstChar = getch();
    if (firstChar == '\033' && !dirChangeLocked) {
      getch();
      switch(getch()) {
        case 'A':
          if (dir != "down")
            dir = "up";
          dirChangeLocked = true;
          break;
        case 'B':
          if (dir != "up")
            dir = "down";
          dirChangeLocked = true;
          break;
        case 'C':
          if (dir != "left")
            dir = "right";
          dirChangeLocked = true;
          break;
        case 'D':
          if (dir != "right")
            dir = "left";
          dirChangeLocked = true;
          break;
      }
    } else if (firstChar == 'q' || firstChar == 'Q') {
      exitRequired = true;
    }
    if (exitRequired)
      return 0;
    napms(10);
  }
}
struct gameMap askForMapSize() {
  struct gameMap snakeMap;
  snakeMap.width = 24;
  snakeMap.height = 24;
  std::cout << "Input map size (max 24x24, min 3x3)\n";
  std::cout << "Width: ";
  std::cin >> snakeMap.width;
  std::cout << "Height: ";
  std::cin >> snakeMap.height;
  if (snakeMap.width > 24)
    snakeMap.width = 24;
  if (snakeMap.height > 24)
    snakeMap.height = 24;
  if (snakeMap.width < 3)
    snakeMap.width = 3;
  if (snakeMap.height < 3)
    snakeMap.height = 3;
  std::cout << "Map size set as (" << snakeMap.width << "x" << snakeMap.height << ")\n";
  std::cout << "Game is going to be started";
  napms(1000 * 2);
  return snakeMap;
}
void renderGameBorders(struct gameMap snakeMap) {
  for (int i = 0; i <= snakeMap.height; i++) {
    mvprintw(i, 0, "|");
    mvprintw(i, snakeMap.width + 1, "|");
  }
  for (int i = 0; i <= snakeMap.width + 1; i++) {
    mvprintw(0, i, "-");
    mvprintw(snakeMap.height + 1, i, "-");
  }
}
void renderGameTick(std::vector<coords> snake, struct gameMap snakeMap, std::vector<coords> food, std::vector<coords> obstacles) {
  erase();
  renderGameBorders(snakeMap);
  for (int i = 0; i < (int)obstacles.size(); i++) {
    mvprintw(obstacles[i].y, obstacles[i].x, "x");
  }
  for (int f = 0; f < foodAmount; f++) {
    mvprintw(food[f].y, food[f].x, "o");
  }
  for (int i = 0; i < (int)snake.size(); i++) {
    mvprintw(snake[i].y, snake[i].x, i == 0 ? "@" : "#");
  }
  mvprintw(snake[0].y, snake[0].x, "@");
  mvprintw(snakeMap.height + 1, 0, "");
}

std::vector<coords> handleSnakePartsPostions(std::vector<coords> snake, std::string currentDir, int snakeSize, struct gameMap snakeMap, std::vector<coords> obstacles) {
  while (snake.size() > (unsigned int)snakeSize) {
    snake.pop_back();
  }
  if (snake.size() == 0) {
    struct coords singleHead;
    singleHead.x = snakeMap.width / 2;
    singleHead.y = snakeMap.height / 2;
    if (singleHead.x == 0)
      singleHead.x = 1;
    if (singleHead.y == 0)
      singleHead.y = 1;
    snake.push_back(singleHead);
  }
  struct coords tail;
  tail.x = snake[snake.size() - 1].x;
  tail.y = snake[snake.size() - 1].y;
  for (int i = (unsigned int)(snake.size() - 1); i > 0; i--) {
    snake[i].x = snake[i - 1].x;
    snake[i].y = snake[i - 1].y;
  }
  if (currentDir == "up") {
    snake[0].y = snake[0].y - 1;
    if (snake[0].y <= 0)
      snake[0].y = snakeMap.height;
  } else if (currentDir == "down") {
    snake[0].y = snake[0].y + 1;
    if (snake[0].y > snakeMap.height)
      snake[0].y = 1;
  } else if (currentDir == "left") {
    snake[0].x = snake[0].x - 1;
    if (snake[0].x <= 0)
      snake[0].x = snakeMap.width;
  } else if (currentDir == "right") {
    snake[0].x = snake[0].x + 1;
    if (snake[0].x > snakeMap.width)
      snake[0].x = 1;
  }
  if (snake.size() < (unsigned int)snakeSize) {
    snake.push_back(tail);
  }
  return snake;
}
bool checkIfWin(std::vector<coords> snake, struct gameMap snakeMap, std::vector<coords> obstacles) {
  return (int)snake.size() >= snakeMap.width * snakeMap.height - (int)obstacles.size() - 1;
}
struct coords genFoodPosition(struct gameMap snakeMap, std::vector<coords> currentFood) {
  struct coords food;
  food.x = 0;
  food.y = 0;
  while (true) {
    if (snakeMap.width >= 2)
      food.x = rand() % (snakeMap.width) + 1;
    else
      food.x = 1;
    if (snakeMap.height >= 2)
      food.y = rand() % (snakeMap.height) + 1;
    else
      food.y = 1;
    bool needToMove = false;
    for (int f = 0; f < foodAmount; f++) {
      if (currentFood[f].x == food.x && currentFood[f].y == food.y) {
        needToMove = true;
        break;
      }
    }
    if (!needToMove && food.x != 0 && food.y != 0) {
      break;
    }
  }
  return food;
}
std::vector<coords> handleFoodPosition(std::vector<coords> snake, std::vector<coords> food, struct gameMap snakeMap, std::vector<coords> obstacles) {
  for (int f = 0; f < foodAmount; f++) {
    int t = 0;
    while (true) {
      t++;
      if (t > 100)
        break;
      bool foodNotMoved = true;
      for (int i = 0; i < (int)snake.size(); i++) {
        if ((snake[i].x == food[f].x && snake[i].y == food[f].y) || (food[f].x == 0 && food[f].y == 0)) {
          food[f] = genFoodPosition(snakeMap, food);
          foodNotMoved = false;
        }
      }
      for (int i = 0; i < (int)obstacles.size(); i++) {
        if (food[f].x == obstacles[i].x && food[f].y == obstacles[i].y) {
          food[f] = genFoodPosition(snakeMap, food);
          foodNotMoved = false;
        }
      }
      if (foodNotMoved)
        break;
    }
  }
  return food;
}
int handleSnakeSize(std::vector<coords> snake, int snakeSize, std::vector<coords> food, struct gameMap snakeMap) {
  for (int f = 0; f < foodAmount; f++) {
    if (snake[0].x == food[f].x && snake[0].y == food[f].y) {
      food[f] = genFoodPosition(snakeMap, food);
      return snakeSize + 1;
    }
  }
  return snakeSize;
}
bool handleFail(std::vector<coords> snake, std::vector<coords> obstacles) {
  for (int i = 1; i < (int)snake.size(); i++) {
    if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
      return true;
    }
  }
  for (int i = 0; i < (int)snake.size(); i++) {
    for (int k = 0; k < (int)obstacles.size(); k++) {
      if (snake[i].x == obstacles[k].x && snake[i].y == obstacles[k].y) {
        return true;
      }
    }
  }
  return false;
}
int snakeGame(struct gameMap snakeMap, std::vector<coords> obstacles, int snakeSize) {
  std::string dir = "right";
  bool exitRequired = false;
  bool dirChangeLocked = false;
  std::thread keyboardThread(keyboardCatcher, std::ref(dir), std::ref(exitRequired), std::ref(dirChangeLocked));
  keyboardThread.detach();
  std::vector<coords> snake;
  std::vector<coords> food;
  for (int f = 0; f < foodAmount; f++) {
    struct coords oneFood = { 0, 0 };
    food.push_back(oneFood);
  }
  food = handleFoodPosition(snake, food, snakeMap, obstacles);
  int t = 0;
  while (true) {
    // if (dirChangeLocked) {
      t++;
      if (t > 100000)
        break;
      snake = handleSnakePartsPostions(snake, dir, snakeSize, snakeMap, obstacles);
      snakeSize = handleSnakeSize(snake, snakeSize, food, snakeMap);
      food = handleFoodPosition(snake, food, snakeMap, obstacles);
      if (checkIfWin(snake, snakeMap, obstacles) && false) {
        mvprintw(snakeMap.height + 3, 2, "You won, cool");
        // exitRequired = true;
        napms(10 * 1000);
      } else if (handleFail(snake, obstacles)) {
        renderGameTick(snake, snakeMap, food, obstacles);
        mvprintw(3, snakeMap.width + 5, "Game over");
        exitRequired = true;
        napms(10 * 1000);
      }
      if (exitRequired) {
        mvprintw(snakeMap.height + 3, 2, "Exit");
        break;
      } else {
        renderGameTick(snake, snakeMap, food, obstacles);
      }
      dirChangeLocked = false;
    // }
    napms(300);
  }
  endwin();
  return 0;
}
void renderObstaclesEditor(struct gameMap editorMap, std::vector<coords> obstacles, coords cursor) {
  erase();
  renderGameBorders(editorMap);
  for (int i = 0; i < (int)obstacles.size(); i++) {
    mvprintw(obstacles[i].y, obstacles[i].x, "x");
  }
  mvprintw(cursor.y, cursor.x, "@");
  mvprintw(editorMap.height + 1, 0, "");
}
std::vector<coords> obstaclesEditor(struct gameMap editorMap) {
  std::vector<coords> obstacles;
  struct coords cursor;
  cursor.x = 1;
  cursor.y = 1;
  while(true) {
    char firstChar = getch();
    if (firstChar == '\033') {
      getch();
      switch(getch()) {
        case 'A':
          cursor.y--;
          break;
        case 'B':
          cursor.y++;
          break;
        case 'C':
          cursor.x++;
          break;
        case 'D':
          cursor.x--;
          break;
      }
    } else if (firstChar == '\n') {
      return obstacles;
    } else if (firstChar == ' ') {
      int obstacleIndex = -1;
      for (int i = 0; i < (int)(obstacles.size()); i++) {
        if (obstacles[i].x == cursor.x && obstacles[i].y == cursor.y) {
          obstacleIndex = i;
          break;
        }
      }
      if (obstacleIndex != -1) {
        obstacles.erase(obstacles.begin() + obstacleIndex);
      } else {
        obstacles.push_back(cursor);
      }
    }
    renderObstaclesEditor(editorMap, obstacles, cursor);
  }
}
int main() {
  
  struct gameMap snakeMap;
  int snakeSize = 3;
  std::vector<coords> obstacles;
    
  snakeMap = askForMapSize();
  
  initscr();
  cbreak();
  noecho();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  obstacles = obstaclesEditor(snakeMap);
  snakeGame(snakeMap, obstacles, snakeSize);
  return 0;
}
