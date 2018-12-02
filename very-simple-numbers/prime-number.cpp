#include <iostream>
#include <cmath>
#include <string>

long double max = pow(2, 32);

bool isPrime(unsigned long long int number) {
  for (unsigned long long int i = 2; i <= sqrt(number); i++) {
    if (number % i == 0)
      return false;
  }
  return true;
}

std::string decToBin (unsigned long long int number) {
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

int itrerator(unsigned long long int number) {
  int amount = 0;
  for (int i = 0; i < 10; i++) {
    unsigned long long int subNumber = number * 10 + i;
    if (subNumber >= max)
      continue;
    if (isPrime(subNumber)) {
      if (subNumber != 0) {
        amount++;
        std::cout << subNumber << ":" << decToBin(subNumber) << "\n";
        amount += itrerator(subNumber);
      }
    }
  }
  return amount;
}

int main() {
  int amount = itrerator(0);
  std::cout << "\n-----\n" << amount << "\n";
}