#include <iostream>
#include <string>
#include <sstream>

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