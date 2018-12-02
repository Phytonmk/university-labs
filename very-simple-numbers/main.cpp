#include <iostream>   
#include <vector>
#include <cmath>
#include <string>
#include <thread>
#include <ncurses.h>

bool isSimple (long unsigned int number) {
  for (long unsigned int i = 2; i < sqrt(number); i++) {
    if (number % i == 0)
      return false;
  }
  return true;
}

bool isVerySimple (long unsigned int number) {
  while (number > 10) {
    if (!isSimple(number)) {
      return false;
    }
    number /= 10;
  }
  return true;
}

std::string decToBin (long unsigned int number) {
  std::string result = "";
  while (number >= 2) {
    if (number % 2 == 0)
      result = "0" + result;
    else
      result = "1" + result;
    number /= 2;
  }
  if (number == 0)
    result = "0" + result;
  else
    result = "1" + result;
  return result;
}

void handleNumbersRange (long unsigned int startNumber, long unsigned  int finishNumber, int &finishedThreads, int &simpleNumbersAmount) {
  for (long unsigned int currentNumber = startNumber; currentNumber <= finishNumber; currentNumber++) {
    if (isVerySimple(currentNumber)) {
      std::cout << simpleNumbersAmount << " - " << currentNumber << ":" << decToBin(currentNumber) << "\n";
      simpleNumbersAmount++;
    }
  }
  finishedThreads++;
}

int main() {
  long unsigned int startNumber = 0;
  long unsigned int finishNumber = pow(2, 32);
  int simpleNumbersAmount = 0;
  int threadsAmount = 1;
  int finishedThreads = 0;
  long unsigned int rangeSize = (finishNumber - startNumber) / threadsAmount;
  for (int threadIndex = 0; threadIndex < threadsAmount; threadIndex++) {
    std::thread numbersHandler(handleNumbersRange, rangeSize * threadIndex, rangeSize * (threadIndex + 1), std::ref(finishedThreads), std::ref(simpleNumbersAmount));
    numbersHandler.detach();
  }
  while (finishedThreads < threadsAmount);
  // std::cout << "------\nVery simple numbers amount: " << simpleNumbersAmount; 
  return 0;
}