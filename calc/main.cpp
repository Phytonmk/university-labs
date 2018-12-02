#include <iostream>
#include <string.h>
#include <vector>
#include <ctype.h>
#include <stdexcept>
#include <ncurses.h>
#include <cmath>

const int screenHeight = 60;
const int screenWidth = 80;

struct monomial {
  float factor;
  char variable;
  float degree;
  bool rightHalf;
  int partnersFactor;
  bool numericFactor;
};

int charToInt(char symbol) {
  return (int) (symbol - '0');
}

std::vector<monomial> getPolynomialByString(std::string input) {
  std::vector<monomial> polynomial;
  int partnersFactor = 1;
  int monomialParsingStep = 0;
  bool leftHalf = true;
  bool yPresents = false;
  /*
    Steps:
    0 -- monomial is not found
    1 -- reading factor
    2 -- waiting for a character
    3 -- waiting for a degree
    4 -- starting reading degree
    5 -- reading degree
  */
  for (unsigned int i = 0; i < input.length(); i++) {
    if (isdigit(input.at(i))) {
      if (monomialParsingStep == 0) {
        monomial newMonomial;
        newMonomial.partnersFactor = partnersFactor;
        newMonomial.factor = charToInt(input.at(i));
        newMonomial.numericFactor = true;
        // newMonomial.factor *= leftHalf ? 1 : -1;
        newMonomial.rightHalf = !leftHalf;
        newMonomial.degree = 1;
        polynomial.push_back(newMonomial);
        monomialParsingStep = 1;
      } else if (monomialParsingStep == 1) {
        float addToFactor = charToInt(input.at(i)) * (abs(polynomial[polynomial.size() - 1].factor) / polynomial[polynomial.size() - 1].factor);
        if (polynomial[polynomial.size() - 1].numericFactor) {
          polynomial[polynomial.size() - 1].factor = polynomial[polynomial.size() - 1].factor * 10 + addToFactor;
        } else {
          polynomial[polynomial.size() - 1].factor = + addToFactor;
        }
        polynomial[polynomial.size() - 1].numericFactor = true;
      } else if (monomialParsingStep == 4) {
        polynomial[polynomial.size() - 1].degree = charToInt(input.at(i));
        monomialParsingStep = 5;
      } else if (monomialParsingStep == 5) {
        polynomial[polynomial.size() - 1].degree = 
          polynomial[polynomial.size() - 1].degree * 10 + charToInt(input.at(i));
      } else {
        throw std::invalid_argument("Unexpected character at position " + std::to_string(i));
      }
    } else if (input.at(i) == '-'/* && monomialParsingStep == 0*/) {
      monomial newMonomial;
      newMonomial.partnersFactor = partnersFactor;
      newMonomial.factor = -1;
      newMonomial.numericFactor = false;
      // newMonomial.factor *= leftHalf ? 1 : -1;
      newMonomial.rightHalf = !leftHalf;
      newMonomial.degree = 1;
      polynomial.push_back(newMonomial);
      monomialParsingStep = 1;
    } else if (input.at(i) == '*' && monomialParsingStep == 1) {
      monomialParsingStep = 2;
    } else if ((input.at(i) == 'x' || input.at(i) == 'y') && (monomialParsingStep == 0 || monomialParsingStep == 1 || monomialParsingStep == 2)) {
      if (monomialParsingStep == 0) {
        monomial newMonomial;
        newMonomial.partnersFactor = partnersFactor;
        // newMonomial.factor = leftHalf ? 1 : -1;
        newMonomial.numericFactor = false;
        newMonomial.factor = 1;
        newMonomial.rightHalf = !leftHalf;
        newMonomial.degree = 1;
        polynomial.push_back(newMonomial);
      }
      if (input.at(i) == 'y') {
        if (yPresents)
          throw std::invalid_argument("Double Y monomial at position " + std::to_string(i));
        yPresents = true;
      }
      polynomial[polynomial.size() - 1].variable = input.at(i);
      monomialParsingStep = 3;
    } else if (input.at(i) == '^' && monomialParsingStep == 3) {
      monomialParsingStep = 4;
    } else if (/*(monomialParsingStep == 3 || monomialParsingStep == 4 || monomialParsingStep == 5) && */(/*input.at(i) == ' ' || */input.at(i) == '+')) {
      monomialParsingStep = 0;
    } else if (monomialParsingStep == 0 && (input.at(i) == ' ' || input.at(i) == '+')) {
      // do nothing
    } else if (input.at(i) == '=' && leftHalf == true) {
      monomialParsingStep = 0;
      leftHalf = false;
    } else if (input.at(i) == '(' && partnersFactor == 1) {
      if (polynomial.size() > 0)
        partnersFactor = polynomial[polynomial.size() - 1].factor;
      monomialParsingStep = 0;
       polynomial.pop_back();
    } else if (input.at(i) == ')') { 
      monomialParsingStep = 0;
      partnersFactor = 1;
    } else {
      throw std::invalid_argument("Unexpected character at position " + std::to_string(i));
    }
  }
  return polynomial;
}

struct FinalFunction {
  std::vector<monomial> polynomial;
  float factor;  
  float degree;  
};

FinalFunction convertPolynomialToFunction(std::vector<monomial> polynomial) {
  FinalFunction finalFunction;
  monomial yMonomial;
  bool yOnRightHalf;
  for (unsigned int i = 0; i < polynomial.size(); i++) {
    if (polynomial[i].variable == 'y') {
      yMonomial.factor = polynomial[i].factor * polynomial[i].partnersFactor;
      yMonomial.variable = polynomial[i].variable;
      yMonomial.degree = polynomial[i].degree;
      yOnRightHalf = !polynomial[i].rightHalf;
      polynomial.erase(polynomial.begin() + i);
      break;
    }
  }
  if (yMonomial.variable != 'y') {
    throw std::invalid_argument("No Y monomial");
  }
  for (unsigned int i = 0; i < polynomial.size(); i++) {
    polynomial[i].factor *= polynomial[i].partnersFactor;
    if (yOnRightHalf != polynomial[i].rightHalf)
      polynomial[i].factor *= -1;
  }
  finalFunction.polynomial = polynomial;
  finalFunction.factor = 1 / yMonomial.factor;
  finalFunction.degree = 1 / yMonomial.degree;
  return finalFunction;
}

std::vector<int> calculateFunction(FinalFunction finalFunction) {
  std::vector<float> floatChart;
  for (int x = -(screenWidth / 2); x < screenWidth / 2; x++) {
    float y = 0;
    for (unsigned int i = 0; i < finalFunction.polynomial.size(); i++) {
      int tmpY = 0;
      if (finalFunction.polynomial[i].variable == 'x')
        tmpY += std::pow(finalFunction.polynomial[i].factor * x, finalFunction.polynomial[i].degree);
      else
        tmpY += std::pow(finalFunction.polynomial[i].factor, finalFunction.polynomial[i].degree);
      // std::cout <<  (finalFunction.polynomial[i].factor / abs(finalFunction.polynomial[i].factor)) << "\n";
      if ((int) finalFunction.polynomial[i].degree % 2 == 0)
        tmpY *= (finalFunction.polynomial[i].factor / abs(finalFunction.polynomial[i].factor));
      y += tmpY;
    }
    y *= finalFunction.factor;
    y = std::pow(y, finalFunction.degree);
    floatChart.push_back(y);
  }
  float maxHeight = 0;
  for (unsigned int i = 0; i < floatChart.size(); i++) {
    if (abs(floatChart[i]) > maxHeight)
      maxHeight = abs(floatChart[i]);
  }
  std::vector<int> chart;
  for (unsigned int i = 0; i < floatChart.size(); i++) {
    int y = (int) (floatChart[i]/* * ((screenHeight / 2) / maxHeight)*/);
    chart.push_back(y);
  }
  return chart;
}

void drawChart(std::vector<int> chart) {
  initscr();
  cbreak();
  noecho();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  for (int x = 0; x < screenWidth; x++) {
    mvprintw(screenHeight / 2, x, "-");
  }
  for (int y = 0; y < screenHeight; y++) {
    mvprintw(y, screenWidth / 2, "|");
  }
  mvprintw(screenHeight / 2, screenWidth / 2, "+");
  for (unsigned int x = 0; x < chart.size(); x++) {
    mvprintw(screenHeight / 2 - chart[x], x, "*");
  }
  mvprintw(screenHeight + 5, 0, "");
  mvprintw(screenHeight + 5, 0, "");
  getch();
  napms(30000);
  endwin();
}

int main(int argc, char const *argv[]) {
  std::string input = argc == 2 ? argv[1] : "x^2+y^2=40";//"y + x^4=1";
  drawChart(calculateFunction(convertPolynomialToFunction(getPolynomialByString(input))));
  return 0;
}