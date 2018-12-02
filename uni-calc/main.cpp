#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

std::string alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
std::string calc(std::string first, std::string second, int basis, std::string operation);

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

long long int toDec (std::string number, int basis) {
  if (number == "infinity")
    return 0;
  long long int result = 0;
  for (int i = 0; i < number.length(); i++) {
    result += alphabet.find(number.at(i), 0) * pow(basis, number.length() - i - 1);
  }
  return result;
}

std::string sum(std::string first, std::string second, int basis) {
  std::string result = "";
  int buffer = 0;
  for (int i = first.length() - 1; i>= 0; i--) {
    int localSum = alphabet.find(first.at(i), 0) + alphabet.find(second.at(i), 0) + buffer;
    buffer = 0;
    while (localSum >= basis) {
      buffer++;
      localSum -= basis;  
    }
    result = alphabet.at(localSum) + result;
  }
  while (buffer > 0) {
    result = alphabet.at(buffer) + result;
    buffer -= basis;
  }
  return result;
}
std::string substruct(std::string first, std::string second, int basis) {
  std::string result = "";
  int buffer = 0;
  for (int i = first.length() - 1; i>= 0; i--) {
    int localSum = alphabet.find(first.at(i), 0) - alphabet.find(second.at(i), 0) - buffer;
    buffer = 0;
    while (localSum < 0) {
      buffer++;
      localSum += basis;  
    }
    result = alphabet.at(localSum) + result;
  }
  return result;
}
std::string multiply(std::string first, std::string second, int basis) {
  std::string result = "0";
  for (int i = 0; i < toDec(second, basis); i++) {
    result = calc(result, first, basis, "+");
  }
  return result;
}
std::string divide(std::string first, std::string second, int basis) {
  std::string result = "0";
  int i = 0;
  while (toDec(first, basis) >= toDec(second, basis)) {
    result = calc(result, "1", basis, "+");
    first = substruct(first, second, basis);
    i++;
    if (i > 1000000)
      return "infinity";
  }
  return result;
}

std::string calc(std::string first, std::string second, int basis, std::string operation) {
  bool firstGreater = true;
  if (first.length() < second.length()) {
    firstGreater = false;
  } else if (first.length() == second.length()) {
    for (int i = 0; i < first.length(); i++) {
      if (first.at(i) != second.at(i)) {
        if (alphabet.find(first.at(i)) < alphabet.find(second.at(i))) {
          firstGreater = false;
        }
        break;
      }
    }
  }
  if (!firstGreater) {
    std::string tmp = second;
    second = first;
    first = tmp;
  }
  while (second.length() < first.length())
    second = "0" + second;
  std::string result = "";
  if (operation == "+")
    result = sum(first, second, basis);
  if (operation == "-")
    result = substruct(first, second, basis);
  if (operation == "*")
    result = multiply(first, second, basis);
  if (operation == "/")
    result = divide(first, second, basis);
  if (result == "")
    result = "0";
  while (result.length() > 1 && result.at(0) == '0')
    result = result.substr(1, result.length() - 1);
  return result;
}

void tests();

int main(int argc, char const *argv[]) {
  
  if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 't') {
    tests();
    return 0;
  }

  int basis = 0;
  while (basis < 2 || basis > 36)
    basis = inputNumber("Input basis (2-36): ");
  std::string first = "";
  std::string second = "";
  std::cout << "Input first number: ";
  std::cin >> first;
  std::cout << "Input second number: ";
  std::cin >> second;
  for (int i = 0; i < first.length(); i++) {
    if (alphabet.find(first.at(i)) == std::string::npos) {
      std::cout << "Invalid characters in first number\n";
      return 0;
    }
  }
  for (int i = 0; i < second.length(); i++) {
    if (alphabet.find(second.at(i)) == std::string::npos) {
      std::cout << "Invalid characters in second number\n";
      return 0;
    }
  }
  std::string operation = "x";
  while (operation != "+" && operation != "-" && operation != "*" && operation != "/") {
    std::cout << "Input operation sign (+,-,*,/)\n";
    std::cin >> operation;
  }
  std::cout << "First number: " << first << "\nFirst number (decimal)" << toDec(first, basis) << "\n";
  std::cout << "Second number: " << second << "\nSecond number (decimal)" << toDec(second, basis) << "\n";
  std::cout << first << " " << operation << " " << second << " = ";
  std::cout << calc(first, second, basis, operation) << " (" << basis << "), ";
  std::cout << toDec(calc(first, second, basis, operation), basis) << " (decimal)\n";
  return 0;
}

void tests () {
int passed = 0, failed = 0;
  if (calc("2o", "22", 31, "-") == "m") {
    passed++;
  } else {
    std::cout << "Test 2o - 22 (31) is failed: must return m, returns: " << calc("2o", "22", 31, "-") << "\n";
    failed++;
  }
  if (calc("hp", "16", 35, "/") == "f") {
    passed++;
  } else {
    std::cout << "Test hp / 16 (35) is failed: must return f, returns: " << calc("hp", "16", 35, "/") << "\n";
    failed++;
  }
  if (calc("1132", "20", 4, "-") == "1112") {
    passed++;
  } else {
    std::cout << "Test 1132 - 20 (4) is failed: must return 1112, returns: " << calc("1132", "20", 4, "-") << "\n";
    failed++;
  }
  if (calc("292", "18d", 14, "*") == "44b8c") {
    passed++;
  } else {
    std::cout << "Test 292 * 18d (14) is failed: must return 44b8c, returns: " << calc("292", "18d", 14, "*") << "\n";
    failed++;
  }
  if (calc("3a", "1i", 23, "*") == "62j") {
    passed++;
  } else {
    std::cout << "Test 3a * 1i (23) is failed: must return 62j, returns: " << calc("3a", "1i", 23, "*") << "\n";
    failed++;
  }
  if (calc("1cd", "142", 23, "-") == "8b") {
    passed++;
  } else {
    std::cout << "Test 1cd - 142 (23) is failed: must return 8b, returns: " << calc("1cd", "142", 23, "-") << "\n";
    failed++;
  }
  if (calc("fj", "93", 25, "+") == "om") {
    passed++;
  } else {
    std::cout << "Test fj + 93 (25) is failed: must return om, returns: " << calc("fj", "93", 25, "+") << "\n";
    failed++;
  }
  if (calc("465", "250", 10, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 465 / 250 (10) is failed: must return 1, returns: " << calc("465", "250", 10, "/") << "\n";
    failed++;
  }
  if (calc("239", "159", 10, "+") == "398") {
    passed++;
  } else {
    std::cout << "Test 239 + 159 (10) is failed: must return 398, returns: " << calc("239", "159", 10, "+") << "\n";
    failed++;
  }
  if (calc("350", "331", 16, "-") == "1f") {
    passed++;
  } else {
    std::cout << "Test 350 - 331 (16) is failed: must return 1f, returns: " << calc("350", "331", 16, "-") << "\n";
    failed++;
  }
  if (calc("507", "2b4", 12, "-") == "213") {
    passed++;
  } else {
    std::cout << "Test 507 - 2b4 (12) is failed: must return 213, returns: " << calc("507", "2b4", 12, "-") << "\n";
    failed++;
  }
  if (calc("gd", "36", 18, "*") == "31d6") {
    passed++;
  } else {
    std::cout << "Test gd * 36 (18) is failed: must return 31d6, returns: " << calc("gd", "36", 18, "*") << "\n";
    failed++;
  }
  if (calc("33b", "1d2", 17, "-") == "179") {
    passed++;
  } else {
    std::cout << "Test 33b - 1d2 (17) is failed: must return 179, returns: " << calc("33b", "1d2", 17, "-") << "\n";
    failed++;
  }
  if (calc("ob", "f", 31, "/") == "1j") {
    passed++;
  } else {
    std::cout << "Test ob / f (31) is failed: must return 1j, returns: " << calc("ob", "f", 31, "/") << "\n";
    failed++;
  }
  if (calc("6o", "d", 30, "-") == "6b") {
    passed++;
  } else {
    std::cout << "Test 6o - d (30) is failed: must return 6b, returns: " << calc("6o", "d", 30, "-") << "\n";
    failed++;
  }
  if (calc("1020010", "202122", 3, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1020010 / 202122 (3) is failed: must return 1, returns: " << calc("1020010", "202122", 3, "/") << "\n";
    failed++;
  }
  if (calc("1220", "1", 4, "+") == "1221") {
    passed++;
  } else {
    std::cout << "Test 1220 + 1 (4) is failed: must return 1221, returns: " << calc("1220", "1", 4, "+") << "\n";
    failed++;
  }
  if (calc("359", "1ff", 16, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 359 / 1ff (16) is failed: must return 1, returns: " << calc("359", "1ff", 16, "/") << "\n";
    failed++;
  }
  if (calc("13123", "11311", 4, "-") == "1212") {
    passed++;
  } else {
    std::cout << "Test 13123 - 11311 (4) is failed: must return 1212, returns: " << calc("13123", "11311", 4, "-") << "\n";
    failed++;
  }
  if (calc("9w", "5l", 34, "-") == "4b") {
    passed++;
  } else {
    std::cout << "Test 9w - 5l (34) is failed: must return 4b, returns: " << calc("9w", "5l", 34, "-") << "\n";
    failed++;
  }
  if (calc("19e", "7", 27, "/") == "55") {
    passed++;
  } else {
    std::cout << "Test 19e / 7 (27) is failed: must return 55, returns: " << calc("19e", "7", 27, "/") << "\n";
    failed++;
  }
  if (calc("2d2", "20b", 19, "+") == "4dd") {
    passed++;
  } else {
    std::cout << "Test 2d2 + 20b (19) is failed: must return 4dd, returns: " << calc("2d2", "20b", 19, "+") << "\n";
    failed++;
  }
  if (calc("1d3", "1c0", 18, "-") == "13") {
    passed++;
  } else {
    std::cout << "Test 1d3 - 1c0 (18) is failed: must return 13, returns: " << calc("1d3", "1c0", 18, "-") << "\n";
    failed++;
  }
  if (calc("266", "224", 19, "*") == "4hei5") {
    passed++;
  } else {
    std::cout << "Test 266 * 224 (19) is failed: must return 4hei5, returns: " << calc("266", "224", 19, "*") << "\n";
    failed++;
  }
  if (calc("2262", "536", 7, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 2262 / 536 (7) is failed: must return 3, returns: " << calc("2262", "536", 7, "/") << "\n";
    failed++;
  }
  if (calc("62", "16", 8, "*") == "1274") {
    passed++;
  } else {
    std::cout << "Test 62 * 16 (8) is failed: must return 1274, returns: " << calc("62", "16", 8, "*") << "\n";
    failed++;
  }
  if (calc("1216", "430", 8, "+") == "1646") {
    passed++;
  } else {
    std::cout << "Test 1216 + 430 (8) is failed: must return 1646, returns: " << calc("1216", "430", 8, "+") << "\n";
    failed++;
  }
  if (calc("110012", "1222", 3, "/") == "20") {
    passed++;
  } else {
    std::cout << "Test 110012 / 1222 (3) is failed: must return 20, returns: " << calc("110012", "1222", 3, "/") << "\n";
    failed++;
  }
  if (calc("1d8", "nj", 25, "*") == "1bac2") {
    passed++;
  } else {
    std::cout << "Test 1d8 * nj (25) is failed: must return 1bac2, returns: " << calc("1d8", "nj", 25, "*") << "\n";
    failed++;
  }
  if (calc("192", "18", 11, "+") == "1aa") {
    passed++;
  } else {
    std::cout << "Test 192 + 18 (11) is failed: must return 1aa, returns: " << calc("192", "18", 11, "+") << "\n";
    failed++;
  }
  if (calc("145", "5c", 20, "-") == "id") {
    passed++;
  } else {
    std::cout << "Test 145 - 5c (20) is failed: must return id, returns: " << calc("145", "5c", 20, "-") << "\n";
    failed++;
  }
  if (calc("444", "291", 10, "*") == "129204") {
    passed++;
  } else {
    std::cout << "Test 444 * 291 (10) is failed: must return 129204, returns: " << calc("444", "291", 10, "*") << "\n";
    failed++;
  }
  if (calc("jl", "cp", 35, "-") == "6v") {
    passed++;
  } else {
    std::cout << "Test jl - cp (35) is failed: must return 6v, returns: " << calc("jl", "cp", 35, "-") << "\n";
    failed++;
  }
  if (calc("e4", "cj", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test e4 / cj (30) is failed: must return 1, returns: " << calc("e4", "cj", 30, "/") << "\n";
    failed++;
  }
  if (calc("b3", "92", 35, "+") == "k5") {
    passed++;
  } else {
    std::cout << "Test b3 + 92 (35) is failed: must return k5, returns: " << calc("b3", "92", 35, "+") << "\n";
    failed++;
  }
  if (calc("1n", "5", 24, "/") == "9") {
    passed++;
  } else {
    std::cout << "Test 1n / 5 (24) is failed: must return 9, returns: " << calc("1n", "5", 24, "/") << "\n";
    failed++;
  }
  if (calc("7g", "44", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 7g / 44 (33) is failed: must return 1, returns: " << calc("7g", "44", 33, "/") << "\n";
    failed++;
  }
  if (calc("1000110011", "100100101", 2, "*") == "101000010001011111") {
    passed++;
  } else {
    std::cout << "Test 1000110011 * 100100101 (2) is failed: must return 101000010001011111, returns: " << calc("1000110011", "100100101", 2, "*") << "\n";
    failed++;
  }
  if (calc("10102", "2001", 3, "*") == "20221102") {
    passed++;
  } else {
    std::cout << "Test 10102 * 2001 (3) is failed: must return 20221102, returns: " << calc("10102", "2001", 3, "*") << "\n";
    failed++;
  }
  if (calc("130", "1a", 11, "-") == "111") {
    passed++;
  } else {
    std::cout << "Test 130 - 1a (11) is failed: must return 111, returns: " << calc("130", "1a", 11, "-") << "\n";
    failed++;
  }
  if (calc("10n", "8f", 30, "*") == "8lff") {
    passed++;
  } else {
    std::cout << "Test 10n * 8f (30) is failed: must return 8lff, returns: " << calc("10n", "8f", 30, "*") << "\n";
    failed++;
  }
  if (calc("445", "242", 13, "+") == "687") {
    passed++;
  } else {
    std::cout << "Test 445 + 242 (13) is failed: must return 687, returns: " << calc("445", "242", 13, "+") << "\n";
    failed++;
  }
  if (calc("14e", "106", 17, "*") == "154bg") {
    passed++;
  } else {
    std::cout << "Test 14e * 106 (17) is failed: must return 154bg, returns: " << calc("14e", "106", 17, "*") << "\n";
    failed++;
  }
  if (calc("1a7", "49", 20, "+") == "1eg") {
    passed++;
  } else {
    std::cout << "Test 1a7 + 49 (20) is failed: must return 1eg, returns: " << calc("1a7", "49", 20, "+") << "\n";
    failed++;
  }
  if (calc("223", "32", 5, "-") == "141") {
    passed++;
  } else {
    std::cout << "Test 223 - 32 (5) is failed: must return 141, returns: " << calc("223", "32", 5, "-") << "\n";
    failed++;
  }
  if (calc("34", "4", 7, "+") == "41") {
    passed++;
  } else {
    std::cout << "Test 34 + 4 (7) is failed: must return 41, returns: " << calc("34", "4", 7, "+") << "\n";
    failed++;
  }
  if (calc("211002", "100012", 3, "*") == "21111202101") {
    passed++;
  } else {
    std::cout << "Test 211002 * 100012 (3) is failed: must return 21111202101, returns: " << calc("211002", "100012", 3, "*") << "\n";
    failed++;
  }
  if (calc("392", "350", 12, "-") == "42") {
    passed++;
  } else {
    std::cout << "Test 392 - 350 (12) is failed: must return 42, returns: " << calc("392", "350", 12, "-") << "\n";
    failed++;
  }
  if (calc("mh", "22", 24, "*") == "1n7a") {
    passed++;
  } else {
    std::cout << "Test mh * 22 (24) is failed: must return 1n7a, returns: " << calc("mh", "22", 24, "*") << "\n";
    failed++;
  }
  if (calc("oe", "ae", 25, "-") == "e0") {
    passed++;
  } else {
    std::cout << "Test oe - ae (25) is failed: must return e0, returns: " << calc("oe", "ae", 25, "-") << "\n";
    failed++;
  }
  if (calc("24d", "144", 15, "-") == "109") {
    passed++;
  } else {
    std::cout << "Test 24d - 144 (15) is failed: must return 109, returns: " << calc("24d", "144", 15, "-") << "\n";
    failed++;
  }
  if (calc("k9", "ap", 29, "-") == "9d") {
    passed++;
  } else {
    std::cout << "Test k9 - ap (29) is failed: must return 9d, returns: " << calc("k9", "ap", 29, "-") << "\n";
    failed++;
  }
  if (calc("38", "37", 26, "*") == "al4") {
    passed++;
  } else {
    std::cout << "Test 38 * 37 (26) is failed: must return al4, returns: " << calc("38", "37", 26, "*") << "\n";
    failed++;
  }
  if (calc("2103", "325", 6, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 2103 / 325 (6) is failed: must return 3, returns: " << calc("2103", "325", 6, "/") << "\n";
    failed++;
  }
  if (calc("1h", "19", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1h / 19 (33) is failed: must return 1, returns: " << calc("1h", "19", 33, "/") << "\n";
    failed++;
  }
  if (calc("439", "2e3", 15, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 439 / 2e3 (15) is failed: must return 1, returns: " << calc("439", "2e3", 15, "/") << "\n";
    failed++;
  }
  if (calc("mg", "ak", 31, "-") == "br") {
    passed++;
  } else {
    std::cout << "Test mg - ak (31) is failed: must return br, returns: " << calc("mg", "ak", 31, "-") << "\n";
    failed++;
  }
  if (calc("317", "2ab", 13, "+") == "5c5") {
    passed++;
  } else {
    std::cout << "Test 317 + 2ab (13) is failed: must return 5c5, returns: " << calc("317", "2ab", 13, "+") << "\n";
    failed++;
  }
  if (calc("c7", "11", 13, "*") == "1067") {
    passed++;
  } else {
    std::cout << "Test c7 * 11 (13) is failed: must return 1067, returns: " << calc("c7", "11", 13, "*") << "\n";
    failed++;
  }
  if (calc("12m", "ii", 26, "-") == "a4") {
    passed++;
  } else {
    std::cout << "Test 12m - ii (26) is failed: must return a4, returns: " << calc("12m", "ii", 26, "-") << "\n";
    failed++;
  }
  if (calc("ry", "lp", 35, "-") == "69") {
    passed++;
  } else {
    std::cout << "Test ry - lp (35) is failed: must return 69, returns: " << calc("ry", "lp", 35, "-") << "\n";
    failed++;
  }
  if (calc("i6", "9i", 27, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test i6 / 9i (27) is failed: must return 1, returns: " << calc("i6", "9i", 27, "/") << "\n";
    failed++;
  }
  if (calc("298", "293", 20, "+") == "4ib") {
    passed++;
  } else {
    std::cout << "Test 298 + 293 (20) is failed: must return 4ib, returns: " << calc("298", "293", 20, "+") << "\n";
    failed++;
  }
  if (calc("dj", "9i", 20, "-") == "41") {
    passed++;
  } else {
    std::cout << "Test dj - 9i (20) is failed: must return 41, returns: " << calc("dj", "9i", 20, "-") << "\n";
    failed++;
  }
  if (calc("p", "n", 26, "*") == "m3") {
    passed++;
  } else {
    std::cout << "Test p * n (26) is failed: must return m3, returns: " << calc("p", "n", 26, "*") << "\n";
    failed++;
  }
  if (calc("377", "154", 15, "*") == "4ad4d") {
    passed++;
  } else {
    std::cout << "Test 377 * 154 (15) is failed: must return 4ad4d, returns: " << calc("377", "154", 15, "*") << "\n";
    failed++;
  }
  if (calc("1c0", "139", 17, "-") == "88") {
    passed++;
  } else {
    std::cout << "Test 1c0 - 139 (17) is failed: must return 88, returns: " << calc("1c0", "139", 17, "-") << "\n";
    failed++;
  }
  if (calc("21120", "20221", 3, "+") == "112111") {
    passed++;
  } else {
    std::cout << "Test 21120 + 20221 (3) is failed: must return 112111, returns: " << calc("21120", "20221", 3, "+") << "\n";
    failed++;
  }
  if (calc("1a1", "147", 15, "+") == "2e8") {
    passed++;
  } else {
    std::cout << "Test 1a1 + 147 (15) is failed: must return 2e8, returns: " << calc("1a1", "147", 15, "+") << "\n";
    failed++;
  }
  if (calc("12004", "1101", 5, "*") == "13221404") {
    passed++;
  } else {
    std::cout << "Test 12004 * 1101 (5) is failed: must return 13221404, returns: " << calc("12004", "1101", 5, "*") << "\n";
    failed++;
  }
  if (calc("12122", "10122", 4, "-") == "2000") {
    passed++;
  } else {
    std::cout << "Test 12122 - 10122 (4) is failed: must return 2000, returns: " << calc("12122", "10122", 4, "-") << "\n";
    failed++;
  }
  if (calc("23333", "22211", 4, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 23333 / 22211 (4) is failed: must return 1, returns: " << calc("23333", "22211", 4, "/") << "\n";
    failed++;
  }
  if (calc("1c8", "i0", 21, "*") == "17ci0") {
    passed++;
  } else {
    std::cout << "Test 1c8 * i0 (21) is failed: must return 17ci0, returns: " << calc("1c8", "i0", 21, "*") << "\n";
    failed++;
  }
  if (calc("1455", "350", 6, "*") == "1100210") {
    passed++;
  } else {
    std::cout << "Test 1455 * 350 (6) is failed: must return 1100210, returns: " << calc("1455", "350", 6, "*") << "\n";
    failed++;
  }
  if (calc("in", "3w", 35, "-") == "eq") {
    passed++;
  } else {
    std::cout << "Test in - 3w (35) is failed: must return eq, returns: " << calc("in", "3w", 35, "-") << "\n";
    failed++;
  }
  if (calc("2aa", "171", 12, "*") == "4748a") {
    passed++;
  } else {
    std::cout << "Test 2aa * 171 (12) is failed: must return 4748a, returns: " << calc("2aa", "171", 12, "*") << "\n";
    failed++;
  }
  if (calc("if", "7b", 22, "*") == "682b") {
    passed++;
  } else {
    std::cout << "Test if * 7b (22) is failed: must return 682b, returns: " << calc("if", "7b", 22, "*") << "\n";
    failed++;
  }
  if (calc("99", "3c", 17, "*") == "2156") {
    passed++;
  } else {
    std::cout << "Test 99 * 3c (17) is failed: must return 2156, returns: " << calc("99", "3c", 17, "*") << "\n";
    failed++;
  }
  if (calc("1100111001", "11011", 2, "/") == "11110") {
    passed++;
  } else {
    std::cout << "Test 1100111001 / 11011 (2) is failed: must return 11110, returns: " << calc("1100111001", "11011", 2, "/") << "\n";
    failed++;
  }
  if (calc("5b", "1f", 30, "+") == "6q") {
    passed++;
  } else {
    std::cout << "Test 5b + 1f (30) is failed: must return 6q, returns: " << calc("5b", "1f", 30, "+") << "\n";
    failed++;
  }
  if (calc("9f", "3e", 18, "-") == "61") {
    passed++;
  } else {
    std::cout << "Test 9f - 3e (18) is failed: must return 61, returns: " << calc("9f", "3e", 18, "-") << "\n";
    failed++;
  }
  if (calc("1p", "m", 31, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1p / m (31) is failed: must return 2, returns: " << calc("1p", "m", 31, "/") << "\n";
    failed++;
  }
  if (calc("131", "jf", 22, "+") == "20g") {
    passed++;
  } else {
    std::cout << "Test 131 + jf (22) is failed: must return 20g, returns: " << calc("131", "jf", 22, "+") << "\n";
    failed++;
  }
  if (calc("af", "18", 32, "/") == "8") {
    passed++;
  } else {
    std::cout << "Test af / 18 (32) is failed: must return 8, returns: " << calc("af", "18", 32, "/") << "\n";
    failed++;
  }
  if (calc("nb", "gm", 29, "-") == "6i") {
    passed++;
  } else {
    std::cout << "Test nb - gm (29) is failed: must return 6i, returns: " << calc("nb", "gm", 29, "-") << "\n";
    failed++;
  }
  if (calc("n5", "jb", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test n5 / jb (30) is failed: must return 1, returns: " << calc("n5", "jb", 30, "/") << "\n";
    failed++;
  }
  if (calc("215", "9a", 13, "+") == "2b2") {
    passed++;
  } else {
    std::cout << "Test 215 + 9a (13) is failed: must return 2b2, returns: " << calc("215", "9a", 13, "+") << "\n";
    failed++;
  }
  if (calc("h1", "67", 27, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test h1 / 67 (27) is failed: must return 2, returns: " << calc("h1", "67", 27, "/") << "\n";
    failed++;
  }
  if (calc("1", "0", 11, "-") == "1") {
    passed++;
  } else {
    std::cout << "Test 1 - 0 (11) is failed: must return 1, returns: " << calc("1", "0", 11, "-") << "\n";
    failed++;
  }
  if (calc("p1", "7c", 30, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test p1 / 7c (30) is failed: must return 3, returns: " << calc("p1", "7c", 30, "/") << "\n";
    failed++;
  }
  if (calc("1b1", "74", 21, "+") == "1i5") {
    passed++;
  } else {
    std::cout << "Test 1b1 + 74 (21) is failed: must return 1i5, returns: " << calc("1b1", "74", 21, "+") << "\n";
    failed++;
  }
  if (calc("23", "1b", 17, "*") == "39g") {
    passed++;
  } else {
    std::cout << "Test 23 * 1b (17) is failed: must return 39g, returns: " << calc("23", "1b", 17, "*") << "\n";
    failed++;
  }
  if (calc("13a", "11", 13, "+") == "14b") {
    passed++;
  } else {
    std::cout << "Test 13a + 11 (13) is failed: must return 14b, returns: " << calc("13a", "11", 13, "+") << "\n";
    failed++;
  }
  if (calc("501", "15", 11, "-") == "497") {
    passed++;
  } else {
    std::cout << "Test 501 - 15 (11) is failed: must return 497, returns: " << calc("501", "15", 11, "-") << "\n";
    failed++;
  }
  if (calc("2c", "o", 33, "-") == "1l") {
    passed++;
  } else {
    std::cout << "Test 2c - o (33) is failed: must return 1l, returns: " << calc("2c", "o", 33, "-") << "\n";
    failed++;
  }
  if (calc("6j", "17", 27, "+") == "7q") {
    passed++;
  } else {
    std::cout << "Test 6j + 17 (27) is failed: must return 7q, returns: " << calc("6j", "17", 27, "+") << "\n";
    failed++;
  }
  if (calc("m4", "42", 23, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test m4 / 42 (23) is failed: must return 5, returns: " << calc("m4", "42", 23, "/") << "\n";
    failed++;
  }
  if (calc("3c2", "45", 13, "*") == "1433a") {
    passed++;
  } else {
    std::cout << "Test 3c2 * 45 (13) is failed: must return 1433a, returns: " << calc("3c2", "45", 13, "*") << "\n";
    failed++;
  }
  if (calc("52", "32", 6, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 52 / 32 (6) is failed: must return 1, returns: " << calc("52", "32", 6, "/") << "\n";
    failed++;
  }
  if (calc("342", "320", 7, "+") == "662") {
    passed++;
  } else {
    std::cout << "Test 342 + 320 (7) is failed: must return 662, returns: " << calc("342", "320", 7, "+") << "\n";
    failed++;
  }
  if (calc("132", "a2", 13, "-") == "60") {
    passed++;
  } else {
    std::cout << "Test 132 - a2 (13) is failed: must return 60, returns: " << calc("132", "a2", 13, "-") << "\n";
    failed++;
  }
  if (calc("230", "1b3", 13, "*") == "42090") {
    passed++;
  } else {
    std::cout << "Test 230 * 1b3 (13) is failed: must return 42090, returns: " << calc("230", "1b3", 13, "*") << "\n";
    failed++;
  }
  if (calc("4a", "3", 17, "/") == "19") {
    passed++;
  } else {
    std::cout << "Test 4a / 3 (17) is failed: must return 19, returns: " << calc("4a", "3", 17, "/") << "\n";
    failed++;
  }
  if (calc("4321", "1502", 6, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 4321 / 1502 (6) is failed: must return 2, returns: " << calc("4321", "1502", 6, "/") << "\n";
    failed++;
  }
  if (calc("m1", "gt", 30, "*") == "cdot") {
    passed++;
  } else {
    std::cout << "Test m1 * gt (30) is failed: must return cdot, returns: " << calc("m1", "gt", 30, "*") << "\n";
    failed++;
  }
  if (calc("83", "36", 12, "+") == "b9") {
    passed++;
  } else {
    std::cout << "Test 83 + 36 (12) is failed: must return b9, returns: " << calc("83", "36", 12, "+") << "\n";
    failed++;
  }
  if (calc("110100110", "10010", 2, "*") == "1110110101100") {
    passed++;
  } else {
    std::cout << "Test 110100110 * 10010 (2) is failed: must return 1110110101100, returns: " << calc("110100110", "10010", 2, "*") << "\n";
    failed++;
  }
  if (calc("391", "257", 15, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 391 / 257 (15) is failed: must return 1, returns: " << calc("391", "257", 15, "/") << "\n";
    failed++;
  }
  if (calc("1a0", "4", 12, "-") == "198") {
    passed++;
  } else {
    std::cout << "Test 1a0 - 4 (12) is failed: must return 198, returns: " << calc("1a0", "4", 12, "-") << "\n";
    failed++;
  }
  if (calc("q", "g", 31, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test q / g (31) is failed: must return 1, returns: " << calc("q", "g", 31, "/") << "\n";
    failed++;
  }
  if (calc("173", "me", 23, "+") == "26h") {
    passed++;
  } else {
    std::cout << "Test 173 + me (23) is failed: must return 26h, returns: " << calc("173", "me", 23, "+") << "\n";
    failed++;
  }
  if (calc("1ba", "2g", 22, "-") == "18g") {
    passed++;
  } else {
    std::cout << "Test 1ba - 2g (22) is failed: must return 18g, returns: " << calc("1ba", "2g", 22, "-") << "\n";
    failed++;
  }
  if (calc("803", "125", 10, "*") == "100375") {
    passed++;
  } else {
    std::cout << "Test 803 * 125 (10) is failed: must return 100375, returns: " << calc("803", "125", 10, "*") << "\n";
    failed++;
  }
  if (calc("121", "50", 27, "+") == "171") {
    passed++;
  } else {
    std::cout << "Test 121 + 50 (27) is failed: must return 171, returns: " << calc("121", "50", 27, "+") << "\n";
    failed++;
  }
  if (calc("2ac", "240", 13, "+") == "51c") {
    passed++;
  } else {
    std::cout << "Test 2ac + 240 (13) is failed: must return 51c, returns: " << calc("2ac", "240", 13, "+") << "\n";
    failed++;
  }
  if (calc("11", "g", 21, "-") == "6") {
    passed++;
  } else {
    std::cout << "Test 11 - g (21) is failed: must return 6, returns: " << calc("11", "g", 21, "-") << "\n";
    failed++;
  }
  if (calc("2h1", "a3", 18, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test 2h1 / a3 (18) is failed: must return 5, returns: " << calc("2h1", "a3", 18, "/") << "\n";
    failed++;
  }
  if (calc("166", "b2", 22, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 166 / b2 (22) is failed: must return 2, returns: " << calc("166", "b2", 22, "/") << "\n";
    failed++;
  }
  if (calc("2", "2", 33, "*") == "4") {
    passed++;
  } else {
    std::cout << "Test 2 * 2 (33) is failed: must return 4, returns: " << calc("2", "2", 33, "*") << "\n";
    failed++;
  }
  if (calc("1gc", "68", 22, "*") == "b368") {
    passed++;
  } else {
    std::cout << "Test 1gc * 68 (22) is failed: must return b368, returns: " << calc("1gc", "68", 22, "*") << "\n";
    failed++;
  }
  if (calc("1b2", "10c", 17, "-") == "a7") {
    passed++;
  } else {
    std::cout << "Test 1b2 - 10c (17) is failed: must return a7, returns: " << calc("1b2", "10c", 17, "-") << "\n";
    failed++;
  }
  if (calc("qn", "pb", 27, "+") == "1p7") {
    passed++;
  } else {
    std::cout << "Test qn + pb (27) is failed: must return 1p7, returns: " << calc("qn", "pb", 27, "+") << "\n";
    failed++;
  }
  if (calc("152", "104", 15, "*") == "15758") {
    passed++;
  } else {
    std::cout << "Test 152 * 104 (15) is failed: must return 15758, returns: " << calc("152", "104", 15, "*") << "\n";
    failed++;
  }
  if (calc("10k", "61", 25, "-") == "jj") {
    passed++;
  } else {
    std::cout << "Test 10k - 61 (25) is failed: must return jj, returns: " << calc("10k", "61", 25, "-") << "\n";
    failed++;
  }
  if (calc("1221", "426", 8, "-") == "573") {
    passed++;
  } else {
    std::cout << "Test 1221 - 426 (8) is failed: must return 573, returns: " << calc("1221", "426", 8, "-") << "\n";
    failed++;
  }
  if (calc("24e", "138", 16, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 24e / 138 (16) is failed: must return 1, returns: " << calc("24e", "138", 16, "/") << "\n";
    failed++;
  }
  if (calc("6aa", "62a", 12, "+") == "1118") {
    passed++;
  } else {
    std::cout << "Test 6aa + 62a (12) is failed: must return 1118, returns: " << calc("6aa", "62a", 12, "+") << "\n";
    failed++;
  }
  if (calc("1bg", "a1", 17, "+") == "250") {
    passed++;
  } else {
    std::cout << "Test 1bg + a1 (17) is failed: must return 250, returns: " << calc("1bg", "a1", 17, "+") << "\n";
    failed++;
  }
  if (calc("1000", "414", 5, "-") == "31") {
    passed++;
  } else {
    std::cout << "Test 1000 - 414 (5) is failed: must return 31, returns: " << calc("1000", "414", 5, "-") << "\n";
    failed++;
  }
  if (calc("1450", "1343", 7, "-") == "104") {
    passed++;
  } else {
    std::cout << "Test 1450 - 1343 (7) is failed: must return 104, returns: " << calc("1450", "1343", 7, "-") << "\n";
    failed++;
  }
  if (calc("40", "2c", 35, "*") == "9d0") {
    passed++;
  } else {
    std::cout << "Test 40 * 2c (35) is failed: must return 9d0, returns: " << calc("40", "2c", 35, "*") << "\n";
    failed++;
  }
  if (calc("9j", "2h", 27, "*") == "pdq") {
    passed++;
  } else {
    std::cout << "Test 9j * 2h (27) is failed: must return pdq, returns: " << calc("9j", "2h", 27, "*") << "\n";
    failed++;
  }
  if (calc("430", "a9", 14, "*") == "32bd0") {
    passed++;
  } else {
    std::cout << "Test 430 * a9 (14) is failed: must return 32bd0, returns: " << calc("430", "a9", 14, "*") << "\n";
    failed++;
  }
  if (calc("18j", "hd", 23, "-") == "e6") {
    passed++;
  } else {
    std::cout << "Test 18j - hd (23) is failed: must return e6, returns: " << calc("18j", "hd", 23, "-") << "\n";
    failed++;
  }
  if (calc("22", "12", 3, "*") == "1111") {
    passed++;
  } else {
    std::cout << "Test 22 * 12 (3) is failed: must return 1111, returns: " << calc("22", "12", 3, "*") << "\n";
    failed++;
  }
  if (calc("781", "117", 10, "+") == "898") {
    passed++;
  } else {
    std::cout << "Test 781 + 117 (10) is failed: must return 898, returns: " << calc("781", "117", 10, "+") << "\n";
    failed++;
  }
  if (calc("835", "141", 9, "-") == "684") {
    passed++;
  } else {
    std::cout << "Test 835 - 141 (9) is failed: must return 684, returns: " << calc("835", "141", 9, "-") << "\n";
    failed++;
  }
  if (calc("dc", "4c", 26, "+") == "ho") {
    passed++;
  } else {
    std::cout << "Test dc + 4c (26) is failed: must return ho, returns: " << calc("dc", "4c", 26, "+") << "\n";
    failed++;
  }
  if (calc("379", "32", 13, "+") == "3ab") {
    passed++;
  } else {
    std::cout << "Test 379 + 32 (13) is failed: must return 3ab, returns: " << calc("379", "32", 13, "+") << "\n";
    failed++;
  }
  if (calc("3d2", "131", 15, "-") == "2a1") {
    passed++;
  } else {
    std::cout << "Test 3d2 - 131 (15) is failed: must return 2a1, returns: " << calc("3d2", "131", 15, "-") << "\n";
    failed++;
  }
  if (calc("22b", "1c9", 14, "*") == "42831") {
    passed++;
  } else {
    std::cout << "Test 22b * 1c9 (14) is failed: must return 42831, returns: " << calc("22b", "1c9", 14, "*") << "\n";
    failed++;
  }
  if (calc("12002", "1100", 3, "-") == "10202") {
    passed++;
  } else {
    std::cout << "Test 12002 - 1100 (3) is failed: must return 10202, returns: " << calc("12002", "1100", 3, "-") << "\n";
    failed++;
  }
  if (calc("1022102", "120010", 3, "*") == "201211001020") {
    passed++;
  } else {
    std::cout << "Test 1022102 * 120010 (3) is failed: must return 201211001020, returns: " << calc("1022102", "120010", 3, "*") << "\n";
    failed++;
  }
  if (calc("10100", "1212", 3, "+") == "12012") {
    passed++;
  } else {
    std::cout << "Test 10100 + 1212 (3) is failed: must return 12012, returns: " << calc("10100", "1212", 3, "+") << "\n";
    failed++;
  }
  if (calc("7g", "71", 21, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 7g / 71 (21) is failed: must return 1, returns: " << calc("7g", "71", 21, "/") << "\n";
    failed++;
  }
  if (calc("181", "6b", 26, "-") == "11g") {
    passed++;
  } else {
    std::cout << "Test 181 - 6b (26) is failed: must return 11g, returns: " << calc("181", "6b", 26, "-") << "\n";
    failed++;
  }
  if (calc("11e", "62", 21, "+") == "17g") {
    passed++;
  } else {
    std::cout << "Test 11e + 62 (21) is failed: must return 17g, returns: " << calc("11e", "62", 21, "+") << "\n";
    failed++;
  }
  if (calc("e8", "5e", 32, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test e8 / 5e (32) is failed: must return 2, returns: " << calc("e8", "5e", 32, "/") << "\n";
    failed++;
  }
  if (calc("1f0", "1c2", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1f0 / 1c2 (20) is failed: must return 1, returns: " << calc("1f0", "1c2", 20, "/") << "\n";
    failed++;
  }
  if (calc("hs", "hf", 34, "*") == "94tc") {
    passed++;
  } else {
    std::cout << "Test hs * hf (34) is failed: must return 94tc, returns: " << calc("hs", "hf", 34, "*") << "\n";
    failed++;
  }
  if (calc("4331", "1354", 6, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 4331 / 1354 (6) is failed: must return 2, returns: " << calc("4331", "1354", 6, "/") << "\n";
    failed++;
  }
  if (calc("33003", "10003", 4, "*") == "330321021") {
    passed++;
  } else {
    std::cout << "Test 33003 * 10003 (4) is failed: must return 330321021, returns: " << calc("33003", "10003", 4, "*") << "\n";
    failed++;
  }
  if (calc("47a", "180", 14, "+") == "61a") {
    passed++;
  } else {
    std::cout << "Test 47a + 180 (14) is failed: must return 61a, returns: " << calc("47a", "180", 14, "+") << "\n";
    failed++;
  }
  if (calc("101211", "20021", 3, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 101211 / 20021 (3) is failed: must return 1, returns: " << calc("101211", "20021", 3, "/") << "\n";
    failed++;
  }
  if (calc("1021120", "200112", 3, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1021120 / 200112 (3) is failed: must return 1, returns: " << calc("1021120", "200112", 3, "/") << "\n";
    failed++;
  }
  if (calc("1010011111", "101001011", 2, "+") == "1111101010") {
    passed++;
  } else {
    std::cout << "Test 1010011111 + 101001011 (2) is failed: must return 1111101010, returns: " << calc("1010011111", "101001011", 2, "+") << "\n";
    failed++;
  }
  if (calc("1260", "1101", 7, "+") == "2361") {
    passed++;
  } else {
    std::cout << "Test 1260 + 1101 (7) is failed: must return 2361, returns: " << calc("1260", "1101", 7, "+") << "\n";
    failed++;
  }
  if (calc("11101", "1", 2, "/") == "11101") {
    passed++;
  } else {
    std::cout << "Test 11101 / 1 (2) is failed: must return 11101, returns: " << calc("11101", "1", 2, "/") << "\n";
    failed++;
  }
  if (calc("202", "18a", 11, "-") == "23") {
    passed++;
  } else {
    std::cout << "Test 202 - 18a (11) is failed: must return 23, returns: " << calc("202", "18a", 11, "-") << "\n";
    failed++;
  }
  if (calc("169", "8e", 22, "-") == "jh") {
    passed++;
  } else {
    std::cout << "Test 169 - 8e (22) is failed: must return jh, returns: " << calc("169", "8e", 22, "-") << "\n";
    failed++;
  }
  if (calc("1534", "504", 6, "+") == "2442") {
    passed++;
  } else {
    std::cout << "Test 1534 + 504 (6) is failed: must return 2442, returns: " << calc("1534", "504", 6, "+") << "\n";
    failed++;
  }
  if (calc("1jk", "i7", 23, "+") == "2f4") {
    passed++;
  } else {
    std::cout << "Test 1jk + i7 (23) is failed: must return 2f4, returns: " << calc("1jk", "i7", 23, "+") << "\n";
    failed++;
  }
  if (calc("98", "7j", 30, "+") == "gr") {
    passed++;
  } else {
    std::cout << "Test 98 + 7j (30) is failed: must return gr, returns: " << calc("98", "7j", 30, "+") << "\n";
    failed++;
  }
  if (calc("745", "80", 9, "+") == "835") {
    passed++;
  } else {
    std::cout << "Test 745 + 80 (9) is failed: must return 835, returns: " << calc("745", "80", 9, "+") << "\n";
    failed++;
  }
  if (calc("95", "19", 27, "+") == "ae") {
    passed++;
  } else {
    std::cout << "Test 95 + 19 (27) is failed: must return ae, returns: " << calc("95", "19", 27, "+") << "\n";
    failed++;
  }
  if (calc("4a1", "487", 11, "+") == "978") {
    passed++;
  } else {
    std::cout << "Test 4a1 + 487 (11) is failed: must return 978, returns: " << calc("4a1", "487", 11, "+") << "\n";
    failed++;
  }
  if (calc("fy", "59", 35, "-") == "ap") {
    passed++;
  } else {
    std::cout << "Test fy - 59 (35) is failed: must return ap, returns: " << calc("fy", "59", 35, "-") << "\n";
    failed++;
  }
  if (calc("695", "523", 10, "*") == "363485") {
    passed++;
  } else {
    std::cout << "Test 695 * 523 (10) is failed: must return 363485, returns: " << calc("695", "523", 10, "*") << "\n";
    failed++;
  }
  if (calc("1dm", "d7", 24, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1dm / d7 (24) is failed: must return 2, returns: " << calc("1dm", "d7", 24, "/") << "\n";
    failed++;
  }
  if (calc("ig", "g2", 25, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ig / g2 (25) is failed: must return 1, returns: " << calc("ig", "g2", 25, "/") << "\n";
    failed++;
  }
  if (calc("l2", "1c", 27, "/") == "e") {
    passed++;
  } else {
    std::cout << "Test l2 / 1c (27) is failed: must return e, returns: " << calc("l2", "1c", 27, "/") << "\n";
    failed++;
  }
  if (calc("im", "8f", 26, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test im / 8f (26) is failed: must return 2, returns: " << calc("im", "8f", 26, "/") << "\n";
    failed++;
  }
  if (calc("1110100100", "10001100", 2, "+") == "10000110000") {
    passed++;
  } else {
    std::cout << "Test 1110100100 + 10001100 (2) is failed: must return 10000110000, returns: " << calc("1110100100", "10001100", 2, "+") << "\n";
    failed++;
  }
  if (calc("52", "13", 24, "-") == "3n") {
    passed++;
  } else {
    std::cout << "Test 52 - 13 (24) is failed: must return 3n, returns: " << calc("52", "13", 24, "-") << "\n";
    failed++;
  }
  if (calc("jl", "92", 26, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test jl / 92 (26) is failed: must return 2, returns: " << calc("jl", "92", 26, "/") << "\n";
    failed++;
  }
  if (calc("rh", "50", 28, "+") == "14h") {
    passed++;
  } else {
    std::cout << "Test rh + 50 (28) is failed: must return 14h, returns: " << calc("rh", "50", 28, "+") << "\n";
    failed++;
  }
  if (calc("7p", "6o", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 7p / 6o (30) is failed: must return 1, returns: " << calc("7p", "6o", 30, "/") << "\n";
    failed++;
  }
  if (calc("226", "55", 9, "+") == "282") {
    passed++;
  } else {
    std::cout << "Test 226 + 55 (9) is failed: must return 282, returns: " << calc("226", "55", 9, "+") << "\n";
    failed++;
  }
  if (calc("2c8", "136", 15, "*") == "37293") {
    passed++;
  } else {
    std::cout << "Test 2c8 * 136 (15) is failed: must return 37293, returns: " << calc("2c8", "136", 15, "*") << "\n";
    failed++;
  }
  if (calc("163", "om", 26, "*") == "14hoe") {
    passed++;
  } else {
    std::cout << "Test 163 * om (26) is failed: must return 14hoe, returns: " << calc("163", "om", 26, "*") << "\n";
    failed++;
  }
  if (calc("20c", "e7", 15, "+") == "304") {
    passed++;
  } else {
    std::cout << "Test 20c + e7 (15) is failed: must return 304, returns: " << calc("20c", "e7", 15, "+") << "\n";
    failed++;
  }
  if (calc("ok", "bf", 32, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test ok / bf (32) is failed: must return 2, returns: " << calc("ok", "bf", 32, "/") << "\n";
    failed++;
  }
  if (calc("310", "201", 12, "+") == "511") {
    passed++;
  } else {
    std::cout << "Test 310 + 201 (12) is failed: must return 511, returns: " << calc("310", "201", 12, "+") << "\n";
    failed++;
  }
  if (calc("2402", "320", 5, "*") == "1424140") {
    passed++;
  } else {
    std::cout << "Test 2402 * 320 (5) is failed: must return 1424140, returns: " << calc("2402", "320", 5, "*") << "\n";
    failed++;
  }
  if (calc("390", "102", 10, "-") == "288") {
    passed++;
  } else {
    std::cout << "Test 390 - 102 (10) is failed: must return 288, returns: " << calc("390", "102", 10, "-") << "\n";
    failed++;
  }
  if (calc("ni", "1a", 28, "-") == "m8") {
    passed++;
  } else {
    std::cout << "Test ni - 1a (28) is failed: must return m8, returns: " << calc("ni", "1a", 28, "-") << "\n";
    failed++;
  }
  if (calc("p9", "f6", 26, "+") == "1ef") {
    passed++;
  } else {
    std::cout << "Test p9 + f6 (26) is failed: must return 1ef, returns: " << calc("p9", "f6", 26, "+") << "\n";
    failed++;
  }
  if (calc("mb", "ae", 33, "-") == "bu") {
    passed++;
  } else {
    std::cout << "Test mb - ae (33) is failed: must return bu, returns: " << calc("mb", "ae", 33, "-") << "\n";
    failed++;
  }
  if (calc("135", "111", 19, "*") == "14985") {
    passed++;
  } else {
    std::cout << "Test 135 * 111 (19) is failed: must return 14985, returns: " << calc("135", "111", 19, "*") << "\n";
    failed++;
  }
  if (calc("5a", "28", 13, "*") == "1212") {
    passed++;
  } else {
    std::cout << "Test 5a * 28 (13) is failed: must return 1212, returns: " << calc("5a", "28", 13, "*") << "\n";
    failed++;
  }
  if (calc("av", "15", 35, "*") == "cff") {
    passed++;
  } else {
    std::cout << "Test av * 15 (35) is failed: must return cff, returns: " << calc("av", "15", 35, "*") << "\n";
    failed++;
  }
  if (calc("8l", "f", 34, "/") == "j") {
    passed++;
  } else {
    std::cout << "Test 8l / f (34) is failed: must return j, returns: " << calc("8l", "f", 34, "/") << "\n";
    failed++;
  }
  if (calc("ad", "2", 20, "/") == "56") {
    passed++;
  } else {
    std::cout << "Test ad / 2 (20) is failed: must return 56, returns: " << calc("ad", "2", 20, "/") << "\n";
    failed++;
  }
  if (calc("1bc", "e5", 22, "+") == "23h") {
    passed++;
  } else {
    std::cout << "Test 1bc + e5 (22) is failed: must return 23h, returns: " << calc("1bc", "e5", 22, "+") << "\n";
    failed++;
  }
  if (calc("101", "4", 8, "-") == "75") {
    passed++;
  } else {
    std::cout << "Test 101 - 4 (8) is failed: must return 75, returns: " << calc("101", "4", 8, "-") << "\n";
    failed++;
  }
  if (calc("2b", "1", 16, "+") == "2c") {
    passed++;
  } else {
    std::cout << "Test 2b + 1 (16) is failed: must return 2c, returns: " << calc("2b", "1", 16, "+") << "\n";
    failed++;
  }
  if (calc("3k", "i", 32, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 3k / i (32) is failed: must return 6, returns: " << calc("3k", "i", 32, "/") << "\n";
    failed++;
  }
  if (calc("2hg", "17h", 18, "-") == "19h") {
    passed++;
  } else {
    std::cout << "Test 2hg - 17h (18) is failed: must return 19h, returns: " << calc("2hg", "17h", 18, "-") << "\n";
    failed++;
  }
  if (calc("234", "46", 7, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 234 / 46 (7) is failed: must return 3, returns: " << calc("234", "46", 7, "/") << "\n";
    failed++;
  }
  if (calc("ec", "c", 17, "-") == "e0") {
    passed++;
  } else {
    std::cout << "Test ec - c (17) is failed: must return e0, returns: " << calc("ec", "c", 17, "-") << "\n";
    failed++;
  }
  if (calc("146", "35", 14, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test 146 / 35 (14) is failed: must return 5, returns: " << calc("146", "35", 14, "/") << "\n";
    failed++;
  }
  if (calc("439", "31", 10, "*") == "13609") {
    passed++;
  } else {
    std::cout << "Test 439 * 31 (10) is failed: must return 13609, returns: " << calc("439", "31", 10, "*") << "\n";
    failed++;
  }
  if (calc("pr", "of", 32, "+") == "1ia") {
    passed++;
  } else {
    std::cout << "Test pr + of (32) is failed: must return 1ia, returns: " << calc("pr", "of", 32, "+") << "\n";
    failed++;
  }
  if (calc("48b", "427", 13, "*") == "16835c") {
    passed++;
  } else {
    std::cout << "Test 48b * 427 (13) is failed: must return 16835c, returns: " << calc("48b", "427", 13, "*") << "\n";
    failed++;
  }
  if (calc("1230", "21", 6, "/") == "35") {
    passed++;
  } else {
    std::cout << "Test 1230 / 21 (6) is failed: must return 35, returns: " << calc("1230", "21", 6, "/") << "\n";
    failed++;
  }
  if (calc("244", "34", 9, "*") == "8527") {
    passed++;
  } else {
    std::cout << "Test 244 * 34 (9) is failed: must return 8527, returns: " << calc("244", "34", 9, "*") << "\n";
    failed++;
  }
  if (calc("2162", "502", 7, "*") == "1430654") {
    passed++;
  } else {
    std::cout << "Test 2162 * 502 (7) is failed: must return 1430654, returns: " << calc("2162", "502", 7, "*") << "\n";
    failed++;
  }
  if (calc("16f", "1d", 21, "/") == "h") {
    passed++;
  } else {
    std::cout << "Test 16f / 1d (21) is failed: must return h, returns: " << calc("16f", "1d", 21, "/") << "\n";
    failed++;
  }
  if (calc("4", "3", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 4 / 3 (7) is failed: must return 1, returns: " << calc("4", "3", 7, "/") << "\n";
    failed++;
  }
  if (calc("3344", "1343", 5, "*") == "11340302") {
    passed++;
  } else {
    std::cout << "Test 3344 * 1343 (5) is failed: must return 11340302, returns: " << calc("3344", "1343", 5, "*") << "\n";
    failed++;
  }
  if (calc("9h", "l", 27, "*") == "7d6") {
    passed++;
  } else {
    std::cout << "Test 9h * l (27) is failed: must return 7d6, returns: " << calc("9h", "l", 27, "*") << "\n";
    failed++;
  }
  if (calc("396", "278", 11, "+") == "663") {
    passed++;
  } else {
    std::cout << "Test 396 + 278 (11) is failed: must return 663, returns: " << calc("396", "278", 11, "+") << "\n";
    failed++;
  }
  if (calc("19", "6", 21, "+") == "1f") {
    passed++;
  } else {
    std::cout << "Test 19 + 6 (21) is failed: must return 1f, returns: " << calc("19", "6", 21, "+") << "\n";
    failed++;
  }
  if (calc("ee", "32", 22, "+") == "hg") {
    passed++;
  } else {
    std::cout << "Test ee + 32 (22) is failed: must return hg, returns: " << calc("ee", "32", 22, "+") << "\n";
    failed++;
  }
  if (calc("h1", "8", 23, "/") == "23") {
    passed++;
  } else {
    std::cout << "Test h1 / 8 (23) is failed: must return 23, returns: " << calc("h1", "8", 23, "/") << "\n";
    failed++;
  }
  if (calc("199", "j0", 26, "*") == "plf0") {
    passed++;
  } else {
    std::cout << "Test 199 * j0 (26) is failed: must return plf0, returns: " << calc("199", "j0", 26, "*") << "\n";
    failed++;
  }
  if (calc("fa", "66", 16, "-") == "94") {
    passed++;
  } else {
    std::cout << "Test fa - 66 (16) is failed: must return 94, returns: " << calc("fa", "66", 16, "-") << "\n";
    failed++;
  }
  if (calc("35", "s", 29, "+") == "44") {
    passed++;
  } else {
    std::cout << "Test 35 + s (29) is failed: must return 44, returns: " << calc("35", "s", 29, "+") << "\n";
    failed++;
  }
  if (calc("226", "cf", 17, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 226 / cf (17) is failed: must return 2, returns: " << calc("226", "cf", 17, "/") << "\n";
    failed++;
  }
  if (calc("172", "143", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 172 / 143 (14) is failed: must return 1, returns: " << calc("172", "143", 14, "/") << "\n";
    failed++;
  }
  if (calc("364", "193", 16, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 364 / 193 (16) is failed: must return 2, returns: " << calc("364", "193", 16, "/") << "\n";
    failed++;
  }
  if (calc("2g", "c", 20, "+") == "38") {
    passed++;
  } else {
    std::cout << "Test 2g + c (20) is failed: must return 38, returns: " << calc("2g", "c", 20, "+") << "\n";
    failed++;
  }
  if (calc("qv", "kk", 35, "-") == "6b") {
    passed++;
  } else {
    std::cout << "Test qv - kk (35) is failed: must return 6b, returns: " << calc("qv", "kk", 35, "-") << "\n";
    failed++;
  }
  if (calc("8s", "3q", 32, "*") == "11qo") {
    passed++;
  } else {
    std::cout << "Test 8s * 3q (32) is failed: must return 11qo, returns: " << calc("8s", "3q", 32, "*") << "\n";
    failed++;
  }
  if (calc("10101", "1120", 3, "-") == "1211") {
    passed++;
  } else {
    std::cout << "Test 10101 - 1120 (3) is failed: must return 1211, returns: " << calc("10101", "1120", 3, "-") << "\n";
    failed++;
  }
  if (calc("37", "32", 23, "+") == "69") {
    passed++;
  } else {
    std::cout << "Test 37 + 32 (23) is failed: must return 69, returns: " << calc("37", "32", 23, "+") << "\n";
    failed++;
  }
  if (calc("1241", "1047", 9, "+") == "2288") {
    passed++;
  } else {
    std::cout << "Test 1241 + 1047 (9) is failed: must return 2288, returns: " << calc("1241", "1047", 9, "+") << "\n";
    failed++;
  }
  if (calc("11e", "fj", 29, "-") == "eo") {
    passed++;
  } else {
    std::cout << "Test 11e - fj (29) is failed: must return eo, returns: " << calc("11e", "fj", 29, "-") << "\n";
    failed++;
  }
  if (calc("101002", "10001", 3, "*") == "1010121002") {
    passed++;
  } else {
    std::cout << "Test 101002 * 10001 (3) is failed: must return 1010121002, returns: " << calc("101002", "10001", 3, "*") << "\n";
    failed++;
  }
  if (calc("1cb", "nk", 25, "-") == "dg") {
    passed++;
  } else {
    std::cout << "Test 1cb - nk (25) is failed: must return dg, returns: " << calc("1cb", "nk", 25, "-") << "\n";
    failed++;
  }
  if (calc("dj", "b3", 20, "*") == "7fah") {
    passed++;
  } else {
    std::cout << "Test dj * b3 (20) is failed: must return 7fah, returns: " << calc("dj", "b3", 20, "*") << "\n";
    failed++;
  }
  if (calc("1125", "475", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1125 / 475 (8) is failed: must return 1, returns: " << calc("1125", "475", 8, "/") << "\n";
    failed++;
  }
  if (calc("3330", "2303", 4, "-") == "1021") {
    passed++;
  } else {
    std::cout << "Test 3330 - 2303 (4) is failed: must return 1021, returns: " << calc("3330", "2303", 4, "-") << "\n";
    failed++;
  }
  if (calc("h6", "e8", 27, "*") == "935l") {
    passed++;
  } else {
    std::cout << "Test h6 * e8 (27) is failed: must return 935l, returns: " << calc("h6", "e8", 27, "*") << "\n";
    failed++;
  }
  if (calc("4b2", "417", 13, "+") == "8c9") {
    passed++;
  } else {
    std::cout << "Test 4b2 + 417 (13) is failed: must return 8c9, returns: " << calc("4b2", "417", 13, "+") << "\n";
    failed++;
  }
  if (calc("1b1", "ac", 17, "-") == "106") {
    passed++;
  } else {
    std::cout << "Test 1b1 - ac (17) is failed: must return 106, returns: " << calc("1b1", "ac", 17, "-") << "\n";
    failed++;
  }
  if (calc("100022", "1200", 3, "/") == "12") {
    passed++;
  } else {
    std::cout << "Test 100022 / 1200 (3) is failed: must return 12, returns: " << calc("100022", "1200", 3, "/") << "\n";
    failed++;
  }
  if (calc("165", "53", 17, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 165 / 53 (17) is failed: must return 4, returns: " << calc("165", "53", 17, "/") << "\n";
    failed++;
  }
  if (calc("147", "2c", 19, "-") == "11e") {
    passed++;
  } else {
    std::cout << "Test 147 - 2c (19) is failed: must return 11e, returns: " << calc("147", "2c", 19, "-") << "\n";
    failed++;
  }
  if (calc("k0", "6c", 27, "-") == "df") {
    passed++;
  } else {
    std::cout << "Test k0 - 6c (27) is failed: must return df, returns: " << calc("k0", "6c", 27, "-") << "\n";
    failed++;
  }
  if (calc("2511", "554", 6, "+") == "3505") {
    passed++;
  } else {
    std::cout << "Test 2511 + 554 (6) is failed: must return 3505, returns: " << calc("2511", "554", 6, "+") << "\n";
    failed++;
  }
  if (calc("277", "69", 14, "*") == "12bb7") {
    passed++;
  } else {
    std::cout << "Test 277 * 69 (14) is failed: must return 12bb7, returns: " << calc("277", "69", 14, "*") << "\n";
    failed++;
  }
  if (calc("bj", "a0", 31, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test bj / a0 (31) is failed: must return 1, returns: " << calc("bj", "a0", 31, "/") << "\n";
    failed++;
  }
  if (calc("as", "14", 31, "/") == "9") {
    passed++;
  } else {
    std::cout << "Test as / 14 (31) is failed: must return 9, returns: " << calc("as", "14", 31, "/") << "\n";
    failed++;
  }
  if (calc("9c", "19", 30, "-") == "83") {
    passed++;
  } else {
    std::cout << "Test 9c - 19 (30) is failed: must return 83, returns: " << calc("9c", "19", 30, "-") << "\n";
    failed++;
  }
  if (calc("1b1", "5e", 23, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 1b1 / 5e (23) is failed: must return 6, returns: " << calc("1b1", "5e", 23, "/") << "\n";
    failed++;
  }
  if (calc("513", "310", 10, "*") == "159030") {
    passed++;
  } else {
    std::cout << "Test 513 * 310 (10) is failed: must return 159030, returns: " << calc("513", "310", 10, "*") << "\n";
    failed++;
  }
  if (calc("83", "50", 13, "+") == "103") {
    passed++;
  } else {
    std::cout << "Test 83 + 50 (13) is failed: must return 103, returns: " << calc("83", "50", 13, "+") << "\n";
    failed++;
  }
  if (calc("46", "31", 18, "+") == "77") {
    passed++;
  } else {
    std::cout << "Test 46 + 31 (18) is failed: must return 77, returns: " << calc("46", "31", 18, "+") << "\n";
    failed++;
  }
  if (calc("tt", "ei", 33, "-") == "fb") {
    passed++;
  } else {
    std::cout << "Test tt - ei (33) is failed: must return fb, returns: " << calc("tt", "ei", 33, "-") << "\n";
    failed++;
  }
  if (calc("pk", "i8", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test pk / i8 (30) is failed: must return 1, returns: " << calc("pk", "i8", 30, "/") << "\n";
    failed++;
  }
  if (calc("26j", "9", 20, "*") == "112b") {
    passed++;
  } else {
    std::cout << "Test 26j * 9 (20) is failed: must return 112b, returns: " << calc("26j", "9", 20, "*") << "\n";
    failed++;
  }
  if (calc("88", "3", 9, "*") == "286") {
    passed++;
  } else {
    std::cout << "Test 88 * 3 (9) is failed: must return 286, returns: " << calc("88", "3", 9, "*") << "\n";
    failed++;
  }
  if (calc("le", "cj", 27, "+") == "176") {
    passed++;
  } else {
    std::cout << "Test le + cj (27) is failed: must return 176, returns: " << calc("le", "cj", 27, "+") << "\n";
    failed++;
  }
  if (calc("72", "11", 21, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 72 / 11 (21) is failed: must return 6, returns: " << calc("72", "11", 21, "/") << "\n";
    failed++;
  }
  if (calc("1722", "742", 8, "-") == "760") {
    passed++;
  } else {
    std::cout << "Test 1722 - 742 (8) is failed: must return 760, returns: " << calc("1722", "742", 8, "-") << "\n";
    failed++;
  }
  if (calc("p9", "lh", 29, "*") == "ioa8") {
    passed++;
  } else {
    std::cout << "Test p9 * lh (29) is failed: must return ioa8, returns: " << calc("p9", "lh", 29, "*") << "\n";
    failed++;
  }
  if (calc("4m", "23", 32, "+") == "6p") {
    passed++;
  } else {
    std::cout << "Test 4m + 23 (32) is failed: must return 6p, returns: " << calc("4m", "23", 32, "+") << "\n";
    failed++;
  }
  if (calc("ki", "ir", 28, "+") == "1bh") {
    passed++;
  } else {
    std::cout << "Test ki + ir (28) is failed: must return 1bh, returns: " << calc("ki", "ir", 28, "+") << "\n";
    failed++;
  }
  if (calc("1f9", "1bf", 21, "*") == "2eff9") {
    passed++;
  } else {
    std::cout << "Test 1f9 * 1bf (21) is failed: must return 2eff9, returns: " << calc("1f9", "1bf", 21, "*") << "\n";
    failed++;
  }
  if (calc("3d", "2f", 33, "*") == "8au") {
    passed++;
  } else {
    std::cout << "Test 3d * 2f (33) is failed: must return 8au, returns: " << calc("3d", "2f", 33, "*") << "\n";
    failed++;
  }
  if (calc("381", "353", 11, "*") == "11aa73") {
    passed++;
  } else {
    std::cout << "Test 381 * 353 (11) is failed: must return 11aa73, returns: " << calc("381", "353", 11, "*") << "\n";
    failed++;
  }
  if (calc("de", "91", 35, "-") == "4d") {
    passed++;
  } else {
    std::cout << "Test de - 91 (35) is failed: must return 4d, returns: " << calc("de", "91", 35, "-") << "\n";
    failed++;
  }
  if (calc("np", "kk", 27, "*") == "ia6e") {
    passed++;
  } else {
    std::cout << "Test np * kk (27) is failed: must return ia6e, returns: " << calc("np", "kk", 27, "*") << "\n";
    failed++;
  }
  if (calc("189", "h1", 24, "*") == "mnh9") {
    passed++;
  } else {
    std::cout << "Test 189 * h1 (24) is failed: must return mnh9, returns: " << calc("189", "h1", 24, "*") << "\n";
    failed++;
  }
  if (calc("1503", "362", 8, "*") == "612526") {
    passed++;
  } else {
    std::cout << "Test 1503 * 362 (8) is failed: must return 612526, returns: " << calc("1503", "362", 8, "*") << "\n";
    failed++;
  }
  if (calc("549", "443", 12, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 549 / 443 (12) is failed: must return 1, returns: " << calc("549", "443", 12, "/") << "\n";
    failed++;
  }
  if (calc("15e", "9b", 18, "*") == "cc9a") {
    passed++;
  } else {
    std::cout << "Test 15e * 9b (18) is failed: must return cc9a, returns: " << calc("15e", "9b", 18, "*") << "\n";
    failed++;
  }
  if (calc("87", "64", 23, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 87 / 64 (23) is failed: must return 1, returns: " << calc("87", "64", 23, "/") << "\n";
    failed++;
  }
  if (calc("bp", "3m", 29, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test bp / 3m (29) is failed: must return 3, returns: " << calc("bp", "3m", 29, "/") << "\n";
    failed++;
  }
  if (calc("1ac", "188", 17, "*") == "278bb") {
    passed++;
  } else {
    std::cout << "Test 1ac * 188 (17) is failed: must return 278bb, returns: " << calc("1ac", "188", 17, "*") << "\n";
    failed++;
  }
  if (calc("1ch", "19l", 25, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1ch / 19l (25) is failed: must return 1, returns: " << calc("1ch", "19l", 25, "/") << "\n";
    failed++;
  }
  if (calc("122", "34", 5, "+") == "211") {
    passed++;
  } else {
    std::cout << "Test 122 + 34 (5) is failed: must return 211, returns: " << calc("122", "34", 5, "+") << "\n";
    failed++;
  }
  if (calc("24b", "226", 13, "+") == "474") {
    passed++;
  } else {
    std::cout << "Test 24b + 226 (13) is failed: must return 474, returns: " << calc("24b", "226", 13, "+") << "\n";
    failed++;
  }
  if (calc("36", "35", 28, "+") == "6b") {
    passed++;
  } else {
    std::cout << "Test 36 + 35 (28) is failed: must return 6b, returns: " << calc("36", "35", 28, "+") << "\n";
    failed++;
  }
  if (calc("qk", "1j", 31, "-") == "p1") {
    passed++;
  } else {
    std::cout << "Test qk - 1j (31) is failed: must return p1, returns: " << calc("qk", "1j", 31, "-") << "\n";
    failed++;
  }
  if (calc("1174", "1151", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1174 / 1151 (8) is failed: must return 1, returns: " << calc("1174", "1151", 8, "/") << "\n";
    failed++;
  }
  if (calc("em", "82", 25, "+") == "mo") {
    passed++;
  } else {
    std::cout << "Test em + 82 (25) is failed: must return mo, returns: " << calc("em", "82", 25, "+") << "\n";
    failed++;
  }
  if (calc("56", "u", 35, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 56 / u (35) is failed: must return 6, returns: " << calc("56", "u", 35, "/") << "\n";
    failed++;
  }
  if (calc("hf", "af", 25, "-") == "70") {
    passed++;
  } else {
    std::cout << "Test hf - af (25) is failed: must return 70, returns: " << calc("hf", "af", 25, "-") << "\n";
    failed++;
  }
  if (calc("73", "1l", 23, "*") == "deh") {
    passed++;
  } else {
    std::cout << "Test 73 * 1l (23) is failed: must return deh, returns: " << calc("73", "1l", 23, "*") << "\n";
    failed++;
  }
  if (calc("1h8", "1c6", 23, "*") == "2fjg2") {
    passed++;
  } else {
    std::cout << "Test 1h8 * 1c6 (23) is failed: must return 2fjg2, returns: " << calc("1h8", "1c6", 23, "*") << "\n";
    failed++;
  }
  if (calc("16o", "9d", 27, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 16o / 9d (27) is failed: must return 3, returns: " << calc("16o", "9d", 27, "/") << "\n";
    failed++;
  }
  if (calc("100", "65", 14, "+") == "165") {
    passed++;
  } else {
    std::cout << "Test 100 + 65 (14) is failed: must return 165, returns: " << calc("100", "65", 14, "+") << "\n";
    failed++;
  }
  if (calc("2h", "2c", 28, "+") == "51") {
    passed++;
  } else {
    std::cout << "Test 2h + 2c (28) is failed: must return 51, returns: " << calc("2h", "2c", 28, "+") << "\n";
    failed++;
  }
  if (calc("1ca", "165", 17, "*") == "2693g") {
    passed++;
  } else {
    std::cout << "Test 1ca * 165 (17) is failed: must return 2693g, returns: " << calc("1ca", "165", 17, "*") << "\n";
    failed++;
  }
  if (calc("12e", "8n", 27, "+") == "1ba") {
    passed++;
  } else {
    std::cout << "Test 12e + 8n (27) is failed: must return 1ba, returns: " << calc("12e", "8n", 27, "+") << "\n";
    failed++;
  }
  if (calc("194", "f8", 26, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 194 / f8 (26) is failed: must return 2, returns: " << calc("194", "f8", 26, "/") << "\n";
    failed++;
  }
  if (calc("401", "331", 6, "*") == "221131") {
    passed++;
  } else {
    std::cout << "Test 401 * 331 (6) is failed: must return 221131, returns: " << calc("401", "331", 6, "*") << "\n";
    failed++;
  }
  if (calc("1022210", "211010", 3, "-") == "111200") {
    passed++;
  } else {
    std::cout << "Test 1022210 - 211010 (3) is failed: must return 111200, returns: " << calc("1022210", "211010", 3, "-") << "\n";
    failed++;
  }
  if (calc("11342", "3240", 5, "-") == "3102") {
    passed++;
  } else {
    std::cout << "Test 11342 - 3240 (5) is failed: must return 3102, returns: " << calc("11342", "3240", 5, "-") << "\n";
    failed++;
  }
  if (calc("98", "51", 14, "+") == "109") {
    passed++;
  } else {
    std::cout << "Test 98 + 51 (14) is failed: must return 109, returns: " << calc("98", "51", 14, "+") << "\n";
    failed++;
  }
  if (calc("17b", "j1", 22, "+") == "24c") {
    passed++;
  } else {
    std::cout << "Test 17b + j1 (22) is failed: must return 24c, returns: " << calc("17b", "j1", 22, "+") << "\n";
    failed++;
  }
  if (calc("610", "b9", 12, "*") == "5b590") {
    passed++;
  } else {
    std::cout << "Test 610 * b9 (12) is failed: must return 5b590, returns: " << calc("610", "b9", 12, "*") << "\n";
    failed++;
  }
  if (calc("23j", "20", 20, "/") == "11") {
    passed++;
  } else {
    std::cout << "Test 23j / 20 (20) is failed: must return 11, returns: " << calc("23j", "20", 20, "/") << "\n";
    failed++;
  }
  if (calc("1j", "g", 27, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1j / g (27) is failed: must return 2, returns: " << calc("1j", "g", 27, "/") << "\n";
    failed++;
  }
  if (calc("1e9", "e9", 20, "*") == "14hg1") {
    passed++;
  } else {
    std::cout << "Test 1e9 * e9 (20) is failed: must return 14hg1, returns: " << calc("1e9", "e9", 20, "*") << "\n";
    failed++;
  }
  if (calc("7a8", "112", 11, "*") == "89175") {
    passed++;
  } else {
    std::cout << "Test 7a8 * 112 (11) is failed: must return 89175, returns: " << calc("7a8", "112", 11, "*") << "\n";
    failed++;
  }
  if (calc("449", "226", 15, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 449 / 226 (15) is failed: must return 1, returns: " << calc("449", "226", 15, "/") << "\n";
    failed++;
  }
  if (calc("137", "o8", 26, "-") == "4p") {
    passed++;
  } else {
    std::cout << "Test 137 - o8 (26) is failed: must return 4p, returns: " << calc("137", "o8", 26, "-") << "\n";
    failed++;
  }
  if (calc("1j1", "181", 21, "-") == "b0") {
    passed++;
  } else {
    std::cout << "Test 1j1 - 181 (21) is failed: must return b0, returns: " << calc("1j1", "181", 21, "-") << "\n";
    failed++;
  }
  if (calc("a3", "68", 21, "*") == "31f3") {
    passed++;
  } else {
    std::cout << "Test a3 * 68 (21) is failed: must return 31f3, returns: " << calc("a3", "68", 21, "*") << "\n";
    failed++;
  }
  if (calc("1441", "1211", 7, "*") == "2111351") {
    passed++;
  } else {
    std::cout << "Test 1441 * 1211 (7) is failed: must return 2111351, returns: " << calc("1441", "1211", 7, "*") << "\n";
    failed++;
  }
  if (calc("193", "168", 10, "*") == "32424") {
    passed++;
  } else {
    std::cout << "Test 193 * 168 (10) is failed: must return 32424, returns: " << calc("193", "168", 10, "*") << "\n";
    failed++;
  }
  if (calc("io", "8m", 28, "-") == "a2") {
    passed++;
  } else {
    std::cout << "Test io - 8m (28) is failed: must return a2, returns: " << calc("io", "8m", 28, "-") << "\n";
    failed++;
  }
  if (calc("13j", "82", 21, "*") == "9cch") {
    passed++;
  } else {
    std::cout << "Test 13j * 82 (21) is failed: must return 9cch, returns: " << calc("13j", "82", 21, "*") << "\n";
    failed++;
  }
  if (calc("528", "41", 10, "+") == "569") {
    passed++;
  } else {
    std::cout << "Test 528 + 41 (10) is failed: must return 569, returns: " << calc("528", "41", 10, "+") << "\n";
    failed++;
  }
  if (calc("b0", "4f", 21, "*") == "29i0") {
    passed++;
  } else {
    std::cout << "Test b0 * 4f (21) is failed: must return 29i0, returns: " << calc("b0", "4f", 21, "*") << "\n";
    failed++;
  }
  if (calc("539", "463", 13, "-") == "a6") {
    passed++;
  } else {
    std::cout << "Test 539 - 463 (13) is failed: must return a6, returns: " << calc("539", "463", 13, "-") << "\n";
    failed++;
  }
  if (calc("35b", "324", 15, "+") == "680") {
    passed++;
  } else {
    std::cout << "Test 35b + 324 (15) is failed: must return 680, returns: " << calc("35b", "324", 15, "+") << "\n";
    failed++;
  }
  if (calc("110110", "20121", 3, "*") == "10001001010") {
    passed++;
  } else {
    std::cout << "Test 110110 * 20121 (3) is failed: must return 10001001010, returns: " << calc("110110", "20121", 3, "*") << "\n";
    failed++;
  }
  if (calc("312", "312", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 312 / 312 (5) is failed: must return 1, returns: " << calc("312", "312", 5, "/") << "\n";
    failed++;
  }
  if (calc("120", "1g", 18, "/") == "a") {
    passed++;
  } else {
    std::cout << "Test 120 / 1g (18) is failed: must return a, returns: " << calc("120", "1g", 18, "/") << "\n";
    failed++;
  }
  if (calc("3ca", "15", 13, "*") == "568b") {
    passed++;
  } else {
    std::cout << "Test 3ca * 15 (13) is failed: must return 568b, returns: " << calc("3ca", "15", 13, "*") << "\n";
    failed++;
  }
  if (calc("4201", "2244", 5, "+") == "12000") {
    passed++;
  } else {
    std::cout << "Test 4201 + 2244 (5) is failed: must return 12000, returns: " << calc("4201", "2244", 5, "+") << "\n";
    failed++;
  }
  if (calc("1011202", "202200", 3, "+") == "1221102") {
    passed++;
  } else {
    std::cout << "Test 1011202 + 202200 (3) is failed: must return 1221102, returns: " << calc("1011202", "202200", 3, "+") << "\n";
    failed++;
  }
  if (calc("245", "1ff", 21, "+") == "3jk") {
    passed++;
  } else {
    std::cout << "Test 245 + 1ff (21) is failed: must return 3jk, returns: " << calc("245", "1ff", 21, "+") << "\n";
    failed++;
  }
  if (calc("1145", "50", 6, "+") == "1235") {
    passed++;
  } else {
    std::cout << "Test 1145 + 50 (6) is failed: must return 1235, returns: " << calc("1145", "50", 6, "+") << "\n";
    failed++;
  }
  if (calc("1001111011", "110000010", 2, "+") == "1111111101") {
    passed++;
  } else {
    std::cout << "Test 1001111011 + 110000010 (2) is failed: must return 1111111101, returns: " << calc("1001111011", "110000010", 2, "+") << "\n";
    failed++;
  }
  if (calc("rl", "bm", 29, "*") == "b6sr") {
    passed++;
  } else {
    std::cout << "Test rl * bm (29) is failed: must return b6sr, returns: " << calc("rl", "bm", 29, "*") << "\n";
    failed++;
  }
  if (calc("1110010000", "110100", 2, "+") == "1111000100") {
    passed++;
  } else {
    std::cout << "Test 1110010000 + 110100 (2) is failed: must return 1111000100, returns: " << calc("1110010000", "110100", 2, "+") << "\n";
    failed++;
  }
  if (calc("gk", "m", 31, "*") == "bp6") {
    passed++;
  } else {
    std::cout << "Test gk * m (31) is failed: must return bp6, returns: " << calc("gk", "m", 31, "*") << "\n";
    failed++;
  }
  if (calc("236", "1b5", 12, "*") == "457b6") {
    passed++;
  } else {
    std::cout << "Test 236 * 1b5 (12) is failed: must return 457b6, returns: " << calc("236", "1b5", 12, "*") << "\n";
    failed++;
  }
  if (calc("29", "21", 19, "*") == "519") {
    passed++;
  } else {
    std::cout << "Test 29 * 21 (19) is failed: must return 519, returns: " << calc("29", "21", 19, "*") << "\n";
    failed++;
  }
  if (calc("lj", "3e", 25, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test lj / 3e (25) is failed: must return 6, returns: " << calc("lj", "3e", 25, "/") << "\n";
    failed++;
  }
  if (calc("ma", "e0", 31, "+") == "15a") {
    passed++;
  } else {
    std::cout << "Test ma + e0 (31) is failed: must return 15a, returns: " << calc("ma", "e0", 31, "+") << "\n";
    failed++;
  }
  if (calc("92", "65", 18, "+") == "f7") {
    passed++;
  } else {
    std::cout << "Test 92 + 65 (18) is failed: must return f7, returns: " << calc("92", "65", 18, "+") << "\n";
    failed++;
  }
  if (calc("1663", "342", 7, "*") == "1011636") {
    passed++;
  } else {
    std::cout << "Test 1663 * 342 (7) is failed: must return 1011636, returns: " << calc("1663", "342", 7, "*") << "\n";
    failed++;
  }
  if (calc("ol", "41", 28, "*") == "3fol") {
    passed++;
  } else {
    std::cout << "Test ol * 41 (28) is failed: must return 3fol, returns: " << calc("ol", "41", 28, "*") << "\n";
    failed++;
  }
  if (calc("1643", "125", 7, "/") == "12") {
    passed++;
  } else {
    std::cout << "Test 1643 / 125 (7) is failed: must return 12, returns: " << calc("1643", "125", 7, "/") << "\n";
    failed++;
  }
  if (calc("2g", "b", 28, "+") == "2r") {
    passed++;
  } else {
    std::cout << "Test 2g + b (28) is failed: must return 2r, returns: " << calc("2g", "b", 28, "+") << "\n";
    failed++;
  }
  if (calc("q0", "92", 34, "+") == "112") {
    passed++;
  } else {
    std::cout << "Test q0 + 92 (34) is failed: must return 112, returns: " << calc("q0", "92", 34, "+") << "\n";
    failed++;
  }
  if (calc("jd", "35", 21, "-") == "g8") {
    passed++;
  } else {
    std::cout << "Test jd - 35 (21) is failed: must return g8, returns: " << calc("jd", "35", 21, "-") << "\n";
    failed++;
  }
  if (calc("442", "26", 15, "/") == "1b") {
    passed++;
  } else {
    std::cout << "Test 442 / 26 (15) is failed: must return 1b, returns: " << calc("442", "26", 15, "/") << "\n";
    failed++;
  }
  if (calc("89", "55", 17, "-") == "34") {
    passed++;
  } else {
    std::cout << "Test 89 - 55 (17) is failed: must return 34, returns: " << calc("89", "55", 17, "-") << "\n";
    failed++;
  }
  if (calc("145", "59", 18, "+") == "19e") {
    passed++;
  } else {
    std::cout << "Test 145 + 59 (18) is failed: must return 19e, returns: " << calc("145", "59", 18, "+") << "\n";
    failed++;
  }
  if (calc("h9", "22", 30, "-") == "f7") {
    passed++;
  } else {
    std::cout << "Test h9 - 22 (30) is failed: must return f7, returns: " << calc("h9", "22", 30, "-") << "\n";
    failed++;
  }
  if (calc("111001110", "111001", 2, "-") == "110010101") {
    passed++;
  } else {
    std::cout << "Test 111001110 - 111001 (2) is failed: must return 110010101, returns: " << calc("111001110", "111001", 2, "-") << "\n";
    failed++;
  }
  if (calc("101010111", "10001001", 2, "-") == "11001110") {
    passed++;
  } else {
    std::cout << "Test 101010111 - 10001001 (2) is failed: must return 11001110, returns: " << calc("101010111", "10001001", 2, "-") << "\n";
    failed++;
  }
  if (calc("11g", "29", 17, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test 11g / 29 (17) is failed: must return 7, returns: " << calc("11g", "29", 17, "/") << "\n";
    failed++;
  }
  if (calc("221012", "2102", 3, "/") == "101") {
    passed++;
  } else {
    std::cout << "Test 221012 / 2102 (3) is failed: must return 101, returns: " << calc("221012", "2102", 3, "/") << "\n";
    failed++;
  }
  if (calc("8l", "45", 31, "+") == "cq") {
    passed++;
  } else {
    std::cout << "Test 8l + 45 (31) is failed: must return cq, returns: " << calc("8l", "45", 31, "+") << "\n";
    failed++;
  }
  if (calc("1h4", "59", 19, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 1h4 / 59 (19) is failed: must return 6, returns: " << calc("1h4", "59", 19, "/") << "\n";
    failed++;
  }
  if (calc("1431", "1154", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1431 / 1154 (7) is failed: must return 1, returns: " << calc("1431", "1154", 7, "/") << "\n";
    failed++;
  }
  if (calc("ic", "9s", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ic / 9s (35) is failed: must return 1, returns: " << calc("ic", "9s", 35, "/") << "\n";
    failed++;
  }
  if (calc("50", "4s", 30, "+") == "9s") {
    passed++;
  } else {
    std::cout << "Test 50 + 4s (30) is failed: must return 9s, returns: " << calc("50", "4s", 30, "+") << "\n";
    failed++;
  }
  if (calc("1h", "1e", 25, "+") == "36") {
    passed++;
  } else {
    std::cout << "Test 1h + 1e (25) is failed: must return 36, returns: " << calc("1h", "1e", 25, "+") << "\n";
    failed++;
  }
  if (calc("364", "66", 12, "*") == "1ab20") {
    passed++;
  } else {
    std::cout << "Test 364 * 66 (12) is failed: must return 1ab20, returns: " << calc("364", "66", 12, "*") << "\n";
    failed++;
  }
  if (calc("29", "1k", 26, "+") == "43") {
    passed++;
  } else {
    std::cout << "Test 29 + 1k (26) is failed: must return 43, returns: " << calc("29", "1k", 26, "+") << "\n";
    failed++;
  }
  if (calc("21", "1c", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 21 / 1c (14) is failed: must return 1, returns: " << calc("21", "1c", 14, "/") << "\n";
    failed++;
  }
  if (calc("30011", "13210", 4, "*") == "1123111310") {
    passed++;
  } else {
    std::cout << "Test 30011 * 13210 (4) is failed: must return 1123111310, returns: " << calc("30011", "13210", 4, "*") << "\n";
    failed++;
  }
  if (calc("108", "e9", 20, "+") == "1eh") {
    passed++;
  } else {
    std::cout << "Test 108 + e9 (20) is failed: must return 1eh, returns: " << calc("108", "e9", 20, "+") << "\n";
    failed++;
  }
  if (calc("181", "4e", 17, "+") == "1cf") {
    passed++;
  } else {
    std::cout << "Test 181 + 4e (17) is failed: must return 1cf, returns: " << calc("181", "4e", 17, "+") << "\n";
    failed++;
  }
  if (calc("237", "235", 9, "-") == "2") {
    passed++;
  } else {
    std::cout << "Test 237 - 235 (9) is failed: must return 2, returns: " << calc("237", "235", 9, "-") << "\n";
    failed++;
  }
  if (calc("o", "1", 31, "+") == "p") {
    passed++;
  } else {
    std::cout << "Test o + 1 (31) is failed: must return p, returns: " << calc("o", "1", 31, "+") << "\n";
    failed++;
  }
  if (calc("114", "34", 16, "*") == "3810") {
    passed++;
  } else {
    std::cout << "Test 114 * 34 (16) is failed: must return 3810, returns: " << calc("114", "34", 16, "*") << "\n";
    failed++;
  }
  if (calc("915", "818", 10, "*") == "748470") {
    passed++;
  } else {
    std::cout << "Test 915 * 818 (10) is failed: must return 748470, returns: " << calc("915", "818", 10, "*") << "\n";
    failed++;
  }
  if (calc("1g", "g", 31, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1g / g (31) is failed: must return 2, returns: " << calc("1g", "g", 31, "/") << "\n";
    failed++;
  }
  if (calc("2e3", "205", 16, "-") == "de") {
    passed++;
  } else {
    std::cout << "Test 2e3 - 205 (16) is failed: must return de, returns: " << calc("2e3", "205", 16, "-") << "\n";
    failed++;
  }
  if (calc("203", "13g", 19, "*") == "27gba") {
    passed++;
  } else {
    std::cout << "Test 203 * 13g (19) is failed: must return 27gba, returns: " << calc("203", "13g", 19, "*") << "\n";
    failed++;
  }
  if (calc("i5", "62", 30, "+") == "o7") {
    passed++;
  } else {
    std::cout << "Test i5 + 62 (30) is failed: must return o7, returns: " << calc("i5", "62", 30, "+") << "\n";
    failed++;
  }
  if (calc("1c0", "2c", 19, "/") == "b") {
    passed++;
  } else {
    std::cout << "Test 1c0 / 2c (19) is failed: must return b, returns: " << calc("1c0", "2c", 19, "/") << "\n";
    failed++;
  }
  if (calc("9d", "5f", 20, "-") == "3i") {
    passed++;
  } else {
    std::cout << "Test 9d - 5f (20) is failed: must return 3i, returns: " << calc("9d", "5f", 20, "-") << "\n";
    failed++;
  }
  if (calc("69", "2", 13, "*") == "105") {
    passed++;
  } else {
    std::cout << "Test 69 * 2 (13) is failed: must return 105, returns: " << calc("69", "2", 13, "*") << "\n";
    failed++;
  }
  if (calc("1bi", "fh", 24, "+") == "23b") {
    passed++;
  } else {
    std::cout << "Test 1bi + fh (24) is failed: must return 23b, returns: " << calc("1bi", "fh", 24, "+") << "\n";
    failed++;
  }
  if (calc("1j7", "10", 20, "*") == "1j70") {
    passed++;
  } else {
    std::cout << "Test 1j7 * 10 (20) is failed: must return 1j70, returns: " << calc("1j7", "10", 20, "*") << "\n";
    failed++;
  }
  if (calc("m9", "1h", 34, "-") == "kq") {
    passed++;
  } else {
    std::cout << "Test m9 - 1h (34) is failed: must return kq, returns: " << calc("m9", "1h", 34, "-") << "\n";
    failed++;
  }
  if (calc("1110100100", "1110000", 2, "/") == "1000") {
    passed++;
  } else {
    std::cout << "Test 1110100100 / 1110000 (2) is failed: must return 1000, returns: " << calc("1110100100", "1110000", 2, "/") << "\n";
    failed++;
  }
  if (calc("1000010010", "10001", 2, "-") == "1000000001") {
    passed++;
  } else {
    std::cout << "Test 1000010010 - 10001 (2) is failed: must return 1000000001, returns: " << calc("1000010010", "10001", 2, "-") << "\n";
    failed++;
  }
  if (calc("1000000111", "11110111", 2, "/") == "10") {
    passed++;
  } else {
    std::cout << "Test 1000000111 / 11110111 (2) is failed: must return 10, returns: " << calc("1000000111", "11110111", 2, "/") << "\n";
    failed++;
  }
  if (calc("184", "ad", 14, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 184 / ad (14) is failed: must return 2, returns: " << calc("184", "ad", 14, "/") << "\n";
    failed++;
  }
  if (calc("1b0", "jk", 23, "*") == "168d0") {
    passed++;
  } else {
    std::cout << "Test 1b0 * jk (23) is failed: must return 168d0, returns: " << calc("1b0", "jk", 23, "*") << "\n";
    failed++;
  }
  if (calc("o9", "9k", 33, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test o9 / 9k (33) is failed: must return 2, returns: " << calc("o9", "9k", 33, "/") << "\n";
    failed++;
  }
  if (calc("3d", "p", 26, "-") == "2e") {
    passed++;
  } else {
    std::cout << "Test 3d - p (26) is failed: must return 2e, returns: " << calc("3d", "p", 26, "-") << "\n";
    failed++;
  }
  if (calc("166", "fc", 18, "+") == "240") {
    passed++;
  } else {
    std::cout << "Test 166 + fc (18) is failed: must return 240, returns: " << calc("166", "fc", 18, "+") << "\n";
    failed++;
  }
  if (calc("1665", "1105", 8, "+") == "2772") {
    passed++;
  } else {
    std::cout << "Test 1665 + 1105 (8) is failed: must return 2772, returns: " << calc("1665", "1105", 8, "+") << "\n";
    failed++;
  }
  if (calc("2", "2", 3, "*") == "11") {
    passed++;
  } else {
    std::cout << "Test 2 * 2 (3) is failed: must return 11, returns: " << calc("2", "2", 3, "*") << "\n";
    failed++;
  }
  if (calc("1a0", "11b", 19, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1a0 / 11b (19) is failed: must return 1, returns: " << calc("1a0", "11b", 19, "/") << "\n";
    failed++;
  }
  if (calc("7e", "4g", 22, "-") == "2k") {
    passed++;
  } else {
    std::cout << "Test 7e - 4g (22) is failed: must return 2k, returns: " << calc("7e", "4g", 22, "-") << "\n";
    failed++;
  }
  if (calc("142", "8", 11, "-") == "135") {
    passed++;
  } else {
    std::cout << "Test 142 - 8 (11) is failed: must return 135, returns: " << calc("142", "8", 11, "-") << "\n";
    failed++;
  }
  if (calc("80", "23", 25, "+") == "a3") {
    passed++;
  } else {
    std::cout << "Test 80 + 23 (25) is failed: must return a3, returns: " << calc("80", "23", 25, "+") << "\n";
    failed++;
  }
  if (calc("mb", "jg", 27, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test mb / jg (27) is failed: must return 1, returns: " << calc("mb", "jg", 27, "/") << "\n";
    failed++;
  }
  if (calc("qe", "fr", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test qe / fr (30) is failed: must return 1, returns: " << calc("qe", "fr", 30, "/") << "\n";
    failed++;
  }
  if (calc("k4", "8i", 21, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test k4 / 8i (21) is failed: must return 2, returns: " << calc("k4", "8i", 21, "/") << "\n";
    failed++;
  }
  if (calc("83", "38", 35, "*") == "q3o") {
    passed++;
  } else {
    std::cout << "Test 83 * 38 (35) is failed: must return q3o, returns: " << calc("83", "38", 35, "*") << "\n";
    failed++;
  }
  if (calc("267", "65", 8, "*") == "22743") {
    passed++;
  } else {
    std::cout << "Test 267 * 65 (8) is failed: must return 22743, returns: " << calc("267", "65", 8, "*") << "\n";
    failed++;
  }
  if (calc("d7", "ak", 25, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test d7 / ak (25) is failed: must return 1, returns: " << calc("d7", "ak", 25, "/") << "\n";
    failed++;
  }
  if (calc("344", "15", 14, "/") == "26") {
    passed++;
  } else {
    std::cout << "Test 344 / 15 (14) is failed: must return 26, returns: " << calc("344", "15", 14, "/") << "\n";
    failed++;
  }
  if (calc("qs", "dn", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test qs / dn (35) is failed: must return 1, returns: " << calc("qs", "dn", 35, "/") << "\n";
    failed++;
  }
  if (calc("dc", "68", 31, "*") == "2lo3") {
    passed++;
  } else {
    std::cout << "Test dc * 68 (31) is failed: must return 2lo3, returns: " << calc("dc", "68", 31, "*") << "\n";
    failed++;
  }
  if (calc("50", "26", 17, "+") == "76") {
    passed++;
  } else {
    std::cout << "Test 50 + 26 (17) is failed: must return 76, returns: " << calc("50", "26", 17, "+") << "\n";
    failed++;
  }
  if (calc("mv", "dl", 32, "*") == "9plb") {
    passed++;
  } else {
    std::cout << "Test mv * dl (32) is failed: must return 9plb, returns: " << calc("mv", "dl", 32, "*") << "\n";
    failed++;
  }
  if (calc("dn", "3k", 30, "*") == "1kea") {
    passed++;
  } else {
    std::cout << "Test dn * 3k (30) is failed: must return 1kea, returns: " << calc("dn", "3k", 30, "*") << "\n";
    failed++;
  }
  if (calc("67", "3j", 29, "+") == "9q") {
    passed++;
  } else {
    std::cout << "Test 67 + 3j (29) is failed: must return 9q, returns: " << calc("67", "3j", 29, "+") << "\n";
    failed++;
  }
  if (calc("ba", "11", 13, "/") == "a") {
    passed++;
  } else {
    std::cout << "Test ba / 11 (13) is failed: must return a, returns: " << calc("ba", "11", 13, "/") << "\n";
    failed++;
  }
  if (calc("h9", "e6", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test h9 / e6 (26) is failed: must return 1, returns: " << calc("h9", "e6", 26, "/") << "\n";
    failed++;
  }
  if (calc("cb", "a0", 20, "-") == "2b") {
    passed++;
  } else {
    std::cout << "Test cb - a0 (20) is failed: must return 2b, returns: " << calc("cb", "a0", 20, "-") << "\n";
    failed++;
  }
  if (calc("1455", "152", 8, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test 1455 / 152 (8) is failed: must return 7, returns: " << calc("1455", "152", 8, "/") << "\n";
    failed++;
  }
  if (calc("703", "453", 8, "-") == "230") {
    passed++;
  } else {
    std::cout << "Test 703 - 453 (8) is failed: must return 230, returns: " << calc("703", "453", 8, "-") << "\n";
    failed++;
  }
  if (calc("sl", "ln", 31, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test sl / ln (31) is failed: must return 1, returns: " << calc("sl", "ln", 31, "/") << "\n";
    failed++;
  }
  if (calc("1a0", "3", 19, "/") == "9c") {
    passed++;
  } else {
    std::cout << "Test 1a0 / 3 (19) is failed: must return 9c, returns: " << calc("1a0", "3", 19, "/") << "\n";
    failed++;
  }
  if (calc("12c", "8b", 22, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 12c / 8b (22) is failed: must return 2, returns: " << calc("12c", "8b", 22, "/") << "\n";
    failed++;
  }
  if (calc("2c6", "1c1", 17, "*") == "4b3g6") {
    passed++;
  } else {
    std::cout << "Test 2c6 * 1c1 (17) is failed: must return 4b3g6, returns: " << calc("2c6", "1c1", 17, "*") << "\n";
    failed++;
  }
  if (calc("19f", "183", 23, "-") == "1c") {
    passed++;
  } else {
    std::cout << "Test 19f - 183 (23) is failed: must return 1c, returns: " << calc("19f", "183", 23, "-") << "\n";
    failed++;
  }
  if (calc("6c", "3", 32, "*") == "j4") {
    passed++;
  } else {
    std::cout << "Test 6c * 3 (32) is failed: must return j4, returns: " << calc("6c", "3", 32, "*") << "\n";
    failed++;
  }
  if (calc("7h", "5b", 25, "+") == "d3") {
    passed++;
  } else {
    std::cout << "Test 7h + 5b (25) is failed: must return d3, returns: " << calc("7h", "5b", 25, "+") << "\n";
    failed++;
  }
  if (calc("364", "15", 11, "+") == "379") {
    passed++;
  } else {
    std::cout << "Test 364 + 15 (11) is failed: must return 379, returns: " << calc("364", "15", 11, "+") << "\n";
    failed++;
  }
  if (calc("10k", "am", 23, "*") == "b8c3") {
    passed++;
  } else {
    std::cout << "Test 10k * am (23) is failed: must return b8c3, returns: " << calc("10k", "am", 23, "*") << "\n";
    failed++;
  }
  if (calc("109", "2", 21, "*") == "20i") {
    passed++;
  } else {
    std::cout << "Test 109 * 2 (21) is failed: must return 20i, returns: " << calc("109", "2", 21, "*") << "\n";
    failed++;
  }
  if (calc("1d1", "2a", 15, "/") == "a") {
    passed++;
  } else {
    std::cout << "Test 1d1 / 2a (15) is failed: must return a, returns: " << calc("1d1", "2a", 15, "/") << "\n";
    failed++;
  }
  if (calc("920", "148", 10, "-") == "772") {
    passed++;
  } else {
    std::cout << "Test 920 - 148 (10) is failed: must return 772, returns: " << calc("920", "148", 10, "-") << "\n";
    failed++;
  }
  if (calc("326", "18", 10, "-") == "308") {
    passed++;
  } else {
    std::cout << "Test 326 - 18 (10) is failed: must return 308, returns: " << calc("326", "18", 10, "-") << "\n";
    failed++;
  }
  if (calc("54", "5", 11, "-") == "4a") {
    passed++;
  } else {
    std::cout << "Test 54 - 5 (11) is failed: must return 4a, returns: " << calc("54", "5", 11, "-") << "\n";
    failed++;
  }
  if (calc("d5", "19", 24, "*") == "i3l") {
    passed++;
  } else {
    std::cout << "Test d5 * 19 (24) is failed: must return i3l, returns: " << calc("d5", "19", 24, "*") << "\n";
    failed++;
  }
  if (calc("lf", "k5", 22, "+") == "1jk") {
    passed++;
  } else {
    std::cout << "Test lf + k5 (22) is failed: must return 1jk, returns: " << calc("lf", "k5", 22, "+") << "\n";
    failed++;
  }
  if (calc("210", "14a", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 210 / 14a (14) is failed: must return 1, returns: " << calc("210", "14a", 14, "/") << "\n";
    failed++;
  }
  if (calc("10", "7", 16, "*") == "70") {
    passed++;
  } else {
    std::cout << "Test 10 * 7 (16) is failed: must return 70, returns: " << calc("10", "7", 16, "*") << "\n";
    failed++;
  }
  if (calc("hc", "a6", 29, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test hc / a6 (29) is failed: must return 1, returns: " << calc("hc", "a6", 29, "/") << "\n";
    failed++;
  }
  if (calc("16k", "61", 21, "-") == "10j") {
    passed++;
  } else {
    std::cout << "Test 16k - 61 (21) is failed: must return 10j, returns: " << calc("16k", "61", 21, "-") << "\n";
    failed++;
  }
  if (calc("1c1", "6c", 17, "+") == "21d") {
    passed++;
  } else {
    std::cout << "Test 1c1 + 6c (17) is failed: must return 21d, returns: " << calc("1c1", "6c", 17, "+") << "\n";
    failed++;
  }
  if (calc("5b", "40", 28, "*") == "lg0") {
    passed++;
  } else {
    std::cout << "Test 5b * 40 (28) is failed: must return lg0, returns: " << calc("5b", "40", 28, "*") << "\n";
    failed++;
  }
  if (calc("1044", "1000", 6, "-") == "44") {
    passed++;
  } else {
    std::cout << "Test 1044 - 1000 (6) is failed: must return 44, returns: " << calc("1044", "1000", 6, "-") << "\n";
    failed++;
  }
  if (calc("a", "7", 21, "-") == "3") {
    passed++;
  } else {
    std::cout << "Test a - 7 (21) is failed: must return 3, returns: " << calc("a", "7", 21, "-") << "\n";
    failed++;
  }
  if (calc("1hc", "120", 18, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1hc / 120 (18) is failed: must return 1, returns: " << calc("1hc", "120", 18, "/") << "\n";
    failed++;
  }
  if (calc("1011100101", "1010010111", 2, "-") == "1001110") {
    passed++;
  } else {
    std::cout << "Test 1011100101 - 1010010111 (2) is failed: must return 1001110, returns: " << calc("1011100101", "1010010111", 2, "-") << "\n";
    failed++;
  }
  if (calc("9e", "7i", 30, "-") == "1q") {
    passed++;
  } else {
    std::cout << "Test 9e - 7i (30) is failed: must return 1q, returns: " << calc("9e", "7i", 30, "-") << "\n";
    failed++;
  }
  if (calc("378", "34c", 14, "*") == "bbcac") {
    passed++;
  } else {
    std::cout << "Test 378 * 34c (14) is failed: must return bbcac, returns: " << calc("378", "34c", 14, "*") << "\n";
    failed++;
  }
  if (calc("46", "45", 11, "*") == "1928") {
    passed++;
  } else {
    std::cout << "Test 46 * 45 (11) is failed: must return 1928, returns: " << calc("46", "45", 11, "*") << "\n";
    failed++;
  }
  if (calc("jd", "dj", 29, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test jd / dj (29) is failed: must return 1, returns: " << calc("jd", "dj", 29, "/") << "\n";
    failed++;
  }
  if (calc("1011111010", "1011101110", 2, "-") == "1100") {
    passed++;
  } else {
    std::cout << "Test 1011111010 - 1011101110 (2) is failed: must return 1100, returns: " << calc("1011111010", "1011101110", 2, "-") << "\n";
    failed++;
  }
  if (calc("457", "17b", 15, "*") == "69342") {
    passed++;
  } else {
    std::cout << "Test 457 * 17b (15) is failed: must return 69342, returns: " << calc("457", "17b", 15, "*") << "\n";
    failed++;
  }
  if (calc("8k", "55", 30, "-") == "3f") {
    passed++;
  } else {
    std::cout << "Test 8k - 55 (30) is failed: must return 3f, returns: " << calc("8k", "55", 30, "-") << "\n";
    failed++;
  }
  if (calc("210", "64", 19, "*") == "ce40") {
    passed++;
  } else {
    std::cout << "Test 210 * 64 (19) is failed: must return ce40, returns: " << calc("210", "64", 19, "*") << "\n";
    failed++;
  }
  if (calc("460", "221", 9, "+") == "681") {
    passed++;
  } else {
    std::cout << "Test 460 + 221 (9) is failed: must return 681, returns: " << calc("460", "221", 9, "+") << "\n";
    failed++;
  }
  if (calc("uu", "lt", 31, "-") == "91") {
    passed++;
  } else {
    std::cout << "Test uu - lt (31) is failed: must return 91, returns: " << calc("uu", "lt", 31, "-") << "\n";
    failed++;
  }
  if (calc("oc", "he", 27, "-") == "6p") {
    passed++;
  } else {
    std::cout << "Test oc - he (27) is failed: must return 6p, returns: " << calc("oc", "he", 27, "-") << "\n";
    failed++;
  }
  if (calc("2002", "313", 4, "-") == "1023") {
    passed++;
  } else {
    std::cout << "Test 2002 - 313 (4) is failed: must return 1023, returns: " << calc("2002", "313", 4, "-") << "\n";
    failed++;
  }
  if (calc("91", "83", 27, "-") == "p") {
    passed++;
  } else {
    std::cout << "Test 91 - 83 (27) is failed: must return p, returns: " << calc("91", "83", 27, "-") << "\n";
    failed++;
  }
  if (calc("17d", "4d", 16, "+") == "1ca") {
    passed++;
  } else {
    std::cout << "Test 17d + 4d (16) is failed: must return 1ca, returns: " << calc("17d", "4d", 16, "+") << "\n";
    failed++;
  }
  if (calc("46", "8", 19, "+") == "4e") {
    passed++;
  } else {
    std::cout << "Test 46 + 8 (19) is failed: must return 4e, returns: " << calc("46", "8", 19, "+") << "\n";
    failed++;
  }
  if (calc("2213", "332", 7, "*") == "1101346") {
    passed++;
  } else {
    std::cout << "Test 2213 * 332 (7) is failed: must return 1101346, returns: " << calc("2213", "332", 7, "*") << "\n";
    failed++;
  }
  if (calc("ee", "f", 17, "+") == "fc") {
    passed++;
  } else {
    std::cout << "Test ee + f (17) is failed: must return fc, returns: " << calc("ee", "f", 17, "+") << "\n";
    failed++;
  }
  if (calc("13111", "10033", 4, "-") == "3012") {
    passed++;
  } else {
    std::cout << "Test 13111 - 10033 (4) is failed: must return 3012, returns: " << calc("13111", "10033", 4, "-") << "\n";
    failed++;
  }
  if (calc("12442", "10243", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 12442 / 10243 (5) is failed: must return 1, returns: " << calc("12442", "10243", 5, "/") << "\n";
    failed++;
  }
  if (calc("220", "12b", 12, "-") == "b1") {
    passed++;
  } else {
    std::cout << "Test 220 - 12b (12) is failed: must return b1, returns: " << calc("220", "12b", 12, "-") << "\n";
    failed++;
  }
  if (calc("227", "b6", 12, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 227 / b6 (12) is failed: must return 2, returns: " << calc("227", "b6", 12, "/") << "\n";
    failed++;
  }
  if (calc("644", "4a4", 12, "*") == "26b094") {
    passed++;
  } else {
    std::cout << "Test 644 * 4a4 (12) is failed: must return 26b094, returns: " << calc("644", "4a4", 12, "*") << "\n";
    failed++;
  }
  if (calc("1de", "18", 23, "/") == "14") {
    passed++;
  } else {
    std::cout << "Test 1de / 18 (23) is failed: must return 14, returns: " << calc("1de", "18", 23, "/") << "\n";
    failed++;
  }
  if (calc("202122", "122020", 3, "-") == "10102") {
    passed++;
  } else {
    std::cout << "Test 202122 - 122020 (3) is failed: must return 10102, returns: " << calc("202122", "122020", 3, "-") << "\n";
    failed++;
  }
  if (calc("124", "67", 24, "*") == "6kf4") {
    passed++;
  } else {
    std::cout << "Test 124 * 67 (24) is failed: must return 6kf4, returns: " << calc("124", "67", 24, "*") << "\n";
    failed++;
  }
  if (calc("130", "120", 5, "*") == "21100") {
    passed++;
  } else {
    std::cout << "Test 130 * 120 (5) is failed: must return 21100, returns: " << calc("130", "120", 5, "*") << "\n";
    failed++;
  }
  if (calc("p9", "hs", 29, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test p9 / hs (29) is failed: must return 1, returns: " << calc("p9", "hs", 29, "/") << "\n";
    failed++;
  }
  if (calc("1dl", "f0", 24, "-") == "ml") {
    passed++;
  } else {
    std::cout << "Test 1dl - f0 (24) is failed: must return ml, returns: " << calc("1dl", "f0", 24, "-") << "\n";
    failed++;
  }
  if (calc("if", "30", 29, "*") == "1qg0") {
    passed++;
  } else {
    std::cout << "Test if * 30 (29) is failed: must return 1qg0, returns: " << calc("if", "30", 29, "*") << "\n";
    failed++;
  }
  if (calc("ll", "j9", 35, "+") == "15u") {
    passed++;
  } else {
    std::cout << "Test ll + j9 (35) is failed: must return 15u, returns: " << calc("ll", "j9", 35, "+") << "\n";
    failed++;
  }
  if (calc("3132", "2202", 4, "+") == "12000") {
    passed++;
  } else {
    std::cout << "Test 3132 + 2202 (4) is failed: must return 12000, returns: " << calc("3132", "2202", 4, "+") << "\n";
    failed++;
  }
  if (calc("3t", "1b", 32, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 3t / 1b (32) is failed: must return 2, returns: " << calc("3t", "1b", 32, "/") << "\n";
    failed++;
  }
  if (calc("12123", "10111", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 12123 / 10111 (5) is failed: must return 1, returns: " << calc("12123", "10111", 5, "/") << "\n";
    failed++;
  }
  if (calc("1020021", "212020", 3, "*") == "1001121100120") {
    passed++;
  } else {
    std::cout << "Test 1020021 * 212020 (3) is failed: must return 1001121100120, returns: " << calc("1020021", "212020", 3, "*") << "\n";
    failed++;
  }
  if (calc("g2", "32", 19, "*") == "2c04") {
    passed++;
  } else {
    std::cout << "Test g2 * 32 (19) is failed: must return 2c04, returns: " << calc("g2", "32", 19, "*") << "\n";
    failed++;
  }
  if (calc("d5", "71", 28, "-") == "64") {
    passed++;
  } else {
    std::cout << "Test d5 - 71 (28) is failed: must return 64, returns: " << calc("d5", "71", 28, "-") << "\n";
    failed++;
  }
  if (calc("24f", "166", 19, "+") == "3b2") {
    passed++;
  } else {
    std::cout << "Test 24f + 166 (19) is failed: must return 3b2, returns: " << calc("24f", "166", 19, "+") << "\n";
    failed++;
  }
  if (calc("1bf", "8f", 17, "*") == "f194") {
    passed++;
  } else {
    std::cout << "Test 1bf * 8f (17) is failed: must return f194, returns: " << calc("1bf", "8f", 17, "*") << "\n";
    failed++;
  }
  if (calc("110", "cg", 21, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 110 / cg (21) is failed: must return 1, returns: " << calc("110", "cg", 21, "/") << "\n";
    failed++;
  }
  if (calc("182", "40", 9, "-") == "142") {
    passed++;
  } else {
    std::cout << "Test 182 - 40 (9) is failed: must return 142, returns: " << calc("182", "40", 9, "-") << "\n";
    failed++;
  }
  if (calc("11014", "1", 5, "-") == "11013") {
    passed++;
  } else {
    std::cout << "Test 11014 - 1 (5) is failed: must return 11013, returns: " << calc("11014", "1", 5, "-") << "\n";
    failed++;
  }
  if (calc("jt", "a0", 30, "*") == "6jk0") {
    passed++;
  } else {
    std::cout << "Test jt * a0 (30) is failed: must return 6jk0, returns: " << calc("jt", "a0", 30, "*") << "\n";
    failed++;
  }
  if (calc("73", "5d", 15, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 73 / 5d (15) is failed: must return 1, returns: " << calc("73", "5d", 15, "/") << "\n";
    failed++;
  }
  if (calc("2d3", "1f2", 17, "-") == "f1") {
    passed++;
  } else {
    std::cout << "Test 2d3 - 1f2 (17) is failed: must return f1, returns: " << calc("2d3", "1f2", 17, "-") << "\n";
    failed++;
  }
  if (calc("2g9", "101", 17, "*") == "2gbg9") {
    passed++;
  } else {
    std::cout << "Test 2g9 * 101 (17) is failed: must return 2gbg9, returns: " << calc("2g9", "101", 17, "*") << "\n";
    failed++;
  }
  if (calc("421", "12", 5, "+") == "433") {
    passed++;
  } else {
    std::cout << "Test 421 + 12 (5) is failed: must return 433, returns: " << calc("421", "12", 5, "+") << "\n";
    failed++;
  }
  if (calc("1201", "655", 7, "*") == "1153255") {
    passed++;
  } else {
    std::cout << "Test 1201 * 655 (7) is failed: must return 1153255, returns: " << calc("1201", "655", 7, "*") << "\n";
    failed++;
  }
  if (calc("20d", "ke", 21, "-") == "10k") {
    passed++;
  } else {
    std::cout << "Test 20d - ke (21) is failed: must return 10k, returns: " << calc("20d", "ke", 21, "-") << "\n";
    failed++;
  }
  if (calc("2000", "530", 6, "-") == "1030") {
    passed++;
  } else {
    std::cout << "Test 2000 - 530 (6) is failed: must return 1030, returns: " << calc("2000", "530", 6, "-") << "\n";
    failed++;
  }
  if (calc("100111000", "100010000", 2, "-") == "101000") {
    passed++;
  } else {
    std::cout << "Test 100111000 - 100010000 (2) is failed: must return 101000, returns: " << calc("100111000", "100010000", 2, "-") << "\n";
    failed++;
  }
  if (calc("1d1", "1a1", 16, "-") == "30") {
    passed++;
  } else {
    std::cout << "Test 1d1 - 1a1 (16) is failed: must return 30, returns: " << calc("1d1", "1a1", 16, "-") << "\n";
    failed++;
  }
  if (calc("247", "157", 9, "-") == "80") {
    passed++;
  } else {
    std::cout << "Test 247 - 157 (9) is failed: must return 80, returns: " << calc("247", "157", 9, "-") << "\n";
    failed++;
  }
  if (calc("939", "129", 10, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test 939 / 129 (10) is failed: must return 7, returns: " << calc("939", "129", 10, "/") << "\n";
    failed++;
  }
  if (calc("6p", "n", 31, "-") == "62") {
    passed++;
  } else {
    std::cout << "Test 6p - n (31) is failed: must return 62, returns: " << calc("6p", "n", 31, "-") << "\n";
    failed++;
  }
  if (calc("lm", "4h", 28, "+") == "qb") {
    passed++;
  } else {
    std::cout << "Test lm + 4h (28) is failed: must return qb, returns: " << calc("lm", "4h", 28, "+") << "\n";
    failed++;
  }
  if (calc("f4", "g", 21, "+") == "fk") {
    passed++;
  } else {
    std::cout << "Test f4 + g (21) is failed: must return fk, returns: " << calc("f4", "g", 21, "+") << "\n";
    failed++;
  }
  if (calc("1b", "5", 32, "/") == "8") {
    passed++;
  } else {
    std::cout << "Test 1b / 5 (32) is failed: must return 8, returns: " << calc("1b", "5", 32, "/") << "\n";
    failed++;
  }
  if (calc("8g", "57", 26, "-") == "39") {
    passed++;
  } else {
    std::cout << "Test 8g - 57 (26) is failed: must return 39, returns: " << calc("8g", "57", 26, "-") << "\n";
    failed++;
  }
  if (calc("11102", "11", 5, "*") == "122122") {
    passed++;
  } else {
    std::cout << "Test 11102 * 11 (5) is failed: must return 122122, returns: " << calc("11102", "11", 5, "*") << "\n";
    failed++;
  }
  if (calc("1a3", "18a", 12, "*") == "32766") {
    passed++;
  } else {
    std::cout << "Test 1a3 * 18a (12) is failed: must return 32766, returns: " << calc("1a3", "18a", 12, "*") << "\n";
    failed++;
  }
  if (calc("175", "154", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 175 / 154 (8) is failed: must return 1, returns: " << calc("175", "154", 8, "/") << "\n";
    failed++;
  }
  if (calc("672", "383", 12, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 672 / 383 (12) is failed: must return 1, returns: " << calc("672", "383", 12, "/") << "\n";
    failed++;
  }
  if (calc("1r", "11", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1r / 11 (33) is failed: must return 1, returns: " << calc("1r", "11", 33, "/") << "\n";
    failed++;
  }
  if (calc("2a", "5", 16, "+") == "2f") {
    passed++;
  } else {
    std::cout << "Test 2a + 5 (16) is failed: must return 2f, returns: " << calc("2a", "5", 16, "+") << "\n";
    failed++;
  }
  if (calc("63", "24", 28, "+") == "87") {
    passed++;
  } else {
    std::cout << "Test 63 + 24 (28) is failed: must return 87, returns: " << calc("63", "24", 28, "+") << "\n";
    failed++;
  }
  if (calc("20120", "2", 3, "-") == "20111") {
    passed++;
  } else {
    std::cout << "Test 20120 - 2 (3) is failed: must return 20111, returns: " << calc("20120", "2", 3, "-") << "\n";
    failed++;
  }
  if (calc("2422", "2330", 5, "*") == "12414310") {
    passed++;
  } else {
    std::cout << "Test 2422 * 2330 (5) is failed: must return 12414310, returns: " << calc("2422", "2330", 5, "*") << "\n";
    failed++;
  }
  if (calc("k7", "h8", 32, "-") == "2v") {
    passed++;
  } else {
    std::cout << "Test k7 - h8 (32) is failed: must return 2v, returns: " << calc("k7", "h8", 32, "-") << "\n";
    failed++;
  }
  if (calc("312", "68", 9, "+") == "381") {
    passed++;
  } else {
    std::cout << "Test 312 + 68 (9) is failed: must return 381, returns: " << calc("312", "68", 9, "+") << "\n";
    failed++;
  }
  if (calc("4c", "1b", 22, "*") == "6i0") {
    passed++;
  } else {
    std::cout << "Test 4c * 1b (22) is failed: must return 6i0, returns: " << calc("4c", "1b", 22, "*") << "\n";
    failed++;
  }
  if (calc("9n", "30", 25, "*") == "14j0") {
    passed++;
  } else {
    std::cout << "Test 9n * 30 (25) is failed: must return 14j0, returns: " << calc("9n", "30", 25, "*") << "\n";
    failed++;
  }
  if (calc("352", "17c", 14, "+") == "4d0") {
    passed++;
  } else {
    std::cout << "Test 352 + 17c (14) is failed: must return 4d0, returns: " << calc("352", "17c", 14, "+") << "\n";
    failed++;
  }
  if (calc("3a0", "9a", 14, "*") == "28120") {
    passed++;
  } else {
    std::cout << "Test 3a0 * 9a (14) is failed: must return 28120, returns: " << calc("3a0", "9a", 14, "*") << "\n";
    failed++;
  }
  if (calc("10d", "hk", 29, "-") == "bm") {
    passed++;
  } else {
    std::cout << "Test 10d - hk (29) is failed: must return bm, returns: " << calc("10d", "hk", 29, "-") << "\n";
    failed++;
  }
  if (calc("db", "a", 22, "/") == "17") {
    passed++;
  } else {
    std::cout << "Test db / a (22) is failed: must return 17, returns: " << calc("db", "a", 22, "/") << "\n";
    failed++;
  }
  if (calc("hn", "98", 31, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test hn / 98 (31) is failed: must return 1, returns: " << calc("hn", "98", 31, "/") << "\n";
    failed++;
  }
  if (calc("4e", "d", 17, "*") == "3bc") {
    passed++;
  } else {
    std::cout << "Test 4e * d (17) is failed: must return 3bc, returns: " << calc("4e", "d", 17, "*") << "\n";
    failed++;
  }
  if (calc("jv", "d8", 33, "+") == "106") {
    passed++;
  } else {
    std::cout << "Test jv + d8 (33) is failed: must return 106, returns: " << calc("jv", "d8", 33, "+") << "\n";
    failed++;
  }
  if (calc("92", "6w", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 92 / 6w (33) is failed: must return 1, returns: " << calc("92", "6w", 33, "/") << "\n";
    failed++;
  }
  if (calc("12312", "2403", 5, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 12312 / 2403 (5) is failed: must return 2, returns: " << calc("12312", "2403", 5, "/") << "\n";
    failed++;
  }
  if (calc("348", "73", 13, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 348 / 73 (13) is failed: must return 6, returns: " << calc("348", "73", 13, "/") << "\n";
    failed++;
  }
  if (calc("312", "21", 11, "+") == "333") {
    passed++;
  } else {
    std::cout << "Test 312 + 21 (11) is failed: must return 333, returns: " << calc("312", "21", 11, "+") << "\n";
    failed++;
  }
  if (calc("11", "1", 15, "+") == "12") {
    passed++;
  } else {
    std::cout << "Test 11 + 1 (15) is failed: must return 12, returns: " << calc("11", "1", 15, "+") << "\n";
    failed++;
  }
  if (calc("2k", "5", 22, "+") == "33") {
    passed++;
  } else {
    std::cout << "Test 2k + 5 (22) is failed: must return 33, returns: " << calc("2k", "5", 22, "+") << "\n";
    failed++;
  }
  if (calc("21032", "230", 4, "+") == "21322") {
    passed++;
  } else {
    std::cout << "Test 21032 + 230 (4) is failed: must return 21322, returns: " << calc("21032", "230", 4, "+") << "\n";
    failed++;
  }
  if (calc("1hf", "e3", 22, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1hf / e3 (22) is failed: must return 2, returns: " << calc("1hf", "e3", 22, "/") << "\n";
    failed++;
  }
  if (calc("32203", "2230", 4, "*") == "213012010") {
    passed++;
  } else {
    std::cout << "Test 32203 * 2230 (4) is failed: must return 213012010, returns: " << calc("32203", "2230", 4, "*") << "\n";
    failed++;
  }
  if (calc("1gd", "192", 18, "-") == "7b") {
    passed++;
  } else {
    std::cout << "Test 1gd - 192 (18) is failed: must return 7b, returns: " << calc("1gd", "192", 18, "-") << "\n";
    failed++;
  }
  if (calc("779", "76", 10, "-") == "703") {
    passed++;
  } else {
    std::cout << "Test 779 - 76 (10) is failed: must return 703, returns: " << calc("779", "76", 10, "-") << "\n";
    failed++;
  }
  if (calc("43", "c", 20, "*") == "29g") {
    passed++;
  } else {
    std::cout << "Test 43 * c (20) is failed: must return 29g, returns: " << calc("43", "c", 20, "*") << "\n";
    failed++;
  }
  if (calc("sg", "hf", 29, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test sg / hf (29) is failed: must return 1, returns: " << calc("sg", "hf", 29, "/") << "\n";
    failed++;
  }
  if (calc("ki", "c9", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ki / c9 (24) is failed: must return 1, returns: " << calc("ki", "c9", 24, "/") << "\n";
    failed++;
  }
  if (calc("1l", "g", 29, "+") == "28") {
    passed++;
  } else {
    std::cout << "Test 1l + g (29) is failed: must return 28, returns: " << calc("1l", "g", 29, "+") << "\n";
    failed++;
  }
  if (calc("242", "137", 20, "+") == "379") {
    passed++;
  } else {
    std::cout << "Test 242 + 137 (20) is failed: must return 379, returns: " << calc("242", "137", 20, "+") << "\n";
    failed++;
  }
  if (calc("1506", "301", 7, "+") == "2110") {
    passed++;
  } else {
    std::cout << "Test 1506 + 301 (7) is failed: must return 2110, returns: " << calc("1506", "301", 7, "+") << "\n";
    failed++;
  }
  if (calc("hl", "fh", 29, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test hl / fh (29) is failed: must return 1, returns: " << calc("hl", "fh", 29, "/") << "\n";
    failed++;
  }
  if (calc("2111", "445", 6, "+") == "3000") {
    passed++;
  } else {
    std::cout << "Test 2111 + 445 (6) is failed: must return 3000, returns: " << calc("2111", "445", 6, "+") << "\n";
    failed++;
  }
  if (calc("10r", "ec", 29, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 10r / ec (29) is failed: must return 2, returns: " << calc("10r", "ec", 29, "/") << "\n";
    failed++;
  }
  if (calc("1k", "1f", 27, "*") == "2j3") {
    passed++;
  } else {
    std::cout << "Test 1k * 1f (27) is failed: must return 2j3, returns: " << calc("1k", "1f", 27, "*") << "\n";
    failed++;
  }
  if (calc("g0", "50", 35, "-") == "b0") {
    passed++;
  } else {
    std::cout << "Test g0 - 50 (35) is failed: must return b0, returns: " << calc("g0", "50", 35, "-") << "\n";
    failed++;
  }
  if (calc("i6", "bg", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test i6 / bg (30) is failed: must return 1, returns: " << calc("i6", "bg", 30, "/") << "\n";
    failed++;
  }
  if (calc("32113", "31312", 4, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 32113 / 31312 (4) is failed: must return 1, returns: " << calc("32113", "31312", 4, "/") << "\n";
    failed++;
  }
  if (calc("fk", "4o", 35, "*") == "22xp") {
    passed++;
  } else {
    std::cout << "Test fk * 4o (35) is failed: must return 22xp, returns: " << calc("fk", "4o", 35, "*") << "\n";
    failed++;
  }
  if (calc("565", "6a", 11, "+") == "624") {
    passed++;
  } else {
    std::cout << "Test 565 + 6a (11) is failed: must return 624, returns: " << calc("565", "6a", 11, "+") << "\n";
    failed++;
  }
  if (calc("678", "173", 11, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 678 / 173 (11) is failed: must return 4, returns: " << calc("678", "173", 11, "/") << "\n";
    failed++;
  }
  if (calc("122101", "12021", 3, "/") == "10") {
    passed++;
  } else {
    std::cout << "Test 122101 / 12021 (3) is failed: must return 10, returns: " << calc("122101", "12021", 3, "/") << "\n";
    failed++;
  }
  if (calc("an", "2n", 32, "*") == "t4h") {
    passed++;
  } else {
    std::cout << "Test an * 2n (32) is failed: must return t4h, returns: " << calc("an", "2n", 32, "*") << "\n";
    failed++;
  }
  if (calc("8g", "2g", 18, "+") == "be") {
    passed++;
  } else {
    std::cout << "Test 8g + 2g (18) is failed: must return be, returns: " << calc("8g", "2g", 18, "+") << "\n";
    failed++;
  }
  if (calc("109", "2o", 28, "/") == "9") {
    passed++;
  } else {
    std::cout << "Test 109 / 2o (28) is failed: must return 9, returns: " << calc("109", "2o", 28, "/") << "\n";
    failed++;
  }
  if (calc("1gg", "6a", 20, "+") == "236") {
    passed++;
  } else {
    std::cout << "Test 1gg + 6a (20) is failed: must return 236, returns: " << calc("1gg", "6a", 20, "+") << "\n";
    failed++;
  }
  if (calc("16c", "3e", 16, "*") == "5828") {
    passed++;
  } else {
    std::cout << "Test 16c * 3e (16) is failed: must return 5828, returns: " << calc("16c", "3e", 16, "*") << "\n";
    failed++;
  }
  if (calc("1211", "341", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1211 / 341 (5) is failed: must return 1, returns: " << calc("1211", "341", 5, "/") << "\n";
    failed++;
  }
  if (calc("512", "216", 10, "-") == "296") {
    passed++;
  } else {
    std::cout << "Test 512 - 216 (10) is failed: must return 296, returns: " << calc("512", "216", 10, "-") << "\n";
    failed++;
  }
  if (calc("176", "8j", 23, "+") == "1g2") {
    passed++;
  } else {
    std::cout << "Test 176 + 8j (23) is failed: must return 1g2, returns: " << calc("176", "8j", 23, "+") << "\n";
    failed++;
  }
  if (calc("127", "92", 10, "*") == "11684") {
    passed++;
  } else {
    std::cout << "Test 127 * 92 (10) is failed: must return 11684, returns: " << calc("127", "92", 10, "*") << "\n";
    failed++;
  }
  if (calc("7", "4", 21, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 7 / 4 (21) is failed: must return 1, returns: " << calc("7", "4", 21, "/") << "\n";
    failed++;
  }
  if (calc("31303", "1102", 4, "*") == "101223112") {
    passed++;
  } else {
    std::cout << "Test 31303 * 1102 (4) is failed: must return 101223112, returns: " << calc("31303", "1102", 4, "*") << "\n";
    failed++;
  }
  if (calc("2dg", "1b1", 17, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 2dg / 1b1 (17) is failed: must return 1, returns: " << calc("2dg", "1b1", 17, "/") << "\n";
    failed++;
  }
  if (calc("257", "231", 11, "*") == "58047") {
    passed++;
  } else {
    std::cout << "Test 257 * 231 (11) is failed: must return 58047, returns: " << calc("257", "231", 11, "*") << "\n";
    failed++;
  }
  if (calc("20e", "49", 21, "*") == "8kk0") {
    passed++;
  } else {
    std::cout << "Test 20e * 49 (21) is failed: must return 8kk0, returns: " << calc("20e", "49", 21, "*") << "\n";
    failed++;
  }
  if (calc("1720", "1477", 8, "-") == "221") {
    passed++;
  } else {
    std::cout << "Test 1720 - 1477 (8) is failed: must return 221, returns: " << calc("1720", "1477", 8, "-") << "\n";
    failed++;
  }
  if (calc("86", "4l", 28, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 86 / 4l (28) is failed: must return 1, returns: " << calc("86", "4l", 28, "/") << "\n";
    failed++;
  }
  if (calc("1110001010", "11100101", 2, "*") == "110010101001110010") {
    passed++;
  } else {
    std::cout << "Test 1110001010 * 11100101 (2) is failed: must return 110010101001110010, returns: " << calc("1110001010", "11100101", 2, "*") << "\n";
    failed++;
  }
  if (calc("197", "126", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 197 / 126 (20) is failed: must return 1, returns: " << calc("197", "126", 20, "/") << "\n";
    failed++;
  }
  if (calc("157", "jg", 24, "+") == "20n") {
    passed++;
  } else {
    std::cout << "Test 157 + jg (24) is failed: must return 20n, returns: " << calc("157", "jg", 24, "+") << "\n";
    failed++;
  }
  if (calc("b5", "29", 22, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test b5 / 29 (22) is failed: must return 4, returns: " << calc("b5", "29", 22, "/") << "\n";
    failed++;
  }
  if (calc("2114", "1153", 6, "+") == "3311") {
    passed++;
  } else {
    std::cout << "Test 2114 + 1153 (6) is failed: must return 3311, returns: " << calc("2114", "1153", 6, "+") << "\n";
    failed++;
  }
  if (calc("v", "l", 35, "+") == "1h") {
    passed++;
  } else {
    std::cout << "Test v + l (35) is failed: must return 1h, returns: " << calc("v", "l", 35, "+") << "\n";
    failed++;
  }
  if (calc("371", "2e8", 17, "+") == "649") {
    passed++;
  } else {
    std::cout << "Test 371 + 2e8 (17) is failed: must return 649, returns: " << calc("371", "2e8", 17, "+") << "\n";
    failed++;
  }
  if (calc("180", "153", 15, "*") == "20e90") {
    passed++;
  } else {
    std::cout << "Test 180 * 153 (15) is failed: must return 20e90, returns: " << calc("180", "153", 15, "*") << "\n";
    failed++;
  }
  if (calc("5b7", "357", 13, "*") == "1722ba") {
    passed++;
  } else {
    std::cout << "Test 5b7 * 357 (13) is failed: must return 1722ba, returns: " << calc("5b7", "357", 13, "*") << "\n";
    failed++;
  }
  if (calc("1de", "5b", 22, "+") == "1j3") {
    passed++;
  } else {
    std::cout << "Test 1de + 5b (22) is failed: must return 1j3, returns: " << calc("1de", "5b", 22, "+") << "\n";
    failed++;
  }
  if (calc("7d", "46", 30, "-") == "37") {
    passed++;
  } else {
    std::cout << "Test 7d - 46 (30) is failed: must return 37, returns: " << calc("7d", "46", 30, "-") << "\n";
    failed++;
  }
  if (calc("10d", "105", 27, "-") == "8") {
    passed++;
  } else {
    std::cout << "Test 10d - 105 (27) is failed: must return 8, returns: " << calc("10d", "105", 27, "-") << "\n";
    failed++;
  }
  if (calc("ej", "1h", 22, "+") == "ge") {
    passed++;
  } else {
    std::cout << "Test ej + 1h (22) is failed: must return ge, returns: " << calc("ej", "1h", 22, "+") << "\n";
    failed++;
  }
  if (calc("1j", "1", 26, "+") == "1k") {
    passed++;
  } else {
    std::cout << "Test 1j + 1 (26) is failed: must return 1k, returns: " << calc("1j", "1", 26, "+") << "\n";
    failed++;
  }
  if (calc("13221", "312", 4, "-") == "12303") {
    passed++;
  } else {
    std::cout << "Test 13221 - 312 (4) is failed: must return 12303, returns: " << calc("13221", "312", 4, "-") << "\n";
    failed++;
  }
  if (calc("km", "gf", 34, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test km / gf (34) is failed: must return 1, returns: " << calc("km", "gf", 34, "/") << "\n";
    failed++;
  }
  if (calc("je", "i5", 29, "-") == "19") {
    passed++;
  } else {
    std::cout << "Test je - i5 (29) is failed: must return 19, returns: " << calc("je", "i5", 29, "-") << "\n";
    failed++;
  }
  if (calc("63", "21", 13, "-") == "42") {
    passed++;
  } else {
    std::cout << "Test 63 - 21 (13) is failed: must return 42, returns: " << calc("63", "21", 13, "-") << "\n";
    failed++;
  }
  if (calc("1cl", "9a", 25, "+") == "1m6") {
    passed++;
  } else {
    std::cout << "Test 1cl + 9a (25) is failed: must return 1m6, returns: " << calc("1cl", "9a", 25, "+") << "\n";
    failed++;
  }
  if (calc("2113", "1660", 7, "-") == "123") {
    passed++;
  } else {
    std::cout << "Test 2113 - 1660 (7) is failed: must return 123, returns: " << calc("2113", "1660", 7, "-") << "\n";
    failed++;
  }
  if (calc("351", "22", 9, "/") == "15") {
    passed++;
  } else {
    std::cout << "Test 351 / 22 (9) is failed: must return 15, returns: " << calc("351", "22", 9, "/") << "\n";
    failed++;
  }
  if (calc("3000", "2", 4, "-") == "2332") {
    passed++;
  } else {
    std::cout << "Test 3000 - 2 (4) is failed: must return 2332, returns: " << calc("3000", "2", 4, "-") << "\n";
    failed++;
  }
  if (calc("29b", "153", 14, "-") == "148") {
    passed++;
  } else {
    std::cout << "Test 29b - 153 (14) is failed: must return 148, returns: " << calc("29b", "153", 14, "-") << "\n";
    failed++;
  }
  if (calc("83", "5a", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 83 / 5a (20) is failed: must return 1, returns: " << calc("83", "5a", 20, "/") << "\n";
    failed++;
  }
  if (calc("ku", "jn", 31, "*") == "dat8") {
    passed++;
  } else {
    std::cout << "Test ku * jn (31) is failed: must return dat8, returns: " << calc("ku", "jn", 31, "*") << "\n";
    failed++;
  }
  if (calc("cl", "5h", 30, "*") == "2akr") {
    passed++;
  } else {
    std::cout << "Test cl * 5h (30) is failed: must return 2akr, returns: " << calc("cl", "5h", 30, "*") << "\n";
    failed++;
  }
  if (calc("4114", "304", 6, "/") == "12") {
    passed++;
  } else {
    std::cout << "Test 4114 / 304 (6) is failed: must return 12, returns: " << calc("4114", "304", 6, "/") << "\n";
    failed++;
  }
  if (calc("1dh", "13", 21, "/") == "19") {
    passed++;
  } else {
    std::cout << "Test 1dh / 13 (21) is failed: must return 19, returns: " << calc("1dh", "13", 21, "/") << "\n";
    failed++;
  }
  if (calc("pr", "26", 30, "/") == "b") {
    passed++;
  } else {
    std::cout << "Test pr / 26 (30) is failed: must return b, returns: " << calc("pr", "26", 30, "/") << "\n";
    failed++;
  }
  if (calc("a7", "97", 14, "*") == "71a7") {
    passed++;
  } else {
    std::cout << "Test a7 * 97 (14) is failed: must return 71a7, returns: " << calc("a7", "97", 14, "*") << "\n";
    failed++;
  }
  if (calc("138", "52", 11, "*") == "6a35") {
    passed++;
  } else {
    std::cout << "Test 138 * 52 (11) is failed: must return 6a35, returns: " << calc("138", "52", 11, "*") << "\n";
    failed++;
  }
  if (calc("1324", "4", 6, "*") == "10144") {
    passed++;
  } else {
    std::cout << "Test 1324 * 4 (6) is failed: must return 10144, returns: " << calc("1324", "4", 6, "*") << "\n";
    failed++;
  }
  if (calc("a0", "67", 12, "+") == "147") {
    passed++;
  } else {
    std::cout << "Test a0 + 67 (12) is failed: must return 147, returns: " << calc("a0", "67", 12, "+") << "\n";
    failed++;
  }
  if (calc("234", "59", 15, "-") == "1ca") {
    passed++;
  } else {
    std::cout << "Test 234 - 59 (15) is failed: must return 1ca, returns: " << calc("234", "59", 15, "-") << "\n";
    failed++;
  }
  if (calc("69", "a", 15, "-") == "5e") {
    passed++;
  } else {
    std::cout << "Test 69 - a (15) is failed: must return 5e, returns: " << calc("69", "a", 15, "-") << "\n";
    failed++;
  }
  if (calc("10i", "82", 24, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 10i / 82 (24) is failed: must return 3, returns: " << calc("10i", "82", 24, "/") << "\n";
    failed++;
  }
  if (calc("di", "2m", 25, "-") == "al") {
    passed++;
  } else {
    std::cout << "Test di - 2m (25) is failed: must return al, returns: " << calc("di", "2m", 25, "-") << "\n";
    failed++;
  }
  if (calc("13", "11", 12, "+") == "24") {
    passed++;
  } else {
    std::cout << "Test 13 + 11 (12) is failed: must return 24, returns: " << calc("13", "11", 12, "+") << "\n";
    failed++;
  }
  if (calc("1ib", "1be", 21, "*") == "2ja27") {
    passed++;
  } else {
    std::cout << "Test 1ib * 1be (21) is failed: must return 2ja27, returns: " << calc("1ib", "1be", 21, "*") << "\n";
    failed++;
  }
  if (calc("5", "4", 30, "-") == "1") {
    passed++;
  } else {
    std::cout << "Test 5 - 4 (30) is failed: must return 1, returns: " << calc("5", "4", 30, "-") << "\n";
    failed++;
  }
  if (calc("6n", "51", 24, "*") == "1b1n") {
    passed++;
  } else {
    std::cout << "Test 6n * 51 (24) is failed: must return 1b1n, returns: " << calc("6n", "51", 24, "*") << "\n";
    failed++;
  }
  if (calc("2j", "1d", 24, "-") == "16") {
    passed++;
  } else {
    std::cout << "Test 2j - 1d (24) is failed: must return 16, returns: " << calc("2j", "1d", 24, "-") << "\n";
    failed++;
  }
  if (calc("13", "13", 17, "*") == "169") {
    passed++;
  } else {
    std::cout << "Test 13 * 13 (17) is failed: must return 169, returns: " << calc("13", "13", 17, "*") << "\n";
    failed++;
  }
  if (calc("2121", "2110", 3, "*") == "20100010") {
    passed++;
  } else {
    std::cout << "Test 2121 * 2110 (3) is failed: must return 20100010, returns: " << calc("2121", "2110", 3, "*") << "\n";
    failed++;
  }
  if (calc("28b", "9", 20, "/") == "57") {
    passed++;
  } else {
    std::cout << "Test 28b / 9 (20) is failed: must return 57, returns: " << calc("28b", "9", 20, "/") << "\n";
    failed++;
  }
  if (calc("183", "98", 11, "+") == "270") {
    passed++;
  } else {
    std::cout << "Test 183 + 98 (11) is failed: must return 270, returns: " << calc("183", "98", 11, "+") << "\n";
    failed++;
  }
  if (calc("430", "36b", 13, "-") == "92") {
    passed++;
  } else {
    std::cout << "Test 430 - 36b (13) is failed: must return 92, returns: " << calc("430", "36b", 13, "-") << "\n";
    failed++;
  }
  if (calc("4b", "1o", 33, "-") == "2k") {
    passed++;
  } else {
    std::cout << "Test 4b - 1o (33) is failed: must return 2k, returns: " << calc("4b", "1o", 33, "-") << "\n";
    failed++;
  }
  if (calc("29b", "16i", 19, "-") == "12c") {
    passed++;
  } else {
    std::cout << "Test 29b - 16i (19) is failed: must return 12c, returns: " << calc("29b", "16i", 19, "-") << "\n";
    failed++;
  }
  if (calc("68", "5b", 15, "*") == "276d") {
    passed++;
  } else {
    std::cout << "Test 68 * 5b (15) is failed: must return 276d, returns: " << calc("68", "5b", 15, "*") << "\n";
    failed++;
  }
  if (calc("3050", "1100", 6, "-") == "1550") {
    passed++;
  } else {
    std::cout << "Test 3050 - 1100 (6) is failed: must return 1550, returns: " << calc("3050", "1100", 6, "-") << "\n";
    failed++;
  }
  if (calc("530", "ac", 13, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 530 / ac (13) is failed: must return 6, returns: " << calc("530", "ac", 13, "/") << "\n";
    failed++;
  }
  if (calc("24b", "1dc", 16, "+") == "427") {
    passed++;
  } else {
    std::cout << "Test 24b + 1dc (16) is failed: must return 427, returns: " << calc("24b", "1dc", 16, "+") << "\n";
    failed++;
  }
  if (calc("9g", "8k", 29, "*") == "2p01") {
    passed++;
  } else {
    std::cout << "Test 9g * 8k (29) is failed: must return 2p01, returns: " << calc("9g", "8k", 29, "*") << "\n";
    failed++;
  }
  if (calc("146", "3", 17, "-") == "143") {
    passed++;
  } else {
    std::cout << "Test 146 - 3 (17) is failed: must return 143, returns: " << calc("146", "3", 17, "-") << "\n";
    failed++;
  }
  if (calc("n6", "3b", 25, "+") == "11h") {
    passed++;
  } else {
    std::cout << "Test n6 + 3b (25) is failed: must return 11h, returns: " << calc("n6", "3b", 25, "+") << "\n";
    failed++;
  }
  if (calc("3n", "1o", 33, "+") == "5e") {
    passed++;
  } else {
    std::cout << "Test 3n + 1o (33) is failed: must return 5e, returns: " << calc("3n", "1o", 33, "+") << "\n";
    failed++;
  }
  if (calc("2033", "1222", 4, "+") == "3321") {
    passed++;
  } else {
    std::cout << "Test 2033 + 1222 (4) is failed: must return 3321, returns: " << calc("2033", "1222", 4, "+") << "\n";
    failed++;
  }
  if (calc("2103", "1322", 6, "+") == "3425") {
    passed++;
  } else {
    std::cout << "Test 2103 + 1322 (6) is failed: must return 3425, returns: " << calc("2103", "1322", 6, "+") << "\n";
    failed++;
  }
  if (calc("180", "157", 10, "*") == "28260") {
    passed++;
  } else {
    std::cout << "Test 180 * 157 (10) is failed: must return 28260, returns: " << calc("180", "157", 10, "*") << "\n";
    failed++;
  }
  if (calc("ic", "dg", 21, "-") == "4h") {
    passed++;
  } else {
    std::cout << "Test ic - dg (21) is failed: must return 4h, returns: " << calc("ic", "dg", 21, "-") << "\n";
    failed++;
  }
  if (calc("nc", "ie", 25, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test nc / ie (25) is failed: must return 1, returns: " << calc("nc", "ie", 25, "/") << "\n";
    failed++;
  }
  if (calc("ef", "ed", 29, "+") == "ss") {
    passed++;
  } else {
    std::cout << "Test ef + ed (29) is failed: must return ss, returns: " << calc("ef", "ed", 29, "+") << "\n";
    failed++;
  }
  if (calc("11k", "k8", 26, "-") == "7c") {
    passed++;
  } else {
    std::cout << "Test 11k - k8 (26) is failed: must return 7c, returns: " << calc("11k", "k8", 26, "-") << "\n";
    failed++;
  }
  if (calc("dh", "85", 22, "-") == "5c") {
    passed++;
  } else {
    std::cout << "Test dh - 85 (22) is failed: must return 5c, returns: " << calc("dh", "85", 22, "-") << "\n";
    failed++;
  }
  if (calc("553", "12", 7, "/") == "43") {
    passed++;
  } else {
    std::cout << "Test 553 / 12 (7) is failed: must return 43, returns: " << calc("553", "12", 7, "/") << "\n";
    failed++;
  }
  if (calc("0", "0", 19, "-") == "0") {
    passed++;
  } else {
    std::cout << "Test 0 - 0 (19) is failed: must return 0, returns: " << calc("0", "0", 19, "-") << "\n";
    failed++;
  }
  if (calc("1g4", "188", 20, "-") == "7g") {
    passed++;
  } else {
    std::cout << "Test 1g4 - 188 (20) is failed: must return 7g, returns: " << calc("1g4", "188", 20, "-") << "\n";
    failed++;
  }
  if (calc("9p", "1m", 34, "-") == "83") {
    passed++;
  } else {
    std::cout << "Test 9p - 1m (34) is failed: must return 83, returns: " << calc("9p", "1m", 34, "-") << "\n";
    failed++;
  }
  if (calc("687", "620", 10, "-") == "67") {
    passed++;
  } else {
    std::cout << "Test 687 - 620 (10) is failed: must return 67, returns: " << calc("687", "620", 10, "-") << "\n";
    failed++;
  }
  if (calc("76", "6d", 28, "+") == "dj") {
    passed++;
  } else {
    std::cout << "Test 76 + 6d (28) is failed: must return dj, returns: " << calc("76", "6d", 28, "+") << "\n";
    failed++;
  }
  if (calc("2d1", "14b", 14, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 2d1 / 14b (14) is failed: must return 2, returns: " << calc("2d1", "14b", 14, "/") << "\n";
    failed++;
  }
  if (calc("c7", "9t", 33, "+") == "m3") {
    passed++;
  } else {
    std::cout << "Test c7 + 9t (33) is failed: must return m3, returns: " << calc("c7", "9t", 33, "+") << "\n";
    failed++;
  }
  if (calc("422", "417", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 422 / 417 (9) is failed: must return 1, returns: " << calc("422", "417", 9, "/") << "\n";
    failed++;
  }
  if (calc("199", "bn", 27, "+") == "1l5") {
    passed++;
  } else {
    std::cout << "Test 199 + bn (27) is failed: must return 1l5, returns: " << calc("199", "bn", 27, "+") << "\n";
    failed++;
  }
  if (calc("1eb", "196", 18, "*") == "2d96c") {
    passed++;
  } else {
    std::cout << "Test 1eb * 196 (18) is failed: must return 2d96c, returns: " << calc("1eb", "196", 18, "*") << "\n";
    failed++;
  }
  if (calc("kq", "8r", 34, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test kq / 8r (34) is failed: must return 2, returns: " << calc("kq", "8r", 34, "/") << "\n";
    failed++;
  }
  if (calc("671", "181", 12, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 671 / 181 (12) is failed: must return 3, returns: " << calc("671", "181", 12, "/") << "\n";
    failed++;
  }
  if (calc("jk", "dh", 35, "-") == "63") {
    passed++;
  } else {
    std::cout << "Test jk - dh (35) is failed: must return 63, returns: " << calc("jk", "dh", 35, "-") << "\n";
    failed++;
  }
  if (calc("1a1", "11i", 19, "+") == "2c0") {
    passed++;
  } else {
    std::cout << "Test 1a1 + 11i (19) is failed: must return 2c0, returns: " << calc("1a1", "11i", 19, "+") << "\n";
    failed++;
  }
  if (calc("21211", "10220", 3, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 21211 / 10220 (3) is failed: must return 2, returns: " << calc("21211", "10220", 3, "/") << "\n";
    failed++;
  }
  if (calc("12003", "10310", 5, "*") == "124301430") {
    passed++;
  } else {
    std::cout << "Test 12003 * 10310 (5) is failed: must return 124301430, returns: " << calc("12003", "10310", 5, "*") << "\n";
    failed++;
  }
  if (calc("15f", "ca", 24, "-") == "h5") {
    passed++;
  } else {
    std::cout << "Test 15f - ca (24) is failed: must return h5, returns: " << calc("15f", "ca", 24, "-") << "\n";
    failed++;
  }
  if (calc("cg", "c0", 26, "-") == "g") {
    passed++;
  } else {
    std::cout << "Test cg - c0 (26) is failed: must return g, returns: " << calc("cg", "c0", 26, "-") << "\n";
    failed++;
  }
  if (calc("ne", "1l", 28, "*") == "1d3e") {
    passed++;
  } else {
    std::cout << "Test ne * 1l (28) is failed: must return 1d3e, returns: " << calc("ne", "1l", 28, "*") << "\n";
    failed++;
  }
  if (calc("753", "217", 11, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 753 / 217 (11) is failed: must return 3, returns: " << calc("753", "217", 11, "/") << "\n";
    failed++;
  }
  if (calc("336", "272", 10, "*") == "91392") {
    passed++;
  } else {
    std::cout << "Test 336 * 272 (10) is failed: must return 91392, returns: " << calc("336", "272", 10, "*") << "\n";
    failed++;
  }
  if (calc("2143", "1321", 7, "*") == "3200533") {
    passed++;
  } else {
    std::cout << "Test 2143 * 1321 (7) is failed: must return 3200533, returns: " << calc("2143", "1321", 7, "*") << "\n";
    failed++;
  }
  if (calc("3340", "1120", 6, "-") == "2220") {
    passed++;
  } else {
    std::cout << "Test 3340 - 1120 (6) is failed: must return 2220, returns: " << calc("3340", "1120", 6, "-") << "\n";
    failed++;
  }
  if (calc("104", "77", 11, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 104 / 77 (11) is failed: must return 1, returns: " << calc("104", "77", 11, "/") << "\n";
    failed++;
  }
  if (calc("rr", "qp", 34, "*") == "lt2t") {
    passed++;
  } else {
    std::cout << "Test rr * qp (34) is failed: must return lt2t, returns: " << calc("rr", "qp", 34, "*") << "\n";
    failed++;
  }
  if (calc("bm", "9e", 35, "+") == "l1") {
    passed++;
  } else {
    std::cout << "Test bm + 9e (35) is failed: must return l1, returns: " << calc("bm", "9e", 35, "+") << "\n";
    failed++;
  }
  if (calc("254", "114", 15, "-") == "140") {
    passed++;
  } else {
    std::cout << "Test 254 - 114 (15) is failed: must return 140, returns: " << calc("254", "114", 15, "-") << "\n";
    failed++;
  }
  if (calc("354", "126", 7, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 354 / 126 (7) is failed: must return 2, returns: " << calc("354", "126", 7, "/") << "\n";
    failed++;
  }
  if (calc("f4", "d5", 35, "-") == "1y") {
    passed++;
  } else {
    std::cout << "Test f4 - d5 (35) is failed: must return 1y, returns: " << calc("f4", "d5", 35, "-") << "\n";
    failed++;
  }
  if (calc("37", "31", 11, "-") == "6") {
    passed++;
  } else {
    std::cout << "Test 37 - 31 (11) is failed: must return 6, returns: " << calc("37", "31", 11, "-") << "\n";
    failed++;
  }
  if (calc("2c8", "211", 14, "+") == "4d9") {
    passed++;
  } else {
    std::cout << "Test 2c8 + 211 (14) is failed: must return 4d9, returns: " << calc("2c8", "211", 14, "+") << "\n";
    failed++;
  }
  if (calc("464", "364", 8, "+") == "1050") {
    passed++;
  } else {
    std::cout << "Test 464 + 364 (8) is failed: must return 1050, returns: " << calc("464", "364", 8, "+") << "\n";
    failed++;
  }
  if (calc("am", "8n", 27, "-") == "1q") {
    passed++;
  } else {
    std::cout << "Test am - 8n (27) is failed: must return 1q, returns: " << calc("am", "8n", 27, "-") << "\n";
    failed++;
  }
  if (calc("1c7", "16", 19, "*") == "2354") {
    passed++;
  } else {
    std::cout << "Test 1c7 * 16 (19) is failed: must return 2354, returns: " << calc("1c7", "16", 19, "*") << "\n";
    failed++;
  }
  if (calc("76", "6h", 24, "-") == "d") {
    passed++;
  } else {
    std::cout << "Test 76 - 6h (24) is failed: must return d, returns: " << calc("76", "6h", 24, "-") << "\n";
    failed++;
  }
  if (calc("1gg", "gk", 24, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1gg / gk (24) is failed: must return 2, returns: " << calc("1gg", "gk", 24, "/") << "\n";
    failed++;
  }
  if (calc("76", "72", 21, "+") == "e8") {
    passed++;
  } else {
    std::cout << "Test 76 + 72 (21) is failed: must return e8, returns: " << calc("76", "72", 21, "+") << "\n";
    failed++;
  }
  if (calc("mo", "bj", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test mo / bj (35) is failed: must return 1, returns: " << calc("mo", "bj", 35, "/") << "\n";
    failed++;
  }
  if (calc("12e", "7a", 16, "+") == "1a8") {
    passed++;
  } else {
    std::cout << "Test 12e + 7a (16) is failed: must return 1a8, returns: " << calc("12e", "7a", 16, "+") << "\n";
    failed++;
  }
  if (calc("20", "1v", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 20 / 1v (33) is failed: must return 1, returns: " << calc("20", "1v", 33, "/") << "\n";
    failed++;
  }
  if (calc("2f4", "148", 17, "*") == "3b31f") {
    passed++;
  } else {
    std::cout << "Test 2f4 * 148 (17) is failed: must return 3b31f, returns: " << calc("2f4", "148", 17, "*") << "\n";
    failed++;
  }
  if (calc("10320", "3101", 5, "*") == "33102320") {
    passed++;
  } else {
    std::cout << "Test 10320 * 3101 (5) is failed: must return 33102320, returns: " << calc("10320", "3101", 5, "*") << "\n";
    failed++;
  }
  if (calc("338", "291", 10, "*") == "98358") {
    passed++;
  } else {
    std::cout << "Test 338 * 291 (10) is failed: must return 98358, returns: " << calc("338", "291", 10, "*") << "\n";
    failed++;
  }
  if (calc("7s", "6i", 30, "*") == "1mao") {
    passed++;
  } else {
    std::cout << "Test 7s * 6i (30) is failed: must return 1mao, returns: " << calc("7s", "6i", 30, "*") << "\n";
    failed++;
  }
  if (calc("349", "92", 11, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 349 / 92 (11) is failed: must return 4, returns: " << calc("349", "92", 11, "/") << "\n";
    failed++;
  }
  if (calc("1011100000", "1001111100", 2, "-") == "1100100") {
    passed++;
  } else {
    std::cout << "Test 1011100000 - 1001111100 (2) is failed: must return 1100100, returns: " << calc("1011100000", "1001111100", 2, "-") << "\n";
    failed++;
  }
  if (calc("247", "ae", 21, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 247 / ae (21) is failed: must return 4, returns: " << calc("247", "ae", 21, "/") << "\n";
    failed++;
  }
  if (calc("1511", "1464", 8, "*") == "2502724") {
    passed++;
  } else {
    std::cout << "Test 1511 * 1464 (8) is failed: must return 2502724, returns: " << calc("1511", "1464", 8, "*") << "\n";
    failed++;
  }
  if (calc("o7", "5b", 27, "*") == "4n4n") {
    passed++;
  } else {
    std::cout << "Test o7 * 5b (27) is failed: must return 4n4n, returns: " << calc("o7", "5b", 27, "*") << "\n";
    failed++;
  }
  if (calc("13m", "fh", 23, "-") == "b5") {
    passed++;
  } else {
    std::cout << "Test 13m - fh (23) is failed: must return b5, returns: " << calc("13m", "fh", 23, "-") << "\n";
    failed++;
  }
  if (calc("111000110", "1101111", 2, "/") == "100") {
    passed++;
  } else {
    std::cout << "Test 111000110 / 1101111 (2) is failed: must return 100, returns: " << calc("111000110", "1101111", 2, "/") << "\n";
    failed++;
  }
  if (calc("fy", "3f", 35, "-") == "cj") {
    passed++;
  } else {
    std::cout << "Test fy - 3f (35) is failed: must return cj, returns: " << calc("fy", "3f", 35, "-") << "\n";
    failed++;
  }
  if (calc("22b", "122", 20, "*") == "27072") {
    passed++;
  } else {
    std::cout << "Test 22b * 122 (20) is failed: must return 27072, returns: " << calc("22b", "122", 20, "*") << "\n";
    failed++;
  }
  if (calc("9r", "66", 35, "+") == "fx") {
    passed++;
  } else {
    std::cout << "Test 9r + 66 (35) is failed: must return fx, returns: " << calc("9r", "66", 35, "+") << "\n";
    failed++;
  }
  if (calc("101120", "100220", 3, "-") == "200") {
    passed++;
  } else {
    std::cout << "Test 101120 - 100220 (3) is failed: must return 200, returns: " << calc("101120", "100220", 3, "-") << "\n";
    failed++;
  }
  if (calc("1c9", "1d", 20, "*") == "2dah") {
    passed++;
  } else {
    std::cout << "Test 1c9 * 1d (20) is failed: must return 2dah, returns: " << calc("1c9", "1d", 20, "*") << "\n";
    failed++;
  }
  if (calc("200", "30", 4, "*") == "12000") {
    passed++;
  } else {
    std::cout << "Test 200 * 30 (4) is failed: must return 12000, returns: " << calc("200", "30", 4, "*") << "\n";
    failed++;
  }
  if (calc("21", "0", 19, "/") == "infinity") {
    passed++;
  } else {
    std::cout << "Test 21 / 0 (19) is failed: must return infinity, returns: " << calc("21", "0", 19, "/") << "\n";
    failed++;
  }
  if (calc("126", "pl", 29, "-") == "5e") {
    passed++;
  } else {
    std::cout << "Test 126 - pl (29) is failed: must return 5e, returns: " << calc("126", "pl", 29, "-") << "\n";
    failed++;
  }
  if (calc("190", "187", 14, "-") == "7") {
    passed++;
  } else {
    std::cout << "Test 190 - 187 (14) is failed: must return 7, returns: " << calc("190", "187", 14, "-") << "\n";
    failed++;
  }
  if (calc("235", "171", 8, "*") == "45065") {
    passed++;
  } else {
    std::cout << "Test 235 * 171 (8) is failed: must return 45065, returns: " << calc("235", "171", 8, "*") << "\n";
    failed++;
  }
  if (calc("329", "321", 10, "+") == "650") {
    passed++;
  } else {
    std::cout << "Test 329 + 321 (10) is failed: must return 650, returns: " << calc("329", "321", 10, "+") << "\n";
    failed++;
  }
  if (calc("n", "i", 25, "*") == "ge") {
    passed++;
  } else {
    std::cout << "Test n * i (25) is failed: must return ge, returns: " << calc("n", "i", 25, "*") << "\n";
    failed++;
  }
  if (calc("815", "471", 11, "+") == "1186") {
    passed++;
  } else {
    std::cout << "Test 815 + 471 (11) is failed: must return 1186, returns: " << calc("815", "471", 11, "+") << "\n";
    failed++;
  }
  if (calc("419", "247", 12, "*") == "9a603") {
    passed++;
  } else {
    std::cout << "Test 419 * 247 (12) is failed: must return 9a603, returns: " << calc("419", "247", 12, "*") << "\n";
    failed++;
  }
  if (calc("100", "7", 10, "+") == "107") {
    passed++;
  } else {
    std::cout << "Test 100 + 7 (10) is failed: must return 107, returns: " << calc("100", "7", 10, "+") << "\n";
    failed++;
  }
  if (calc("3b8", "b8", 15, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 3b8 / b8 (15) is failed: must return 4, returns: " << calc("3b8", "b8", 15, "/") << "\n";
    failed++;
  }
  if (calc("33a", "240", 11, "*") == "7a270") {
    passed++;
  } else {
    std::cout << "Test 33a * 240 (11) is failed: must return 7a270, returns: " << calc("33a", "240", 11, "*") << "\n";
    failed++;
  }
  if (calc("46a", "348", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 46a / 348 (13) is failed: must return 1, returns: " << calc("46a", "348", 13, "/") << "\n";
    failed++;
  }
  if (calc("4a", "26", 15, "*") == "b30") {
    passed++;
  } else {
    std::cout << "Test 4a * 26 (15) is failed: must return b30, returns: " << calc("4a", "26", 15, "*") << "\n";
    failed++;
  }
  if (calc("2011", "232", 5, "-") == "1224") {
    passed++;
  } else {
    std::cout << "Test 2011 - 232 (5) is failed: must return 1224, returns: " << calc("2011", "232", 5, "-") << "\n";
    failed++;
  }
  if (calc("1aj", "if", 25, "-") == "h4") {
    passed++;
  } else {
    std::cout << "Test 1aj - if (25) is failed: must return h4, returns: " << calc("1aj", "if", 25, "-") << "\n";
    failed++;
  }
  if (calc("159", "6", 25, "+") == "15f") {
    passed++;
  } else {
    std::cout << "Test 159 + 6 (25) is failed: must return 15f, returns: " << calc("159", "6", 25, "+") << "\n";
    failed++;
  }
  if (calc("95", "6c", 30, "-") == "2n") {
    passed++;
  } else {
    std::cout << "Test 95 - 6c (30) is failed: must return 2n, returns: " << calc("95", "6c", 30, "-") << "\n";
    failed++;
  }
  if (calc("378", "6d", 17, "-") == "30c") {
    passed++;
  } else {
    std::cout << "Test 378 - 6d (17) is failed: must return 30c, returns: " << calc("378", "6d", 17, "-") << "\n";
    failed++;
  }
  if (calc("12e", "114", 22, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 12e / 114 (22) is failed: must return 1, returns: " << calc("12e", "114", 22, "/") << "\n";
    failed++;
  }
  if (calc("cm", "cf", 32, "*") == "4u6a") {
    passed++;
  } else {
    std::cout << "Test cm * cf (32) is failed: must return 4u6a, returns: " << calc("cm", "cf", 32, "*") << "\n";
    failed++;
  }
  if (calc("1c4", "44", 16, "-") == "180") {
    passed++;
  } else {
    std::cout << "Test 1c4 - 44 (16) is failed: must return 180, returns: " << calc("1c4", "44", 16, "-") << "\n";
    failed++;
  }
  if (calc("bm", "2v", 34, "-") == "8p") {
    passed++;
  } else {
    std::cout << "Test bm - 2v (34) is failed: must return 8p, returns: " << calc("bm", "2v", 34, "-") << "\n";
    failed++;
  }
  if (calc("26", "h", 23, "+") == "30") {
    passed++;
  } else {
    std::cout << "Test 26 + h (23) is failed: must return 30, returns: " << calc("26", "h", 23, "+") << "\n";
    failed++;
  }
  if (calc("la", "bp", 29, "*") == "8l5i") {
    passed++;
  } else {
    std::cout << "Test la * bp (29) is failed: must return 8l5i, returns: " << calc("la", "bp", 29, "*") << "\n";
    failed++;
  }
  if (calc("11222", "1202", 5, "-") == "10020") {
    passed++;
  } else {
    std::cout << "Test 11222 - 1202 (5) is failed: must return 10020, returns: " << calc("11222", "1202", 5, "-") << "\n";
    failed++;
  }
  if (calc("1d8", "86", 20, "-") == "152") {
    passed++;
  } else {
    std::cout << "Test 1d8 - 86 (20) is failed: must return 152, returns: " << calc("1d8", "86", 20, "-") << "\n";
    failed++;
  }
  if (calc("140", "51", 11, "-") == "9a") {
    passed++;
  } else {
    std::cout << "Test 140 - 51 (11) is failed: must return 9a, returns: " << calc("140", "51", 11, "-") << "\n";
    failed++;
  }
  if (calc("178", "d7", 24, "*") == "h8b8") {
    passed++;
  } else {
    std::cout << "Test 178 * d7 (24) is failed: must return h8b8, returns: " << calc("178", "d7", 24, "*") << "\n";
    failed++;
  }
  if (calc("748", "637", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 748 / 637 (9) is failed: must return 1, returns: " << calc("748", "637", 9, "/") << "\n";
    failed++;
  }
  if (calc("2c", "m", 32, "+") == "32") {
    passed++;
  } else {
    std::cout << "Test 2c + m (32) is failed: must return 32, returns: " << calc("2c", "m", 32, "+") << "\n";
    failed++;
  }
  if (calc("3g", "37", 18, "-") == "9") {
    passed++;
  } else {
    std::cout << "Test 3g - 37 (18) is failed: must return 9, returns: " << calc("3g", "37", 18, "-") << "\n";
    failed++;
  }
  if (calc("2cc", "d7", 14, "+") == "3c5") {
    passed++;
  } else {
    std::cout << "Test 2cc + d7 (14) is failed: must return 3c5, returns: " << calc("2cc", "d7", 14, "+") << "\n";
    failed++;
  }
  if (calc("64", "3k", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 64 / 3k (24) is failed: must return 1, returns: " << calc("64", "3k", 24, "/") << "\n";
    failed++;
  }
  if (calc("9i", "6q", 33, "-") == "2p") {
    passed++;
  } else {
    std::cout << "Test 9i - 6q (33) is failed: must return 2p, returns: " << calc("9i", "6q", 33, "-") << "\n";
    failed++;
  }
  if (calc("12a", "c4", 17, "-") == "76") {
    passed++;
  } else {
    std::cout << "Test 12a - c4 (17) is failed: must return 76, returns: " << calc("12a", "c4", 17, "-") << "\n";
    failed++;
  }
  if (calc("196", "ii", 21, "*") == "16423") {
    passed++;
  } else {
    std::cout << "Test 196 * ii (21) is failed: must return 16423, returns: " << calc("196", "ii", 21, "*") << "\n";
    failed++;
  }
  if (calc("l9", "68", 31, "+") == "rh") {
    passed++;
  } else {
    std::cout << "Test l9 + 68 (31) is failed: must return rh, returns: " << calc("l9", "68", 31, "+") << "\n";
    failed++;
  }
  if (calc("1345", "12", 7, "/") == "112") {
    passed++;
  } else {
    std::cout << "Test 1345 / 12 (7) is failed: must return 112, returns: " << calc("1345", "12", 7, "/") << "\n";
    failed++;
  }
  if (calc("3g", "6", 26, "-") == "3a") {
    passed++;
  } else {
    std::cout << "Test 3g - 6 (26) is failed: must return 3a, returns: " << calc("3g", "6", 26, "-") << "\n";
    failed++;
  }
  if (calc("136", "j7", 21, "*") == "117b0") {
    passed++;
  } else {
    std::cout << "Test 136 * j7 (21) is failed: must return 117b0, returns: " << calc("136", "j7", 21, "*") << "\n";
    failed++;
  }
  if (calc("ar", "7f", 29, "-") == "3c") {
    passed++;
  } else {
    std::cout << "Test ar - 7f (29) is failed: must return 3c, returns: " << calc("ar", "7f", 29, "-") << "\n";
    failed++;
  }
  if (calc("220", "4f", 20, "/") == "8") {
    passed++;
  } else {
    std::cout << "Test 220 / 4f (20) is failed: must return 8, returns: " << calc("220", "4f", 20, "/") << "\n";
    failed++;
  }
  if (calc("2524", "2051", 6, "-") == "433") {
    passed++;
  } else {
    std::cout << "Test 2524 - 2051 (6) is failed: must return 433, returns: " << calc("2524", "2051", 6, "-") << "\n";
    failed++;
  }
  if (calc("kd", "c6", 23, "-") == "87") {
    passed++;
  } else {
    std::cout << "Test kd - c6 (23) is failed: must return 87, returns: " << calc("kd", "c6", 23, "-") << "\n";
    failed++;
  }
  if (calc("1173", "252", 8, "-") == "721") {
    passed++;
  } else {
    std::cout << "Test 1173 - 252 (8) is failed: must return 721, returns: " << calc("1173", "252", 8, "-") << "\n";
    failed++;
  }
  if (calc("fh", "e4", 19, "-") == "1d") {
    passed++;
  } else {
    std::cout << "Test fh - e4 (19) is failed: must return 1d, returns: " << calc("fh", "e4", 19, "-") << "\n";
    failed++;
  }
  if (calc("ml", "c0", 28, "*") == "9l00") {
    passed++;
  } else {
    std::cout << "Test ml * c0 (28) is failed: must return 9l00, returns: " << calc("ml", "c0", 28, "*") << "\n";
    failed++;
  }
  if (calc("ag", "43", 31, "*") == "1c2h") {
    passed++;
  } else {
    std::cout << "Test ag * 43 (31) is failed: must return 1c2h, returns: " << calc("ag", "43", 31, "*") << "\n";
    failed++;
  }
  if (calc("3511", "1250", 6, "+") == "5201") {
    passed++;
  } else {
    std::cout << "Test 3511 + 1250 (6) is failed: must return 5201, returns: " << calc("3511", "1250", 6, "+") << "\n";
    failed++;
  }
  if (calc("hh", "dh", 28, "-") == "40") {
    passed++;
  } else {
    std::cout << "Test hh - dh (28) is failed: must return 40, returns: " << calc("hh", "dh", 28, "-") << "\n";
    failed++;
  }
  if (calc("17f", "km", 23, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 17f / km (23) is failed: must return 1, returns: " << calc("17f", "km", 23, "/") << "\n";
    failed++;
  }
  if (calc("ia", "bb", 20, "+") == "1a1") {
    passed++;
  } else {
    std::cout << "Test ia + bb (20) is failed: must return 1a1, returns: " << calc("ia", "bb", 20, "+") << "\n";
    failed++;
  }
  if (calc("549", "406", 12, "-") == "143") {
    passed++;
  } else {
    std::cout << "Test 549 - 406 (12) is failed: must return 143, returns: " << calc("549", "406", 12, "-") << "\n";
    failed++;
  }
  if (calc("532", "179", 11, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 532 / 179 (11) is failed: must return 3, returns: " << calc("532", "179", 11, "/") << "\n";
    failed++;
  }
  if (calc("38", "w", 33, "+") == "47") {
    passed++;
  } else {
    std::cout << "Test 38 + w (33) is failed: must return 47, returns: " << calc("38", "w", 33, "+") << "\n";
    failed++;
  }
  if (calc("1j3", "ef", 21, "*") == "172e3") {
    passed++;
  } else {
    std::cout << "Test 1j3 * ef (21) is failed: must return 172e3, returns: " << calc("1j3", "ef", 21, "*") << "\n";
    failed++;
  }
  if (calc("21g", "17e", 20, "+") == "39a") {
    passed++;
  } else {
    std::cout << "Test 21g + 17e (20) is failed: must return 39a, returns: " << calc("21g", "17e", 20, "+") << "\n";
    failed++;
  }
  if (calc("12e", "ca", 16, "*") == "ee4c") {
    passed++;
  } else {
    std::cout << "Test 12e * ca (16) is failed: must return ee4c, returns: " << calc("12e", "ca", 16, "*") << "\n";
    failed++;
  }
  if (calc("ie", "15", 25, "/") == "f") {
    passed++;
  } else {
    std::cout << "Test ie / 15 (25) is failed: must return f, returns: " << calc("ie", "15", 25, "/") << "\n";
    failed++;
  }
  if (calc("qp", "m0", 31, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test qp / m0 (31) is failed: must return 1, returns: " << calc("qp", "m0", 31, "/") << "\n";
    failed++;
  }
  if (calc("642", "81", 10, "+") == "723") {
    passed++;
  } else {
    std::cout << "Test 642 + 81 (10) is failed: must return 723, returns: " << calc("642", "81", 10, "+") << "\n";
    failed++;
  }
  if (calc("631", "359", 11, "*") == "2018a9") {
    passed++;
  } else {
    std::cout << "Test 631 * 359 (11) is failed: must return 2018a9, returns: " << calc("631", "359", 11, "*") << "\n";
    failed++;
  }
  if (calc("8i", "42", 25, "*") == "1aeb") {
    passed++;
  } else {
    std::cout << "Test 8i * 42 (25) is failed: must return 1aeb, returns: " << calc("8i", "42", 25, "*") << "\n";
    failed++;
  }
  if (calc("182", "61", 23, "*") == "83k2") {
    passed++;
  } else {
    std::cout << "Test 182 * 61 (23) is failed: must return 83k2, returns: " << calc("182", "61", 23, "*") << "\n";
    failed++;
  }
  if (calc("2006", "244", 7, "*") == "523163") {
    passed++;
  } else {
    std::cout << "Test 2006 * 244 (7) is failed: must return 523163, returns: " << calc("2006", "244", 7, "*") << "\n";
    failed++;
  }
  if (calc("9", "4", 11, "*") == "33") {
    passed++;
  } else {
    std::cout << "Test 9 * 4 (11) is failed: must return 33, returns: " << calc("9", "4", 11, "*") << "\n";
    failed++;
  }
  if (calc("jn", "47", 24, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test jn / 47 (24) is failed: must return 4, returns: " << calc("jn", "47", 24, "/") << "\n";
    failed++;
  }
  if (calc("me", "d0", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test me / d0 (30) is failed: must return 1, returns: " << calc("me", "d0", 30, "/") << "\n";
    failed++;
  }
  if (calc("25d", "39", 14, "/") == "9") {
    passed++;
  } else {
    std::cout << "Test 25d / 39 (14) is failed: must return 9, returns: " << calc("25d", "39", 14, "/") << "\n";
    failed++;
  }
  if (calc("fs", "70", 35, "*") == "35l0") {
    passed++;
  } else {
    std::cout << "Test fs * 70 (35) is failed: must return 35l0, returns: " << calc("fs", "70", 35, "*") << "\n";
    failed++;
  }
  if (calc("1666", "454", 8, "+") == "2342") {
    passed++;
  } else {
    std::cout << "Test 1666 + 454 (8) is failed: must return 2342, returns: " << calc("1666", "454", 8, "+") << "\n";
    failed++;
  }
  if (calc("104", "3j", 31, "/") == "8") {
    passed++;
  } else {
    std::cout << "Test 104 / 3j (31) is failed: must return 8, returns: " << calc("104", "3j", 31, "/") << "\n";
    failed++;
  }
  if (calc("qf", "ol", 34, "+") == "1h2") {
    passed++;
  } else {
    std::cout << "Test qf + ol (34) is failed: must return 1h2, returns: " << calc("qf", "ol", 34, "+") << "\n";
    failed++;
  }
  if (calc("hb", "8g", 23, "*") == "6dmf") {
    passed++;
  } else {
    std::cout << "Test hb * 8g (23) is failed: must return 6dmf, returns: " << calc("hb", "8g", 23, "*") << "\n";
    failed++;
  }
  if (calc("17k", "o8", 26, "-") == "9c") {
    passed++;
  } else {
    std::cout << "Test 17k - o8 (26) is failed: must return 9c, returns: " << calc("17k", "o8", 26, "-") << "\n";
    failed++;
  }
  if (calc("21000", "13330", 4, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 21000 / 13330 (4) is failed: must return 1, returns: " << calc("21000", "13330", 4, "/") << "\n";
    failed++;
  }
  if (calc("15m", "lb", 26, "+") == "217") {
    passed++;
  } else {
    std::cout << "Test 15m + lb (26) is failed: must return 217, returns: " << calc("15m", "lb", 26, "+") << "\n";
    failed++;
  }
  if (calc("580", "447", 9, "+") == "1137") {
    passed++;
  } else {
    std::cout << "Test 580 + 447 (9) is failed: must return 1137, returns: " << calc("580", "447", 9, "+") << "\n";
    failed++;
  }
  if (calc("3m", "3k", 23, "*") == "f73") {
    passed++;
  } else {
    std::cout << "Test 3m * 3k (23) is failed: must return f73, returns: " << calc("3m", "3k", 23, "*") << "\n";
    failed++;
  }
  if (calc("1001101011", "1000100001", 2, "-") == "1001010") {
    passed++;
  } else {
    std::cout << "Test 1001101011 - 1000100001 (2) is failed: must return 1001010, returns: " << calc("1001101011", "1000100001", 2, "-") << "\n";
    failed++;
  }
  if (calc("2ci", "1c1", 19, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 2ci / 1c1 (19) is failed: must return 1, returns: " << calc("2ci", "1c1", 19, "/") << "\n";
    failed++;
  }
  if (calc("276", "23", 8, "*") == "7032") {
    passed++;
  } else {
    std::cout << "Test 276 * 23 (8) is failed: must return 7032, returns: " << calc("276", "23", 8, "*") << "\n";
    failed++;
  }
  if (calc("h0", "6j", 32, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test h0 / 6j (32) is failed: must return 2, returns: " << calc("h0", "6j", 32, "/") << "\n";
    failed++;
  }
  if (calc("3a3", "79", 12, "*") == "25a53") {
    passed++;
  } else {
    std::cout << "Test 3a3 * 79 (12) is failed: must return 25a53, returns: " << calc("3a3", "79", 12, "*") << "\n";
    failed++;
  }
  if (calc("19e", "129", 21, "+") == "2c2") {
    passed++;
  } else {
    std::cout << "Test 19e + 129 (21) is failed: must return 2c2, returns: " << calc("19e", "129", 21, "+") << "\n";
    failed++;
  }
  if (calc("6p", "6d", 35, "+") == "d3") {
    passed++;
  } else {
    std::cout << "Test 6p + 6d (35) is failed: must return d3, returns: " << calc("6p", "6d", 35, "+") << "\n";
    failed++;
  }
  if (calc("353", "25a", 11, "-") == "a4") {
    passed++;
  } else {
    std::cout << "Test 353 - 25a (11) is failed: must return a4, returns: " << calc("353", "25a", 11, "-") << "\n";
    failed++;
  }
  if (calc("ek", "44", 32, "-") == "ag") {
    passed++;
  } else {
    std::cout << "Test ek - 44 (32) is failed: must return ag, returns: " << calc("ek", "44", 32, "-") << "\n";
    failed++;
  }
  if (calc("228", "225", 14, "+") == "44d") {
    passed++;
  } else {
    std::cout << "Test 228 + 225 (14) is failed: must return 44d, returns: " << calc("228", "225", 14, "+") << "\n";
    failed++;
  }
  if (calc("1hg", "14", 19, "*") == "26b7") {
    passed++;
  } else {
    std::cout << "Test 1hg * 14 (19) is failed: must return 26b7, returns: " << calc("1hg", "14", 19, "*") << "\n";
    failed++;
  }
  if (calc("o8", "m1", 28, "+") == "1i9") {
    passed++;
  } else {
    std::cout << "Test o8 + m1 (28) is failed: must return 1i9, returns: " << calc("o8", "m1", 28, "+") << "\n";
    failed++;
  }
  if (calc("l4", "gp", 34, "-") == "4d") {
    passed++;
  } else {
    std::cout << "Test l4 - gp (34) is failed: must return 4d, returns: " << calc("l4", "gp", 34, "-") << "\n";
    failed++;
  }
  if (calc("qh", "ec", 30, "+") == "1at") {
    passed++;
  } else {
    std::cout << "Test qh + ec (30) is failed: must return 1at, returns: " << calc("qh", "ec", 30, "+") << "\n";
    failed++;
  }
  if (calc("k4", "7f", 25, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test k4 / 7f (25) is failed: must return 2, returns: " << calc("k4", "7f", 25, "/") << "\n";
    failed++;
  }
  if (calc("2e", "6", 15, "-") == "28") {
    passed++;
  } else {
    std::cout << "Test 2e - 6 (15) is failed: must return 28, returns: " << calc("2e", "6", 15, "-") << "\n";
    failed++;
  }
  if (calc("387", "2b5", 15, "+") == "64c") {
    passed++;
  } else {
    std::cout << "Test 387 + 2b5 (15) is failed: must return 64c, returns: " << calc("387", "2b5", 15, "+") << "\n";
    failed++;
  }
  if (calc("1306", "535", 7, "-") == "441") {
    passed++;
  } else {
    std::cout << "Test 1306 - 535 (7) is failed: must return 441, returns: " << calc("1306", "535", 7, "-") << "\n";
    failed++;
  }
  if (calc("125", "b4", 22, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 125 / b4 (22) is failed: must return 2, returns: " << calc("125", "b4", 22, "/") << "\n";
    failed++;
  }
  if (calc("gj", "ed", 20, "*") == "c867") {
    passed++;
  } else {
    std::cout << "Test gj * ed (20) is failed: must return c867, returns: " << calc("gj", "ed", 20, "*") << "\n";
    failed++;
  }
  if (calc("p9", "o0", 31, "-") == "19") {
    passed++;
  } else {
    std::cout << "Test p9 - o0 (31) is failed: must return 19, returns: " << calc("p9", "o0", 31, "-") << "\n";
    failed++;
  }
  if (calc("qe", "i2", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test qe / i2 (35) is failed: must return 1, returns: " << calc("qe", "i2", 35, "/") << "\n";
    failed++;
  }
  if (calc("289", "22a", 17, "+") == "4b2") {
    passed++;
  } else {
    std::cout << "Test 289 + 22a (17) is failed: must return 4b2, returns: " << calc("289", "22a", 17, "+") << "\n";
    failed++;
  }
  if (calc("10c", "2f", 17, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 10c / 2f (17) is failed: must return 6, returns: " << calc("10c", "2f", 17, "/") << "\n";
    failed++;
  }
  if (calc("213", "112", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 213 / 112 (7) is failed: must return 1, returns: " << calc("213", "112", 7, "/") << "\n";
    failed++;
  }
  if (calc("ua", "fc", 31, "*") == "f1hr") {
    passed++;
  } else {
    std::cout << "Test ua * fc (31) is failed: must return f1hr, returns: " << calc("ua", "fc", 31, "*") << "\n";
    failed++;
  }
  if (calc("3435", "30", 6, "+") == "3505") {
    passed++;
  } else {
    std::cout << "Test 3435 + 30 (6) is failed: must return 3505, returns: " << calc("3435", "30", 6, "+") << "\n";
    failed++;
  }
  if (calc("gg", "g4", 34, "-") == "c") {
    passed++;
  } else {
    std::cout << "Test gg - g4 (34) is failed: must return c, returns: " << calc("gg", "g4", 34, "-") << "\n";
    failed++;
  }
  if (calc("139", "116", 11, "+") == "254") {
    passed++;
  } else {
    std::cout << "Test 139 + 116 (11) is failed: must return 254, returns: " << calc("139", "116", 11, "+") << "\n";
    failed++;
  }
  if (calc("209", "1b3", 15, "*") == "386ac") {
    passed++;
  } else {
    std::cout << "Test 209 * 1b3 (15) is failed: must return 386ac, returns: " << calc("209", "1b3", 15, "*") << "\n";
    failed++;
  }
  if (calc("735", "392", 11, "+") == "1017") {
    passed++;
  } else {
    std::cout << "Test 735 + 392 (11) is failed: must return 1017, returns: " << calc("735", "392", 11, "+") << "\n";
    failed++;
  }
  if (calc("j6", "6", 24, "*") == "4jc") {
    passed++;
  } else {
    std::cout << "Test j6 * 6 (24) is failed: must return 4jc, returns: " << calc("j6", "6", 24, "*") << "\n";
    failed++;
  }
  if (calc("89", "56", 20, "-") == "33") {
    passed++;
  } else {
    std::cout << "Test 89 - 56 (20) is failed: must return 33, returns: " << calc("89", "56", 20, "-") << "\n";
    failed++;
  }
  if (calc("a9", "64", 16, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test a9 / 64 (16) is failed: must return 1, returns: " << calc("a9", "64", 16, "/") << "\n";
    failed++;
  }
  if (calc("87", "4m", 28, "-") == "3d") {
    passed++;
  } else {
    std::cout << "Test 87 - 4m (28) is failed: must return 3d, returns: " << calc("87", "4m", 28, "-") << "\n";
    failed++;
  }
  if (calc("101111011", "1010110", 2, "/") == "100") {
    passed++;
  } else {
    std::cout << "Test 101111011 / 1010110 (2) is failed: must return 100, returns: " << calc("101111011", "1010110", 2, "/") << "\n";
    failed++;
  }
  if (calc("101100", "1111", 2, "-") == "11101") {
    passed++;
  } else {
    std::cout << "Test 101100 - 1111 (2) is failed: must return 11101, returns: " << calc("101100", "1111", 2, "-") << "\n";
    failed++;
  }
  if (calc("38c", "67", 14, "+") == "415") {
    passed++;
  } else {
    std::cout << "Test 38c + 67 (14) is failed: must return 415, returns: " << calc("38c", "67", 14, "+") << "\n";
    failed++;
  }
  if (calc("7a", "41", 15, "*") == "212a") {
    passed++;
  } else {
    std::cout << "Test 7a * 41 (15) is failed: must return 212a, returns: " << calc("7a", "41", 15, "*") << "\n";
    failed++;
  }
  if (calc("12f", "c5", 27, "+") == "1ek") {
    passed++;
  } else {
    std::cout << "Test 12f + c5 (27) is failed: must return 1ek, returns: " << calc("12f", "c5", 27, "+") << "\n";
    failed++;
  }
  if (calc("128", "63", 18, "+") == "18b") {
    passed++;
  } else {
    std::cout << "Test 128 + 63 (18) is failed: must return 18b, returns: " << calc("128", "63", 18, "+") << "\n";
    failed++;
  }
  if (calc("da", "2d", 23, "*") == "1baf") {
    passed++;
  } else {
    std::cout << "Test da * 2d (23) is failed: must return 1baf, returns: " << calc("da", "2d", 23, "*") << "\n";
    failed++;
  }
  if (calc("93", "21", 18, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 93 / 21 (18) is failed: must return 4, returns: " << calc("93", "21", 18, "/") << "\n";
    failed++;
  }
  if (calc("gd", "4v", 35, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test gd / 4v (35) is failed: must return 3, returns: " << calc("gd", "4v", 35, "/") << "\n";
    failed++;
  }
  if (calc("42b", "38", 12, "-") == "3b3") {
    passed++;
  } else {
    std::cout << "Test 42b - 38 (12) is failed: must return 3b3, returns: " << calc("42b", "38", 12, "-") << "\n";
    failed++;
  }
  if (calc("1ga", "1dk", 22, "*") == "2igj2") {
    passed++;
  } else {
    std::cout << "Test 1ga * 1dk (22) is failed: must return 2igj2, returns: " << calc("1ga", "1dk", 22, "*") << "\n";
    failed++;
  }
  if (calc("1100010000", "1011001011", 2, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1100010000 / 1011001011 (2) is failed: must return 1, returns: " << calc("1100010000", "1011001011", 2, "/") << "\n";
    failed++;
  }
  if (calc("78", "52", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 78 / 52 (20) is failed: must return 1, returns: " << calc("78", "52", 20, "/") << "\n";
    failed++;
  }
  if (calc("k4", "89", 29, "*") == "5ma7") {
    passed++;
  } else {
    std::cout << "Test k4 * 89 (29) is failed: must return 5ma7, returns: " << calc("k4", "89", 29, "*") << "\n";
    failed++;
  }
  if (calc("5h", "5g", 30, "-") == "1") {
    passed++;
  } else {
    std::cout << "Test 5h - 5g (30) is failed: must return 1, returns: " << calc("5h", "5g", 30, "-") << "\n";
    failed++;
  }
  if (calc("697", "449", 10, "*") == "312953") {
    passed++;
  } else {
    std::cout << "Test 697 * 449 (10) is failed: must return 312953, returns: " << calc("697", "449", 10, "*") << "\n";
    failed++;
  }
  if (calc("a6", "68", 18, "+") == "ge") {
    passed++;
  } else {
    std::cout << "Test a6 + 68 (18) is failed: must return ge, returns: " << calc("a6", "68", 18, "+") << "\n";
    failed++;
  }
  if (calc("175", "j0", 26, "+") == "205") {
    passed++;
  } else {
    std::cout << "Test 175 + j0 (26) is failed: must return 205, returns: " << calc("175", "j0", 26, "+") << "\n";
    failed++;
  }
  if (calc("1bb", "1a4", 25, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1bb / 1a4 (25) is failed: must return 1, returns: " << calc("1bb", "1a4", 25, "/") << "\n";
    failed++;
  }
  if (calc("tt", "1f", 31, "/") == "k") {
    passed++;
  } else {
    std::cout << "Test tt / 1f (31) is failed: must return k, returns: " << calc("tt", "1f", 31, "/") << "\n";
    failed++;
  }
  if (calc("m6", "bb", 23, "+") == "1ah") {
    passed++;
  } else {
    std::cout << "Test m6 + bb (23) is failed: must return 1ah, returns: " << calc("m6", "bb", 23, "+") << "\n";
    failed++;
  }
  if (calc("24b", "150", 20, "+") == "39b") {
    passed++;
  } else {
    std::cout << "Test 24b + 150 (20) is failed: must return 39b, returns: " << calc("24b", "150", 20, "+") << "\n";
    failed++;
  }
  if (calc("1000220", "211000", 3, "*") == "211201120000") {
    passed++;
  } else {
    std::cout << "Test 1000220 * 211000 (3) is failed: must return 211201120000, returns: " << calc("1000220", "211000", 3, "*") << "\n";
    failed++;
  }
  if (calc("561", "404", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 561 / 404 (9) is failed: must return 1, returns: " << calc("561", "404", 9, "/") << "\n";
    failed++;
  }
  if (calc("10", "6", 9, "*") == "60") {
    passed++;
  } else {
    std::cout << "Test 10 * 6 (9) is failed: must return 60, returns: " << calc("10", "6", 9, "*") << "\n";
    failed++;
  }
  if (calc("165", "64", 20, "-") == "101") {
    passed++;
  } else {
    std::cout << "Test 165 - 64 (20) is failed: must return 101, returns: " << calc("165", "64", 20, "-") << "\n";
    failed++;
  }
  if (calc("17h", "a5", 26, "+") == "1hm") {
    passed++;
  } else {
    std::cout << "Test 17h + a5 (26) is failed: must return 1hm, returns: " << calc("17h", "a5", 26, "+") << "\n";
    failed++;
  }
  if (calc("17f", "142", 26, "*") == "1cln4") {
    passed++;
  } else {
    std::cout << "Test 17f * 142 (26) is failed: must return 1cln4, returns: " << calc("17f", "142", 26, "*") << "\n";
    failed++;
  }
  if (calc("4f", "3e", 26, "*") == "g52") {
    passed++;
  } else {
    std::cout << "Test 4f * 3e (26) is failed: must return g52, returns: " << calc("4f", "3e", 26, "*") << "\n";
    failed++;
  }
  if (calc("533", "349", 13, "*") == "148421") {
    passed++;
  } else {
    std::cout << "Test 533 * 349 (13) is failed: must return 148421, returns: " << calc("533", "349", 13, "*") << "\n";
    failed++;
  }
  if (calc("1010110100", "1000010111", 2, "-") == "10011101") {
    passed++;
  } else {
    std::cout << "Test 1010110100 - 1000010111 (2) is failed: must return 10011101, returns: " << calc("1010110100", "1000010111", 2, "-") << "\n";
    failed++;
  }
  if (calc("g4", "6c", 22, "+") == "10g") {
    passed++;
  } else {
    std::cout << "Test g4 + 6c (22) is failed: must return 10g, returns: " << calc("g4", "6c", 22, "+") << "\n";
    failed++;
  }
  if (calc("264", "1c5", 16, "-") == "9f") {
    passed++;
  } else {
    std::cout << "Test 264 - 1c5 (16) is failed: must return 9f, returns: " << calc("264", "1c5", 16, "-") << "\n";
    failed++;
  }
  if (calc("ni", "2l", 24, "-") == "kl") {
    passed++;
  } else {
    std::cout << "Test ni - 2l (24) is failed: must return kl, returns: " << calc("ni", "2l", 24, "-") << "\n";
    failed++;
  }
  if (calc("33a", "1cb", 13, "+") == "538") {
    passed++;
  } else {
    std::cout << "Test 33a + 1cb (13) is failed: must return 538, returns: " << calc("33a", "1cb", 13, "+") << "\n";
    failed++;
  }
  if (calc("10232", "4021", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 10232 / 4021 (5) is failed: must return 1, returns: " << calc("10232", "4021", 5, "/") << "\n";
    failed++;
  }
  if (calc("12103", "212", 4, "-") == "11231") {
    passed++;
  } else {
    std::cout << "Test 12103 - 212 (4) is failed: must return 11231, returns: " << calc("12103", "212", 4, "-") << "\n";
    failed++;
  }
  if (calc("1ca", "15", 22, "-") == "1b5") {
    passed++;
  } else {
    std::cout << "Test 1ca - 15 (22) is failed: must return 1b5, returns: " << calc("1ca", "15", 22, "-") << "\n";
    failed++;
  }
  if (calc("112", "33", 7, "*") == "4026") {
    passed++;
  } else {
    std::cout << "Test 112 * 33 (7) is failed: must return 4026, returns: " << calc("112", "33", 7, "*") << "\n";
    failed++;
  }
  if (calc("107", "7e", 20, "*") == "7gdi") {
    passed++;
  } else {
    std::cout << "Test 107 * 7e (20) is failed: must return 7gdi, returns: " << calc("107", "7e", 20, "*") << "\n";
    failed++;
  }
  if (calc("110", "12", 4, "*") == "1320") {
    passed++;
  } else {
    std::cout << "Test 110 * 12 (4) is failed: must return 1320, returns: " << calc("110", "12", 4, "*") << "\n";
    failed++;
  }
  if (calc("1eg", "19e", 22, "-") == "52") {
    passed++;
  } else {
    std::cout << "Test 1eg - 19e (22) is failed: must return 52, returns: " << calc("1eg", "19e", 22, "-") << "\n";
    failed++;
  }
  if (calc("g1", "eg", 26, "+") == "14h") {
    passed++;
  } else {
    std::cout << "Test g1 + eg (26) is failed: must return 14h, returns: " << calc("g1", "eg", 26, "+") << "\n";
    failed++;
  }
  if (calc("d6", "72", 33, "-") == "64") {
    passed++;
  } else {
    std::cout << "Test d6 - 72 (33) is failed: must return 64, returns: " << calc("d6", "72", 33, "-") << "\n";
    failed++;
  }
  if (calc("3h", "23", 21, "-") == "1e") {
    passed++;
  } else {
    std::cout << "Test 3h - 23 (21) is failed: must return 1e, returns: " << calc("3h", "23", 21, "-") << "\n";
    failed++;
  }
  if (calc("pa", "ca", 31, "*") == "a217") {
    passed++;
  } else {
    std::cout << "Test pa * ca (31) is failed: must return a217, returns: " << calc("pa", "ca", 31, "*") << "\n";
    failed++;
  }
  if (calc("c8", "30", 27, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test c8 / 30 (27) is failed: must return 4, returns: " << calc("c8", "30", 27, "/") << "\n";
    failed++;
  }
  if (calc("282", "1a2", 17, "-") == "f0") {
    passed++;
  } else {
    std::cout << "Test 282 - 1a2 (17) is failed: must return f0, returns: " << calc("282", "1a2", 17, "-") << "\n";
    failed++;
  }
  if (calc("40", "3m", 32, "-") == "a") {
    passed++;
  } else {
    std::cout << "Test 40 - 3m (32) is failed: must return a, returns: " << calc("40", "3m", 32, "-") << "\n";
    failed++;
  }
  if (calc("55", "3g", 34, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 55 / 3g (34) is failed: must return 1, returns: " << calc("55", "3g", 34, "/") << "\n";
    failed++;
  }
  if (calc("186", "93", 26, "*") == "c00i") {
    passed++;
  } else {
    std::cout << "Test 186 * 93 (26) is failed: must return c00i, returns: " << calc("186", "93", 26, "*") << "\n";
    failed++;
  }
  if (calc("ca", "a0", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ca / a0 (14) is failed: must return 1, returns: " << calc("ca", "a0", 14, "/") << "\n";
    failed++;
  }
  if (calc("666", "663", 7, "+") == "1662") {
    passed++;
  } else {
    std::cout << "Test 666 + 663 (7) is failed: must return 1662, returns: " << calc("666", "663", 7, "+") << "\n";
    failed++;
  }
  if (calc("13h", "l3", 22, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 13h / l3 (22) is failed: must return 1, returns: " << calc("13h", "l3", 22, "/") << "\n";
    failed++;
  }
  if (calc("27f", "1d4", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 27f / 1d4 (20) is failed: must return 1, returns: " << calc("27f", "1d4", 20, "/") << "\n";
    failed++;
  }
  if (calc("31", "h", 23, "+") == "3i") {
    passed++;
  } else {
    std::cout << "Test 31 + h (23) is failed: must return 3i, returns: " << calc("31", "h", 23, "+") << "\n";
    failed++;
  }
  if (calc("ta", "aj", 32, "-") == "in") {
    passed++;
  } else {
    std::cout << "Test ta - aj (32) is failed: must return in, returns: " << calc("ta", "aj", 32, "-") << "\n";
    failed++;
  }
  if (calc("ka", "9c", 24, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test ka / 9c (24) is failed: must return 2, returns: " << calc("ka", "9c", 24, "/") << "\n";
    failed++;
  }
  if (calc("m3", "1w", 33, "*") == "1agu") {
    passed++;
  } else {
    std::cout << "Test m3 * 1w (33) is failed: must return 1agu, returns: " << calc("m3", "1w", 33, "*") << "\n";
    failed++;
  }
  if (calc("11441", "4421", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 11441 / 4421 (5) is failed: must return 1, returns: " << calc("11441", "4421", 5, "/") << "\n";
    failed++;
  }
  if (calc("24h", "g7", 18, "+") == "336") {
    passed++;
  } else {
    std::cout << "Test 24h + g7 (18) is failed: must return 336, returns: " << calc("24h", "g7", 18, "+") << "\n";
    failed++;
  }
  if (calc("147", "13", 17, "/") == "11") {
    passed++;
  } else {
    std::cout << "Test 147 / 13 (17) is failed: must return 11, returns: " << calc("147", "13", 17, "/") << "\n";
    failed++;
  }
  if (calc("93", "8h", 21, "*") == "3hb9") {
    passed++;
  } else {
    std::cout << "Test 93 * 8h (21) is failed: must return 3hb9, returns: " << calc("93", "8h", 21, "*") << "\n";
    failed++;
  }
  if (calc("1111001111", "100010110", 2, "/") == "11") {
    passed++;
  } else {
    std::cout << "Test 1111001111 / 100010110 (2) is failed: must return 11, returns: " << calc("1111001111", "100010110", 2, "/") << "\n";
    failed++;
  }
  if (calc("65", "40", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 65 / 40 (9) is failed: must return 1, returns: " << calc("65", "40", 9, "/") << "\n";
    failed++;
  }
  if (calc("1c5", "9p", 26, "+") == "1m4") {
    passed++;
  } else {
    std::cout << "Test 1c5 + 9p (26) is failed: must return 1m4, returns: " << calc("1c5", "9p", 26, "+") << "\n";
    failed++;
  }
  if (calc("2b6", "1fh", 19, "*") == "4eag7") {
    passed++;
  } else {
    std::cout << "Test 2b6 * 1fh (19) is failed: must return 4eag7, returns: " << calc("2b6", "1fh", 19, "*") << "\n";
    failed++;
  }
  if (calc("r2", "ho", 35, "*") == "dnid") {
    passed++;
  } else {
    std::cout << "Test r2 * ho (35) is failed: must return dnid, returns: " << calc("r2", "ho", 35, "*") << "\n";
    failed++;
  }
  if (calc("4401", "222", 5, "+") == "10123") {
    passed++;
  } else {
    std::cout << "Test 4401 + 222 (5) is failed: must return 10123, returns: " << calc("4401", "222", 5, "+") << "\n";
    failed++;
  }
  if (calc("e1", "51", 24, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test e1 / 51 (24) is failed: must return 2, returns: " << calc("e1", "51", 24, "/") << "\n";
    failed++;
  }
  if (calc("253", "77", 11, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 253 / 77 (11) is failed: must return 3, returns: " << calc("253", "77", 11, "/") << "\n";
    failed++;
  }
  if (calc("nm", "lm", 27, "+") == "1ih") {
    passed++;
  } else {
    std::cout << "Test nm + lm (27) is failed: must return 1ih, returns: " << calc("nm", "lm", 27, "+") << "\n";
    failed++;
  }
  if (calc("11", "10", 11, "-") == "1") {
    passed++;
  } else {
    std::cout << "Test 11 - 10 (11) is failed: must return 1, returns: " << calc("11", "10", 11, "-") << "\n";
    failed++;
  }
  if (calc("72", "17", 12, "+") == "89") {
    passed++;
  } else {
    std::cout << "Test 72 + 17 (12) is failed: must return 89, returns: " << calc("72", "17", 12, "+") << "\n";
    failed++;
  }
  if (calc("4b9", "d4", 14, "+") == "5ad") {
    passed++;
  } else {
    std::cout << "Test 4b9 + d4 (14) is failed: must return 5ad, returns: " << calc("4b9", "d4", 14, "+") << "\n";
    failed++;
  }
  if (calc("201211", "122022", 3, "-") == "2112") {
    passed++;
  } else {
    std::cout << "Test 201211 - 122022 (3) is failed: must return 2112, returns: " << calc("201211", "122022", 3, "-") << "\n";
    failed++;
  }
  if (calc("e7", "b5", 18, "-") == "32") {
    passed++;
  } else {
    std::cout << "Test e7 - b5 (18) is failed: must return 32, returns: " << calc("e7", "b5", 18, "-") << "\n";
    failed++;
  }
  if (calc("414", "13b", 12, "*") == "55528") {
    passed++;
  } else {
    std::cout << "Test 414 * 13b (12) is failed: must return 55528, returns: " << calc("414", "13b", 12, "*") << "\n";
    failed++;
  }
  if (calc("eq", "3a", 29, "+") == "i7") {
    passed++;
  } else {
    std::cout << "Test eq + 3a (29) is failed: must return i7, returns: " << calc("eq", "3a", 29, "+") << "\n";
    failed++;
  }
  if (calc("71", "67", 20, "-") == "e") {
    passed++;
  } else {
    std::cout << "Test 71 - 67 (20) is failed: must return e, returns: " << calc("71", "67", 20, "-") << "\n";
    failed++;
  }
  if (calc("5b", "3f", 29, "+") == "8q") {
    passed++;
  } else {
    std::cout << "Test 5b + 3f (29) is failed: must return 8q, returns: " << calc("5b", "3f", 29, "+") << "\n";
    failed++;
  }
  if (calc("1ic", "1aa", 21, "+") == "381") {
    passed++;
  } else {
    std::cout << "Test 1ic + 1aa (21) is failed: must return 381, returns: " << calc("1ic", "1aa", 21, "+") << "\n";
    failed++;
  }
  if (calc("a7", "6p", 33, "-") == "3f") {
    passed++;
  } else {
    std::cout << "Test a7 - 6p (33) is failed: must return 3f, returns: " << calc("a7", "6p", 33, "-") << "\n";
    failed++;
  }
  if (calc("138", "a1", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 138 / a1 (13) is failed: must return 1, returns: " << calc("138", "a1", 13, "/") << "\n";
    failed++;
  }
  if (calc("ce", "a9", 20, "+") == "133") {
    passed++;
  } else {
    std::cout << "Test ce + a9 (20) is failed: must return 133, returns: " << calc("ce", "a9", 20, "+") << "\n";
    failed++;
  }
  if (calc("jt", "ir", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test jt / ir (35) is failed: must return 1, returns: " << calc("jt", "ir", 35, "/") << "\n";
    failed++;
  }
  if (calc("1h7", "131", 20, "-") == "e6") {
    passed++;
  } else {
    std::cout << "Test 1h7 - 131 (20) is failed: must return e6, returns: " << calc("1h7", "131", 20, "-") << "\n";
    failed++;
  }
  if (calc("2a1", "4a", 13, "-") == "254") {
    passed++;
  } else {
    std::cout << "Test 2a1 - 4a (13) is failed: must return 254, returns: " << calc("2a1", "4a", 13, "-") << "\n";
    failed++;
  }
  if (calc("18i", "db", 20, "+") == "229") {
    passed++;
  } else {
    std::cout << "Test 18i + db (20) is failed: must return 229, returns: " << calc("18i", "db", 20, "+") << "\n";
    failed++;
  }
  if (calc("e2", "6j", 21, "*") == "4d6h") {
    passed++;
  } else {
    std::cout << "Test e2 * 6j (21) is failed: must return 4d6h, returns: " << calc("e2", "6j", 21, "*") << "\n";
    failed++;
  }
  if (calc("8a", "1g", 23, "+") == "a3") {
    passed++;
  } else {
    std::cout << "Test 8a + 1g (23) is failed: must return a3, returns: " << calc("8a", "1g", 23, "+") << "\n";
    failed++;
  }
  if (calc("131", "88", 10, "-") == "43") {
    passed++;
  } else {
    std::cout << "Test 131 - 88 (10) is failed: must return 43, returns: " << calc("131", "88", 10, "-") << "\n";
    failed++;
  }
  if (calc("124", "120", 10, "-") == "4") {
    passed++;
  } else {
    std::cout << "Test 124 - 120 (10) is failed: must return 4, returns: " << calc("124", "120", 10, "-") << "\n";
    failed++;
  }
  if (calc("1032", "31", 5, "+") == "1113") {
    passed++;
  } else {
    std::cout << "Test 1032 + 31 (5) is failed: must return 1113, returns: " << calc("1032", "31", 5, "+") << "\n";
    failed++;
  }
  if (calc("6a8", "200", 11, "-") == "4a8") {
    passed++;
  } else {
    std::cout << "Test 6a8 - 200 (11) is failed: must return 4a8, returns: " << calc("6a8", "200", 11, "-") << "\n";
    failed++;
  }
  if (calc("mj", "n", 28, "/") == "r") {
    passed++;
  } else {
    std::cout << "Test mj / n (28) is failed: must return r, returns: " << calc("mj", "n", 28, "/") << "\n";
    failed++;
  }
  if (calc("111021", "102212", 3, "-") == "1102") {
    passed++;
  } else {
    std::cout << "Test 111021 - 102212 (3) is failed: must return 1102, returns: " << calc("111021", "102212", 3, "-") << "\n";
    failed++;
  }
  if (calc("2f6", "30", 16, "*") == "8e20") {
    passed++;
  } else {
    std::cout << "Test 2f6 * 30 (16) is failed: must return 8e20, returns: " << calc("2f6", "30", 16, "*") << "\n";
    failed++;
  }
  if (calc("28c", "53", 13, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 28c / 53 (13) is failed: must return 6, returns: " << calc("28c", "53", 13, "/") << "\n";
    failed++;
  }
  if (calc("ja", "b9", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ja / b9 (24) is failed: must return 1, returns: " << calc("ja", "b9", 24, "/") << "\n";
    failed++;
  }
  if (calc("254", "d1", 17, "-") == "193") {
    passed++;
  } else {
    std::cout << "Test 254 - d1 (17) is failed: must return 193, returns: " << calc("254", "d1", 17, "-") << "\n";
    failed++;
  }
  if (calc("1hk", "56", 22, "+") == "214") {
    passed++;
  } else {
    std::cout << "Test 1hk + 56 (22) is failed: must return 214, returns: " << calc("1hk", "56", 22, "+") << "\n";
    failed++;
  }
  if (calc("l6", "a0", 23, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test l6 / a0 (23) is failed: must return 2, returns: " << calc("l6", "a0", 23, "/") << "\n";
    failed++;
  }
  if (calc("f9", "d9", 18, "+") == "1b0") {
    passed++;
  } else {
    std::cout << "Test f9 + d9 (18) is failed: must return 1b0, returns: " << calc("f9", "d9", 18, "+") << "\n";
    failed++;
  }
  if (calc("hh", "1g", 25, "+") == "j8") {
    passed++;
  } else {
    std::cout << "Test hh + 1g (25) is failed: must return j8, returns: " << calc("hh", "1g", 25, "+") << "\n";
    failed++;
  }
  if (calc("3h", "1a", 21, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 3h / 1a (21) is failed: must return 2, returns: " << calc("3h", "1a", 21, "/") << "\n";
    failed++;
  }
  if (calc("e1", "3w", 34, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test e1 / 3w (34) is failed: must return 3, returns: " << calc("e1", "3w", 34, "/") << "\n";
    failed++;
  }
  if (calc("la", "a9", 25, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test la / a9 (25) is failed: must return 2, returns: " << calc("la", "a9", 25, "/") << "\n";
    failed++;
  }
  if (calc("cd", "bi", 33, "+") == "nv") {
    passed++;
  } else {
    std::cout << "Test cd + bi (33) is failed: must return nv, returns: " << calc("cd", "bi", 33, "+") << "\n";
    failed++;
  }
  if (calc("294", "281", 17, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 294 / 281 (17) is failed: must return 1, returns: " << calc("294", "281", 17, "/") << "\n";
    failed++;
  }
  if (calc("27a", "162", 11, "*") == "42799") {
    passed++;
  } else {
    std::cout << "Test 27a * 162 (11) is failed: must return 42799, returns: " << calc("27a", "162", 11, "*") << "\n";
    failed++;
  }
  if (calc("d3", "74", 16, "-") == "5f") {
    passed++;
  } else {
    std::cout << "Test d3 - 74 (16) is failed: must return 5f, returns: " << calc("d3", "74", 16, "-") << "\n";
    failed++;
  }
  if (calc("ok", "5i", 28, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test ok / 5i (28) is failed: must return 4, returns: " << calc("ok", "5i", 28, "/") << "\n";
    failed++;
  }
  if (calc("13i", "5i", 24, "*") == "6fdc") {
    passed++;
  } else {
    std::cout << "Test 13i * 5i (24) is failed: must return 6fdc, returns: " << calc("13i", "5i", 24, "*") << "\n";
    failed++;
  }
  if (calc("103", "1d", 23, "*") == "1d4g") {
    passed++;
  } else {
    std::cout << "Test 103 * 1d (23) is failed: must return 1d4g, returns: " << calc("103", "1d", 23, "*") << "\n";
    failed++;
  }
  if (calc("4333", "1104", 6, "+") == "5441") {
    passed++;
  } else {
    std::cout << "Test 4333 + 1104 (6) is failed: must return 5441, returns: " << calc("4333", "1104", 6, "+") << "\n";
    failed++;
  }
  if (calc("g0", "dh", 27, "*") == "8220") {
    passed++;
  } else {
    std::cout << "Test g0 * dh (27) is failed: must return 8220, returns: " << calc("g0", "dh", 27, "*") << "\n";
    failed++;
  }
  if (calc("l3", "fk", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test l3 / fk (24) is failed: must return 1, returns: " << calc("l3", "fk", 24, "/") << "\n";
    failed++;
  }
  if (calc("25", "5", 12, "-") == "20") {
    passed++;
  } else {
    std::cout << "Test 25 - 5 (12) is failed: must return 20, returns: " << calc("25", "5", 12, "-") << "\n";
    failed++;
  }
  if (calc("d2", "40", 31, "-") == "92") {
    passed++;
  } else {
    std::cout << "Test d2 - 40 (31) is failed: must return 92, returns: " << calc("d2", "40", 31, "-") << "\n";
    failed++;
  }
  if (calc("1fn", "150", 24, "-") == "an") {
    passed++;
  } else {
    std::cout << "Test 1fn - 150 (24) is failed: must return an, returns: " << calc("1fn", "150", 24, "-") << "\n";
    failed++;
  }
  if (calc("1441", "160", 7, "-") == "1251") {
    passed++;
  } else {
    std::cout << "Test 1441 - 160 (7) is failed: must return 1251, returns: " << calc("1441", "160", 7, "-") << "\n";
    failed++;
  }
  if (calc("eh", "79", 23, "*") == "4glf") {
    passed++;
  } else {
    std::cout << "Test eh * 79 (23) is failed: must return 4glf, returns: " << calc("eh", "79", 23, "*") << "\n";
    failed++;
  }
  if (calc("963", "84", 10, "*") == "80892") {
    passed++;
  } else {
    std::cout << "Test 963 * 84 (10) is failed: must return 80892, returns: " << calc("963", "84", 10, "*") << "\n";
    failed++;
  }
  if (calc("oo", "72", 31, "-") == "hm") {
    passed++;
  } else {
    std::cout << "Test oo - 72 (31) is failed: must return hm, returns: " << calc("oo", "72", 31, "-") << "\n";
    failed++;
  }
  if (calc("29", "2", 22, "-") == "27") {
    passed++;
  } else {
    std::cout << "Test 29 - 2 (22) is failed: must return 27, returns: " << calc("29", "2", 22, "-") << "\n";
    failed++;
  }
  if (calc("37b", "220", 17, "+") == "59b") {
    passed++;
  } else {
    std::cout << "Test 37b + 220 (17) is failed: must return 59b, returns: " << calc("37b", "220", 17, "+") << "\n";
    failed++;
  }
  if (calc("f5", "e1", 16, "+") == "1d6") {
    passed++;
  } else {
    std::cout << "Test f5 + e1 (16) is failed: must return 1d6, returns: " << calc("f5", "e1", 16, "+") << "\n";
    failed++;
  }
  if (calc("654", "567", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 654 / 567 (8) is failed: must return 1, returns: " << calc("654", "567", 8, "/") << "\n";
    failed++;
  }
  if (calc("42", "3a", 23, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 42 / 3a (23) is failed: must return 1, returns: " << calc("42", "3a", 23, "/") << "\n";
    failed++;
  }
  if (calc("105", "6i", 27, "*") == "6j69") {
    passed++;
  } else {
    std::cout << "Test 105 * 6i (27) is failed: must return 6j69, returns: " << calc("105", "6i", 27, "*") << "\n";
    failed++;
  }
  if (calc("14f", "5e", 19, "-") == "i1") {
    passed++;
  } else {
    std::cout << "Test 14f - 5e (19) is failed: must return i1, returns: " << calc("14f", "5e", 19, "-") << "\n";
    failed++;
  }
  if (calc("62", "j", 21, "-") == "54") {
    passed++;
  } else {
    std::cout << "Test 62 - j (21) is failed: must return 54, returns: " << calc("62", "j", 21, "-") << "\n";
    failed++;
  }
  if (calc("2142", "1355", 6, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 2142 / 1355 (6) is failed: must return 1, returns: " << calc("2142", "1355", 6, "/") << "\n";
    failed++;
  }
  if (calc("18i", "175", 20, "+") == "2g3") {
    passed++;
  } else {
    std::cout << "Test 18i + 175 (20) is failed: must return 2g3, returns: " << calc("18i", "175", 20, "+") << "\n";
    failed++;
  }
  if (calc("bl", "43", 26, "+") == "fo") {
    passed++;
  } else {
    std::cout << "Test bl + 43 (26) is failed: must return fo, returns: " << calc("bl", "43", 26, "+") << "\n";
    failed++;
  }
  if (calc("j7", "40", 26, "*") == "2p20") {
    passed++;
  } else {
    std::cout << "Test j7 * 40 (26) is failed: must return 2p20, returns: " << calc("j7", "40", 26, "*") << "\n";
    failed++;
  }
  if (calc("1370", "753", 8, "-") == "415") {
    passed++;
  } else {
    std::cout << "Test 1370 - 753 (8) is failed: must return 415, returns: " << calc("1370", "753", 8, "-") << "\n";
    failed++;
  }
  if (calc("19f", "109", 22, "*") == "1a5l3") {
    passed++;
  } else {
    std::cout << "Test 19f * 109 (22) is failed: must return 1a5l3, returns: " << calc("19f", "109", 22, "*") << "\n";
    failed++;
  }
  if (calc("758", "350", 11, "+") == "aa8") {
    passed++;
  } else {
    std::cout << "Test 758 + 350 (11) is failed: must return aa8, returns: " << calc("758", "350", 11, "+") << "\n";
    failed++;
  }
  if (calc("3e4", "60", 16, "/") == "a") {
    passed++;
  } else {
    std::cout << "Test 3e4 / 60 (16) is failed: must return a, returns: " << calc("3e4", "60", 16, "/") << "\n";
    failed++;
  }
  if (calc("263", "15e", 19, "*") == "30964") {
    passed++;
  } else {
    std::cout << "Test 263 * 15e (19) is failed: must return 30964, returns: " << calc("263", "15e", 19, "*") << "\n";
    failed++;
  }
  if (calc("2025", "60", 7, "/") == "22") {
    passed++;
  } else {
    std::cout << "Test 2025 / 60 (7) is failed: must return 22, returns: " << calc("2025", "60", 7, "/") << "\n";
    failed++;
  }
  if (calc("es", "9g", 33, "*") == "48rj") {
    passed++;
  } else {
    std::cout << "Test es * 9g (33) is failed: must return 48rj, returns: " << calc("es", "9g", 33, "*") << "\n";
    failed++;
  }
  if (calc("210122", "101100", 3, "+") == "1011222") {
    passed++;
  } else {
    std::cout << "Test 210122 + 101100 (3) is failed: must return 1011222, returns: " << calc("210122", "101100", 3, "+") << "\n";
    failed++;
  }
  if (calc("11100", "10131", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 11100 / 10131 (5) is failed: must return 1, returns: " << calc("11100", "10131", 5, "/") << "\n";
    failed++;
  }
  if (calc("1m", "13", 30, "-") == "j") {
    passed++;
  } else {
    std::cout << "Test 1m - 13 (30) is failed: must return j, returns: " << calc("1m", "13", 30, "-") << "\n";
    failed++;
  }
  if (calc("93", "25", 30, "-") == "6s") {
    passed++;
  } else {
    std::cout << "Test 93 - 25 (30) is failed: must return 6s, returns: " << calc("93", "25", 30, "-") << "\n";
    failed++;
  }
  if (calc("177", "91", 12, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 177 / 91 (12) is failed: must return 2, returns: " << calc("177", "91", 12, "/") << "\n";
    failed++;
  }
  if (calc("qn", "if", 34, "+") == "1b4") {
    passed++;
  } else {
    std::cout << "Test qn + if (34) is failed: must return 1b4, returns: " << calc("qn", "if", 34, "+") << "\n";
    failed++;
  }
  if (calc("18k", "cc", 27, "*") == "gcko") {
    passed++;
  } else {
    std::cout << "Test 18k * cc (27) is failed: must return gcko, returns: " << calc("18k", "cc", 27, "*") << "\n";
    failed++;
  }
  if (calc("125", "38", 19, "*") == "3fe2") {
    passed++;
  } else {
    std::cout << "Test 125 * 38 (19) is failed: must return 3fe2, returns: " << calc("125", "38", 19, "*") << "\n";
    failed++;
  }
  if (calc("3015", "1310", 6, "+") == "4325") {
    passed++;
  } else {
    std::cout << "Test 3015 + 1310 (6) is failed: must return 4325, returns: " << calc("3015", "1310", 6, "+") << "\n";
    failed++;
  }
  if (calc("488", "bb", 13, "-") == "39a") {
    passed++;
  } else {
    std::cout << "Test 488 - bb (13) is failed: must return 39a, returns: " << calc("488", "bb", 13, "-") << "\n";
    failed++;
  }
  if (calc("f5", "ck", 30, "+") == "rp") {
    passed++;
  } else {
    std::cout << "Test f5 + ck (30) is failed: must return rp, returns: " << calc("f5", "ck", 30, "+") << "\n";
    failed++;
  }
  if (calc("ck", "7t", 33, "+") == "kg") {
    passed++;
  } else {
    std::cout << "Test ck + 7t (33) is failed: must return kg, returns: " << calc("ck", "7t", 33, "+") << "\n";
    failed++;
  }
  if (calc("68", "3n", 31, "*") == "nct") {
    passed++;
  } else {
    std::cout << "Test 68 * 3n (31) is failed: must return nct, returns: " << calc("68", "3n", 31, "*") << "\n";
    failed++;
  }
  if (calc("193", "h", 22, "/") == "1i") {
    passed++;
  } else {
    std::cout << "Test 193 / h (22) is failed: must return 1i, returns: " << calc("193", "h", 22, "/") << "\n";
    failed++;
  }
  if (calc("6k", "p", 31, "-") == "5q") {
    passed++;
  } else {
    std::cout << "Test 6k - p (31) is failed: must return 5q, returns: " << calc("6k", "p", 31, "-") << "\n";
    failed++;
  }
  if (calc("6a", "50", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 6a / 50 (33) is failed: must return 1, returns: " << calc("6a", "50", 33, "/") << "\n";
    failed++;
  }
  if (calc("32a", "60", 13, "+") == "38a") {
    passed++;
  } else {
    std::cout << "Test 32a + 60 (13) is failed: must return 38a, returns: " << calc("32a", "60", 13, "+") << "\n";
    failed++;
  }
  if (calc("11111011", "11000000", 2, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 11111011 / 11000000 (2) is failed: must return 1, returns: " << calc("11111011", "11000000", 2, "/") << "\n";
    failed++;
  }
  if (calc("899", "47", 10, "+") == "946") {
    passed++;
  } else {
    std::cout << "Test 899 + 47 (10) is failed: must return 946, returns: " << calc("899", "47", 10, "+") << "\n";
    failed++;
  }
  if (calc("dl", "81", 32, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test dl / 81 (32) is failed: must return 1, returns: " << calc("dl", "81", 32, "/") << "\n";
    failed++;
  }
  if (calc("161", "i2", 25, "*") == "mb52") {
    passed++;
  } else {
    std::cout << "Test 161 * i2 (25) is failed: must return mb52, returns: " << calc("161", "i2", 25, "*") << "\n";
    failed++;
  }
  if (calc("27f", "aa", 17, "*") == "1917e") {
    passed++;
  } else {
    std::cout << "Test 27f * aa (17) is failed: must return 1917e, returns: " << calc("27f", "aa", 17, "*") << "\n";
    failed++;
  }
  if (calc("h2", "fa", 21, "*") == "ccbk") {
    passed++;
  } else {
    std::cout << "Test h2 * fa (21) is failed: must return ccbk, returns: " << calc("h2", "fa", 21, "*") << "\n";
    failed++;
  }
  if (calc("bi", "84", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test bi / 84 (30) is failed: must return 1, returns: " << calc("bi", "84", 30, "/") << "\n";
    failed++;
  }
  if (calc("4h", "10", 25, "+") == "5h") {
    passed++;
  } else {
    std::cout << "Test 4h + 10 (25) is failed: must return 5h, returns: " << calc("4h", "10", 25, "+") << "\n";
    failed++;
  }
  if (calc("o3", "ai", 28, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test o3 / ai (28) is failed: must return 2, returns: " << calc("o3", "ai", 28, "/") << "\n";
    failed++;
  }
  if (calc("ot", "k0", 31, "-") == "4t") {
    passed++;
  } else {
    std::cout << "Test ot - k0 (31) is failed: must return 4t, returns: " << calc("ot", "k0", 31, "-") << "\n";
    failed++;
  }
  if (calc("32b", "168", 14, "*") == "49b44") {
    passed++;
  } else {
    std::cout << "Test 32b * 168 (14) is failed: must return 49b44, returns: " << calc("32b", "168", 14, "*") << "\n";
    failed++;
  }
  if (calc("12q", "if", 30, "-") == "eb") {
    passed++;
  } else {
    std::cout << "Test 12q - if (30) is failed: must return eb, returns: " << calc("12q", "if", 30, "-") << "\n";
    failed++;
  }
  if (calc("g2", "be", 18, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test g2 / be (18) is failed: must return 1, returns: " << calc("g2", "be", 18, "/") << "\n";
    failed++;
  }
  if (calc("18", "15", 11, "+") == "32") {
    passed++;
  } else {
    std::cout << "Test 18 + 15 (11) is failed: must return 32, returns: " << calc("18", "15", 11, "+") << "\n";
    failed++;
  }
  if (calc("l4", "gf", 33, "*") == "ahhr") {
    passed++;
  } else {
    std::cout << "Test l4 * gf (33) is failed: must return ahhr, returns: " << calc("l4", "gf", 33, "*") << "\n";
    failed++;
  }
  if (calc("341", "28a", 15, "+") == "5cb") {
    passed++;
  } else {
    std::cout << "Test 341 + 28a (15) is failed: must return 5cb, returns: " << calc("341", "28a", 15, "+") << "\n";
    failed++;
  }
  if (calc("11e", "a9", 17, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 11e / a9 (17) is failed: must return 1, returns: " << calc("11e", "a9", 17, "/") << "\n";
    failed++;
  }
  if (calc("dg", "bc", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test dg / bc (30) is failed: must return 1, returns: " << calc("dg", "bc", 30, "/") << "\n";
    failed++;
  }
  if (calc("1411", "540", 6, "*") == "1334340") {
    passed++;
  } else {
    std::cout << "Test 1411 * 540 (6) is failed: must return 1334340, returns: " << calc("1411", "540", 6, "*") << "\n";
    failed++;
  }
  if (calc("18c", "ab", 25, "-") == "n1") {
    passed++;
  } else {
    std::cout << "Test 18c - ab (25) is failed: must return n1, returns: " << calc("18c", "ab", 25, "-") << "\n";
    failed++;
  }
  if (calc("203", "200", 8, "+") == "403") {
    passed++;
  } else {
    std::cout << "Test 203 + 200 (8) is failed: must return 403, returns: " << calc("203", "200", 8, "+") << "\n";
    failed++;
  }
  if (calc("24k", "54", 21, "+") == "2a3") {
    passed++;
  } else {
    std::cout << "Test 24k + 54 (21) is failed: must return 2a3, returns: " << calc("24k", "54", 21, "+") << "\n";
    failed++;
  }
  if (calc("350", "133", 8, "-") == "215") {
    passed++;
  } else {
    std::cout << "Test 350 - 133 (8) is failed: must return 215, returns: " << calc("350", "133", 8, "-") << "\n";
    failed++;
  }
  if (calc("102", "lj", 26, "*") == "lkhc") {
    passed++;
  } else {
    std::cout << "Test 102 * lj (26) is failed: must return lkhc, returns: " << calc("102", "lj", 26, "*") << "\n";
    failed++;
  }
  if (calc("33", "12", 26, "*") == "396") {
    passed++;
  } else {
    std::cout << "Test 33 * 12 (26) is failed: must return 396, returns: " << calc("33", "12", 26, "*") << "\n";
    failed++;
  }
  if (calc("2001", "210", 4, "-") == "1131") {
    passed++;
  } else {
    std::cout << "Test 2001 - 210 (4) is failed: must return 1131, returns: " << calc("2001", "210", 4, "-") << "\n";
    failed++;
  }
  if (calc("lw", "4c", 34, "+") == "qa") {
    passed++;
  } else {
    std::cout << "Test lw + 4c (34) is failed: must return qa, returns: " << calc("lw", "4c", 34, "+") << "\n";
    failed++;
  }
  if (calc("603", "440", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 603 / 440 (9) is failed: must return 1, returns: " << calc("603", "440", 9, "/") << "\n";
    failed++;
  }
  if (calc("25", "8", 10, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 25 / 8 (10) is failed: must return 3, returns: " << calc("25", "8", 10, "/") << "\n";
    failed++;
  }
  if (calc("510", "364", 8, "*") == "234240") {
    passed++;
  } else {
    std::cout << "Test 510 * 364 (8) is failed: must return 234240, returns: " << calc("510", "364", 8, "*") << "\n";
    failed++;
  }
  if (calc("265", "131", 8, "-") == "134") {
    passed++;
  } else {
    std::cout << "Test 265 - 131 (8) is failed: must return 134, returns: " << calc("265", "131", 8, "-") << "\n";
    failed++;
  }
  if (calc("ps", "76", 34, "+") == "x0") {
    passed++;
  } else {
    std::cout << "Test ps + 76 (34) is failed: must return x0, returns: " << calc("ps", "76", 34, "+") << "\n";
    failed++;
  }
  if (calc("m9", "37", 31, "-") == "j2") {
    passed++;
  } else {
    std::cout << "Test m9 - 37 (31) is failed: must return j2, returns: " << calc("m9", "37", 31, "-") << "\n";
    failed++;
  }
  if (calc("43a", "3d3", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 43a / 3d3 (14) is failed: must return 1, returns: " << calc("43a", "3d3", 14, "/") << "\n";
    failed++;
  }
  if (calc("187", "i1", 24, "+") == "228") {
    passed++;
  } else {
    std::cout << "Test 187 + i1 (24) is failed: must return 228, returns: " << calc("187", "i1", 24, "+") << "\n";
    failed++;
  }
  if (calc("425", "313", 6, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 425 / 313 (6) is failed: must return 1, returns: " << calc("425", "313", 6, "/") << "\n";
    failed++;
  }
  if (calc("50", "50", 6, "-") == "0") {
    passed++;
  } else {
    std::cout << "Test 50 - 50 (6) is failed: must return 0, returns: " << calc("50", "50", 6, "-") << "\n";
    failed++;
  }
  if (calc("1042", "364", 7, "-") == "345") {
    passed++;
  } else {
    std::cout << "Test 1042 - 364 (7) is failed: must return 345, returns: " << calc("1042", "364", 7, "-") << "\n";
    failed++;
  }
  if (calc("2311", "1052", 7, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 2311 / 1052 (7) is failed: must return 2, returns: " << calc("2311", "1052", 7, "/") << "\n";
    failed++;
  }
  if (calc("16g", "he", 19, "*") == "1526f") {
    passed++;
  } else {
    std::cout << "Test 16g * he (19) is failed: must return 1526f, returns: " << calc("16g", "he", 19, "*") << "\n";
    failed++;
  }
  if (calc("c6", "2g", 23, "*") == "1a14") {
    passed++;
  } else {
    std::cout << "Test c6 * 2g (23) is failed: must return 1a14, returns: " << calc("c6", "2g", 23, "*") << "\n";
    failed++;
  }
  if (calc("10022", "333", 4, "+") == "11021") {
    passed++;
  } else {
    std::cout << "Test 10022 + 333 (4) is failed: must return 11021, returns: " << calc("10022", "333", 4, "+") << "\n";
    failed++;
  }
  if (calc("1f", "14", 23, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1f / 14 (23) is failed: must return 1, returns: " << calc("1f", "14", 23, "/") << "\n";
    failed++;
  }
  if (calc("229", "135", 20, "+") == "35e") {
    passed++;
  } else {
    std::cout << "Test 229 + 135 (20) is failed: must return 35e, returns: " << calc("229", "135", 20, "+") << "\n";
    failed++;
  }
  if (calc("h6", "ey", 35, "+") == "w5") {
    passed++;
  } else {
    std::cout << "Test h6 + ey (35) is failed: must return w5, returns: " << calc("h6", "ey", 35, "+") << "\n";
    failed++;
  }
  if (calc("119", "87", 22, "+") == "19g") {
    passed++;
  } else {
    std::cout << "Test 119 + 87 (22) is failed: must return 19g, returns: " << calc("119", "87", 22, "+") << "\n";
    failed++;
  }
  if (calc("2e", "1g", 30, "*") == "3ne") {
    passed++;
  } else {
    std::cout << "Test 2e * 1g (30) is failed: must return 3ne, returns: " << calc("2e", "1g", 30, "*") << "\n";
    failed++;
  }
  if (calc("127", "49", 19, "+") == "16g") {
    passed++;
  } else {
    std::cout << "Test 127 + 49 (19) is failed: must return 16g, returns: " << calc("127", "49", 19, "+") << "\n";
    failed++;
  }
  if (calc("2c1", "1d7", 16, "-") == "ea") {
    passed++;
  } else {
    std::cout << "Test 2c1 - 1d7 (16) is failed: must return ea, returns: " << calc("2c1", "1d7", 16, "-") << "\n";
    failed++;
  }
  if (calc("15", "12", 7, "*") == "213") {
    passed++;
  } else {
    std::cout << "Test 15 * 12 (7) is failed: must return 213, returns: " << calc("15", "12", 7, "*") << "\n";
    failed++;
  }
  if (calc("rv", "m6", 35, "*") == "hn9b") {
    passed++;
  } else {
    std::cout << "Test rv * m6 (35) is failed: must return hn9b, returns: " << calc("rv", "m6", 35, "*") << "\n";
    failed++;
  }
  if (calc("664", "84", 9, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test 664 / 84 (9) is failed: must return 7, returns: " << calc("664", "84", 9, "/") << "\n";
    failed++;
  }
  if (calc("5b", "1g", 28, "-") == "3n") {
    passed++;
  } else {
    std::cout << "Test 5b - 1g (28) is failed: must return 3n, returns: " << calc("5b", "1g", 28, "-") << "\n";
    failed++;
  }
  if (calc("46", "1k", 26, "+") == "60") {
    passed++;
  } else {
    std::cout << "Test 46 + 1k (26) is failed: must return 60, returns: " << calc("46", "1k", 26, "+") << "\n";
    failed++;
  }
  if (calc("1000", "110", 2, "+") == "1110") {
    passed++;
  } else {
    std::cout << "Test 1000 + 110 (2) is failed: must return 1110, returns: " << calc("1000", "110", 2, "+") << "\n";
    failed++;
  }
  if (calc("1000101", "1000011", 2, "*") == "1001000001111") {
    passed++;
  } else {
    std::cout << "Test 1000101 * 1000011 (2) is failed: must return 1001000001111, returns: " << calc("1000101", "1000011", 2, "*") << "\n";
    failed++;
  }
  if (calc("pp", "kg", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test pp / kg (35) is failed: must return 1, returns: " << calc("pp", "kg", 35, "/") << "\n";
    failed++;
  }
  if (calc("268", "254", 17, "*") == "586ef") {
    passed++;
  } else {
    std::cout << "Test 268 * 254 (17) is failed: must return 586ef, returns: " << calc("268", "254", 17, "*") << "\n";
    failed++;
  }
  if (calc("45", "1e", 31, "*") == "618") {
    passed++;
  } else {
    std::cout << "Test 45 * 1e (31) is failed: must return 618, returns: " << calc("45", "1e", 31, "*") << "\n";
    failed++;
  }
  if (calc("11101000", "101010", 2, "*") == "10011000010000") {
    passed++;
  } else {
    std::cout << "Test 11101000 * 101010 (2) is failed: must return 10011000010000, returns: " << calc("11101000", "101010", 2, "*") << "\n";
    failed++;
  }
  if (calc("9a", "3j", 21, "*") == "1g01") {
    passed++;
  } else {
    std::cout << "Test 9a * 3j (21) is failed: must return 1g01, returns: " << calc("9a", "3j", 21, "*") << "\n";
    failed++;
  }
  if (calc("136", "70", 15, "*") == "88c0") {
    passed++;
  } else {
    std::cout << "Test 136 * 70 (15) is failed: must return 88c0, returns: " << calc("136", "70", 15, "*") << "\n";
    failed++;
  }
  if (calc("111", "cj", 21, "+") == "1dk") {
    passed++;
  } else {
    std::cout << "Test 111 + cj (21) is failed: must return 1dk, returns: " << calc("111", "cj", 21, "+") << "\n";
    failed++;
  }
  if (calc("r3", "h1", 29, "*") == "fqk3") {
    passed++;
  } else {
    std::cout << "Test r3 * h1 (29) is failed: must return fqk3, returns: " << calc("r3", "h1", 29, "*") << "\n";
    failed++;
  }
  if (calc("a7", "77", 22, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test a7 / 77 (22) is failed: must return 1, returns: " << calc("a7", "77", 22, "/") << "\n";
    failed++;
  }
  if (calc("1512", "566", 7, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1512 / 566 (7) is failed: must return 2, returns: " << calc("1512", "566", 7, "/") << "\n";
    failed++;
  }
  if (calc("b8", "2b", 22, "*") == "1690") {
    passed++;
  } else {
    std::cout << "Test b8 * 2b (22) is failed: must return 1690, returns: " << calc("b8", "2b", 22, "*") << "\n";
    failed++;
  }
  if (calc("2344", "2222", 6, "+") == "5010") {
    passed++;
  } else {
    std::cout << "Test 2344 + 2222 (6) is failed: must return 5010, returns: " << calc("2344", "2222", 6, "+") << "\n";
    failed++;
  }
  if (calc("p4", "ei", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test p4 / ei (30) is failed: must return 1, returns: " << calc("p4", "ei", 30, "/") << "\n";
    failed++;
  }
  if (calc("i0", "eb", 22, "-") == "3b") {
    passed++;
  } else {
    std::cout << "Test i0 - eb (22) is failed: must return 3b, returns: " << calc("i0", "eb", 22, "-") << "\n";
    failed++;
  }
  if (calc("am", "j", 25, "-") == "a3") {
    passed++;
  } else {
    std::cout << "Test am - j (25) is failed: must return a3, returns: " << calc("am", "j", 25, "-") << "\n";
    failed++;
  }
  if (calc("290", "1c7", 13, "+") == "487") {
    passed++;
  } else {
    std::cout << "Test 290 + 1c7 (13) is failed: must return 487, returns: " << calc("290", "1c7", 13, "+") << "\n";
    failed++;
  }
  if (calc("3333", "332", 4, "-") == "3001") {
    passed++;
  } else {
    std::cout << "Test 3333 - 332 (4) is failed: must return 3001, returns: " << calc("3333", "332", 4, "-") << "\n";
    failed++;
  }
  if (calc("1ck", "2l", 23, "*") == "4cb6") {
    passed++;
  } else {
    std::cout << "Test 1ck * 2l (23) is failed: must return 4cb6, returns: " << calc("1ck", "2l", 23, "*") << "\n";
    failed++;
  }
  if (calc("o7", "la", 31, "*") == "gkh8") {
    passed++;
  } else {
    std::cout << "Test o7 * la (31) is failed: must return gkh8, returns: " << calc("o7", "la", 31, "*") << "\n";
    failed++;
  }
  if (calc("1ee", "1f", 17, "/") == "g") {
    passed++;
  } else {
    std::cout << "Test 1ee / 1f (17) is failed: must return g, returns: " << calc("1ee", "1f", 17, "/") << "\n";
    failed++;
  }
  if (calc("1fi", "1bf", 22, "*") == "2djh6") {
    passed++;
  } else {
    std::cout << "Test 1fi * 1bf (22) is failed: must return 2djh6, returns: " << calc("1fi", "1bf", 22, "*") << "\n";
    failed++;
  }
  if (calc("if", "9m", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test if / 9m (26) is failed: must return 1, returns: " << calc("if", "9m", 26, "/") << "\n";
    failed++;
  }
  if (calc("tn", "no", 32, "-") == "5v") {
    passed++;
  } else {
    std::cout << "Test tn - no (32) is failed: must return 5v, returns: " << calc("tn", "no", 32, "-") << "\n";
    failed++;
  }
  if (calc("po", "id", 29, "-") == "7b") {
    passed++;
  } else {
    std::cout << "Test po - id (29) is failed: must return 7b, returns: " << calc("po", "id", 29, "-") << "\n";
    failed++;
  }
  if (calc("1011000000", "111001001", 2, "-") == "11110111") {
    passed++;
  } else {
    std::cout << "Test 1011000000 - 111001001 (2) is failed: must return 11110111, returns: " << calc("1011000000", "111001001", 2, "-") << "\n";
    failed++;
  }
  if (calc("12h", "l4", 27, "-") == "8d") {
    passed++;
  } else {
    std::cout << "Test 12h - l4 (27) is failed: must return 8d, returns: " << calc("12h", "l4", 27, "-") << "\n";
    failed++;
  }
  if (calc("16k", "9", 24, "+") == "175") {
    passed++;
  } else {
    std::cout << "Test 16k + 9 (24) is failed: must return 175, returns: " << calc("16k", "9", 24, "+") << "\n";
    failed++;
  }
  if (calc("3312", "2211", 4, "-") == "1101") {
    passed++;
  } else {
    std::cout << "Test 3312 - 2211 (4) is failed: must return 1101, returns: " << calc("3312", "2211", 4, "-") << "\n";
    failed++;
  }
  if (calc("15a", "d4", 19, "-") == "b6") {
    passed++;
  } else {
    std::cout << "Test 15a - d4 (19) is failed: must return b6, returns: " << calc("15a", "d4", 19, "-") << "\n";
    failed++;
  }
  if (calc("15h", "fh", 21, "*") == "k3hg") {
    passed++;
  } else {
    std::cout << "Test 15h * fh (21) is failed: must return k3hg, returns: " << calc("15h", "fh", 21, "*") << "\n";
    failed++;
  }
  if (calc("534", "402", 8, "+") == "1136") {
    passed++;
  } else {
    std::cout << "Test 534 + 402 (8) is failed: must return 1136, returns: " << calc("534", "402", 8, "+") << "\n";
    failed++;
  }
  if (calc("10d", "ma", 30, "*") == "mjka") {
    passed++;
  } else {
    std::cout << "Test 10d * ma (30) is failed: must return mjka, returns: " << calc("10d", "ma", 30, "*") << "\n";
    failed++;
  }
  if (calc("1062", "232", 7, "*") == "253344") {
    passed++;
  } else {
    std::cout << "Test 1062 * 232 (7) is failed: must return 253344, returns: " << calc("1062", "232", 7, "*") << "\n";
    failed++;
  }
  if (calc("1af", "9", 18, "/") == "33") {
    passed++;
  } else {
    std::cout << "Test 1af / 9 (18) is failed: must return 33, returns: " << calc("1af", "9", 18, "/") << "\n";
    failed++;
  }
  if (calc("14r", "j5", 29, "+") == "1o3") {
    passed++;
  } else {
    std::cout << "Test 14r + j5 (29) is failed: must return 1o3, returns: " << calc("14r", "j5", 29, "+") << "\n";
    failed++;
  }
  if (calc("713", "202", 8, "-") == "511") {
    passed++;
  } else {
    std::cout << "Test 713 - 202 (8) is failed: must return 511, returns: " << calc("713", "202", 8, "-") << "\n";
    failed++;
  }
  if (calc("n2", "93", 33, "*") == "6bl6") {
    passed++;
  } else {
    std::cout << "Test n2 * 93 (33) is failed: must return 6bl6, returns: " << calc("n2", "93", 33, "*") << "\n";
    failed++;
  }
  if (calc("ab", "18", 20, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test ab / 18 (20) is failed: must return 7, returns: " << calc("ab", "18", 20, "/") << "\n";
    failed++;
  }
  if (calc("115", "lr", 29, "+") == "1n3") {
    passed++;
  } else {
    std::cout << "Test 115 + lr (29) is failed: must return 1n3, returns: " << calc("115", "lr", 29, "+") << "\n";
    failed++;
  }
  if (calc("1ia", "4j", 20, "-") == "1db") {
    passed++;
  } else {
    std::cout << "Test 1ia - 4j (20) is failed: must return 1db, returns: " << calc("1ia", "4j", 20, "-") << "\n";
    failed++;
  }
  if (calc("1321", "7", 9, "*") == "10457") {
    passed++;
  } else {
    std::cout << "Test 1321 * 7 (9) is failed: must return 10457, returns: " << calc("1321", "7", 9, "*") << "\n";
    failed++;
  }
  if (calc("310", "166", 12, "*") == "49060") {
    passed++;
  } else {
    std::cout << "Test 310 * 166 (12) is failed: must return 49060, returns: " << calc("310", "166", 12, "*") << "\n";
    failed++;
  }
  if (calc("13", "l", 30, "*") == "n3") {
    passed++;
  } else {
    std::cout << "Test 13 * l (30) is failed: must return n3, returns: " << calc("13", "l", 30, "*") << "\n";
    failed++;
  }
  if (calc("122", "d4", 16, "*") == "f028") {
    passed++;
  } else {
    std::cout << "Test 122 * d4 (16) is failed: must return f028, returns: " << calc("122", "d4", 16, "*") << "\n";
    failed++;
  }
  if (calc("10i", "cj", 30, "+") == "1d7") {
    passed++;
  } else {
    std::cout << "Test 10i + cj (30) is failed: must return 1d7, returns: " << calc("10i", "cj", 30, "+") << "\n";
    failed++;
  }
  if (calc("21", "1e", 15, "-") == "2") {
    passed++;
  } else {
    std::cout << "Test 21 - 1e (15) is failed: must return 2, returns: " << calc("21", "1e", 15, "-") << "\n";
    failed++;
  }
  if (calc("172", "g4", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 172 / g4 (20) is failed: must return 1, returns: " << calc("172", "g4", 20, "/") << "\n";
    failed++;
  }
  if (calc("588", "270", 9, "+") == "868") {
    passed++;
  } else {
    std::cout << "Test 588 + 270 (9) is failed: must return 868, returns: " << calc("588", "270", 9, "+") << "\n";
    failed++;
  }
  if (calc("712", "152", 8, "*") == "136644") {
    passed++;
  } else {
    std::cout << "Test 712 * 152 (8) is failed: must return 136644, returns: " << calc("712", "152", 8, "*") << "\n";
    failed++;
  }
  if (calc("ie", "dk", 21, "-") == "4f") {
    passed++;
  } else {
    std::cout << "Test ie - dk (21) is failed: must return 4f, returns: " << calc("ie", "dk", 21, "-") << "\n";
    failed++;
  }
  if (calc("1001010101", "1001000010", 2, "+") == "10010010111") {
    passed++;
  } else {
    std::cout << "Test 1001010101 + 1001000010 (2) is failed: must return 10010010111, returns: " << calc("1001010101", "1001000010", 2, "+") << "\n";
    failed++;
  }
  if (calc("1110111011", "100000110", 2, "-") == "1010110101") {
    passed++;
  } else {
    std::cout << "Test 1110111011 - 100000110 (2) is failed: must return 1010110101, returns: " << calc("1110111011", "100000110", 2, "-") << "\n";
    failed++;
  }
  if (calc("304", "256", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 304 / 256 (13) is failed: must return 1, returns: " << calc("304", "256", 13, "/") << "\n";
    failed++;
  }
  if (calc("1kj", "eb", 21, "*") == "17kck") {
    passed++;
  } else {
    std::cout << "Test 1kj * eb (21) is failed: must return 17kck, returns: " << calc("1kj", "eb", 21, "*") << "\n";
    failed++;
  }
  if (calc("253", "25", 14, "*") == "5841") {
    passed++;
  } else {
    std::cout << "Test 253 * 25 (14) is failed: must return 5841, returns: " << calc("253", "25", 14, "*") << "\n";
    failed++;
  }
  if (calc("177", "65", 9, "+") == "253") {
    passed++;
  } else {
    std::cout << "Test 177 + 65 (9) is failed: must return 253, returns: " << calc("177", "65", 9, "+") << "\n";
    failed++;
  }
  if (calc("461", "2b4", 12, "+") == "755") {
    passed++;
  } else {
    std::cout << "Test 461 + 2b4 (12) is failed: must return 755, returns: " << calc("461", "2b4", 12, "+") << "\n";
    failed++;
  }
  if (calc("13100", "333", 4, "*") == "13020300") {
    passed++;
  } else {
    std::cout << "Test 13100 * 333 (4) is failed: must return 13020300, returns: " << calc("13100", "333", 4, "*") << "\n";
    failed++;
  }
  if (calc("253", "252", 9, "*") == "66176") {
    passed++;
  } else {
    std::cout << "Test 253 * 252 (9) is failed: must return 66176, returns: " << calc("253", "252", 9, "*") << "\n";
    failed++;
  }
  if (calc("o8", "hm", 28, "+") == "1e2") {
    passed++;
  } else {
    std::cout << "Test o8 + hm (28) is failed: must return 1e2, returns: " << calc("o8", "hm", 28, "+") << "\n";
    failed++;
  }
  if (calc("48", "39", 23, "+") == "7h") {
    passed++;
  } else {
    std::cout << "Test 48 + 39 (23) is failed: must return 7h, returns: " << calc("48", "39", 23, "+") << "\n";
    failed++;
  }
  if (calc("cj", "7g", 27, "-") == "53") {
    passed++;
  } else {
    std::cout << "Test cj - 7g (27) is failed: must return 53, returns: " << calc("cj", "7g", 27, "-") << "\n";
    failed++;
  }
  if (calc("3340", "330", 5, "*") == "2323200") {
    passed++;
  } else {
    std::cout << "Test 3340 * 330 (5) is failed: must return 2323200, returns: " << calc("3340", "330", 5, "*") << "\n";
    failed++;
  }
  if (calc("m3", "kf", 23, "+") == "1ji") {
    passed++;
  } else {
    std::cout << "Test m3 + kf (23) is failed: must return 1ji, returns: " << calc("m3", "kf", 23, "+") << "\n";
    failed++;
  }
  if (calc("lx", "fm", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test lx / fm (35) is failed: must return 1, returns: " << calc("lx", "fm", 35, "/") << "\n";
    failed++;
  }
  if (calc("83", "14", 12, "*") == "b00") {
    passed++;
  } else {
    std::cout << "Test 83 * 14 (12) is failed: must return b00, returns: " << calc("83", "14", 12, "*") << "\n";
    failed++;
  }
  if (calc("13j", "k4", 21, "+") == "232") {
    passed++;
  } else {
    std::cout << "Test 13j + k4 (21) is failed: must return 232, returns: " << calc("13j", "k4", 21, "+") << "\n";
    failed++;
  }
  if (calc("ka", "d8", 21, "+") == "1ci") {
    passed++;
  } else {
    std::cout << "Test ka + d8 (21) is failed: must return 1ci, returns: " << calc("ka", "d8", 21, "+") << "\n";
    failed++;
  }
  if (calc("25g", "f0", 19, "+") == "31g") {
    passed++;
  } else {
    std::cout << "Test 25g + f0 (19) is failed: must return 31g, returns: " << calc("25g", "f0", 19, "+") << "\n";
    failed++;
  }
  if (calc("15", "2", 6, "-") == "13") {
    passed++;
  } else {
    std::cout << "Test 15 - 2 (6) is failed: must return 13, returns: " << calc("15", "2", 6, "-") << "\n";
    failed++;
  }
  if (calc("531", "351", 12, "-") == "1a0") {
    passed++;
  } else {
    std::cout << "Test 531 - 351 (12) is failed: must return 1a0, returns: " << calc("531", "351", 12, "-") << "\n";
    failed++;
  }
  if (calc("1021110", "102122", 3, "*") == "112211120120") {
    passed++;
  } else {
    std::cout << "Test 1021110 * 102122 (3) is failed: must return 112211120120, returns: " << calc("1021110", "102122", 3, "*") << "\n";
    failed++;
  }
  if (calc("422", "325", 14, "-") == "db") {
    passed++;
  } else {
    std::cout << "Test 422 - 325 (14) is failed: must return db, returns: " << calc("422", "325", 14, "-") << "\n";
    failed++;
  }
  if (calc("1432", "266", 8, "*") == "432174") {
    passed++;
  } else {
    std::cout << "Test 1432 * 266 (8) is failed: must return 432174, returns: " << calc("1432", "266", 8, "*") << "\n";
    failed++;
  }
  if (calc("1021", "312", 4, "-") == "103") {
    passed++;
  } else {
    std::cout << "Test 1021 - 312 (4) is failed: must return 103, returns: " << calc("1021", "312", 4, "-") << "\n";
    failed++;
  }
  if (calc("290", "5c", 20, "*") == "de80") {
    passed++;
  } else {
    std::cout << "Test 290 * 5c (20) is failed: must return de80, returns: " << calc("290", "5c", 20, "*") << "\n";
    failed++;
  }
  if (calc("576", "537", 9, "*") == "346386") {
    passed++;
  } else {
    std::cout << "Test 576 * 537 (9) is failed: must return 346386, returns: " << calc("576", "537", 9, "*") << "\n";
    failed++;
  }
  if (calc("1ec", "87", 21, "-") == "165") {
    passed++;
  } else {
    std::cout << "Test 1ec - 87 (21) is failed: must return 165, returns: " << calc("1ec", "87", 21, "-") << "\n";
    failed++;
  }
  if (calc("248", "71", 9, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 248 / 71 (9) is failed: must return 3, returns: " << calc("248", "71", 9, "/") << "\n";
    failed++;
  }
  if (calc("p7", "fc", 33, "-") == "9s") {
    passed++;
  } else {
    std::cout << "Test p7 - fc (33) is failed: must return 9s, returns: " << calc("p7", "fc", 33, "-") << "\n";
    failed++;
  }
  if (calc("75", "2n", 26, "+") == "a2") {
    passed++;
  } else {
    std::cout << "Test 75 + 2n (26) is failed: must return a2, returns: " << calc("75", "2n", 26, "+") << "\n";
    failed++;
  }
  if (calc("1110", "203", 4, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1110 / 203 (4) is failed: must return 2, returns: " << calc("1110", "203", 4, "/") << "\n";
    failed++;
  }
  if (calc("8", "2", 19, "-") == "6") {
    passed++;
  } else {
    std::cout << "Test 8 - 2 (19) is failed: must return 6, returns: " << calc("8", "2", 19, "-") << "\n";
    failed++;
  }
  if (calc("q6", "i3", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test q6 / i3 (33) is failed: must return 1, returns: " << calc("q6", "i3", 33, "/") << "\n";
    failed++;
  }
  if (calc("1143", "166", 8, "*") == "214642") {
    passed++;
  } else {
    std::cout << "Test 1143 * 166 (8) is failed: must return 214642, returns: " << calc("1143", "166", 8, "*") << "\n";
    failed++;
  }
  if (calc("a0", "92", 16, "+") == "132") {
    passed++;
  } else {
    std::cout << "Test a0 + 92 (16) is failed: must return 132, returns: " << calc("a0", "92", 16, "+") << "\n";
    failed++;
  }
  if (calc("8t", "3b", 34, "*") == "ted") {
    passed++;
  } else {
    std::cout << "Test 8t * 3b (34) is failed: must return ted, returns: " << calc("8t", "3b", 34, "*") << "\n";
    failed++;
  }
  if (calc("r1", "3s", 35, "+") == "ut") {
    passed++;
  } else {
    std::cout << "Test r1 + 3s (35) is failed: must return ut, returns: " << calc("r1", "3s", 35, "+") << "\n";
    failed++;
  }
  if (calc("111", "76", 11, "*") == "8326") {
    passed++;
  } else {
    std::cout << "Test 111 * 76 (11) is failed: must return 8326, returns: " << calc("111", "76", 11, "*") << "\n";
    failed++;
  }
  if (calc("746", "434", 8, "-") == "312") {
    passed++;
  } else {
    std::cout << "Test 746 - 434 (8) is failed: must return 312, returns: " << calc("746", "434", 8, "-") << "\n";
    failed++;
  }
  if (calc("b4", "ab", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test b4 / ab (20) is failed: must return 1, returns: " << calc("b4", "ab", 20, "/") << "\n";
    failed++;
  }
  if (calc("427", "246", 15, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 427 / 246 (15) is failed: must return 1, returns: " << calc("427", "246", 15, "/") << "\n";
    failed++;
  }
  if (calc("3b6", "25b", 15, "+") == "622") {
    passed++;
  } else {
    std::cout << "Test 3b6 + 25b (15) is failed: must return 622, returns: " << calc("3b6", "25b", 15, "+") << "\n";
    failed++;
  }
  if (calc("7g", "7", 26, "/") == "12") {
    passed++;
  } else {
    std::cout << "Test 7g / 7 (26) is failed: must return 12, returns: " << calc("7g", "7", 26, "/") << "\n";
    failed++;
  }
  if (calc("ch", "3g", 30, "+") == "g3") {
    passed++;
  } else {
    std::cout << "Test ch + 3g (30) is failed: must return g3, returns: " << calc("ch", "3g", 30, "+") << "\n";
    failed++;
  }
  if (calc("3043", "1231", 5, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 3043 / 1231 (5) is failed: must return 2, returns: " << calc("3043", "1231", 5, "/") << "\n";
    failed++;
  }
  if (calc("230", "226", 16, "-") == "a") {
    passed++;
  } else {
    std::cout << "Test 230 - 226 (16) is failed: must return a, returns: " << calc("230", "226", 16, "-") << "\n";
    failed++;
  }
  if (calc("2212", "1030", 4, "-") == "1122") {
    passed++;
  } else {
    std::cout << "Test 2212 - 1030 (4) is failed: must return 1122, returns: " << calc("2212", "1030", 4, "-") << "\n";
    failed++;
  }
  if (calc("4231", "1512", 6, "+") == "10143") {
    passed++;
  } else {
    std::cout << "Test 4231 + 1512 (6) is failed: must return 10143, returns: " << calc("4231", "1512", 6, "+") << "\n";
    failed++;
  }
  if (calc("9n", "92", 30, "+") == "ip") {
    passed++;
  } else {
    std::cout << "Test 9n + 92 (30) is failed: must return ip, returns: " << calc("9n", "92", 30, "+") << "\n";
    failed++;
  }
  if (calc("16h", "9d", 26, "*") == "bo5d") {
    passed++;
  } else {
    std::cout << "Test 16h * 9d (26) is failed: must return bo5d, returns: " << calc("16h", "9d", 26, "*") << "\n";
    failed++;
  }
  if (calc("10210", "2102", 3, "+") == "20012") {
    passed++;
  } else {
    std::cout << "Test 10210 + 2102 (3) is failed: must return 20012, returns: " << calc("10210", "2102", 3, "+") << "\n";
    failed++;
  }
  if (calc("1n", "11", 30, "+") == "2o") {
    passed++;
  } else {
    std::cout << "Test 1n + 11 (30) is failed: must return 2o, returns: " << calc("1n", "11", 30, "+") << "\n";
    failed++;
  }
  if (calc("84", "7m", 28, "-") == "a") {
    passed++;
  } else {
    std::cout << "Test 84 - 7m (28) is failed: must return a, returns: " << calc("84", "7m", 28, "-") << "\n";
    failed++;
  }
  if (calc("821", "673", 9, "+") == "1604") {
    passed++;
  } else {
    std::cout << "Test 821 + 673 (9) is failed: must return 1604, returns: " << calc("821", "673", 9, "+") << "\n";
    failed++;
  }
  if (calc("mq", "fd", 30, "-") == "7d") {
    passed++;
  } else {
    std::cout << "Test mq - fd (30) is failed: must return 7d, returns: " << calc("mq", "fd", 30, "-") << "\n";
    failed++;
  }
  if (calc("1id", "lc", 22, "-") == "j1") {
    passed++;
  } else {
    std::cout << "Test 1id - lc (22) is failed: must return j1, returns: " << calc("1id", "lc", 22, "-") << "\n";
    failed++;
  }
  if (calc("r4", "io", 28, "+") == "1i0") {
    passed++;
  } else {
    std::cout << "Test r4 + io (28) is failed: must return 1i0, returns: " << calc("r4", "io", 28, "+") << "\n";
    failed++;
  }
  if (calc("12d", "6j", 21, "-") == "gf") {
    passed++;
  } else {
    std::cout << "Test 12d - 6j (21) is failed: must return gf, returns: " << calc("12d", "6j", 21, "-") << "\n";
    failed++;
  }
  if (calc("f2", "ao", 30, "*") == "5cli") {
    passed++;
  } else {
    std::cout << "Test f2 * ao (30) is failed: must return 5cli, returns: " << calc("f2", "ao", 30, "*") << "\n";
    failed++;
  }
  if (calc("33c", "ca", 15, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 33c / ca (15) is failed: must return 3, returns: " << calc("33c", "ca", 15, "/") << "\n";
    failed++;
  }
  if (calc("28", "27", 34, "+") == "4f") {
    passed++;
  } else {
    std::cout << "Test 28 + 27 (34) is failed: must return 4f, returns: " << calc("28", "27", 34, "+") << "\n";
    failed++;
  }
  if (calc("300", "151", 8, "*") == "47300") {
    passed++;
  } else {
    std::cout << "Test 300 * 151 (8) is failed: must return 47300, returns: " << calc("300", "151", 8, "*") << "\n";
    failed++;
  }
  if (calc("4e", "h", 33, "-") == "3u") {
    passed++;
  } else {
    std::cout << "Test 4e - h (33) is failed: must return 3u, returns: " << calc("4e", "h", 33, "-") << "\n";
    failed++;
  }
  if (calc("180", "oc", 25, "*") == "177l0") {
    passed++;
  } else {
    std::cout << "Test 180 * oc (25) is failed: must return 177l0, returns: " << calc("180", "oc", 25, "*") << "\n";
    failed++;
  }
  if (calc("1074", "873", 9, "+") == "2057") {
    passed++;
  } else {
    std::cout << "Test 1074 + 873 (9) is failed: must return 2057, returns: " << calc("1074", "873", 9, "+") << "\n";
    failed++;
  }
  if (calc("ir", "1q", 35, "/") == "a") {
    passed++;
  } else {
    std::cout << "Test ir / 1q (35) is failed: must return a, returns: " << calc("ir", "1q", 35, "/") << "\n";
    failed++;
  }
  if (calc("81", "16", 21, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 81 / 16 (21) is failed: must return 6, returns: " << calc("81", "16", 21, "/") << "\n";
    failed++;
  }
  if (calc("12b", "d5", 22, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 12b / d5 (22) is failed: must return 1, returns: " << calc("12b", "d5", 22, "/") << "\n";
    failed++;
  }
  if (calc("4b", "f", 27, "+") == "4q") {
    passed++;
  } else {
    std::cout << "Test 4b + f (27) is failed: must return 4q, returns: " << calc("4b", "f", 27, "+") << "\n";
    failed++;
  }
  if (calc("730", "310", 11, "*") == "205300") {
    passed++;
  } else {
    std::cout << "Test 730 * 310 (11) is failed: must return 205300, returns: " << calc("730", "310", 11, "*") << "\n";
    failed++;
  }
  if (calc("1204", "253", 9, "*") == "315733") {
    passed++;
  } else {
    std::cout << "Test 1204 * 253 (9) is failed: must return 315733, returns: " << calc("1204", "253", 9, "*") << "\n";
    failed++;
  }
  if (calc("1100000010", "1011011110", 2, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1100000010 / 1011011110 (2) is failed: must return 1, returns: " << calc("1100000010", "1011011110", 2, "/") << "\n";
    failed++;
  }
  if (calc("1400", "552", 7, "+") == "2252") {
    passed++;
  } else {
    std::cout << "Test 1400 + 552 (7) is failed: must return 2252, returns: " << calc("1400", "552", 7, "+") << "\n";
    failed++;
  }
  if (calc("1000222", "22102", 3, "*") == "22201002121") {
    passed++;
  } else {
    std::cout << "Test 1000222 * 22102 (3) is failed: must return 22201002121, returns: " << calc("1000222", "22102", 3, "*") << "\n";
    failed++;
  }
  if (calc("kr", "ad", 29, "*") == "7fk3") {
    passed++;
  } else {
    std::cout << "Test kr * ad (29) is failed: must return 7fk3, returns: " << calc("kr", "ad", 29, "*") << "\n";
    failed++;
  }
  if (calc("1414", "412", 6, "*") == "1112212") {
    passed++;
  } else {
    std::cout << "Test 1414 * 412 (6) is failed: must return 1112212, returns: " << calc("1414", "412", 6, "*") << "\n";
    failed++;
  }
  if (calc("o", "m", 29, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test o / m (29) is failed: must return 1, returns: " << calc("o", "m", 29, "/") << "\n";
    failed++;
  }
  if (calc("14n", "10h", 26, "+") == "25e") {
    passed++;
  } else {
    std::cout << "Test 14n + 10h (26) is failed: must return 25e, returns: " << calc("14n", "10h", 26, "+") << "\n";
    failed++;
  }
  if (calc("326", "316", 11, "-") == "10") {
    passed++;
  } else {
    std::cout << "Test 326 - 316 (11) is failed: must return 10, returns: " << calc("326", "316", 11, "-") << "\n";
    failed++;
  }
  if (calc("oi", "2b", 31, "*") == "1qrc") {
    passed++;
  } else {
    std::cout << "Test oi * 2b (31) is failed: must return 1qrc, returns: " << calc("oi", "2b", 31, "*") << "\n";
    failed++;
  }
  if (calc("107", "dc", 28, "-") == "en") {
    passed++;
  } else {
    std::cout << "Test 107 - dc (28) is failed: must return en, returns: " << calc("107", "dc", 28, "-") << "\n";
    failed++;
  }
  if (calc("301", "202", 8, "*") == "61002") {
    passed++;
  } else {
    std::cout << "Test 301 * 202 (8) is failed: must return 61002, returns: " << calc("301", "202", 8, "*") << "\n";
    failed++;
  }
  if (calc("1563", "605", 7, "*") == "1415541") {
    passed++;
  } else {
    std::cout << "Test 1563 * 605 (7) is failed: must return 1415541, returns: " << calc("1563", "605", 7, "*") << "\n";
    failed++;
  }
  if (calc("j6", "bl", 26, "+") == "151") {
    passed++;
  } else {
    std::cout << "Test j6 + bl (26) is failed: must return 151, returns: " << calc("j6", "bl", 26, "+") << "\n";
    failed++;
  }
  if (calc("1d2", "2", 15, "+") == "1d4") {
    passed++;
  } else {
    std::cout << "Test 1d2 + 2 (15) is failed: must return 1d4, returns: " << calc("1d2", "2", 15, "+") << "\n";
    failed++;
  }
  if (calc("id", "bk", 24, "-") == "6h") {
    passed++;
  } else {
    std::cout << "Test id - bk (24) is failed: must return 6h, returns: " << calc("id", "bk", 24, "-") << "\n";
    failed++;
  }
  if (calc("dp", "6l", 27, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test dp / 6l (27) is failed: must return 2, returns: " << calc("dp", "6l", 27, "/") << "\n";
    failed++;
  }
  if (calc("1144", "525", 7, "+") == "2002") {
    passed++;
  } else {
    std::cout << "Test 1144 + 525 (7) is failed: must return 2002, returns: " << calc("1144", "525", 7, "+") << "\n";
    failed++;
  }
  if (calc("292", "d0", 18, "-") == "1e2") {
    passed++;
  } else {
    std::cout << "Test 292 - d0 (18) is failed: must return 1e2, returns: " << calc("292", "d0", 18, "-") << "\n";
    failed++;
  }
  if (calc("3051", "1000", 6, "-") == "2051") {
    passed++;
  } else {
    std::cout << "Test 3051 - 1000 (6) is failed: must return 2051, returns: " << calc("3051", "1000", 6, "-") << "\n";
    failed++;
  }
  if (calc("1543", "16", 7, "-") == "1524") {
    passed++;
  } else {
    std::cout << "Test 1543 - 16 (7) is failed: must return 1524, returns: " << calc("1543", "16", 7, "-") << "\n";
    failed++;
  }
  if (calc("164", "nr", 28, "*") == "1165o") {
    passed++;
  } else {
    std::cout << "Test 164 * nr (28) is failed: must return 1165o, returns: " << calc("164", "nr", 28, "*") << "\n";
    failed++;
  }
  if (calc("99", "98", 24, "+") == "ih") {
    passed++;
  } else {
    std::cout << "Test 99 + 98 (24) is failed: must return ih, returns: " << calc("99", "98", 24, "+") << "\n";
    failed++;
  }
  if (calc("32002", "22211", 4, "-") == "3131") {
    passed++;
  } else {
    std::cout << "Test 32002 - 22211 (4) is failed: must return 3131, returns: " << calc("32002", "22211", 4, "-") << "\n";
    failed++;
  }
  if (calc("1b3", "182", 19, "+") == "305") {
    passed++;
  } else {
    std::cout << "Test 1b3 + 182 (19) is failed: must return 305, returns: " << calc("1b3", "182", 19, "+") << "\n";
    failed++;
  }
  if (calc("336", "167", 9, "*") == "58676") {
    passed++;
  } else {
    std::cout << "Test 336 * 167 (9) is failed: must return 58676, returns: " << calc("336", "167", 9, "*") << "\n";
    failed++;
  }
  if (calc("1fb", "88", 16, "-") == "173") {
    passed++;
  } else {
    std::cout << "Test 1fb - 88 (16) is failed: must return 173, returns: " << calc("1fb", "88", 16, "-") << "\n";
    failed++;
  }
  if (calc("21100", "11200", 4, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 21100 / 11200 (4) is failed: must return 1, returns: " << calc("21100", "11200", 4, "/") << "\n";
    failed++;
  }
  if (calc("1ea", "1d5", 16, "+") == "3bf") {
    passed++;
  } else {
    std::cout << "Test 1ea + 1d5 (16) is failed: must return 3bf, returns: " << calc("1ea", "1d5", 16, "+") << "\n";
    failed++;
  }
  if (calc("ph", "3l", 26, "*") == "3jhj") {
    passed++;
  } else {
    std::cout << "Test ph * 3l (26) is failed: must return 3jhj, returns: " << calc("ph", "3l", 26, "*") << "\n";
    failed++;
  }
  if (calc("a7", "5q", 28, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test a7 / 5q (28) is failed: must return 1, returns: " << calc("a7", "5q", 28, "/") << "\n";
    failed++;
  }
  if (calc("7h", "1f", 32, "*") == "b1v") {
    passed++;
  } else {
    std::cout << "Test 7h * 1f (32) is failed: must return b1v, returns: " << calc("7h", "1f", 32, "*") << "\n";
    failed++;
  }
  if (calc("30221", "23130", 4, "-") == "1031") {
    passed++;
  } else {
    std::cout << "Test 30221 - 23130 (4) is failed: must return 1031, returns: " << calc("30221", "23130", 4, "-") << "\n";
    failed++;
  }
  if (calc("80", "d", 16, "*") == "680") {
    passed++;
  } else {
    std::cout << "Test 80 * d (16) is failed: must return 680, returns: " << calc("80", "d", 16, "*") << "\n";
    failed++;
  }
  if (calc("16", "k", 23, "*") == "125") {
    passed++;
  } else {
    std::cout << "Test 16 * k (23) is failed: must return 125, returns: " << calc("16", "k", 23, "*") << "\n";
    failed++;
  }
  if (calc("7h", "71", 20, "*") == "2f6h") {
    passed++;
  } else {
    std::cout << "Test 7h * 71 (20) is failed: must return 2f6h, returns: " << calc("7h", "71", 20, "*") << "\n";
    failed++;
  }
  if (calc("110000001", "101101000", 2, "+") == "1011101001") {
    passed++;
  } else {
    std::cout << "Test 110000001 + 101101000 (2) is failed: must return 1011101001, returns: " << calc("110000001", "101101000", 2, "+") << "\n";
    failed++;
  }
  if (calc("ed", "4g", 26, "*") == "2eo0") {
    passed++;
  } else {
    std::cout << "Test ed * 4g (26) is failed: must return 2eo0, returns: " << calc("ed", "4g", 26, "*") << "\n";
    failed++;
  }
  if (calc("1162", "516", 7, "-") == "343") {
    passed++;
  } else {
    std::cout << "Test 1162 - 516 (7) is failed: must return 343, returns: " << calc("1162", "516", 7, "-") << "\n";
    failed++;
  }
  if (calc("123", "10g", 26, "+") == "22j") {
    passed++;
  } else {
    std::cout << "Test 123 + 10g (26) is failed: must return 22j, returns: " << calc("123", "10g", 26, "+") << "\n";
    failed++;
  }
  if (calc("j5", "14", 27, "*") == "m0k") {
    passed++;
  } else {
    std::cout << "Test j5 * 14 (27) is failed: must return m0k, returns: " << calc("j5", "14", 27, "*") << "\n";
    failed++;
  }
  if (calc("2n", "28", 27, "+") == "54") {
    passed++;
  } else {
    std::cout << "Test 2n + 28 (27) is failed: must return 54, returns: " << calc("2n", "28", 27, "+") << "\n";
    failed++;
  }
  if (calc("h4", "6w", 34, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test h4 / 6w (34) is failed: must return 2, returns: " << calc("h4", "6w", 34, "/") << "\n";
    failed++;
  }
  if (calc("55a", "57", 11, "+") == "606") {
    passed++;
  } else {
    std::cout << "Test 55a + 57 (11) is failed: must return 606, returns: " << calc("55a", "57", 11, "+") << "\n";
    failed++;
  }
  if (calc("ak", "58", 27, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test ak / 58 (27) is failed: must return 2, returns: " << calc("ak", "58", 27, "/") << "\n";
    failed++;
  }
  if (calc("ki", "8h", 22, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test ki / 8h (22) is failed: must return 2, returns: " << calc("ki", "8h", 22, "/") << "\n";
    failed++;
  }
  if (calc("520", "442", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 520 / 442 (8) is failed: must return 1, returns: " << calc("520", "442", 8, "/") << "\n";
    failed++;
  }
  if (calc("3b7", "171", 13, "-") == "246") {
    passed++;
  } else {
    std::cout << "Test 3b7 - 171 (13) is failed: must return 246, returns: " << calc("3b7", "171", 13, "-") << "\n";
    failed++;
  }
  if (calc("1a6", "f", 19, "+") == "1b2") {
    passed++;
  } else {
    std::cout << "Test 1a6 + f (19) is failed: must return 1b2, returns: " << calc("1a6", "f", 19, "+") << "\n";
    failed++;
  }
  if (calc("ti", "mc", 32, "*") == "kleo") {
    passed++;
  } else {
    std::cout << "Test ti * mc (32) is failed: must return kleo, returns: " << calc("ti", "mc", 32, "*") << "\n";
    failed++;
  }
  if (calc("80", "75", 17, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 80 / 75 (17) is failed: must return 1, returns: " << calc("80", "75", 17, "/") << "\n";
    failed++;
  }
  if (calc("2432", "1212", 6, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 2432 / 1212 (6) is failed: must return 2, returns: " << calc("2432", "1212", 6, "/") << "\n";
    failed++;
  }
  if (calc("12140", "11320", 5, "*") == "144040300") {
    passed++;
  } else {
    std::cout << "Test 12140 * 11320 (5) is failed: must return 144040300, returns: " << calc("12140", "11320", 5, "*") << "\n";
    failed++;
  }
  if (calc("gj", "5n", 29, "*") == "39e2") {
    passed++;
  } else {
    std::cout << "Test gj * 5n (29) is failed: must return 39e2, returns: " << calc("gj", "5n", 29, "*") << "\n";
    failed++;
  }
  if (calc("122101", "112012", 3, "+") == "1011120") {
    passed++;
  } else {
    std::cout << "Test 122101 + 112012 (3) is failed: must return 1011120, returns: " << calc("122101", "112012", 3, "+") << "\n";
    failed++;
  }
  if (calc("110010101", "100010010", 2, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 110010101 / 100010010 (2) is failed: must return 1, returns: " << calc("110010101", "100010010", 2, "/") << "\n";
    failed++;
  }
  if (calc("1d2", "88", 15, "*") == "11011") {
    passed++;
  } else {
    std::cout << "Test 1d2 * 88 (15) is failed: must return 11011, returns: " << calc("1d2", "88", 15, "*") << "\n";
    failed++;
  }
  if (calc("117", "1p", 28, "-") == "ra") {
    passed++;
  } else {
    std::cout << "Test 117 - 1p (28) is failed: must return ra, returns: " << calc("117", "1p", 28, "-") << "\n";
    failed++;
  }
  if (calc("2400", "104", 7, "/") == "22") {
    passed++;
  } else {
    std::cout << "Test 2400 / 104 (7) is failed: must return 22, returns: " << calc("2400", "104", 7, "/") << "\n";
    failed++;
  }
  if (calc("pw", "nj", 33, "+") == "1gi") {
    passed++;
  } else {
    std::cout << "Test pw + nj (33) is failed: must return 1gi, returns: " << calc("pw", "nj", 33, "+") << "\n";
    failed++;
  }
  if (calc("93", "91", 10, "*") == "8463") {
    passed++;
  } else {
    std::cout << "Test 93 * 91 (10) is failed: must return 8463, returns: " << calc("93", "91", 10, "*") << "\n";
    failed++;
  }
  if (calc("86", "50", 11, "*") == "3980") {
    passed++;
  } else {
    std::cout << "Test 86 * 50 (11) is failed: must return 3980, returns: " << calc("86", "50", 11, "*") << "\n";
    failed++;
  }
  if (calc("4252", "512", 6, "*") == "3522304") {
    passed++;
  } else {
    std::cout << "Test 4252 * 512 (6) is failed: must return 3522304, returns: " << calc("4252", "512", 6, "*") << "\n";
    failed++;
  }
  if (calc("971", "423", 10, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 971 / 423 (10) is failed: must return 2, returns: " << calc("971", "423", 10, "/") << "\n";
    failed++;
  }
  if (calc("23c", "59", 16, "+") == "295") {
    passed++;
  } else {
    std::cout << "Test 23c + 59 (16) is failed: must return 295, returns: " << calc("23c", "59", 16, "+") << "\n";
    failed++;
  }
  if (calc("32031", "21000", 4, "*") == "1333311000") {
    passed++;
  } else {
    std::cout << "Test 32031 * 21000 (4) is failed: must return 1333311000, returns: " << calc("32031", "21000", 4, "*") << "\n";
    failed++;
  }
  if (calc("13p", "11o", 27, "-") == "21") {
    passed++;
  } else {
    std::cout << "Test 13p - 11o (27) is failed: must return 21, returns: " << calc("13p", "11o", 27, "-") << "\n";
    failed++;
  }
  if (calc("1d2", "19d", 17, "+") == "35f") {
    passed++;
  } else {
    std::cout << "Test 1d2 + 19d (17) is failed: must return 35f, returns: " << calc("1d2", "19d", 17, "+") << "\n";
    failed++;
  }
  if (calc("72", "13", 15, "-") == "5e") {
    passed++;
  } else {
    std::cout << "Test 72 - 13 (15) is failed: must return 5e, returns: " << calc("72", "13", 15, "-") << "\n";
    failed++;
  }
  if (calc("782", "465", 11, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 782 / 465 (11) is failed: must return 1, returns: " << calc("782", "465", 11, "/") << "\n";
    failed++;
  }
  if (calc("696", "24a", 12, "+") == "924") {
    passed++;
  } else {
    std::cout << "Test 696 + 24a (12) is failed: must return 924, returns: " << calc("696", "24a", 12, "+") << "\n";
    failed++;
  }
  if (calc("l0", "3l", 22, "+") == "12l") {
    passed++;
  } else {
    std::cout << "Test l0 + 3l (22) is failed: must return 12l, returns: " << calc("l0", "3l", 22, "+") << "\n";
    failed++;
  }
  if (calc("c8", "i", 28, "*") == "7p4") {
    passed++;
  } else {
    std::cout << "Test c8 * i (28) is failed: must return 7p4, returns: " << calc("c8", "i", 28, "*") << "\n";
    failed++;
  }
  if (calc("495", "366", 11, "-") == "12a") {
    passed++;
  } else {
    std::cout << "Test 495 - 366 (11) is failed: must return 12a, returns: " << calc("495", "366", 11, "-") << "\n";
    failed++;
  }
  if (calc("9o", "3h", 29, "+") == "dc") {
    passed++;
  } else {
    std::cout << "Test 9o + 3h (29) is failed: must return dc, returns: " << calc("9o", "3h", 29, "+") << "\n";
    failed++;
  }
  if (calc("166", "106", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 166 / 106 (13) is failed: must return 1, returns: " << calc("166", "106", 13, "/") << "\n";
    failed++;
  }
  if (calc("1i5", "86", 20, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 1i5 / 86 (20) is failed: must return 4, returns: " << calc("1i5", "86", 20, "/") << "\n";
    failed++;
  }
  if (calc("31", "22", 6, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 31 / 22 (6) is failed: must return 1, returns: " << calc("31", "22", 6, "/") << "\n";
    failed++;
  }
  if (calc("455", "a2", 12, "-") == "373") {
    passed++;
  } else {
    std::cout << "Test 455 - a2 (12) is failed: must return 373, returns: " << calc("455", "a2", 12, "-") << "\n";
    failed++;
  }
  if (calc("a9", "41", 33, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test a9 / 41 (33) is failed: must return 2, returns: " << calc("a9", "41", 33, "/") << "\n";
    failed++;
  }
  if (calc("1565", "1006", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1565 / 1006 (7) is failed: must return 1, returns: " << calc("1565", "1006", 7, "/") << "\n";
    failed++;
  }
  if (calc("6k", "34", 35, "+") == "9o") {
    passed++;
  } else {
    std::cout << "Test 6k + 34 (35) is failed: must return 9o, returns: " << calc("6k", "34", 35, "+") << "\n";
    failed++;
  }
  if (calc("7i", "4v", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 7i / 4v (33) is failed: must return 1, returns: " << calc("7i", "4v", 33, "/") << "\n";
    failed++;
  }
  if (calc("2322", "31", 5, "/") == "41") {
    passed++;
  } else {
    std::cout << "Test 2322 / 31 (5) is failed: must return 41, returns: " << calc("2322", "31", 5, "/") << "\n";
    failed++;
  }
  if (calc("10121", "3111", 4, "-") == "1010") {
    passed++;
  } else {
    std::cout << "Test 10121 - 3111 (4) is failed: must return 1010, returns: " << calc("10121", "3111", 4, "-") << "\n";
    failed++;
  }
  if (calc("3112", "1112", 5, "*") == "4021044") {
    passed++;
  } else {
    std::cout << "Test 3112 * 1112 (5) is failed: must return 4021044, returns: " << calc("3112", "1112", 5, "*") << "\n";
    failed++;
  }
  if (calc("gr", "3s", 35, "-") == "cy") {
    passed++;
  } else {
    std::cout << "Test gr - 3s (35) is failed: must return cy, returns: " << calc("gr", "3s", 35, "-") << "\n";
    failed++;
  }
  if (calc("39", "9", 23, "-") == "30") {
    passed++;
  } else {
    std::cout << "Test 39 - 9 (23) is failed: must return 30, returns: " << calc("39", "9", 23, "-") << "\n";
    failed++;
  }
  if (calc("277", "89", 18, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test 277 / 89 (18) is failed: must return 5, returns: " << calc("277", "89", 18, "/") << "\n";
    failed++;
  }
  if (calc("6y", "4b", 35, "*") == "u2o") {
    passed++;
  } else {
    std::cout << "Test 6y * 4b (35) is failed: must return u2o, returns: " << calc("6y", "4b", 35, "*") << "\n";
    failed++;
  }
  if (calc("2fb", "294", 16, "+") == "58f") {
    passed++;
  } else {
    std::cout << "Test 2fb + 294 (16) is failed: must return 58f, returns: " << calc("2fb", "294", 16, "+") << "\n";
    failed++;
  }
  if (calc("ak", "6g", 26, "*") == "2j68") {
    passed++;
  } else {
    std::cout << "Test ak * 6g (26) is failed: must return 2j68, returns: " << calc("ak", "6g", 26, "*") << "\n";
    failed++;
  }
  if (calc("1d6", "d7", 14, "+") == "2cd") {
    passed++;
  } else {
    std::cout << "Test 1d6 + d7 (14) is failed: must return 2cd, returns: " << calc("1d6", "d7", 14, "+") << "\n";
    failed++;
  }
  if (calc("101101010", "11111100", 2, "-") == "1101110") {
    passed++;
  } else {
    std::cout << "Test 101101010 - 11111100 (2) is failed: must return 1101110, returns: " << calc("101101010", "11111100", 2, "-") << "\n";
    failed++;
  }
  if (calc("432", "134", 5, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 432 / 134 (5) is failed: must return 2, returns: " << calc("432", "134", 5, "/") << "\n";
    failed++;
  }
  if (calc("36d", "1cb", 16, "*") == "6246f") {
    passed++;
  } else {
    std::cout << "Test 36d * 1cb (16) is failed: must return 6246f, returns: " << calc("36d", "1cb", 16, "*") << "\n";
    failed++;
  }
  if (calc("2bb", "213", 17, "*") == "597bg") {
    passed++;
  } else {
    std::cout << "Test 2bb * 213 (17) is failed: must return 597bg, returns: " << calc("2bb", "213", 17, "*") << "\n";
    failed++;
  }
  if (calc("3301", "1044", 6, "*") == "3543044") {
    passed++;
  } else {
    std::cout << "Test 3301 * 1044 (6) is failed: must return 3543044, returns: " << calc("3301", "1044", 6, "*") << "\n";
    failed++;
  }
  if (calc("c0", "1g", 24, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test c0 / 1g (24) is failed: must return 7, returns: " << calc("c0", "1g", 24, "/") << "\n";
    failed++;
  }
  if (calc("323", "300", 13, "*") == "96900") {
    passed++;
  } else {
    std::cout << "Test 323 * 300 (13) is failed: must return 96900, returns: " << calc("323", "300", 13, "*") << "\n";
    failed++;
  }
  if (calc("64", "41", 26, "+") == "a5") {
    passed++;
  } else {
    std::cout << "Test 64 + 41 (26) is failed: must return a5, returns: " << calc("64", "41", 26, "+") << "\n";
    failed++;
  }
  if (calc("69", "56", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 69 / 56 (13) is failed: must return 1, returns: " << calc("69", "56", 13, "/") << "\n";
    failed++;
  }
  if (calc("14c", "b7", 25, "*") == "d7d9") {
    passed++;
  } else {
    std::cout << "Test 14c * b7 (25) is failed: must return d7d9, returns: " << calc("14c", "b7", 25, "*") << "\n";
    failed++;
  }
  if (calc("119", "kp", 28, "*") == "loh1") {
    passed++;
  } else {
    std::cout << "Test 119 * kp (28) is failed: must return loh1, returns: " << calc("119", "kp", 28, "*") << "\n";
    failed++;
  }
  if (calc("c0", "7p", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test c0 / 7p (35) is failed: must return 1, returns: " << calc("c0", "7p", 35, "/") << "\n";
    failed++;
  }
  if (calc("3e1", "2e6", 16, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 3e1 / 2e6 (16) is failed: must return 1, returns: " << calc("3e1", "2e6", 16, "/") << "\n";
    failed++;
  }
  if (calc("89", "45", 17, "*") == "22ab") {
    passed++;
  } else {
    std::cout << "Test 89 * 45 (17) is failed: must return 22ab, returns: " << calc("89", "45", 17, "*") << "\n";
    failed++;
  }
  if (calc("2104", "535", 6, "+") == "3043") {
    passed++;
  } else {
    std::cout << "Test 2104 + 535 (6) is failed: must return 3043, returns: " << calc("2104", "535", 6, "+") << "\n";
    failed++;
  }
  if (calc("j4", "co", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test j4 / co (30) is failed: must return 1, returns: " << calc("j4", "co", 30, "/") << "\n";
    failed++;
  }
  if (calc("7b", "1d", 29, "+") == "8o") {
    passed++;
  } else {
    std::cout << "Test 7b + 1d (29) is failed: must return 8o, returns: " << calc("7b", "1d", 29, "+") << "\n";
    failed++;
  }
  if (calc("850", "747", 9, "*") == "713780") {
    passed++;
  } else {
    std::cout << "Test 850 * 747 (9) is failed: must return 713780, returns: " << calc("850", "747", 9, "*") << "\n";
    failed++;
  }
  if (calc("18d", "n2", 24, "+") == "27f") {
    passed++;
  } else {
    std::cout << "Test 18d + n2 (24) is failed: must return 27f, returns: " << calc("18d", "n2", 24, "+") << "\n";
    failed++;
  }
  if (calc("316", "89", 13, "-") == "25a") {
    passed++;
  } else {
    std::cout << "Test 316 - 89 (13) is failed: must return 25a, returns: " << calc("316", "89", 13, "-") << "\n";
    failed++;
  }
  if (calc("396", "1ba", 13, "*") == "71448") {
    passed++;
  } else {
    std::cout << "Test 396 * 1ba (13) is failed: must return 71448, returns: " << calc("396", "1ba", 13, "*") << "\n";
    failed++;
  }
  if (calc("70", "4c", 25, "*") == "1690") {
    passed++;
  } else {
    std::cout << "Test 70 * 4c (25) is failed: must return 1690, returns: " << calc("70", "4c", 25, "*") << "\n";
    failed++;
  }
  if (calc("22f", "1fa", 16, "+") == "429") {
    passed++;
  } else {
    std::cout << "Test 22f + 1fa (16) is failed: must return 429, returns: " << calc("22f", "1fa", 16, "+") << "\n";
    failed++;
  }
  if (calc("1010110010", "1001111", 2, "-") == "1001100011") {
    passed++;
  } else {
    std::cout << "Test 1010110010 - 1001111 (2) is failed: must return 1001100011, returns: " << calc("1010110010", "1001111", 2, "-") << "\n";
    failed++;
  }
  if (calc("dp", "12", 26, "/") == "c") {
    passed++;
  } else {
    std::cout << "Test dp / 12 (26) is failed: must return c, returns: " << calc("dp", "12", 26, "/") << "\n";
    failed++;
  }
  if (calc("1di", "l1", 24, "+") == "2aj") {
    passed++;
  } else {
    std::cout << "Test 1di + l1 (24) is failed: must return 2aj, returns: " << calc("1di", "l1", 24, "+") << "\n";
    failed++;
  }
  if (calc("4p", "4k", 28, "*") == "n1o") {
    passed++;
  } else {
    std::cout << "Test 4p * 4k (28) is failed: must return n1o, returns: " << calc("4p", "4k", 28, "*") << "\n";
    failed++;
  }
  if (calc("6c", "18", 21, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 6c / 18 (21) is failed: must return 4, returns: " << calc("6c", "18", 21, "/") << "\n";
    failed++;
  }
  if (calc("47", "34", 8, "-") == "13") {
    passed++;
  } else {
    std::cout << "Test 47 - 34 (8) is failed: must return 13, returns: " << calc("47", "34", 8, "-") << "\n";
    failed++;
  }
  if (calc("kj", "79", 32, "-") == "da") {
    passed++;
  } else {
    std::cout << "Test kj - 79 (32) is failed: must return da, returns: " << calc("kj", "79", 32, "-") << "\n";
    failed++;
  }
  if (calc("451", "251", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 451 / 251 (8) is failed: must return 1, returns: " << calc("451", "251", 8, "/") << "\n";
    failed++;
  }
  if (calc("o2", "ed", 26, "*") == "db30") {
    passed++;
  } else {
    std::cout << "Test o2 * ed (26) is failed: must return db30, returns: " << calc("o2", "ed", 26, "*") << "\n";
    failed++;
  }
  if (calc("6i", "j", 24, "+") == "7d") {
    passed++;
  } else {
    std::cout << "Test 6i + j (24) is failed: must return 7d, returns: " << calc("6i", "j", 24, "+") << "\n";
    failed++;
  }
  if (calc("308", "178", 10, "*") == "54824") {
    passed++;
  } else {
    std::cout << "Test 308 * 178 (10) is failed: must return 54824, returns: " << calc("308", "178", 10, "*") << "\n";
    failed++;
  }
  if (calc("509", "212", 14, "-") == "2d7") {
    passed++;
  } else {
    std::cout << "Test 509 - 212 (14) is failed: must return 2d7, returns: " << calc("509", "212", 14, "-") << "\n";
    failed++;
  }
  if (calc("23k", "13c", 21, "*") == "2bg29") {
    passed++;
  } else {
    std::cout << "Test 23k * 13c (21) is failed: must return 2bg29, returns: " << calc("23k", "13c", 21, "*") << "\n";
    failed++;
  }
  if (calc("852", "222", 10, "*") == "189144") {
    passed++;
  } else {
    std::cout << "Test 852 * 222 (10) is failed: must return 189144, returns: " << calc("852", "222", 10, "*") << "\n";
    failed++;
  }
  if (calc("92", "8r", 31, "-") == "6") {
    passed++;
  } else {
    std::cout << "Test 92 - 8r (31) is failed: must return 6, returns: " << calc("92", "8r", 31, "-") << "\n";
    failed++;
  }
  if (calc("529", "266", 12, "*") == "1135a6") {
    passed++;
  } else {
    std::cout << "Test 529 * 266 (12) is failed: must return 1135a6, returns: " << calc("529", "266", 12, "*") << "\n";
    failed++;
  }
  if (calc("1fg", "14d", 17, "-") == "b3") {
    passed++;
  } else {
    std::cout << "Test 1fg - 14d (17) is failed: must return b3, returns: " << calc("1fg", "14d", 17, "-") << "\n";
    failed++;
  }
  if (calc("670", "158", 10, "-") == "512") {
    passed++;
  } else {
    std::cout << "Test 670 - 158 (10) is failed: must return 512, returns: " << calc("670", "158", 10, "-") << "\n";
    failed++;
  }
  if (calc("6d", "4a", 33, "*") == "rgv") {
    passed++;
  } else {
    std::cout << "Test 6d * 4a (33) is failed: must return rgv, returns: " << calc("6d", "4a", 33, "*") << "\n";
    failed++;
  }
  if (calc("1331", "1201", 4, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1331 / 1201 (4) is failed: must return 1, returns: " << calc("1331", "1201", 4, "/") << "\n";
    failed++;
  }
  if (calc("gl", "8x", 35, "*") == "48fs") {
    passed++;
  } else {
    std::cout << "Test gl * 8x (35) is failed: must return 48fs, returns: " << calc("gl", "8x", 35, "*") << "\n";
    failed++;
  }
  if (calc("hk", "9", 21, "/") == "1k") {
    passed++;
  } else {
    std::cout << "Test hk / 9 (21) is failed: must return 1k, returns: " << calc("hk", "9", 21, "/") << "\n";
    failed++;
  }
  if (calc("352", "2a1", 11, "+") == "643") {
    passed++;
  } else {
    std::cout << "Test 352 + 2a1 (11) is failed: must return 643, returns: " << calc("352", "2a1", 11, "+") << "\n";
    failed++;
  }
  if (calc("mr", "3i", 28, "-") == "j9") {
    passed++;
  } else {
    std::cout << "Test mr - 3i (28) is failed: must return j9, returns: " << calc("mr", "3i", 28, "-") << "\n";
    failed++;
  }
  if (calc("577", "576", 9, "-") == "1") {
    passed++;
  } else {
    std::cout << "Test 577 - 576 (9) is failed: must return 1, returns: " << calc("577", "576", 9, "-") << "\n";
    failed++;
  }
  if (calc("1ca", "1c3", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1ca / 1c3 (24) is failed: must return 1, returns: " << calc("1ca", "1c3", 24, "/") << "\n";
    failed++;
  }
  if (calc("5a5", "96", 12, "*") == "478b6") {
    passed++;
  } else {
    std::cout << "Test 5a5 * 96 (12) is failed: must return 478b6, returns: " << calc("5a5", "96", 12, "*") << "\n";
    failed++;
  }
  if (calc("1450", "1076", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1450 / 1076 (8) is failed: must return 1, returns: " << calc("1450", "1076", 8, "/") << "\n";
    failed++;
  }
  if (calc("13b", "117", 13, "-") == "24") {
    passed++;
  } else {
    std::cout << "Test 13b - 117 (13) is failed: must return 24, returns: " << calc("13b", "117", 13, "-") << "\n";
    failed++;
  }
  if (calc("2433", "153", 6, "+") == "3030") {
    passed++;
  } else {
    std::cout << "Test 2433 + 153 (6) is failed: must return 3030, returns: " << calc("2433", "153", 6, "+") << "\n";
    failed++;
  }
  if (calc("300", "10", 8, "/") == "30") {
    passed++;
  } else {
    std::cout << "Test 300 / 10 (8) is failed: must return 30, returns: " << calc("300", "10", 8, "/") << "\n";
    failed++;
  }
  if (calc("397", "99", 10, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 397 / 99 (10) is failed: must return 4, returns: " << calc("397", "99", 10, "/") << "\n";
    failed++;
  }
  if (calc("332", "123", 5, "-") == "204") {
    passed++;
  } else {
    std::cout << "Test 332 - 123 (5) is failed: must return 204, returns: " << calc("332", "123", 5, "-") << "\n";
    failed++;
  }
  if (calc("1jj", "a0", 20, "*") == "jja0") {
    passed++;
  } else {
    std::cout << "Test 1jj * a0 (20) is failed: must return jja0, returns: " << calc("1jj", "a0", 20, "*") << "\n";
    failed++;
  }
  if (calc("1602", "124", 7, "+") == "2026") {
    passed++;
  } else {
    std::cout << "Test 1602 + 124 (7) is failed: must return 2026, returns: " << calc("1602", "124", 7, "+") << "\n";
    failed++;
  }
  if (calc("1235", "1050", 8, "*") == "1321210") {
    passed++;
  } else {
    std::cout << "Test 1235 * 1050 (8) is failed: must return 1321210, returns: " << calc("1235", "1050", 8, "*") << "\n";
    failed++;
  }
  if (calc("8j", "6j", 32, "-") == "20") {
    passed++;
  } else {
    std::cout << "Test 8j - 6j (32) is failed: must return 20, returns: " << calc("8j", "6j", 32, "-") << "\n";
    failed++;
  }
  if (calc("603", "114", 9, "*") == "70043") {
    passed++;
  } else {
    std::cout << "Test 603 * 114 (9) is failed: must return 70043, returns: " << calc("603", "114", 9, "*") << "\n";
    failed++;
  }
  if (calc("1gd", "1eb", 24, "-") == "22") {
    passed++;
  } else {
    std::cout << "Test 1gd - 1eb (24) is failed: must return 22, returns: " << calc("1gd", "1eb", 24, "-") << "\n";
    failed++;
  }
  if (calc("12243", "2431", 5, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 12243 / 2431 (5) is failed: must return 2, returns: " << calc("12243", "2431", 5, "/") << "\n";
    failed++;
  }
  if (calc("125", "106", 9, "*") == "13363") {
    passed++;
  } else {
    std::cout << "Test 125 * 106 (9) is failed: must return 13363, returns: " << calc("125", "106", 9, "*") << "\n";
    failed++;
  }
  if (calc("1di", "kl", 22, "+") == "2ch") {
    passed++;
  } else {
    std::cout << "Test 1di + kl (22) is failed: must return 2ch, returns: " << calc("1di", "kl", 22, "+") << "\n";
    failed++;
  }
  if (calc("fr", "8s", 30, "+") == "op") {
    passed++;
  } else {
    std::cout << "Test fr + 8s (30) is failed: must return op, returns: " << calc("fr", "8s", 30, "+") << "\n";
    failed++;
  }
  if (calc("803", "210", 9, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 803 / 210 (9) is failed: must return 3, returns: " << calc("803", "210", 9, "/") << "\n";
    failed++;
  }
  if (calc("1226", "712", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1226 / 712 (8) is failed: must return 1, returns: " << calc("1226", "712", 8, "/") << "\n";
    failed++;
  }
  if (calc("1g3", "a2", 18, "+") == "285") {
    passed++;
  } else {
    std::cout << "Test 1g3 + a2 (18) is failed: must return 285, returns: " << calc("1g3", "a2", 18, "+") << "\n";
    failed++;
  }
  if (calc("49", "3g", 32, "*") == "evg") {
    passed++;
  } else {
    std::cout << "Test 49 * 3g (32) is failed: must return evg, returns: " << calc("49", "3g", 32, "*") << "\n";
    failed++;
  }
  if (calc("14i", "3j", 24, "*") == "4d06") {
    passed++;
  } else {
    std::cout << "Test 14i * 3j (24) is failed: must return 4d06, returns: " << calc("14i", "3j", 24, "*") << "\n";
    failed++;
  }
  if (calc("172", "136", 12, "-") == "38") {
    passed++;
  } else {
    std::cout << "Test 172 - 136 (12) is failed: must return 38, returns: " << calc("172", "136", 12, "-") << "\n";
    failed++;
  }
  if (calc("32", "7", 24, "+") == "39") {
    passed++;
  } else {
    std::cout << "Test 32 + 7 (24) is failed: must return 39, returns: " << calc("32", "7", 24, "+") << "\n";
    failed++;
  }
  if (calc("3202", "1113", 6, "*") == "4012230") {
    passed++;
  } else {
    std::cout << "Test 3202 * 1113 (6) is failed: must return 4012230, returns: " << calc("3202", "1113", 6, "*") << "\n";
    failed++;
  }
  if (calc("86", "30", 20, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 86 / 30 (20) is failed: must return 2, returns: " << calc("86", "30", 20, "/") << "\n";
    failed++;
  }
  if (calc("532", "315", 7, "+") == "1150") {
    passed++;
  } else {
    std::cout << "Test 532 + 315 (7) is failed: must return 1150, returns: " << calc("532", "315", 7, "+") << "\n";
    failed++;
  }
  if (calc("123", "41", 13, "+") == "164") {
    passed++;
  } else {
    std::cout << "Test 123 + 41 (13) is failed: must return 164, returns: " << calc("123", "41", 13, "+") << "\n";
    failed++;
  }
  if (calc("1410", "436", 8, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1410 / 436 (8) is failed: must return 2, returns: " << calc("1410", "436", 8, "/") << "\n";
    failed++;
  }
  if (calc("17k", "12b", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 17k / 12b (24) is failed: must return 1, returns: " << calc("17k", "12b", 24, "/") << "\n";
    failed++;
  }
  if (calc("2b7", "28c", 16, "-") == "2b") {
    passed++;
  } else {
    std::cout << "Test 2b7 - 28c (16) is failed: must return 2b, returns: " << calc("2b7", "28c", 16, "-") << "\n";
    failed++;
  }
  if (calc("122222", "22212", 3, "*") == "12220000011") {
    passed++;
  } else {
    std::cout << "Test 122222 * 22212 (3) is failed: must return 12220000011, returns: " << calc("122222", "22212", 3, "*") << "\n";
    failed++;
  }
  if (calc("ba", "74", 15, "*") == "59ba") {
    passed++;
  } else {
    std::cout << "Test ba * 74 (15) is failed: must return 59ba, returns: " << calc("ba", "74", 15, "*") << "\n";
    failed++;
  }
  if (calc("43", "37", 29, "*") == "d8l") {
    passed++;
  } else {
    std::cout << "Test 43 * 37 (29) is failed: must return d8l, returns: " << calc("43", "37", 29, "*") << "\n";
    failed++;
  }
  if (calc("7", "3", 9, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 7 / 3 (9) is failed: must return 2, returns: " << calc("7", "3", 9, "/") << "\n";
    failed++;
  }
  if (calc("24i", "af", 20, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 24i / af (20) is failed: must return 4, returns: " << calc("24i", "af", 20, "/") << "\n";
    failed++;
  }
  if (calc("10r", "10d", 31, "-") == "e") {
    passed++;
  } else {
    std::cout << "Test 10r - 10d (31) is failed: must return e, returns: " << calc("10r", "10d", 31, "-") << "\n";
    failed++;
  }
  if (calc("k6", "hn", 30, "-") == "2d") {
    passed++;
  } else {
    std::cout << "Test k6 - hn (30) is failed: must return 2d, returns: " << calc("k6", "hn", 30, "-") << "\n";
    failed++;
  }
  if (calc("393", "208", 10, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 393 / 208 (10) is failed: must return 1, returns: " << calc("393", "208", 10, "/") << "\n";
    failed++;
  }
  if (calc("12021", "2312", 5, "*") == "33403102") {
    passed++;
  } else {
    std::cout << "Test 12021 * 2312 (5) is failed: must return 33403102, returns: " << calc("12021", "2312", 5, "*") << "\n";
    failed++;
  }
  if (calc("1434", "1201", 7, "+") == "2635") {
    passed++;
  } else {
    std::cout << "Test 1434 + 1201 (7) is failed: must return 2635, returns: " << calc("1434", "1201", 7, "+") << "\n";
    failed++;
  }
  if (calc("1022212", "11110", 3, "/") == "22") {
    passed++;
  } else {
    std::cout << "Test 1022212 / 11110 (3) is failed: must return 22, returns: " << calc("1022212", "11110", 3, "/") << "\n";
    failed++;
  }
  if (calc("295", "1ie", 19, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 295 / 1ie (19) is failed: must return 1, returns: " << calc("295", "1ie", 19, "/") << "\n";
    failed++;
  }
  if (calc("nv", "4v", 35, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test nv / 4v (35) is failed: must return 4, returns: " << calc("nv", "4v", 35, "/") << "\n";
    failed++;
  }
  if (calc("2g", "1h", 32, "-") == "v") {
    passed++;
  } else {
    std::cout << "Test 2g - 1h (32) is failed: must return v, returns: " << calc("2g", "1h", 32, "-") << "\n";
    failed++;
  }
  if (calc("684", "242", 9, "-") == "442") {
    passed++;
  } else {
    std::cout << "Test 684 - 242 (9) is failed: must return 442, returns: " << calc("684", "242", 9, "-") << "\n";
    failed++;
  }
  if (calc("12k", "g7", 21, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 12k / g7 (21) is failed: must return 1, returns: " << calc("12k", "g7", 21, "/") << "\n";
    failed++;
  }
  if (calc("1100100101", "1111111", 2, "*") == "11000111101011011") {
    passed++;
  } else {
    std::cout << "Test 1100100101 * 1111111 (2) is failed: must return 11000111101011011, returns: " << calc("1100100101", "1111111", 2, "*") << "\n";
    failed++;
  }
  if (calc("2515", "1212", 7, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 2515 / 1212 (7) is failed: must return 2, returns: " << calc("2515", "1212", 7, "/") << "\n";
    failed++;
  }
  if (calc("69", "i", 25, "*") == "4ec") {
    passed++;
  } else {
    std::cout << "Test 69 * i (25) is failed: must return 4ec, returns: " << calc("69", "i", 25, "*") << "\n";
    failed++;
  }
  if (calc("13", "k", 23, "-") == "6") {
    passed++;
  } else {
    std::cout << "Test 13 - k (23) is failed: must return 6, returns: " << calc("13", "k", 23, "-") << "\n";
    failed++;
  }
  if (calc("1il", "10a", 22, "+") == "2j9") {
    passed++;
  } else {
    std::cout << "Test 1il + 10a (22) is failed: must return 2j9, returns: " << calc("1il", "10a", 22, "+") << "\n";
    failed++;
  }
  if (calc("gb", "e3", 17, "*") == "degg") {
    passed++;
  } else {
    std::cout << "Test gb * e3 (17) is failed: must return degg, returns: " << calc("gb", "e3", 17, "*") << "\n";
    failed++;
  }
  if (calc("1t", "w", 35, "+") == "2q") {
    passed++;
  } else {
    std::cout << "Test 1t + w (35) is failed: must return 2q, returns: " << calc("1t", "w", 35, "+") << "\n";
    failed++;
  }
  if (calc("e7", "c7", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test e7 / c7 (20) is failed: must return 1, returns: " << calc("e7", "c7", 20, "/") << "\n";
    failed++;
  }
  if (calc("16c", "13b", 16, "*") == "1bfe4") {
    passed++;
  } else {
    std::cout << "Test 16c * 13b (16) is failed: must return 1bfe4, returns: " << calc("16c", "13b", 16, "*") << "\n";
    failed++;
  }
  if (calc("5b", "40", 20, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 5b / 40 (20) is failed: must return 1, returns: " << calc("5b", "40", 20, "/") << "\n";
    failed++;
  }
  if (calc("103", "ah", 20, "-") == "96") {
    passed++;
  } else {
    std::cout << "Test 103 - ah (20) is failed: must return 96, returns: " << calc("103", "ah", 20, "-") << "\n";
    failed++;
  }
  if (calc("2314", "24", 6, "*") == "104424") {
    passed++;
  } else {
    std::cout << "Test 2314 * 24 (6) is failed: must return 104424, returns: " << calc("2314", "24", 6, "*") << "\n";
    failed++;
  }
  if (calc("1256", "1075", 9, "*") == "1363773") {
    passed++;
  } else {
    std::cout << "Test 1256 * 1075 (9) is failed: must return 1363773, returns: " << calc("1256", "1075", 9, "*") << "\n";
    failed++;
  }
  if (calc("57", "3a", 28, "*") == "hhe") {
    passed++;
  } else {
    std::cout << "Test 57 * 3a (28) is failed: must return hhe, returns: " << calc("57", "3a", 28, "*") << "\n";
    failed++;
  }
  if (calc("1ib", "3c", 20, "-") == "1ej") {
    passed++;
  } else {
    std::cout << "Test 1ib - 3c (20) is failed: must return 1ej, returns: " << calc("1ib", "3c", 20, "-") << "\n";
    failed++;
  }
  if (calc("2n", "1k", 29, "-") == "13") {
    passed++;
  } else {
    std::cout << "Test 2n - 1k (29) is failed: must return 13, returns: " << calc("2n", "1k", 29, "-") << "\n";
    failed++;
  }
  if (calc("12431", "1041", 5, "/") == "11") {
    passed++;
  } else {
    std::cout << "Test 12431 / 1041 (5) is failed: must return 11, returns: " << calc("12431", "1041", 5, "/") << "\n";
    failed++;
  }
  if (calc("q2", "1y", 35, "*") == "1gcx") {
    passed++;
  } else {
    std::cout << "Test q2 * 1y (35) is failed: must return 1gcx, returns: " << calc("q2", "1y", 35, "*") << "\n";
    failed++;
  }
  if (calc("711", "161", 8, "+") == "1072") {
    passed++;
  } else {
    std::cout << "Test 711 + 161 (8) is failed: must return 1072, returns: " << calc("711", "161", 8, "+") << "\n";
    failed++;
  }
  if (calc("26", "j", 24, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 26 / j (24) is failed: must return 2, returns: " << calc("26", "j", 24, "/") << "\n";
    failed++;
  }
  if (calc("141", "4i", 24, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test 141 / 4i (24) is failed: must return 5, returns: " << calc("141", "4i", 24, "/") << "\n";
    failed++;
  }
  if (calc("632", "403", 8, "-") == "227") {
    passed++;
  } else {
    std::cout << "Test 632 - 403 (8) is failed: must return 227, returns: " << calc("632", "403", 8, "-") << "\n";
    failed++;
  }
  if (calc("3a2", "156", 13, "-") == "249") {
    passed++;
  } else {
    std::cout << "Test 3a2 - 156 (13) is failed: must return 249, returns: " << calc("3a2", "156", 13, "-") << "\n";
    failed++;
  }
  if (calc("1223", "23", 7, "/") == "35") {
    passed++;
  } else {
    std::cout << "Test 1223 / 23 (7) is failed: must return 35, returns: " << calc("1223", "23", 7, "/") << "\n";
    failed++;
  }
  if (calc("1010101010", "101001111", 2, "*") == "110111110001110110") {
    passed++;
  } else {
    std::cout << "Test 1010101010 * 101001111 (2) is failed: must return 110111110001110110, returns: " << calc("1010101010", "101001111", 2, "*") << "\n";
    failed++;
  }
  if (calc("5f", "4l", 32, "*") == "per") {
    passed++;
  } else {
    std::cout << "Test 5f * 4l (32) is failed: must return per, returns: " << calc("5f", "4l", 32, "*") << "\n";
    failed++;
  }
  if (calc("85", "1t", 35, "+") == "9y") {
    passed++;
  } else {
    std::cout << "Test 85 + 1t (35) is failed: must return 9y, returns: " << calc("85", "1t", 35, "+") << "\n";
    failed++;
  }
  if (calc("94", "2r", 28, "+") == "c3") {
    passed++;
  } else {
    std::cout << "Test 94 + 2r (28) is failed: must return c3, returns: " << calc("94", "2r", 28, "+") << "\n";
    failed++;
  }
  if (calc("dh", "ae", 21, "*") == "7067") {
    passed++;
  } else {
    std::cout << "Test dh * ae (21) is failed: must return 7067, returns: " << calc("dh", "ae", 21, "*") << "\n";
    failed++;
  }
  if (calc("1001110100", "10011010", 2, "/") == "100") {
    passed++;
  } else {
    std::cout << "Test 1001110100 / 10011010 (2) is failed: must return 100, returns: " << calc("1001110100", "10011010", 2, "/") << "\n";
    failed++;
  }
  if (calc("65", "3c", 15, "+") == "a2") {
    passed++;
  } else {
    std::cout << "Test 65 + 3c (15) is failed: must return a2, returns: " << calc("65", "3c", 15, "+") << "\n";
    failed++;
  }
  if (calc("1022", "807", 9, "+") == "1830") {
    passed++;
  } else {
    std::cout << "Test 1022 + 807 (9) is failed: must return 1830, returns: " << calc("1022", "807", 9, "+") << "\n";
    failed++;
  }
  if (calc("37d", "193", 15, "*") == "5a459") {
    passed++;
  } else {
    std::cout << "Test 37d * 193 (15) is failed: must return 5a459, returns: " << calc("37d", "193", 15, "*") << "\n";
    failed++;
  }
  if (calc("482", "251", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 482 / 251 (13) is failed: must return 1, returns: " << calc("482", "251", 13, "/") << "\n";
    failed++;
  }
  if (calc("nq", "j5", 31, "*") == "emo6") {
    passed++;
  } else {
    std::cout << "Test nq * j5 (31) is failed: must return emo6, returns: " << calc("nq", "j5", 31, "*") << "\n";
    failed++;
  }
  if (calc("go", "8o", 28, "-") == "80") {
    passed++;
  } else {
    std::cout << "Test go - 8o (28) is failed: must return 80, returns: " << calc("go", "8o", 28, "-") << "\n";
    failed++;
  }
  if (calc("306", "260", 10, "-") == "46") {
    passed++;
  } else {
    std::cout << "Test 306 - 260 (10) is failed: must return 46, returns: " << calc("306", "260", 10, "-") << "\n";
    failed++;
  }
  if (calc("1ej", "1k", 21, "/") == "i") {
    passed++;
  } else {
    std::cout << "Test 1ej / 1k (21) is failed: must return i, returns: " << calc("1ej", "1k", 21, "/") << "\n";
    failed++;
  }
  if (calc("1447", "550", 8, "*") == "1067330") {
    passed++;
  } else {
    std::cout << "Test 1447 * 550 (8) is failed: must return 1067330, returns: " << calc("1447", "550", 8, "*") << "\n";
    failed++;
  }
  if (calc("202202", "122221", 3, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 202202 / 122221 (3) is failed: must return 1, returns: " << calc("202202", "122221", 3, "/") << "\n";
    failed++;
  }
  if (calc("347", "a8", 11, "+") == "444") {
    passed++;
  } else {
    std::cout << "Test 347 + a8 (11) is failed: must return 444, returns: " << calc("347", "a8", 11, "+") << "\n";
    failed++;
  }
  if (calc("1101111001", "11001100", 2, "*") == "101100010001101100") {
    passed++;
  } else {
    std::cout << "Test 1101111001 * 11001100 (2) is failed: must return 101100010001101100, returns: " << calc("1101111001", "11001100", 2, "*") << "\n";
    failed++;
  }
  if (calc("113", "46", 8, "+") == "161") {
    passed++;
  } else {
    std::cout << "Test 113 + 46 (8) is failed: must return 161, returns: " << calc("113", "46", 8, "+") << "\n";
    failed++;
  }
  if (calc("56", "2j", 27, "+") == "7p") {
    passed++;
  } else {
    std::cout << "Test 56 + 2j (27) is failed: must return 7p, returns: " << calc("56", "2j", 27, "+") << "\n";
    failed++;
  }
  if (calc("1a5", "ge", 20, "-") == "db") {
    passed++;
  } else {
    std::cout << "Test 1a5 - ge (20) is failed: must return db, returns: " << calc("1a5", "ge", 20, "-") << "\n";
    failed++;
  }
  if (calc("8j", "3r", 33, "-") == "4p") {
    passed++;
  } else {
    std::cout << "Test 8j - 3r (33) is failed: must return 4p, returns: " << calc("8j", "3r", 33, "-") << "\n";
    failed++;
  }
  if (calc("89", "54", 17, "*") == "2ab2") {
    passed++;
  } else {
    std::cout << "Test 89 * 54 (17) is failed: must return 2ab2, returns: " << calc("89", "54", 17, "*") << "\n";
    failed++;
  }
  if (calc("29b", "if", 20, "*") == "26915") {
    passed++;
  } else {
    std::cout << "Test 29b * if (20) is failed: must return 26915, returns: " << calc("29b", "if", 20, "*") << "\n";
    failed++;
  }
  if (calc("16", "6", 34, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 16 / 6 (34) is failed: must return 6, returns: " << calc("16", "6", 34, "/") << "\n";
    failed++;
  }
  if (calc("nr", "1w", 35, "*") == "1aho") {
    passed++;
  } else {
    std::cout << "Test nr * 1w (35) is failed: must return 1aho, returns: " << calc("nr", "1w", 35, "*") << "\n";
    failed++;
  }
  if (calc("bd", "7", 25, "+") == "bk") {
    passed++;
  } else {
    std::cout << "Test bd + 7 (25) is failed: must return bk, returns: " << calc("bd", "7", 25, "+") << "\n";
    failed++;
  }
  if (calc("221", "136", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 221 / 136 (8) is failed: must return 1, returns: " << calc("221", "136", 8, "/") << "\n";
    failed++;
  }
  if (calc("c2", "1e", 27, "-") == "af") {
    passed++;
  } else {
    std::cout << "Test c2 - 1e (27) is failed: must return af, returns: " << calc("c2", "1e", 27, "-") << "\n";
    failed++;
  }
  if (calc("n", "2", 24, "-") == "l") {
    passed++;
  } else {
    std::cout << "Test n - 2 (24) is failed: must return l, returns: " << calc("n", "2", 24, "-") << "\n";
    failed++;
  }
  if (calc("287", "25", 10, "/") == "11") {
    passed++;
  } else {
    std::cout << "Test 287 / 25 (10) is failed: must return 11, returns: " << calc("287", "25", 10, "/") << "\n";
    failed++;
  }
  if (calc("5k", "n", 32, "*") == "41c") {
    passed++;
  } else {
    std::cout << "Test 5k * n (32) is failed: must return 41c, returns: " << calc("5k", "n", 32, "*") << "\n";
    failed++;
  }
  if (calc("239", "196", 13, "+") == "402") {
    passed++;
  } else {
    std::cout << "Test 239 + 196 (13) is failed: must return 402, returns: " << calc("239", "196", 13, "+") << "\n";
    failed++;
  }
  if (calc("i4", "3g", 27, "*") == "2b5a") {
    passed++;
  } else {
    std::cout << "Test i4 * 3g (27) is failed: must return 2b5a, returns: " << calc("i4", "3g", 27, "*") << "\n";
    failed++;
  }
  if (calc("1hi", "10", 22, "*") == "1hi0") {
    passed++;
  } else {
    std::cout << "Test 1hi * 10 (22) is failed: must return 1hi0, returns: " << calc("1hi", "10", 22, "*") << "\n";
    failed++;
  }
  if (calc("bd", "6q", 28, "-") == "4f") {
    passed++;
  } else {
    std::cout << "Test bd - 6q (28) is failed: must return 4f, returns: " << calc("bd", "6q", 28, "-") << "\n";
    failed++;
  }
  if (calc("127", "pr", 29, "-") == "59") {
    passed++;
  } else {
    std::cout << "Test 127 - pr (29) is failed: must return 59, returns: " << calc("127", "pr", 29, "-") << "\n";
    failed++;
  }
  if (calc("15", "5", 12, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 15 / 5 (12) is failed: must return 3, returns: " << calc("15", "5", 12, "/") << "\n";
    failed++;
  }
  if (calc("220022", "22210", 3, "+") == "1020002") {
    passed++;
  } else {
    std::cout << "Test 220022 + 22210 (3) is failed: must return 1020002, returns: " << calc("220022", "22210", 3, "+") << "\n";
    failed++;
  }
  if (calc("ps", "bw", 35, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test ps / bw (35) is failed: must return 2, returns: " << calc("ps", "bw", 35, "/") << "\n";
    failed++;
  }
  if (calc("441", "74", 12, "-") == "389") {
    passed++;
  } else {
    std::cout << "Test 441 - 74 (12) is failed: must return 389, returns: " << calc("441", "74", 12, "-") << "\n";
    failed++;
  }
  if (calc("112", "51", 12, "-") == "81") {
    passed++;
  } else {
    std::cout << "Test 112 - 51 (12) is failed: must return 81, returns: " << calc("112", "51", 12, "-") << "\n";
    failed++;
  }
  if (calc("723", "135", 10, "*") == "97605") {
    passed++;
  } else {
    std::cout << "Test 723 * 135 (10) is failed: must return 97605, returns: " << calc("723", "135", 10, "*") << "\n";
    failed++;
  }
  if (calc("412", "74", 8, "-") == "316") {
    passed++;
  } else {
    std::cout << "Test 412 - 74 (8) is failed: must return 316, returns: " << calc("412", "74", 8, "-") << "\n";
    failed++;
  }
  if (calc("22231", "2033", 4, "*") == "113322203") {
    passed++;
  } else {
    std::cout << "Test 22231 * 2033 (4) is failed: must return 113322203, returns: " << calc("22231", "2033", 4, "*") << "\n";
    failed++;
  }
  if (calc("142", "le", 28, "-") == "ag") {
    passed++;
  } else {
    std::cout << "Test 142 - le (28) is failed: must return ag, returns: " << calc("142", "le", 28, "-") << "\n";
    failed++;
  }
  if (calc("2f", "2e", 25, "+") == "54") {
    passed++;
  } else {
    std::cout << "Test 2f + 2e (25) is failed: must return 54, returns: " << calc("2f", "2e", 25, "+") << "\n";
    failed++;
  }
  if (calc("1af", "cg", 26, "-") == "np") {
    passed++;
  } else {
    std::cout << "Test 1af - cg (26) is failed: must return np, returns: " << calc("1af", "cg", 26, "-") << "\n";
    failed++;
  }
  if (calc("53", "1j", 34, "-") == "3i") {
    passed++;
  } else {
    std::cout << "Test 53 - 1j (34) is failed: must return 3i, returns: " << calc("53", "1j", 34, "-") << "\n";
    failed++;
  }
  if (calc("in", "e7", 30, "*") == "8r3b") {
    passed++;
  } else {
    std::cout << "Test in * e7 (30) is failed: must return 8r3b, returns: " << calc("in", "e7", 30, "*") << "\n";
    failed++;
  }
  if (calc("g8", "a7", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test g8 / a7 (26) is failed: must return 1, returns: " << calc("g8", "a7", 26, "/") << "\n";
    failed++;
  }
  if (calc("c1", "6", 14, "+") == "c7") {
    passed++;
  } else {
    std::cout << "Test c1 + 6 (14) is failed: must return c7, returns: " << calc("c1", "6", 14, "+") << "\n";
    failed++;
  }
  if (calc("273", "1eg", 17, "-") == "94") {
    passed++;
  } else {
    std::cout << "Test 273 - 1eg (17) is failed: must return 94, returns: " << calc("273", "1eg", 17, "-") << "\n";
    failed++;
  }
  if (calc("578", "59", 13, "*") == "25a47") {
    passed++;
  } else {
    std::cout << "Test 578 * 59 (13) is failed: must return 25a47, returns: " << calc("578", "59", 13, "*") << "\n";
    failed++;
  }
  if (calc("cc", "a5", 16, "-") == "27") {
    passed++;
  } else {
    std::cout << "Test cc - a5 (16) is failed: must return 27, returns: " << calc("cc", "a5", 16, "-") << "\n";
    failed++;
  }
  if (calc("88", "3a", 17, "+") == "c1") {
    passed++;
  } else {
    std::cout << "Test 88 + 3a (17) is failed: must return c1, returns: " << calc("88", "3a", 17, "+") << "\n";
    failed++;
  }
  if (calc("2j", "16", 30, "+") == "3p") {
    passed++;
  } else {
    std::cout << "Test 2j + 16 (30) is failed: must return 3p, returns: " << calc("2j", "16", 30, "+") << "\n";
    failed++;
  }
  if (calc("58", "2k", 31, "+") == "7s") {
    passed++;
  } else {
    std::cout << "Test 58 + 2k (31) is failed: must return 7s, returns: " << calc("58", "2k", 31, "+") << "\n";
    failed++;
  }
  if (calc("213", "46", 8, "-") == "145") {
    passed++;
  } else {
    std::cout << "Test 213 - 46 (8) is failed: must return 145, returns: " << calc("213", "46", 8, "-") << "\n";
    failed++;
  }
  if (calc("102", "84", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 102 / 84 (9) is failed: must return 1, returns: " << calc("102", "84", 9, "/") << "\n";
    failed++;
  }
  if (calc("429", "345", 12, "*") == "122b19") {
    passed++;
  } else {
    std::cout << "Test 429 * 345 (12) is failed: must return 122b19, returns: " << calc("429", "345", 12, "*") << "\n";
    failed++;
  }
  if (calc("14", "0", 8, "+") == "14") {
    passed++;
  } else {
    std::cout << "Test 14 + 0 (8) is failed: must return 14, returns: " << calc("14", "0", 8, "+") << "\n";
    failed++;
  }
  if (calc("60", "2h", 24, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 60 / 2h (24) is failed: must return 2, returns: " << calc("60", "2h", 24, "/") << "\n";
    failed++;
  }
  if (calc("me", "90", 24, "-") == "de") {
    passed++;
  } else {
    std::cout << "Test me - 90 (24) is failed: must return de, returns: " << calc("me", "90", 24, "-") << "\n";
    failed++;
  }
  if (calc("100212", "11110", 3, "*") == "1121210020") {
    passed++;
  } else {
    std::cout << "Test 100212 * 11110 (3) is failed: must return 1121210020, returns: " << calc("100212", "11110", 3, "*") << "\n";
    failed++;
  }
  if (calc("441", "322", 5, "+") == "1313") {
    passed++;
  } else {
    std::cout << "Test 441 + 322 (5) is failed: must return 1313, returns: " << calc("441", "322", 5, "+") << "\n";
    failed++;
  }
  if (calc("3o", "2d", 25, "+") == "6c") {
    passed++;
  } else {
    std::cout << "Test 3o + 2d (25) is failed: must return 6c, returns: " << calc("3o", "2d", 25, "+") << "\n";
    failed++;
  }
  if (calc("g9", "90", 22, "+") == "139") {
    passed++;
  } else {
    std::cout << "Test g9 + 90 (22) is failed: must return 139, returns: " << calc("g9", "90", 22, "+") << "\n";
    failed++;
  }
  if (calc("10i", "tg", 31, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 10i / tg (31) is failed: must return 1, returns: " << calc("10i", "tg", 31, "/") << "\n";
    failed++;
  }
  if (calc("4223", "1412", 5, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 4223 / 1412 (5) is failed: must return 2, returns: " << calc("4223", "1412", 5, "/") << "\n";
    failed++;
  }
  if (calc("18f", "13i", 20, "*") == "1e72a") {
    passed++;
  } else {
    std::cout << "Test 18f * 13i (20) is failed: must return 1e72a, returns: " << calc("18f", "13i", 20, "*") << "\n";
    failed++;
  }
  if (calc("200", "47", 11, "*") == "9300") {
    passed++;
  } else {
    std::cout << "Test 200 * 47 (11) is failed: must return 9300, returns: " << calc("200", "47", 11, "*") << "\n";
    failed++;
  }
  if (calc("c", "3", 26, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test c / 3 (26) is failed: must return 4, returns: " << calc("c", "3", 26, "/") << "\n";
    failed++;
  }
  if (calc("330", "a3", 14, "*") == "24b90") {
    passed++;
  } else {
    std::cout << "Test 330 * a3 (14) is failed: must return 24b90, returns: " << calc("330", "a3", 14, "*") << "\n";
    failed++;
  }
  if (calc("hl", "g7", 35, "+") == "xs") {
    passed++;
  } else {
    std::cout << "Test hl + g7 (35) is failed: must return xs, returns: " << calc("hl", "g7", 35, "+") << "\n";
    failed++;
  }
  if (calc("j9", "g7", 31, "+") == "14g") {
    passed++;
  } else {
    std::cout << "Test j9 + g7 (31) is failed: must return 14g, returns: " << calc("j9", "g7", 31, "+") << "\n";
    failed++;
  }
  if (calc("837", "802", 9, "*") == "745075") {
    passed++;
  } else {
    std::cout << "Test 837 * 802 (9) is failed: must return 745075, returns: " << calc("837", "802", 9, "*") << "\n";
    failed++;
  }
  if (calc("1ah", "16", 18, "*") == "22ac") {
    passed++;
  } else {
    std::cout << "Test 1ah * 16 (18) is failed: must return 22ac, returns: " << calc("1ah", "16", 18, "*") << "\n";
    failed++;
  }
  if (calc("1100022", "110200", 3, "+") == "1210222") {
    passed++;
  } else {
    std::cout << "Test 1100022 + 110200 (3) is failed: must return 1210222, returns: " << calc("1100022", "110200", 3, "+") << "\n";
    failed++;
  }
  if (calc("1101001000", "1010010011", 2, "+") == "10111011011") {
    passed++;
  } else {
    std::cout << "Test 1101001000 + 1010010011 (2) is failed: must return 10111011011, returns: " << calc("1101001000", "1010010011", 2, "+") << "\n";
    failed++;
  }
  if (calc("2cg", "12g", 19, "+") == "3fd") {
    passed++;
  } else {
    std::cout << "Test 2cg + 12g (19) is failed: must return 3fd, returns: " << calc("2cg", "12g", 19, "+") << "\n";
    failed++;
  }
  if (calc("11", "11", 3, "+") == "22") {
    passed++;
  } else {
    std::cout << "Test 11 + 11 (3) is failed: must return 22, returns: " << calc("11", "11", 3, "+") << "\n";
    failed++;
  }
  if (calc("17b", "61", 12, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 17b / 61 (12) is failed: must return 3, returns: " << calc("17b", "61", 12, "/") << "\n";
    failed++;
  }
  if (calc("at", "8t", 32, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test at / 8t (32) is failed: must return 1, returns: " << calc("at", "8t", 32, "/") << "\n";
    failed++;
  }
  if (calc("1664", "1425", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1664 / 1425 (7) is failed: must return 1, returns: " << calc("1664", "1425", 7, "/") << "\n";
    failed++;
  }
  if (calc("221111", "220102", 3, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 221111 / 220102 (3) is failed: must return 1, returns: " << calc("221111", "220102", 3, "/") << "\n";
    failed++;
  }
  if (calc("32", "1a", 20, "-") == "1c") {
    passed++;
  } else {
    std::cout << "Test 32 - 1a (20) is failed: must return 1c, returns: " << calc("32", "1a", 20, "-") << "\n";
    failed++;
  }
  if (calc("il", "3p", 28, "*") == "2grl") {
    passed++;
  } else {
    std::cout << "Test il * 3p (28) is failed: must return 2grl, returns: " << calc("il", "3p", 28, "*") << "\n";
    failed++;
  }
  if (calc("2a", "19", 24, "-") == "11") {
    passed++;
  } else {
    std::cout << "Test 2a - 19 (24) is failed: must return 11, returns: " << calc("2a", "19", 24, "-") << "\n";
    failed++;
  }
  if (calc("r", "k", 31, "-") == "7") {
    passed++;
  } else {
    std::cout << "Test r - k (31) is failed: must return 7, returns: " << calc("r", "k", 31, "-") << "\n";
    failed++;
  }
  if (calc("df", "c9", 21, "+") == "153") {
    passed++;
  } else {
    std::cout << "Test df + c9 (21) is failed: must return 153, returns: " << calc("df", "c9", 21, "+") << "\n";
    failed++;
  }
  if (calc("337", "154", 8, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 337 / 154 (8) is failed: must return 2, returns: " << calc("337", "154", 8, "/") << "\n";
    failed++;
  }
  if (calc("184", "19", 21, "+") == "19d") {
    passed++;
  } else {
    std::cout << "Test 184 + 19 (21) is failed: must return 19d, returns: " << calc("184", "19", 21, "+") << "\n";
    failed++;
  }
  if (calc("cw", "1x", 34, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test cw / 1x (34) is failed: must return 6, returns: " << calc("cw", "1x", 34, "/") << "\n";
    failed++;
  }
  if (calc("542", "60", 10, "+") == "602") {
    passed++;
  } else {
    std::cout << "Test 542 + 60 (10) is failed: must return 602, returns: " << calc("542", "60", 10, "+") << "\n";
    failed++;
  }
  if (calc("3t", "17", 35, "+") == "51") {
    passed++;
  } else {
    std::cout << "Test 3t + 17 (35) is failed: must return 51, returns: " << calc("3t", "17", 35, "+") << "\n";
    failed++;
  }
  if (calc("r8", "aj", 35, "*") == "872c") {
    passed++;
  } else {
    std::cout << "Test r8 * aj (35) is failed: must return 872c, returns: " << calc("r8", "aj", 35, "*") << "\n";
    failed++;
  }
  if (calc("1174", "776", 9, "+") == "2061") {
    passed++;
  } else {
    std::cout << "Test 1174 + 776 (9) is failed: must return 2061, returns: " << calc("1174", "776", 9, "+") << "\n";
    failed++;
  }
  if (calc("1c", "l", 22, "*") == "1aa") {
    passed++;
  } else {
    std::cout << "Test 1c * l (22) is failed: must return 1aa, returns: " << calc("1c", "l", 22, "*") << "\n";
    failed++;
  }
  if (calc("jc", "gg", 20, "+") == "1g8") {
    passed++;
  } else {
    std::cout << "Test jc + gg (20) is failed: must return 1g8, returns: " << calc("jc", "gg", 20, "+") << "\n";
    failed++;
  }
  if (calc("84", "42", 17, "+") == "c6") {
    passed++;
  } else {
    std::cout << "Test 84 + 42 (17) is failed: must return c6, returns: " << calc("84", "42", 17, "+") << "\n";
    failed++;
  }
  if (calc("2c8", "105", 14, "-") == "1c3") {
    passed++;
  } else {
    std::cout << "Test 2c8 - 105 (14) is failed: must return 1c3, returns: " << calc("2c8", "105", 14, "-") << "\n";
    failed++;
  }
  if (calc("ha", "e5", 22, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ha / e5 (22) is failed: must return 1, returns: " << calc("ha", "e5", 22, "/") << "\n";
    failed++;
  }
  if (calc("687", "4b1", 12, "*") == "290917") {
    passed++;
  } else {
    std::cout << "Test 687 * 4b1 (12) is failed: must return 290917, returns: " << calc("687", "4b1", 12, "*") << "\n";
    failed++;
  }
  if (calc("2o", "2m", 30, "+") == "5g") {
    passed++;
  } else {
    std::cout << "Test 2o + 2m (30) is failed: must return 5g, returns: " << calc("2o", "2m", 30, "+") << "\n";
    failed++;
  }
  if (calc("12i", "a2", 20, "-") == "cg") {
    passed++;
  } else {
    std::cout << "Test 12i - a2 (20) is failed: must return cg, returns: " << calc("12i", "a2", 20, "-") << "\n";
    failed++;
  }
  if (calc("10032", "1233", 4, "-") == "2133") {
    passed++;
  } else {
    std::cout << "Test 10032 - 1233 (4) is failed: must return 2133, returns: " << calc("10032", "1233", 4, "-") << "\n";
    failed++;
  }
  if (calc("1b4", "19n", 24, "-") == "15") {
    passed++;
  } else {
    std::cout << "Test 1b4 - 19n (24) is failed: must return 15, returns: " << calc("1b4", "19n", 24, "-") << "\n";
    failed++;
  }
  if (calc("70", "2l", 30, "-") == "49") {
    passed++;
  } else {
    std::cout << "Test 70 - 2l (30) is failed: must return 49, returns: " << calc("70", "2l", 30, "-") << "\n";
    failed++;
  }
  if (calc("33211", "30112", 4, "-") == "3033") {
    passed++;
  } else {
    std::cout << "Test 33211 - 30112 (4) is failed: must return 3033, returns: " << calc("33211", "30112", 4, "-") << "\n";
    failed++;
  }
  if (calc("153", "149", 16, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 153 / 149 (16) is failed: must return 1, returns: " << calc("153", "149", 16, "/") << "\n";
    failed++;
  }
  if (calc("106", "52", 13, "-") == "84") {
    passed++;
  } else {
    std::cout << "Test 106 - 52 (13) is failed: must return 84, returns: " << calc("106", "52", 13, "-") << "\n";
    failed++;
  }
  if (calc("3o", "35", 32, "-") == "j") {
    passed++;
  } else {
    std::cout << "Test 3o - 35 (32) is failed: must return j, returns: " << calc("3o", "35", 32, "-") << "\n";
    failed++;
  }
  if (calc("7o", "5e", 25, "*") == "1j6b") {
    passed++;
  } else {
    std::cout << "Test 7o * 5e (25) is failed: must return 1j6b, returns: " << calc("7o", "5e", 25, "*") << "\n";
    failed++;
  }
  if (calc("352", "252", 9, "*") == "102124") {
    passed++;
  } else {
    std::cout << "Test 352 * 252 (9) is failed: must return 102124, returns: " << calc("352", "252", 9, "*") << "\n";
    failed++;
  }
  if (calc("ke", "j3", 26, "+") == "1dh") {
    passed++;
  } else {
    std::cout << "Test ke + j3 (26) is failed: must return 1dh, returns: " << calc("ke", "j3", 26, "+") << "\n";
    failed++;
  }
  if (calc("8n", "i", 25, "+") == "9g") {
    passed++;
  } else {
    std::cout << "Test 8n + i (25) is failed: must return 9g, returns: " << calc("8n", "i", 25, "+") << "\n";
    failed++;
  }
  if (calc("448", "83", 9, "*") == "41766") {
    passed++;
  } else {
    std::cout << "Test 448 * 83 (9) is failed: must return 41766, returns: " << calc("448", "83", 9, "*") << "\n";
    failed++;
  }
  if (calc("2244", "433", 5, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 2244 / 433 (5) is failed: must return 2, returns: " << calc("2244", "433", 5, "/") << "\n";
    failed++;
  }
  if (calc("rb", "ef", 28, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test rb / ef (28) is failed: must return 1, returns: " << calc("rb", "ef", 28, "/") << "\n";
    failed++;
  }
  if (calc("d3", "bg", 31, "+") == "oj") {
    passed++;
  } else {
    std::cout << "Test d3 + bg (31) is failed: must return oj, returns: " << calc("d3", "bg", 31, "+") << "\n";
    failed++;
  }
  if (calc("11201", "3110", 4, "*") == "102101110") {
    passed++;
  } else {
    std::cout << "Test 11201 * 3110 (4) is failed: must return 102101110, returns: " << calc("11201", "3110", 4, "*") << "\n";
    failed++;
  }
  if (calc("444", "373", 8, "-") == "51") {
    passed++;
  } else {
    std::cout << "Test 444 - 373 (8) is failed: must return 51, returns: " << calc("444", "373", 8, "-") << "\n";
    failed++;
  }
  if (calc("6b", "31", 26, "*") == "jdb") {
    passed++;
  } else {
    std::cout << "Test 6b * 31 (26) is failed: must return jdb, returns: " << calc("6b", "31", 26, "*") << "\n";
    failed++;
  }
  if (calc("1252", "403", 6, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1252 / 403 (6) is failed: must return 2, returns: " << calc("1252", "403", 6, "/") << "\n";
    failed++;
  }
  if (calc("11230", "4100", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 11230 / 4100 (5) is failed: must return 1, returns: " << calc("11230", "4100", 5, "/") << "\n";
    failed++;
  }
  if (calc("g9", "n", 34, "+") == "gw") {
    passed++;
  } else {
    std::cout << "Test g9 + n (34) is failed: must return gw, returns: " << calc("g9", "n", 34, "+") << "\n";
    failed++;
  }
  if (calc("cm", "28", 34, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test cm / 28 (34) is failed: must return 5, returns: " << calc("cm", "28", 34, "/") << "\n";
    failed++;
  }
  if (calc("2054", "1102", 7, "+") == "3156") {
    passed++;
  } else {
    std::cout << "Test 2054 + 1102 (7) is failed: must return 3156, returns: " << calc("2054", "1102", 7, "+") << "\n";
    failed++;
  }
  if (calc("8m", "d", 29, "/") == "j") {
    passed++;
  } else {
    std::cout << "Test 8m / d (29) is failed: must return j, returns: " << calc("8m", "d", 29, "/") << "\n";
    failed++;
  }
  if (calc("615", "56", 11, "-") == "56a") {
    passed++;
  } else {
    std::cout << "Test 615 - 56 (11) is failed: must return 56a, returns: " << calc("615", "56", 11, "-") << "\n";
    failed++;
  }
  if (calc("48", "2i", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 48 / 2i (24) is failed: must return 1, returns: " << calc("48", "2i", 24, "/") << "\n";
    failed++;
  }
  if (calc("1258", "123", 9, "*") == "156066") {
    passed++;
  } else {
    std::cout << "Test 1258 * 123 (9) is failed: must return 156066, returns: " << calc("1258", "123", 9, "*") << "\n";
    failed++;
  }
  if (calc("21c", "1ji", 21, "-") == "2f") {
    passed++;
  } else {
    std::cout << "Test 21c - 1ji (21) is failed: must return 2f, returns: " << calc("21c", "1ji", 21, "-") << "\n";
    failed++;
  }
  if (calc("ai", "9c", 35, "-") == "16") {
    passed++;
  } else {
    std::cout << "Test ai - 9c (35) is failed: must return 16, returns: " << calc("ai", "9c", 35, "-") << "\n";
    failed++;
  }
  if (calc("148", "i0", 21, "+") == "218") {
    passed++;
  } else {
    std::cout << "Test 148 + i0 (21) is failed: must return 218, returns: " << calc("148", "i0", 21, "+") << "\n";
    failed++;
  }
  if (calc("o8", "ba", 33, "*") == "8a0e") {
    passed++;
  } else {
    std::cout << "Test o8 * ba (33) is failed: must return 8a0e, returns: " << calc("o8", "ba", 33, "*") << "\n";
    failed++;
  }
  if (calc("1042", "1001", 8, "*") == "1043042") {
    passed++;
  } else {
    std::cout << "Test 1042 * 1001 (8) is failed: must return 1043042, returns: " << calc("1042", "1001", 8, "*") << "\n";
    failed++;
  }
  if (calc("4102", "3240", 6, "-") == "422") {
    passed++;
  } else {
    std::cout << "Test 4102 - 3240 (6) is failed: must return 422, returns: " << calc("4102", "3240", 6, "-") << "\n";
    failed++;
  }
  if (calc("1ff", "151", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1ff / 151 (24) is failed: must return 1, returns: " << calc("1ff", "151", 24, "/") << "\n";
    failed++;
  }
  if (calc("1643", "1145", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1643 / 1145 (7) is failed: must return 1, returns: " << calc("1643", "1145", 7, "/") << "\n";
    failed++;
  }
  if (calc("8p", "6u", 31, "+") == "fo") {
    passed++;
  } else {
    std::cout << "Test 8p + 6u (31) is failed: must return fo, returns: " << calc("8p", "6u", 31, "+") << "\n";
    failed++;
  }
  if (calc("16", "6", 34, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 16 / 6 (34) is failed: must return 6, returns: " << calc("16", "6", 34, "/") << "\n";
    failed++;
  }
  if (calc("10a", "8m", 23, "+") == "199") {
    passed++;
  } else {
    std::cout << "Test 10a + 8m (23) is failed: must return 199, returns: " << calc("10a", "8m", 23, "+") << "\n";
    failed++;
  }
  if (calc("3332", "1120", 5, "*") == "4342340") {
    passed++;
  } else {
    std::cout << "Test 3332 * 1120 (5) is failed: must return 4342340, returns: " << calc("3332", "1120", 5, "*") << "\n";
    failed++;
  }
  if (calc("2e3", "8b", 18, "-") == "25a") {
    passed++;
  } else {
    std::cout << "Test 2e3 - 8b (18) is failed: must return 25a, returns: " << calc("2e3", "8b", 18, "-") << "\n";
    failed++;
  }
  if (calc("no", "cd", 32, "-") == "bb") {
    passed++;
  } else {
    std::cout << "Test no - cd (32) is failed: must return bb, returns: " << calc("no", "cd", 32, "-") << "\n";
    failed++;
  }
  if (calc("4ad", "b2", 14, "*") == "3b3ac") {
    passed++;
  } else {
    std::cout << "Test 4ad * b2 (14) is failed: must return 3b3ac, returns: " << calc("4ad", "b2", 14, "*") << "\n";
    failed++;
  }
  if (calc("7d", "4d", 20, "+") == "c6") {
    passed++;
  } else {
    std::cout << "Test 7d + 4d (20) is failed: must return c6, returns: " << calc("7d", "4d", 20, "+") << "\n";
    failed++;
  }
  if (calc("g9", "ce", 20, "+") == "193") {
    passed++;
  } else {
    std::cout << "Test g9 + ce (20) is failed: must return 193, returns: " << calc("g9", "ce", 20, "+") << "\n";
    failed++;
  }
  if (calc("21", "4", 19, "+") == "25") {
    passed++;
  } else {
    std::cout << "Test 21 + 4 (19) is failed: must return 25, returns: " << calc("21", "4", 19, "+") << "\n";
    failed++;
  }
  if (calc("3a", "2h", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 3a / 2h (26) is failed: must return 1, returns: " << calc("3a", "2h", 26, "/") << "\n";
    failed++;
  }
  if (calc("18k", "18e", 22, "+") == "2hc") {
    passed++;
  } else {
    std::cout << "Test 18k + 18e (22) is failed: must return 2hc, returns: " << calc("18k", "18e", 22, "+") << "\n";
    failed++;
  }
  if (calc("10310", "10122", 5, "*") == "104413320") {
    passed++;
  } else {
    std::cout << "Test 10310 * 10122 (5) is failed: must return 104413320, returns: " << calc("10310", "10122", 5, "*") << "\n";
    failed++;
  }
  if (calc("166", "102", 9, "-") == "64") {
    passed++;
  } else {
    std::cout << "Test 166 - 102 (9) is failed: must return 64, returns: " << calc("166", "102", 9, "-") << "\n";
    failed++;
  }
  if (calc("118", "42", 15, "+") == "15a") {
    passed++;
  } else {
    std::cout << "Test 118 + 42 (15) is failed: must return 15a, returns: " << calc("118", "42", 15, "+") << "\n";
    failed++;
  }
  if (calc("18h", "aa", 23, "-") == "l7") {
    passed++;
  } else {
    std::cout << "Test 18h - aa (23) is failed: must return l7, returns: " << calc("18h", "aa", 23, "-") << "\n";
    failed++;
  }
  if (calc("ac", "g", 27, "/") == "h") {
    passed++;
  } else {
    std::cout << "Test ac / g (27) is failed: must return h, returns: " << calc("ac", "g", 27, "/") << "\n";
    failed++;
  }
  if (calc("1hf", "aa", 18, "*") == "13046") {
    passed++;
  } else {
    std::cout << "Test 1hf * aa (18) is failed: must return 13046, returns: " << calc("1hf", "aa", 18, "*") << "\n";
    failed++;
  }
  if (calc("4", "2", 8, "-") == "2") {
    passed++;
  } else {
    std::cout << "Test 4 - 2 (8) is failed: must return 2, returns: " << calc("4", "2", 8, "-") << "\n";
    failed++;
  }
  if (calc("322", "26", 8, "+") == "350") {
    passed++;
  } else {
    std::cout << "Test 322 + 26 (8) is failed: must return 350, returns: " << calc("322", "26", 8, "+") << "\n";
    failed++;
  }
  if (calc("pj", "5h", 27, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test pj / 5h (27) is failed: must return 4, returns: " << calc("pj", "5h", 27, "/") << "\n";
    failed++;
  }
  if (calc("4b1", "11d", 14, "+") == "5d0") {
    passed++;
  } else {
    std::cout << "Test 4b1 + 11d (14) is failed: must return 5d0, returns: " << calc("4b1", "11d", 14, "+") << "\n";
    failed++;
  }
  if (calc("440", "261", 8, "-") == "157") {
    passed++;
  } else {
    std::cout << "Test 440 - 261 (8) is failed: must return 157, returns: " << calc("440", "261", 8, "-") << "\n";
    failed++;
  }
  if (calc("111001000", "11001", 2, "*") == "10110010001000") {
    passed++;
  } else {
    std::cout << "Test 111001000 * 11001 (2) is failed: must return 10110010001000, returns: " << calc("111001000", "11001", 2, "*") << "\n";
    failed++;
  }
  if (calc("8", "7", 32, "+") == "f") {
    passed++;
  } else {
    std::cout << "Test 8 + 7 (32) is failed: must return f, returns: " << calc("8", "7", 32, "+") << "\n";
    failed++;
  }
  if (calc("l1", "7m", 29, "*") == "5i5m") {
    passed++;
  } else {
    std::cout << "Test l1 * 7m (29) is failed: must return 5i5m, returns: " << calc("l1", "7m", 29, "*") << "\n";
    failed++;
  }
  if (calc("2204", "243", 6, "*") == "1024500") {
    passed++;
  } else {
    std::cout << "Test 2204 * 243 (6) is failed: must return 1024500, returns: " << calc("2204", "243", 6, "*") << "\n";
    failed++;
  }
  if (calc("10424", "422", 5, "/") == "11") {
    passed++;
  } else {
    std::cout << "Test 10424 / 422 (5) is failed: must return 11, returns: " << calc("10424", "422", 5, "/") << "\n";
    failed++;
  }
  if (calc("2j", "f", 28, "-") == "24") {
    passed++;
  } else {
    std::cout << "Test 2j - f (28) is failed: must return 24, returns: " << calc("2j", "f", 28, "-") << "\n";
    failed++;
  }
  if (calc("41", "23", 22, "*") == "8e3") {
    passed++;
  } else {
    std::cout << "Test 41 * 23 (22) is failed: must return 8e3, returns: " << calc("41", "23", 22, "*") << "\n";
    failed++;
  }
  if (calc("23b", "157", 14, "+") == "394") {
    passed++;
  } else {
    std::cout << "Test 23b + 157 (14) is failed: must return 394, returns: " << calc("23b", "157", 14, "+") << "\n";
    failed++;
  }
  if (calc("tr", "62", 33, "-") == "np") {
    passed++;
  } else {
    std::cout << "Test tr - 62 (33) is failed: must return np, returns: " << calc("tr", "62", 33, "-") << "\n";
    failed++;
  }
  if (calc("au", "2c", 35, "*") == "pfa") {
    passed++;
  } else {
    std::cout << "Test au * 2c (35) is failed: must return pfa, returns: " << calc("au", "2c", 35, "*") << "\n";
    failed++;
  }
  if (calc("1001", "100", 2, "/") == "10") {
    passed++;
  } else {
    std::cout << "Test 1001 / 100 (2) is failed: must return 10, returns: " << calc("1001", "100", 2, "/") << "\n";
    failed++;
  }
  if (calc("11i", "g2", 27, "*") == "h1l9") {
    passed++;
  } else {
    std::cout << "Test 11i * g2 (27) is failed: must return h1l9, returns: " << calc("11i", "g2", 27, "*") << "\n";
    failed++;
  }
  if (calc("118", "37", 13, "-") == "b1") {
    passed++;
  } else {
    std::cout << "Test 118 - 37 (13) is failed: must return b1, returns: " << calc("118", "37", 13, "-") << "\n";
    failed++;
  }
  if (calc("18", "5", 22, "*") == "6i") {
    passed++;
  } else {
    std::cout << "Test 18 * 5 (22) is failed: must return 6i, returns: " << calc("18", "5", 22, "*") << "\n";
    failed++;
  }
  if (calc("334", "143", 7, "-") == "161") {
    passed++;
  } else {
    std::cout << "Test 334 - 143 (7) is failed: must return 161, returns: " << calc("334", "143", 7, "-") << "\n";
    failed++;
  }
  if (calc("1f6", "de", 17, "+") == "2c3") {
    passed++;
  } else {
    std::cout << "Test 1f6 + de (17) is failed: must return 2c3, returns: " << calc("1f6", "de", 17, "+") << "\n";
    failed++;
  }
  if (calc("19k", "c8", 25, "+") == "1m3") {
    passed++;
  } else {
    std::cout << "Test 19k + c8 (25) is failed: must return 1m3, returns: " << calc("19k", "c8", 25, "+") << "\n";
    failed++;
  }
  if (calc("8h", "1d", 31, "*") == "c44") {
    passed++;
  } else {
    std::cout << "Test 8h * 1d (31) is failed: must return c44, returns: " << calc("8h", "1d", 31, "*") << "\n";
    failed++;
  }
  if (calc("4a1", "db", 14, "*") == "490bb") {
    passed++;
  } else {
    std::cout << "Test 4a1 * db (14) is failed: must return 490bb, returns: " << calc("4a1", "db", 14, "*") << "\n";
    failed++;
  }
  if (calc("1000000101", "110100101", 2, "-") == "1100000") {
    passed++;
  } else {
    std::cout << "Test 1000000101 - 110100101 (2) is failed: must return 1100000, returns: " << calc("1000000101", "110100101", 2, "-") << "\n";
    failed++;
  }
  if (calc("4234", "3303", 5, "+") == "13042") {
    passed++;
  } else {
    std::cout << "Test 4234 + 3303 (5) is failed: must return 13042, returns: " << calc("4234", "3303", 5, "+") << "\n";
    failed++;
  }
  if (calc("3u", "g", 33, "/") == "8") {
    passed++;
  } else {
    std::cout << "Test 3u / g (33) is failed: must return 8, returns: " << calc("3u", "g", 33, "/") << "\n";
    failed++;
  }
  if (calc("55", "50", 11, "-") == "5") {
    passed++;
  } else {
    std::cout << "Test 55 - 50 (11) is failed: must return 5, returns: " << calc("55", "50", 11, "-") << "\n";
    failed++;
  }
  if (calc("14h", "fr", 28, "+") == "1kg") {
    passed++;
  } else {
    std::cout << "Test 14h + fr (28) is failed: must return 1kg, returns: " << calc("14h", "fr", 28, "+") << "\n";
    failed++;
  }
  if (calc("b3", "aa", 30, "-") == "n") {
    passed++;
  } else {
    std::cout << "Test b3 - aa (30) is failed: must return n, returns: " << calc("b3", "aa", 30, "-") << "\n";
    failed++;
  }
  if (calc("1024", "317", 8, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1024 / 317 (8) is failed: must return 2, returns: " << calc("1024", "317", 8, "/") << "\n";
    failed++;
  }
  if (calc("126", "bg", 25, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 126 / bg (25) is failed: must return 2, returns: " << calc("126", "bg", 25, "/") << "\n";
    failed++;
  }
  if (calc("13a", "9i", 19, "+") == "1d9") {
    passed++;
  } else {
    std::cout << "Test 13a + 9i (19) is failed: must return 1d9, returns: " << calc("13a", "9i", 19, "+") << "\n";
    failed++;
  }
  if (calc("3f", "1", 32, "-") == "3e") {
    passed++;
  } else {
    std::cout << "Test 3f - 1 (32) is failed: must return 3e, returns: " << calc("3f", "1", 32, "-") << "\n";
    failed++;
  }
  if (calc("cu", "4k", 35, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test cu / 4k (35) is failed: must return 2, returns: " << calc("cu", "4k", 35, "/") << "\n";
    failed++;
  }
  if (calc("96", "8o", 26, "*") == "349e") {
    passed++;
  } else {
    std::cout << "Test 96 * 8o (26) is failed: must return 349e, returns: " << calc("96", "8o", 26, "*") << "\n";
    failed++;
  }
  if (calc("1t", "1e", 31, "+") == "3c") {
    passed++;
  } else {
    std::cout << "Test 1t + 1e (31) is failed: must return 3c, returns: " << calc("1t", "1e", 31, "+") << "\n";
    failed++;
  }
  if (calc("133", "115", 25, "*") == "14bif") {
    passed++;
  } else {
    std::cout << "Test 133 * 115 (25) is failed: must return 14bif, returns: " << calc("133", "115", 25, "*") << "\n";
    failed++;
  }
  if (calc("15d", "pn", 28, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 15d / pn (28) is failed: must return 1, returns: " << calc("15d", "pn", 28, "/") << "\n";
    failed++;
  }
  if (calc("523", "150", 6, "+") == "1113") {
    passed++;
  } else {
    std::cout << "Test 523 + 150 (6) is failed: must return 1113, returns: " << calc("523", "150", 6, "+") << "\n";
    failed++;
  }
  if (calc("81", "5h", 18, "-") == "22") {
    passed++;
  } else {
    std::cout << "Test 81 - 5h (18) is failed: must return 22, returns: " << calc("81", "5h", 18, "-") << "\n";
    failed++;
  }
  if (calc("122222", "11100", 3, "-") == "111122") {
    passed++;
  } else {
    std::cout << "Test 122222 - 11100 (3) is failed: must return 111122, returns: " << calc("122222", "11100", 3, "-") << "\n";
    failed++;
  }
  if (calc("3123", "431", 5, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 3123 / 431 (5) is failed: must return 3, returns: " << calc("3123", "431", 5, "/") << "\n";
    failed++;
  }
  if (calc("uc", "a5", 31, "+") == "19h") {
    passed++;
  } else {
    std::cout << "Test uc + a5 (31) is failed: must return 19h, returns: " << calc("uc", "a5", 31, "+") << "\n";
    failed++;
  }
  if (calc("8d", "2d", 28, "-") == "60") {
    passed++;
  } else {
    std::cout << "Test 8d - 2d (28) is failed: must return 60, returns: " << calc("8d", "2d", 28, "-") << "\n";
    failed++;
  }
  if (calc("222112", "100012", 3, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 222112 / 100012 (3) is failed: must return 2, returns: " << calc("222112", "100012", 3, "/") << "\n";
    failed++;
  }
  if (calc("k7", "he", 32, "-") == "2p") {
    passed++;
  } else {
    std::cout << "Test k7 - he (32) is failed: must return 2p, returns: " << calc("k7", "he", 32, "-") << "\n";
    failed++;
  }
  if (calc("42", "32", 6, "*") == "2224") {
    passed++;
  } else {
    std::cout << "Test 42 * 32 (6) is failed: must return 2224, returns: " << calc("42", "32", 6, "*") << "\n";
    failed++;
  }
  if (calc("1311", "583", 9, "*") == "787523") {
    passed++;
  } else {
    std::cout << "Test 1311 * 583 (9) is failed: must return 787523, returns: " << calc("1311", "583", 9, "*") << "\n";
    failed++;
  }
  if (calc("251", "1h9", 18, "+") == "44a") {
    passed++;
  } else {
    std::cout << "Test 251 + 1h9 (18) is failed: must return 44a, returns: " << calc("251", "1h9", 18, "+") << "\n";
    failed++;
  }
  if (calc("1000110110", "1000100001", 2, "-") == "10101") {
    passed++;
  } else {
    std::cout << "Test 1000110110 - 1000100001 (2) is failed: must return 10101, returns: " << calc("1000110110", "1000100001", 2, "-") << "\n";
    failed++;
  }
  if (calc("103", "je", 30, "-") == "aj") {
    passed++;
  } else {
    std::cout << "Test 103 - je (30) is failed: must return aj, returns: " << calc("103", "je", 30, "-") << "\n";
    failed++;
  }
  if (calc("ge", "2e", 18, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test ge / 2e (18) is failed: must return 6, returns: " << calc("ge", "2e", 18, "/") << "\n";
    failed++;
  }
  if (calc("30230", "3221", 4, "*") == "232030030") {
    passed++;
  } else {
    std::cout << "Test 30230 * 3221 (4) is failed: must return 232030030, returns: " << calc("30230", "3221", 4, "*") << "\n";
    failed++;
  }
  if (calc("qa", "6f", 33, "*") == "54pi") {
    passed++;
  } else {
    std::cout << "Test qa * 6f (33) is failed: must return 54pi, returns: " << calc("qa", "6f", 33, "*") << "\n";
    failed++;
  }
  if (calc("34a", "1g4", 17, "+") == "53e") {
    passed++;
  } else {
    std::cout << "Test 34a + 1g4 (17) is failed: must return 53e, returns: " << calc("34a", "1g4", 17, "+") << "\n";
    failed++;
  }
  if (calc("or", "em", 32, "+") == "17h") {
    passed++;
  } else {
    std::cout << "Test or + em (32) is failed: must return 17h, returns: " << calc("or", "em", 32, "+") << "\n";
    failed++;
  }
  if (calc("230", "1a2", 12, "+") == "412") {
    passed++;
  } else {
    std::cout << "Test 230 + 1a2 (12) is failed: must return 412, returns: " << calc("230", "1a2", 12, "+") << "\n";
    failed++;
  }
  if (calc("lq", "h7", 32, "-") == "4j") {
    passed++;
  } else {
    std::cout << "Test lq - h7 (32) is failed: must return 4j, returns: " << calc("lq", "h7", 32, "-") << "\n";
    failed++;
  }
  if (calc("3cb", "325", 15, "-") == "a6") {
    passed++;
  } else {
    std::cout << "Test 3cb - 325 (15) is failed: must return a6, returns: " << calc("3cb", "325", 15, "-") << "\n";
    failed++;
  }
  if (calc("2442", "1320", 7, "+") == "4062") {
    passed++;
  } else {
    std::cout << "Test 2442 + 1320 (7) is failed: must return 4062, returns: " << calc("2442", "1320", 7, "+") << "\n";
    failed++;
  }
  if (calc("92", "8", 20, "+") == "9a") {
    passed++;
  } else {
    std::cout << "Test 92 + 8 (20) is failed: must return 9a, returns: " << calc("92", "8", 20, "+") << "\n";
    failed++;
  }
  if (calc("499", "27", 12, "-") == "472") {
    passed++;
  } else {
    std::cout << "Test 499 - 27 (12) is failed: must return 472, returns: " << calc("499", "27", 12, "-") << "\n";
    failed++;
  }
  if (calc("12110", "3322", 5, "-") == "3233") {
    passed++;
  } else {
    std::cout << "Test 12110 - 3322 (5) is failed: must return 3233, returns: " << calc("12110", "3322", 5, "-") << "\n";
    failed++;
  }
  if (calc("f4", "3u", 31, "-") == "b5") {
    passed++;
  } else {
    std::cout << "Test f4 - 3u (31) is failed: must return b5, returns: " << calc("f4", "3u", 31, "-") << "\n";
    failed++;
  }
  if (calc("228", "92", 16, "-") == "196") {
    passed++;
  } else {
    std::cout << "Test 228 - 92 (16) is failed: must return 196, returns: " << calc("228", "92", 16, "-") << "\n";
    failed++;
  }
  if (calc("7h", "3c", 28, "-") == "45") {
    passed++;
  } else {
    std::cout << "Test 7h - 3c (28) is failed: must return 45, returns: " << calc("7h", "3c", 28, "-") << "\n";
    failed++;
  }
  if (calc("422", "333", 7, "+") == "1055") {
    passed++;
  } else {
    std::cout << "Test 422 + 333 (7) is failed: must return 1055, returns: " << calc("422", "333", 7, "+") << "\n";
    failed++;
  }
  if (calc("436", "392", 10, "-") == "44") {
    passed++;
  } else {
    std::cout << "Test 436 - 392 (10) is failed: must return 44, returns: " << calc("436", "392", 10, "-") << "\n";
    failed++;
  }
  if (calc("ba", "52", 17, "-") == "68") {
    passed++;
  } else {
    std::cout << "Test ba - 52 (17) is failed: must return 68, returns: " << calc("ba", "52", 17, "-") << "\n";
    failed++;
  }
  if (calc("505", "205", 7, "*") == "135034") {
    passed++;
  } else {
    std::cout << "Test 505 * 205 (7) is failed: must return 135034, returns: " << calc("505", "205", 7, "*") << "\n";
    failed++;
  }
  if (calc("533", "442", 9, "-") == "81") {
    passed++;
  } else {
    std::cout << "Test 533 - 442 (9) is failed: must return 81, returns: " << calc("533", "442", 9, "-") << "\n";
    failed++;
  }
  if (calc("1gi", "ca", 22, "*") == "lla4") {
    passed++;
  } else {
    std::cout << "Test 1gi * ca (22) is failed: must return lla4, returns: " << calc("1gi", "ca", 22, "*") << "\n";
    failed++;
  }
  if (calc("17i", "127", 21, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 17i / 127 (21) is failed: must return 1, returns: " << calc("17i", "127", 21, "/") << "\n";
    failed++;
  }
  if (calc("394", "358", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 394 / 358 (14) is failed: must return 1, returns: " << calc("394", "358", 14, "/") << "\n";
    failed++;
  }
  if (calc("474", "470", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 474 / 470 (9) is failed: must return 1, returns: " << calc("474", "470", 9, "/") << "\n";
    failed++;
  }
  if (calc("18", "j", 22, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 18 / j (22) is failed: must return 1, returns: " << calc("18", "j", 22, "/") << "\n";
    failed++;
  }
  if (calc("211", "100", 7, "-") == "111") {
    passed++;
  } else {
    std::cout << "Test 211 - 100 (7) is failed: must return 111, returns: " << calc("211", "100", 7, "-") << "\n";
    failed++;
  }
  if (calc("90", "15", 32, "*") == "ad0") {
    passed++;
  } else {
    std::cout << "Test 90 * 15 (32) is failed: must return ad0, returns: " << calc("90", "15", 32, "*") << "\n";
    failed++;
  }
  if (calc("ir", "a9", 32, "-") == "8i") {
    passed++;
  } else {
    std::cout << "Test ir - a9 (32) is failed: must return 8i, returns: " << calc("ir", "a9", 32, "-") << "\n";
    failed++;
  }
  if (calc("c7", "4j", 31, "*") == "1pc9") {
    passed++;
  } else {
    std::cout << "Test c7 * 4j (31) is failed: must return 1pc9, returns: " << calc("c7", "4j", 31, "*") << "\n";
    failed++;
  }
  if (calc("fv", "fn", 34, "*") == "7bex") {
    passed++;
  } else {
    std::cout << "Test fv * fn (34) is failed: must return 7bex, returns: " << calc("fv", "fn", 34, "*") << "\n";
    failed++;
  }
  if (calc("6t", "4j", 31, "-") == "2a") {
    passed++;
  } else {
    std::cout << "Test 6t - 4j (31) is failed: must return 2a, returns: " << calc("6t", "4j", 31, "-") << "\n";
    failed++;
  }
  if (calc("q2", "f9", 28, "+") == "1db") {
    passed++;
  } else {
    std::cout << "Test q2 + f9 (28) is failed: must return 1db, returns: " << calc("q2", "f9", 28, "+") << "\n";
    failed++;
  }
  if (calc("cj", "5o", 28, "*") == "2i78") {
    passed++;
  } else {
    std::cout << "Test cj * 5o (28) is failed: must return 2i78, returns: " << calc("cj", "5o", 28, "*") << "\n";
    failed++;
  }
  if (calc("472", "87", 10, "+") == "559") {
    passed++;
  } else {
    std::cout << "Test 472 + 87 (10) is failed: must return 559, returns: " << calc("472", "87", 10, "+") << "\n";
    failed++;
  }
  if (calc("1db", "44", 19, "+") == "1hf") {
    passed++;
  } else {
    std::cout << "Test 1db + 44 (19) is failed: must return 1hf, returns: " << calc("1db", "44", 19, "+") << "\n";
    failed++;
  }
  if (calc("8e", "65", 23, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 8e / 65 (23) is failed: must return 1, returns: " << calc("8e", "65", 23, "/") << "\n";
    failed++;
  }
  if (calc("iq", "7l", 30, "*") == "4p86") {
    passed++;
  } else {
    std::cout << "Test iq * 7l (30) is failed: must return 4p86, returns: " << calc("iq", "7l", 30, "*") << "\n";
    failed++;
  }
  if (calc("38b", "350", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 38b / 350 (14) is failed: must return 1, returns: " << calc("38b", "350", 14, "/") << "\n";
    failed++;
  }
  if (calc("13c", "38", 20, "*") == "404g") {
    passed++;
  } else {
    std::cout << "Test 13c * 38 (20) is failed: must return 404g, returns: " << calc("13c", "38", 20, "*") << "\n";
    failed++;
  }
  if (calc("380", "294", 14, "-") == "ca") {
    passed++;
  } else {
    std::cout << "Test 380 - 294 (14) is failed: must return ca, returns: " << calc("380", "294", 14, "-") << "\n";
    failed++;
  }
  if (calc("1283", "1238", 9, "*") == "1613036") {
    passed++;
  } else {
    std::cout << "Test 1283 * 1238 (9) is failed: must return 1613036, returns: " << calc("1283", "1238", 9, "*") << "\n";
    failed++;
  }
  if (calc("dg", "6f", 34, "+") == "jv") {
    passed++;
  } else {
    std::cout << "Test dg + 6f (34) is failed: must return jv, returns: " << calc("dg", "6f", 34, "+") << "\n";
    failed++;
  }
  if (calc("194", "111", 13, "-") == "83") {
    passed++;
  } else {
    std::cout << "Test 194 - 111 (13) is failed: must return 83, returns: " << calc("194", "111", 13, "-") << "\n";
    failed++;
  }
  if (calc("26a", "262", 14, "+") == "4cc") {
    passed++;
  } else {
    std::cout << "Test 26a + 262 (14) is failed: must return 4cc, returns: " << calc("26a", "262", 14, "+") << "\n";
    failed++;
  }
  if (calc("413", "3b3", 14, "-") == "40") {
    passed++;
  } else {
    std::cout << "Test 413 - 3b3 (14) is failed: must return 40, returns: " << calc("413", "3b3", 14, "-") << "\n";
    failed++;
  }
  if (calc("109", "dc", 28, "*") == "dg8o") {
    passed++;
  } else {
    std::cout << "Test 109 * dc (28) is failed: must return dg8o, returns: " << calc("109", "dc", 28, "*") << "\n";
    failed++;
  }
  if (calc("864", "444", 9, "*") == "431457") {
    passed++;
  } else {
    std::cout << "Test 864 * 444 (9) is failed: must return 431457, returns: " << calc("864", "444", 9, "*") << "\n";
    failed++;
  }
  if (calc("jj", "2c", 27, "+") == "m4") {
    passed++;
  } else {
    std::cout << "Test jj + 2c (27) is failed: must return m4, returns: " << calc("jj", "2c", 27, "+") << "\n";
    failed++;
  }
  if (calc("13303", "233", 4, "*") == "11232131") {
    passed++;
  } else {
    std::cout << "Test 13303 * 233 (4) is failed: must return 11232131, returns: " << calc("13303", "233", 4, "*") << "\n";
    failed++;
  }
  if (calc("f6", "2g", 19, "*") == "25a1") {
    passed++;
  } else {
    std::cout << "Test f6 * 2g (19) is failed: must return 25a1, returns: " << calc("f6", "2g", 19, "*") << "\n";
    failed++;
  }
  if (calc("250", "152", 10, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 250 / 152 (10) is failed: must return 1, returns: " << calc("250", "152", 10, "/") << "\n";
    failed++;
  }
  if (calc("5o", "59", 25, "+") == "b8") {
    passed++;
  } else {
    std::cout << "Test 5o + 59 (25) is failed: must return b8, returns: " << calc("5o", "59", 25, "+") << "\n";
    failed++;
  }
  if (calc("37", "7", 23, "/") == "a") {
    passed++;
  } else {
    std::cout << "Test 37 / 7 (23) is failed: must return a, returns: " << calc("37", "7", 23, "/") << "\n";
    failed++;
  }
  if (calc("155", "104", 7, "*") == "16516") {
    passed++;
  } else {
    std::cout << "Test 155 * 104 (7) is failed: must return 16516, returns: " << calc("155", "104", 7, "*") << "\n";
    failed++;
  }
  if (calc("c4", "9f", 27, "*") == "4826") {
    passed++;
  } else {
    std::cout << "Test c4 * 9f (27) is failed: must return 4826, returns: " << calc("c4", "9f", 27, "*") << "\n";
    failed++;
  }
  if (calc("6g", "2v", 32, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 6g / 2v (32) is failed: must return 2, returns: " << calc("6g", "2v", 32, "/") << "\n";
    failed++;
  }
  if (calc("1jd", "1hd", 20, "-") == "20") {
    passed++;
  } else {
    std::cout << "Test 1jd - 1hd (20) is failed: must return 20, returns: " << calc("1jd", "1hd", 20, "-") << "\n";
    failed++;
  }
  if (calc("rb", "kq", 35, "+") == "1d2") {
    passed++;
  } else {
    std::cout << "Test rb + kq (35) is failed: must return 1d2, returns: " << calc("rb", "kq", 35, "+") << "\n";
    failed++;
  }
  if (calc("1205", "416", 7, "+") == "1624") {
    passed++;
  } else {
    std::cout << "Test 1205 + 416 (7) is failed: must return 1624, returns: " << calc("1205", "416", 7, "+") << "\n";
    failed++;
  }
  if (calc("829", "806", 11, "-") == "23") {
    passed++;
  } else {
    std::cout << "Test 829 - 806 (11) is failed: must return 23, returns: " << calc("829", "806", 11, "-") << "\n";
    failed++;
  }
  if (calc("1133", "202", 6, "-") == "531") {
    passed++;
  } else {
    std::cout << "Test 1133 - 202 (6) is failed: must return 531, returns: " << calc("1133", "202", 6, "-") << "\n";
    failed++;
  }
  if (calc("ra", "4c", 33, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test ra / 4c (33) is failed: must return 6, returns: " << calc("ra", "4c", 33, "/") << "\n";
    failed++;
  }
  if (calc("365", "2b9", 12, "*") == "a6449") {
    passed++;
  } else {
    std::cout << "Test 365 * 2b9 (12) is failed: must return a6449, returns: " << calc("365", "2b9", 12, "*") << "\n";
    failed++;
  }
  if (calc("1516", "1251", 8, "+") == "2767") {
    passed++;
  } else {
    std::cout << "Test 1516 + 1251 (8) is failed: must return 2767, returns: " << calc("1516", "1251", 8, "+") << "\n";
    failed++;
  }
  if (calc("jg", "e7", 25, "*") == "b5bc") {
    passed++;
  } else {
    std::cout << "Test jg * e7 (25) is failed: must return b5bc, returns: " << calc("jg", "e7", 25, "*") << "\n";
    failed++;
  }
  if (calc("10", "4", 24, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 10 / 4 (24) is failed: must return 6, returns: " << calc("10", "4", 24, "/") << "\n";
    failed++;
  }
  if (calc("6a2", "255", 11, "+") == "947") {
    passed++;
  } else {
    std::cout << "Test 6a2 + 255 (11) is failed: must return 947, returns: " << calc("6a2", "255", 11, "+") << "\n";
    failed++;
  }
  if (calc("qk", "b8", 35, "*") == "8ick") {
    passed++;
  } else {
    std::cout << "Test qk * b8 (35) is failed: must return 8ick, returns: " << calc("qk", "b8", 35, "*") << "\n";
    failed++;
  }
  if (calc("jb", "7f", 23, "+") == "143") {
    passed++;
  } else {
    std::cout << "Test jb + 7f (23) is failed: must return 143, returns: " << calc("jb", "7f", 23, "+") << "\n";
    failed++;
  }
  if (calc("b0", "a0", 18, "*") == "6200") {
    passed++;
  } else {
    std::cout << "Test b0 * a0 (18) is failed: must return 6200, returns: " << calc("b0", "a0", 18, "*") << "\n";
    failed++;
  }
  if (calc("2516", "2360", 7, "+") == "5206") {
    passed++;
  } else {
    std::cout << "Test 2516 + 2360 (7) is failed: must return 5206, returns: " << calc("2516", "2360", 7, "+") << "\n";
    failed++;
  }
  if (calc("12", "5", 21, "*") == "5a") {
    passed++;
  } else {
    std::cout << "Test 12 * 5 (21) is failed: must return 5a, returns: " << calc("12", "5", 21, "*") << "\n";
    failed++;
  }
  if (calc("2010", "1025", 6, "-") == "541") {
    passed++;
  } else {
    std::cout << "Test 2010 - 1025 (6) is failed: must return 541, returns: " << calc("2010", "1025", 6, "-") << "\n";
    failed++;
  }
  if (calc("ho", "4a", 31, "*") == "2epn") {
    passed++;
  } else {
    std::cout << "Test ho * 4a (31) is failed: must return 2epn, returns: " << calc("ho", "4a", 31, "*") << "\n";
    failed++;
  }
  if (calc("15c", "114", 17, "+") == "26g") {
    passed++;
  } else {
    std::cout << "Test 15c + 114 (17) is failed: must return 26g, returns: " << calc("15c", "114", 17, "+") << "\n";
    failed++;
  }
  if (calc("1ei", "n", 24, "-") == "1dj") {
    passed++;
  } else {
    std::cout << "Test 1ei - n (24) is failed: must return 1dj, returns: " << calc("1ei", "n", 24, "-") << "\n";
    failed++;
  }
  if (calc("i2", "27", 32, "*") == "182e") {
    passed++;
  } else {
    std::cout << "Test i2 * 27 (32) is failed: must return 182e, returns: " << calc("i2", "27", 32, "*") << "\n";
    failed++;
  }
  if (calc("303", "201", 5, "*") == "111403") {
    passed++;
  } else {
    std::cout << "Test 303 * 201 (5) is failed: must return 111403, returns: " << calc("303", "201", 5, "*") << "\n";
    failed++;
  }
  if (calc("60", "2a", 16, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 60 / 2a (16) is failed: must return 2, returns: " << calc("60", "2a", 16, "/") << "\n";
    failed++;
  }
  if (calc("2c8", "12b", 15, "*") == "3543d") {
    passed++;
  } else {
    std::cout << "Test 2c8 * 12b (15) is failed: must return 3543d, returns: " << calc("2c8", "12b", 15, "*") << "\n";
    failed++;
  }
  if (calc("13l", "qc", 28, "-") == "59") {
    passed++;
  } else {
    std::cout << "Test 13l - qc (28) is failed: must return 59, returns: " << calc("13l", "qc", 28, "-") << "\n";
    failed++;
  }
  if (calc("r1", "cg", 28, "-") == "ed") {
    passed++;
  } else {
    std::cout << "Test r1 - cg (28) is failed: must return ed, returns: " << calc("r1", "cg", 28, "-") << "\n";
    failed++;
  }
  if (calc("54", "28", 26, "+") == "7c") {
    passed++;
  } else {
    std::cout << "Test 54 + 28 (26) is failed: must return 7c, returns: " << calc("54", "28", 26, "+") << "\n";
    failed++;
  }
  if (calc("121", "ch", 21, "*") == "e14h") {
    passed++;
  } else {
    std::cout << "Test 121 * ch (21) is failed: must return e14h, returns: " << calc("121", "ch", 21, "*") << "\n";
    failed++;
  }
  if (calc("i6", "4h", 25, "+") == "mn") {
    passed++;
  } else {
    std::cout << "Test i6 + 4h (25) is failed: must return mn, returns: " << calc("i6", "4h", 25, "+") << "\n";
    failed++;
  }
  if (calc("1146", "541", 7, "-") == "305") {
    passed++;
  } else {
    std::cout << "Test 1146 - 541 (7) is failed: must return 305, returns: " << calc("1146", "541", 7, "-") << "\n";
    failed++;
  }
  if (calc("1hg", "74", 21, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test 1hg / 74 (21) is failed: must return 5, returns: " << calc("1hg", "74", 21, "/") << "\n";
    failed++;
  }
  if (calc("84", "6h", 35, "*") == "1hlx") {
    passed++;
  } else {
    std::cout << "Test 84 * 6h (35) is failed: must return 1hlx, returns: " << calc("84", "6h", 35, "*") << "\n";
    failed++;
  }
  if (calc("2130", "13", 7, "*") == "31020") {
    passed++;
  } else {
    std::cout << "Test 2130 * 13 (7) is failed: must return 31020, returns: " << calc("2130", "13", 7, "*") << "\n";
    failed++;
  }
  if (calc("146", "7", 27, "*") == "82f") {
    passed++;
  } else {
    std::cout << "Test 146 * 7 (27) is failed: must return 82f, returns: " << calc("146", "7", 27, "*") << "\n";
    failed++;
  }
  if (calc("18e", "32", 22, "-") == "15c") {
    passed++;
  } else {
    std::cout << "Test 18e - 32 (22) is failed: must return 15c, returns: " << calc("18e", "32", 22, "-") << "\n";
    failed++;
  }
  if (calc("89", "6", 20, "+") == "8f") {
    passed++;
  } else {
    std::cout << "Test 89 + 6 (20) is failed: must return 8f, returns: " << calc("89", "6", 20, "+") << "\n";
    failed++;
  }
  if (calc("15a", "j9", 21, "*") == "13a86") {
    passed++;
  } else {
    std::cout << "Test 15a * j9 (21) is failed: must return 13a86, returns: " << calc("15a", "j9", 21, "*") << "\n";
    failed++;
  }
  if (calc("44c", "20", 14, "+") == "46c") {
    passed++;
  } else {
    std::cout << "Test 44c + 20 (14) is failed: must return 46c, returns: " << calc("44c", "20", 14, "+") << "\n";
    failed++;
  }
  if (calc("ui", "nm", 31, "*") == "nc1o") {
    passed++;
  } else {
    std::cout << "Test ui * nm (31) is failed: must return nc1o, returns: " << calc("ui", "nm", 31, "*") << "\n";
    failed++;
  }
  if (calc("341", "69", 15, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test 341 / 69 (15) is failed: must return 7, returns: " << calc("341", "69", 15, "/") << "\n";
    failed++;
  }
  if (calc("s3", "m0", 33, "-") == "63") {
    passed++;
  } else {
    std::cout << "Test s3 - m0 (33) is failed: must return 63, returns: " << calc("s3", "m0", 33, "-") << "\n";
    failed++;
  }
  if (calc("a7", "2p", 29, "-") == "7b") {
    passed++;
  } else {
    std::cout << "Test a7 - 2p (29) is failed: must return 7b, returns: " << calc("a7", "2p", 29, "-") << "\n";
    failed++;
  }
  if (calc("272", "1g8", 17, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 272 / 1g8 (17) is failed: must return 1, returns: " << calc("272", "1g8", 17, "/") << "\n";
    failed++;
  }
  if (calc("409", "27", 10, "*") == "11043") {
    passed++;
  } else {
    std::cout << "Test 409 * 27 (10) is failed: must return 11043, returns: " << calc("409", "27", 10, "*") << "\n";
    failed++;
  }
  if (calc("129", "jb", 20, "*") == "11ihj") {
    passed++;
  } else {
    std::cout << "Test 129 * jb (20) is failed: must return 11ihj, returns: " << calc("129", "jb", 20, "*") << "\n";
    failed++;
  }
  if (calc("b9", "aa", 22, "*") == "5962") {
    passed++;
  } else {
    std::cout << "Test b9 * aa (22) is failed: must return 5962, returns: " << calc("b9", "aa", 22, "*") << "\n";
    failed++;
  }
  if (calc("k9", "j5", 31, "+") == "18e") {
    passed++;
  } else {
    std::cout << "Test k9 + j5 (31) is failed: must return 18e, returns: " << calc("k9", "j5", 31, "+") << "\n";
    failed++;
  }
  if (calc("a9", "9v", 32, "*") == "36fn") {
    passed++;
  } else {
    std::cout << "Test a9 * 9v (32) is failed: must return 36fn, returns: " << calc("a9", "9v", 32, "*") << "\n";
    failed++;
  }
  if (calc("2423", "1520", 7, "*") == "4412560") {
    passed++;
  } else {
    std::cout << "Test 2423 * 1520 (7) is failed: must return 4412560, returns: " << calc("2423", "1520", 7, "*") << "\n";
    failed++;
  }
  if (calc("349", "1bb", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 349 / 1bb (14) is failed: must return 1, returns: " << calc("349", "1bb", 14, "/") << "\n";
    failed++;
  }
  if (calc("338", "48", 13, "-") == "2c0") {
    passed++;
  } else {
    std::cout << "Test 338 - 48 (13) is failed: must return 2c0, returns: " << calc("338", "48", 13, "-") << "\n";
    failed++;
  }
  if (calc("479", "298", 10, "+") == "777") {
    passed++;
  } else {
    std::cout << "Test 479 + 298 (10) is failed: must return 777, returns: " << calc("479", "298", 10, "+") << "\n";
    failed++;
  }
  if (calc("19", "14", 16, "+") == "2d") {
    passed++;
  } else {
    std::cout << "Test 19 + 14 (16) is failed: must return 2d, returns: " << calc("19", "14", 16, "+") << "\n";
    failed++;
  }
  if (calc("a4", "8f", 26, "+") == "ij") {
    passed++;
  } else {
    std::cout << "Test a4 + 8f (26) is failed: must return ij, returns: " << calc("a4", "8f", 26, "+") << "\n";
    failed++;
  }
  if (calc("ek", "a7", 34, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ek / a7 (34) is failed: must return 1, returns: " << calc("ek", "a7", 34, "/") << "\n";
    failed++;
  }
  if (calc("113", "101", 11, "+") == "214") {
    passed++;
  } else {
    std::cout << "Test 113 + 101 (11) is failed: must return 214, returns: " << calc("113", "101", 11, "+") << "\n";
    failed++;
  }
  if (calc("a9", "1m", 29, "-") == "8g") {
    passed++;
  } else {
    std::cout << "Test a9 - 1m (29) is failed: must return 8g, returns: " << calc("a9", "1m", 29, "-") << "\n";
    failed++;
  }
  if (calc("4o", "12", 33, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 4o / 12 (33) is failed: must return 4, returns: " << calc("4o", "12", 33, "/") << "\n";
    failed++;
  }
  if (calc("12103", "1211", 4, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 12103 / 1211 (4) is failed: must return 3, returns: " << calc("12103", "1211", 4, "/") << "\n";
    failed++;
  }
  if (calc("100", "3", 4, "-") == "31") {
    passed++;
  } else {
    std::cout << "Test 100 - 3 (4) is failed: must return 31, returns: " << calc("100", "3", 4, "-") << "\n";
    failed++;
  }
  if (calc("8k", "1g", 22, "/") == "5") {
    passed++;
  } else {
    std::cout << "Test 8k / 1g (22) is failed: must return 5, returns: " << calc("8k", "1g", 22, "/") << "\n";
    failed++;
  }
  if (calc("111001010", "111011", 2, "/") == "111") {
    passed++;
  } else {
    std::cout << "Test 111001010 / 111011 (2) is failed: must return 111, returns: " << calc("111001010", "111011", 2, "/") << "\n";
    failed++;
  }
  if (calc("2132", "1140", 5, "+") == "3322") {
    passed++;
  } else {
    std::cout << "Test 2132 + 1140 (5) is failed: must return 3322, returns: " << calc("2132", "1140", 5, "+") << "\n";
    failed++;
  }
  if (calc("405", "1de", 15, "+") == "5e4") {
    passed++;
  } else {
    std::cout << "Test 405 + 1de (15) is failed: must return 5e4, returns: " << calc("405", "1de", 15, "+") << "\n";
    failed++;
  }
  if (calc("4i", "5", 29, "+") == "4n") {
    passed++;
  } else {
    std::cout << "Test 4i + 5 (29) is failed: must return 4n, returns: " << calc("4i", "5", 29, "+") << "\n";
    failed++;
  }
  if (calc("4c2", "297", 13, "-") == "228") {
    passed++;
  } else {
    std::cout << "Test 4c2 - 297 (13) is failed: must return 228, returns: " << calc("4c2", "297", 13, "-") << "\n";
    failed++;
  }
  if (calc("rg", "qa", 30, "-") == "16") {
    passed++;
  } else {
    std::cout << "Test rg - qa (30) is failed: must return 16, returns: " << calc("rg", "qa", 30, "-") << "\n";
    failed++;
  }
  if (calc("dd", "4a", 27, "+") == "hn") {
    passed++;
  } else {
    std::cout << "Test dd + 4a (27) is failed: must return hn, returns: " << calc("dd", "4a", 27, "+") << "\n";
    failed++;
  }
  if (calc("2032", "1232", 5, "-") == "300") {
    passed++;
  } else {
    std::cout << "Test 2032 - 1232 (5) is failed: must return 300, returns: " << calc("2032", "1232", 5, "-") << "\n";
    failed++;
  }
  if (calc("20d", "10i", 19, "+") == "31c") {
    passed++;
  } else {
    std::cout << "Test 20d + 10i (19) is failed: must return 31c, returns: " << calc("20d", "10i", 19, "+") << "\n";
    failed++;
  }
  if (calc("56", "3", 29, "*") == "fi") {
    passed++;
  } else {
    std::cout << "Test 56 * 3 (29) is failed: must return fi, returns: " << calc("56", "3", 29, "*") << "\n";
    failed++;
  }
  if (calc("84", "74", 24, "+") == "f8") {
    passed++;
  } else {
    std::cout << "Test 84 + 74 (24) is failed: must return f8, returns: " << calc("84", "74", 24, "+") << "\n";
    failed++;
  }
  if (calc("3bb", "2ab", 12, "+") == "6aa") {
    passed++;
  } else {
    std::cout << "Test 3bb + 2ab (12) is failed: must return 6aa, returns: " << calc("3bb", "2ab", 12, "+") << "\n";
    failed++;
  }
  if (calc("5s", "33", 31, "-") == "2p") {
    passed++;
  } else {
    std::cout << "Test 5s - 33 (31) is failed: must return 2p, returns: " << calc("5s", "33", 31, "-") << "\n";
    failed++;
  }
  if (calc("ju", "9l", 32, "+") == "tj") {
    passed++;
  } else {
    std::cout << "Test ju + 9l (32) is failed: must return tj, returns: " << calc("ju", "9l", 32, "+") << "\n";
    failed++;
  }
  if (calc("2h", "2b", 31, "+") == "4s") {
    passed++;
  } else {
    std::cout << "Test 2h + 2b (31) is failed: must return 4s, returns: " << calc("2h", "2b", 31, "+") << "\n";
    failed++;
  }
  if (calc("17h", "12c", 19, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 17h / 12c (19) is failed: must return 1, returns: " << calc("17h", "12c", 19, "/") << "\n";
    failed++;
  }
  if (calc("eq", "49", 29, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test eq / 49 (29) is failed: must return 3, returns: " << calc("eq", "49", 29, "/") << "\n";
    failed++;
  }
  if (calc("4e", "43", 17, "-") == "b") {
    passed++;
  } else {
    std::cout << "Test 4e - 43 (17) is failed: must return b, returns: " << calc("4e", "43", 17, "-") << "\n";
    failed++;
  }
  if (calc("ct", "4", 32, "/") == "37") {
    passed++;
  } else {
    std::cout << "Test ct / 4 (32) is failed: must return 37, returns: " << calc("ct", "4", 32, "/") << "\n";
    failed++;
  }
  if (calc("14", "4", 16, "-") == "10") {
    passed++;
  } else {
    std::cout << "Test 14 - 4 (16) is failed: must return 10, returns: " << calc("14", "4", 16, "-") << "\n";
    failed++;
  }
  if (calc("2214", "224", 5, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 2214 / 224 (5) is failed: must return 4, returns: " << calc("2214", "224", 5, "/") << "\n";
    failed++;
  }
  if (calc("1g0", "f1", 19, "-") == "10i") {
    passed++;
  } else {
    std::cout << "Test 1g0 - f1 (19) is failed: must return 10i, returns: " << calc("1g0", "f1", 19, "-") << "\n";
    failed++;
  }
  if (calc("42", "26", 9, "-") == "15") {
    passed++;
  } else {
    std::cout << "Test 42 - 26 (9) is failed: must return 15, returns: " << calc("42", "26", 9, "-") << "\n";
    failed++;
  }
  if (calc("g3", "4h", 23, "-") == "b9") {
    passed++;
  } else {
    std::cout << "Test g3 - 4h (23) is failed: must return b9, returns: " << calc("g3", "4h", 23, "-") << "\n";
    failed++;
  }
  if (calc("a2", "97", 23, "-") == "i") {
    passed++;
  } else {
    std::cout << "Test a2 - 97 (23) is failed: must return i, returns: " << calc("a2", "97", 23, "-") << "\n";
    failed++;
  }
  if (calc("122211", "11221", 3, "*") == "10020202101") {
    passed++;
  } else {
    std::cout << "Test 122211 * 11221 (3) is failed: must return 10020202101, returns: " << calc("122211", "11221", 3, "*") << "\n";
    failed++;
  }
  if (calc("nv", "bi", 33, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test nv / bi (33) is failed: must return 2, returns: " << calc("nv", "bi", 33, "/") << "\n";
    failed++;
  }
  if (calc("i", "3", 24, "+") == "l") {
    passed++;
  } else {
    std::cout << "Test i + 3 (24) is failed: must return l, returns: " << calc("i", "3", 24, "+") << "\n";
    failed++;
  }
  if (calc("3m", "1h", 23, "+") == "5g") {
    passed++;
  } else {
    std::cout << "Test 3m + 1h (23) is failed: must return 5g, returns: " << calc("3m", "1h", 23, "+") << "\n";
    failed++;
  }
  if (calc("490", "343", 11, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 490 / 343 (11) is failed: must return 1, returns: " << calc("490", "343", 11, "/") << "\n";
    failed++;
  }
  if (calc("52", "2", 25, "-") == "50") {
    passed++;
  } else {
    std::cout << "Test 52 - 2 (25) is failed: must return 50, returns: " << calc("52", "2", 25, "-") << "\n";
    failed++;
  }
  if (calc("265", "h2", 18, "-") == "173") {
    passed++;
  } else {
    std::cout << "Test 265 - h2 (18) is failed: must return 173, returns: " << calc("265", "h2", 18, "-") << "\n";
    failed++;
  }
  if (calc("987", "855", 10, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 987 / 855 (10) is failed: must return 1, returns: " << calc("987", "855", 10, "/") << "\n";
    failed++;
  }
  if (calc("20a", "17e", 17, "+") == "387") {
    passed++;
  } else {
    std::cout << "Test 20a + 17e (17) is failed: must return 387, returns: " << calc("20a", "17e", 17, "+") << "\n";
    failed++;
  }
  if (calc("2c", "0", 13, "*") == "0") {
    passed++;
  } else {
    std::cout << "Test 2c * 0 (13) is failed: must return 0, returns: " << calc("2c", "0", 13, "*") << "\n";
    failed++;
  }
  if (calc("db", "a8", 31, "-") == "33") {
    passed++;
  } else {
    std::cout << "Test db - a8 (31) is failed: must return 33, returns: " << calc("db", "a8", 31, "-") << "\n";
    failed++;
  }
  if (calc("3e6", "169", 16, "-") == "27d") {
    passed++;
  } else {
    std::cout << "Test 3e6 - 169 (16) is failed: must return 27d, returns: " << calc("3e6", "169", 16, "-") << "\n";
    failed++;
  }
  if (calc("471", "10", 10, "*") == "4710") {
    passed++;
  } else {
    std::cout << "Test 471 * 10 (10) is failed: must return 4710, returns: " << calc("471", "10", 10, "*") << "\n";
    failed++;
  }
  if (calc("15i", "f1", 25, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 15i / f1 (25) is failed: must return 2, returns: " << calc("15i", "f1", 25, "/") << "\n";
    failed++;
  }
  if (calc("1d1", "33", 18, "+") == "1g4") {
    passed++;
  } else {
    std::cout << "Test 1d1 + 33 (18) is failed: must return 1g4, returns: " << calc("1d1", "33", 18, "+") << "\n";
    failed++;
  }
  if (calc("477", "435", 9, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 477 / 435 (9) is failed: must return 1, returns: " << calc("477", "435", 9, "/") << "\n";
    failed++;
  }
  if (calc("1cm", "il", 24, "-") == "i1") {
    passed++;
  } else {
    std::cout << "Test 1cm - il (24) is failed: must return i1, returns: " << calc("1cm", "il", 24, "-") << "\n";
    failed++;
  }
  if (calc("525", "438", 13, "*") == "192381") {
    passed++;
  } else {
    std::cout << "Test 525 * 438 (13) is failed: must return 192381, returns: " << calc("525", "438", 13, "*") << "\n";
    failed++;
  }
  if (calc("51", "1m", 32, "*") == "8fm") {
    passed++;
  } else {
    std::cout << "Test 51 * 1m (32) is failed: must return 8fm, returns: " << calc("51", "1m", 32, "*") << "\n";
    failed++;
  }
  if (calc("20c", "170", 22, "-") == "fc") {
    passed++;
  } else {
    std::cout << "Test 20c - 170 (22) is failed: must return fc, returns: " << calc("20c", "170", 22, "-") << "\n";
    failed++;
  }
  if (calc("25e", "15a", 20, "*") == "2i570") {
    passed++;
  } else {
    std::cout << "Test 25e * 15a (20) is failed: must return 2i570, returns: " << calc("25e", "15a", 20, "*") << "\n";
    failed++;
  }
  if (calc("69", "24", 11, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 69 / 24 (11) is failed: must return 2, returns: " << calc("69", "24", 11, "/") << "\n";
    failed++;
  }
  if (calc("7l", "32", 25, "+") == "an") {
    passed++;
  } else {
    std::cout << "Test 7l + 32 (25) is failed: must return an, returns: " << calc("7l", "32", 25, "+") << "\n";
    failed++;
  }
  if (calc("10310", "2012", 4, "-") == "2232") {
    passed++;
  } else {
    std::cout << "Test 10310 - 2012 (4) is failed: must return 2232, returns: " << calc("10310", "2012", 4, "-") << "\n";
    failed++;
  }
  if (calc("kh", "dd", 21, "-") == "74") {
    passed++;
  } else {
    std::cout << "Test kh - dd (21) is failed: must return 74, returns: " << calc("kh", "dd", 21, "-") << "\n";
    failed++;
  }
  if (calc("3102", "231", 6, "/") == "11") {
    passed++;
  } else {
    std::cout << "Test 3102 / 231 (6) is failed: must return 11, returns: " << calc("3102", "231", 6, "/") << "\n";
    failed++;
  }
  if (calc("gg", "d6", 24, "*") == "94k0") {
    passed++;
  } else {
    std::cout << "Test gg * d6 (24) is failed: must return 94k0, returns: " << calc("gg", "d6", 24, "*") << "\n";
    failed++;
  }
  if (calc("2c5", "2b5", 15, "*") == "7b9ba") {
    passed++;
  } else {
    std::cout << "Test 2c5 * 2b5 (15) is failed: must return 7b9ba, returns: " << calc("2c5", "2b5", 15, "*") << "\n";
    failed++;
  }
  if (calc("64", "18", 16, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 64 / 18 (16) is failed: must return 4, returns: " << calc("64", "18", 16, "/") << "\n";
    failed++;
  }
  if (calc("32010", "10312", 4, "-") == "21032") {
    passed++;
  } else {
    std::cout << "Test 32010 - 10312 (4) is failed: must return 21032, returns: " << calc("32010", "10312", 4, "-") << "\n";
    failed++;
  }
  if (calc("k4", "i", 22, "*") == "gb6") {
    passed++;
  } else {
    std::cout << "Test k4 * i (22) is failed: must return gb6, returns: " << calc("k4", "i", 22, "*") << "\n";
    failed++;
  }
  if (calc("107", "6e", 16, "*") == "7102") {
    passed++;
  } else {
    std::cout << "Test 107 * 6e (16) is failed: must return 7102, returns: " << calc("107", "6e", 16, "*") << "\n";
    failed++;
  }
  if (calc("h7", "g3", 24, "+") == "19a") {
    passed++;
  } else {
    std::cout << "Test h7 + g3 (24) is failed: must return 19a, returns: " << calc("h7", "g3", 24, "+") << "\n";
    failed++;
  }
  if (calc("73", "1m", 25, "+") == "90") {
    passed++;
  } else {
    std::cout << "Test 73 + 1m (25) is failed: must return 90, returns: " << calc("73", "1m", 25, "+") << "\n";
    failed++;
  }
  if (calc("874", "618", 10, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 874 / 618 (10) is failed: must return 1, returns: " << calc("874", "618", 10, "/") << "\n";
    failed++;
  }
  if (calc("1365", "705", 8, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1365 / 705 (8) is failed: must return 1, returns: " << calc("1365", "705", 8, "/") << "\n";
    failed++;
  }
  if (calc("jr", "gg", 33, "*") == "9tn3") {
    passed++;
  } else {
    std::cout << "Test jr * gg (33) is failed: must return 9tn3, returns: " << calc("jr", "gg", 33, "*") << "\n";
    failed++;
  }
  if (calc("ad", "6a", 14, "+") == "139") {
    passed++;
  } else {
    std::cout << "Test ad + 6a (14) is failed: must return 139, returns: " << calc("ad", "6a", 14, "+") << "\n";
    failed++;
  }
  if (calc("2241", "2141", 6, "-") == "100") {
    passed++;
  } else {
    std::cout << "Test 2241 - 2141 (6) is failed: must return 100, returns: " << calc("2241", "2141", 6, "-") << "\n";
    failed++;
  }
  if (calc("74", "j", 35, "/") == "d") {
    passed++;
  } else {
    std::cout << "Test 74 / j (35) is failed: must return d, returns: " << calc("74", "j", 35, "/") << "\n";
    failed++;
  }
  if (calc("134", "114", 5, "-") == "20") {
    passed++;
  } else {
    std::cout << "Test 134 - 114 (5) is failed: must return 20, returns: " << calc("134", "114", 5, "-") << "\n";
    failed++;
  }
  if (calc("201", "15g", 17, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 201 / 15g (17) is failed: must return 1, returns: " << calc("201", "15g", 17, "/") << "\n";
    failed++;
  }
  if (calc("uf", "74", 31, "*") == "709t") {
    passed++;
  } else {
    std::cout << "Test uf * 74 (31) is failed: must return 709t, returns: " << calc("uf", "74", 31, "*") << "\n";
    failed++;
  }
  if (calc("g7", "4i", 27, "-") == "bg") {
    passed++;
  } else {
    std::cout << "Test g7 - 4i (27) is failed: must return bg, returns: " << calc("g7", "4i", 27, "-") << "\n";
    failed++;
  }
  if (calc("1117", "200", 8, "-") == "717") {
    passed++;
  } else {
    std::cout << "Test 1117 - 200 (8) is failed: must return 717, returns: " << calc("1117", "200", 8, "-") << "\n";
    failed++;
  }
  if (calc("1300", "230", 8, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 1300 / 230 (8) is failed: must return 4, returns: " << calc("1300", "230", 8, "/") << "\n";
    failed++;
  }
  if (calc("10022", "121", 4, "/") == "22") {
    passed++;
  } else {
    std::cout << "Test 10022 / 121 (4) is failed: must return 22, returns: " << calc("10022", "121", 4, "/") << "\n";
    failed++;
  }
  if (calc("2124", "2112", 5, "*") == "10101443") {
    passed++;
  } else {
    std::cout << "Test 2124 * 2112 (5) is failed: must return 10101443, returns: " << calc("2124", "2112", 5, "*") << "\n";
    failed++;
  }
  if (calc("3c8", "13a", 15, "+") == "513") {
    passed++;
  } else {
    std::cout << "Test 3c8 + 13a (15) is failed: must return 513, returns: " << calc("3c8", "13a", 15, "+") << "\n";
    failed++;
  }
  if (calc("191", "96", 13, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 191 / 96 (13) is failed: must return 2, returns: " << calc("191", "96", 13, "/") << "\n";
    failed++;
  }
  if (calc("8w", "8j", 33, "+") == "hi") {
    passed++;
  } else {
    std::cout << "Test 8w + 8j (33) is failed: must return hi, returns: " << calc("8w", "8j", 33, "+") << "\n";
    failed++;
  }
  if (calc("4a3", "3b9", 12, "*") == "173953") {
    passed++;
  } else {
    std::cout << "Test 4a3 * 3b9 (12) is failed: must return 173953, returns: " << calc("4a3", "3b9", 12, "*") << "\n";
    failed++;
  }
  if (calc("k1", "gm", 24, "+") == "1cn") {
    passed++;
  } else {
    std::cout << "Test k1 + gm (24) is failed: must return 1cn, returns: " << calc("k1", "gm", 24, "+") << "\n";
    failed++;
  }
  if (calc("pf", "em", 31, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test pf / em (31) is failed: must return 1, returns: " << calc("pf", "em", 31, "/") << "\n";
    failed++;
  }
  if (calc("rk", "82", 31, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test rk / 82 (31) is failed: must return 3, returns: " << calc("rk", "82", 31, "/") << "\n";
    failed++;
  }
  if (calc("86", "6s", 32, "+") == "f2") {
    passed++;
  } else {
    std::cout << "Test 86 + 6s (32) is failed: must return f2, returns: " << calc("86", "6s", 32, "+") << "\n";
    failed++;
  }
  if (calc("41", "13", 24, "-") == "2m") {
    passed++;
  } else {
    std::cout << "Test 41 - 13 (24) is failed: must return 2m, returns: " << calc("41", "13", 24, "-") << "\n";
    failed++;
  }
  if (calc("583", "33b", 12, "-") == "244") {
    passed++;
  } else {
    std::cout << "Test 583 - 33b (12) is failed: must return 244, returns: " << calc("583", "33b", 12, "-") << "\n";
    failed++;
  }
  if (calc("3d", "34", 23, "+") == "6h") {
    passed++;
  } else {
    std::cout << "Test 3d + 34 (23) is failed: must return 6h, returns: " << calc("3d", "34", 23, "+") << "\n";
    failed++;
  }
  if (calc("18o", "7a", 27, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 18o / 7a (27) is failed: must return 4, returns: " << calc("18o", "7a", 27, "/") << "\n";
    failed++;
  }
  if (calc("43", "2f", 18, "*") == "be9") {
    passed++;
  } else {
    std::cout << "Test 43 * 2f (18) is failed: must return be9, returns: " << calc("43", "2f", 18, "*") << "\n";
    failed++;
  }
  if (calc("fh", "8", 22, "*") == "5g4") {
    passed++;
  } else {
    std::cout << "Test fh * 8 (22) is failed: must return 5g4, returns: " << calc("fh", "8", 22, "*") << "\n";
    failed++;
  }
  if (calc("g9", "61", 26, "*") == "3ki9") {
    passed++;
  } else {
    std::cout << "Test g9 * 61 (26) is failed: must return 3ki9, returns: " << calc("g9", "61", 26, "*") << "\n";
    failed++;
  }
  if (calc("11003", "10332", 5, "*") == "114234101") {
    passed++;
  } else {
    std::cout << "Test 11003 * 10332 (5) is failed: must return 114234101, returns: " << calc("11003", "10332", 5, "*") << "\n";
    failed++;
  }
  if (calc("1253", "230", 8, "-") == "1023") {
    passed++;
  } else {
    std::cout << "Test 1253 - 230 (8) is failed: must return 1023, returns: " << calc("1253", "230", 8, "-") << "\n";
    failed++;
  }
  if (calc("15a", "159", 13, "*") == "20cac") {
    passed++;
  } else {
    std::cout << "Test 15a * 159 (13) is failed: must return 20cac, returns: " << calc("15a", "159", 13, "*") << "\n";
    failed++;
  }
  if (calc("55", "1l", 30, "*") == "8nf") {
    passed++;
  } else {
    std::cout << "Test 55 * 1l (30) is failed: must return 8nf, returns: " << calc("55", "1l", 30, "*") << "\n";
    failed++;
  }
  if (calc("466", "63", 12, "-") == "403") {
    passed++;
  } else {
    std::cout << "Test 466 - 63 (12) is failed: must return 403, returns: " << calc("466", "63", 12, "-") << "\n";
    failed++;
  }
  if (calc("d6", "l", 28, "-") == "cd") {
    passed++;
  } else {
    std::cout << "Test d6 - l (28) is failed: must return cd, returns: " << calc("d6", "l", 28, "-") << "\n";
    failed++;
  }
  if (calc("1df", "136", 20, "*") == "1j67a") {
    passed++;
  } else {
    std::cout << "Test 1df * 136 (20) is failed: must return 1j67a, returns: " << calc("1df", "136", 20, "*") << "\n";
    failed++;
  }
  if (calc("6h", "7", 26, "+") == "6o") {
    passed++;
  } else {
    std::cout << "Test 6h + 7 (26) is failed: must return 6o, returns: " << calc("6h", "7", 26, "+") << "\n";
    failed++;
  }
  if (calc("139", "hg", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 139 / hg (26) is failed: must return 1, returns: " << calc("139", "hg", 26, "/") << "\n";
    failed++;
  }
  if (calc("230", "27", 16, "/") == "e") {
    passed++;
  } else {
    std::cout << "Test 230 / 27 (16) is failed: must return e, returns: " << calc("230", "27", 16, "/") << "\n";
    failed++;
  }
  if (calc("7w", "5v", 35, "-") == "21") {
    passed++;
  } else {
    std::cout << "Test 7w - 5v (35) is failed: must return 21, returns: " << calc("7w", "5v", 35, "-") << "\n";
    failed++;
  }
  if (calc("79", "3g", 17, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 79 / 3g (17) is failed: must return 1, returns: " << calc("79", "3g", 17, "/") << "\n";
    failed++;
  }
  if (calc("g2", "fg", 29, "*") == "8hq3") {
    passed++;
  } else {
    std::cout << "Test g2 * fg (29) is failed: must return 8hq3, returns: " << calc("g2", "fg", 29, "*") << "\n";
    failed++;
  }
  if (calc("re", "pl", 29, "*") == "oas4") {
    passed++;
  } else {
    std::cout << "Test re * pl (29) is failed: must return oas4, returns: " << calc("re", "pl", 29, "*") << "\n";
    failed++;
  }
  if (calc("101110", "10111", 2, "*") == "10000100010") {
    passed++;
  } else {
    std::cout << "Test 101110 * 10111 (2) is failed: must return 10000100010, returns: " << calc("101110", "10111", 2, "*") << "\n";
    failed++;
  }
  if (calc("84", "1", 10, "-") == "83") {
    passed++;
  } else {
    std::cout << "Test 84 - 1 (10) is failed: must return 83, returns: " << calc("84", "1", 10, "-") << "\n";
    failed++;
  }
  if (calc("142", "b8", 12, "-") == "46") {
    passed++;
  } else {
    std::cout << "Test 142 - b8 (12) is failed: must return 46, returns: " << calc("142", "b8", 12, "-") << "\n";
    failed++;
  }
  if (calc("421", "251", 9, "+") == "672") {
    passed++;
  } else {
    std::cout << "Test 421 + 251 (9) is failed: must return 672, returns: " << calc("421", "251", 9, "+") << "\n";
    failed++;
  }
  if (calc("l8", "id", 32, "-") == "2r") {
    passed++;
  } else {
    std::cout << "Test l8 - id (32) is failed: must return 2r, returns: " << calc("l8", "id", 32, "-") << "\n";
    failed++;
  }
  if (calc("16h", "hl", 25, "*") == "mf47") {
    passed++;
  } else {
    std::cout << "Test 16h * hl (25) is failed: must return mf47, returns: " << calc("16h", "hl", 25, "*") << "\n";
    failed++;
  }
  if (calc("121110", "10101", 3, "*") == "2001102110") {
    passed++;
  } else {
    std::cout << "Test 121110 * 10101 (3) is failed: must return 2001102110, returns: " << calc("121110", "10101", 3, "*") << "\n";
    failed++;
  }
  if (calc("f5", "1c", 34, "-") == "dr") {
    passed++;
  } else {
    std::cout << "Test f5 - 1c (34) is failed: must return dr, returns: " << calc("f5", "1c", 34, "-") << "\n";
    failed++;
  }
  if (calc("202", "75", 19, "+") == "277") {
    passed++;
  } else {
    std::cout << "Test 202 + 75 (19) is failed: must return 277, returns: " << calc("202", "75", 19, "+") << "\n";
    failed++;
  }
  if (calc("70", "4", 20, "+") == "74") {
    passed++;
  } else {
    std::cout << "Test 70 + 4 (20) is failed: must return 74, returns: " << calc("70", "4", 20, "+") << "\n";
    failed++;
  }
  if (calc("11", "a", 13, "*") == "aa") {
    passed++;
  } else {
    std::cout << "Test 11 * a (13) is failed: must return aa, returns: " << calc("11", "a", 13, "*") << "\n";
    failed++;
  }
  if (calc("gn", "dl", 35, "*") == "6gis") {
    passed++;
  } else {
    std::cout << "Test gn * dl (35) is failed: must return 6gis, returns: " << calc("gn", "dl", 35, "*") << "\n";
    failed++;
  }
  if (calc("2266", "1646", 7, "*") == "4513421") {
    passed++;
  } else {
    std::cout << "Test 2266 * 1646 (7) is failed: must return 4513421, returns: " << calc("2266", "1646", 7, "*") << "\n";
    failed++;
  }
  if (calc("51", "4g", 35, "+") == "9h") {
    passed++;
  } else {
    std::cout << "Test 51 + 4g (35) is failed: must return 9h, returns: " << calc("51", "4g", 35, "+") << "\n";
    failed++;
  }
  if (calc("17", "i", 20, "-") == "9") {
    passed++;
  } else {
    std::cout << "Test 17 - i (20) is failed: must return 9, returns: " << calc("17", "i", 20, "-") << "\n";
    failed++;
  }
  if (calc("1gg", "f4", 18, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1gg / f4 (18) is failed: must return 2, returns: " << calc("1gg", "f4", 18, "/") << "\n";
    failed++;
  }
  if (calc("210120", "100111", 3, "-") == "110002") {
    passed++;
  } else {
    std::cout << "Test 210120 - 100111 (3) is failed: must return 110002, returns: " << calc("210120", "100111", 3, "-") << "\n";
    failed++;
  }
  if (calc("1a0", "15b", 18, "+") == "2fb") {
    passed++;
  } else {
    std::cout << "Test 1a0 + 15b (18) is failed: must return 2fb, returns: " << calc("1a0", "15b", 18, "+") << "\n";
    failed++;
  }
  if (calc("bc", "2a", 26, "*") == "118g") {
    passed++;
  } else {
    std::cout << "Test bc * 2a (26) is failed: must return 118g, returns: " << calc("bc", "2a", 26, "*") << "\n";
    failed++;
  }
  if (calc("16d", "10g", 18, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 16d / 10g (18) is failed: must return 1, returns: " << calc("16d", "10g", 18, "/") << "\n";
    failed++;
  }
  if (calc("1110110110", "100110011", 2, "*") == "1000111001101000010") {
    passed++;
  } else {
    std::cout << "Test 1110110110 * 100110011 (2) is failed: must return 1000111001101000010, returns: " << calc("1110110110", "100110011", 2, "*") << "\n";
    failed++;
  }
  if (calc("129", "23", 16, "/") == "8") {
    passed++;
  } else {
    std::cout << "Test 129 / 23 (16) is failed: must return 8, returns: " << calc("129", "23", 16, "/") << "\n";
    failed++;
  }
  if (calc("cc", "2k", 25, "+") == "f7") {
    passed++;
  } else {
    std::cout << "Test cc + 2k (25) is failed: must return f7, returns: " << calc("cc", "2k", 25, "+") << "\n";
    failed++;
  }
  if (calc("1504", "1332", 6, "+") == "3240") {
    passed++;
  } else {
    std::cout << "Test 1504 + 1332 (6) is failed: must return 3240, returns: " << calc("1504", "1332", 6, "+") << "\n";
    failed++;
  }
  if (calc("4n", "32", 24, "+") == "81") {
    passed++;
  } else {
    std::cout << "Test 4n + 32 (24) is failed: must return 81, returns: " << calc("4n", "32", 24, "+") << "\n";
    failed++;
  }
  if (calc("101", "kj", 31, "*") == "kjkj") {
    passed++;
  } else {
    std::cout << "Test 101 * kj (31) is failed: must return kjkj, returns: " << calc("101", "kj", 31, "*") << "\n";
    failed++;
  }
  if (calc("tv", "9g", 32, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test tv / 9g (32) is failed: must return 3, returns: " << calc("tv", "9g", 32, "/") << "\n";
    failed++;
  }
  if (calc("90", "4m", 27, "*") == "1g90") {
    passed++;
  } else {
    std::cout << "Test 90 * 4m (27) is failed: must return 1g90, returns: " << calc("90", "4m", 27, "*") << "\n";
    failed++;
  }
  if (calc("62", "2i", 35, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 62 / 2i (35) is failed: must return 2, returns: " << calc("62", "2i", 35, "/") << "\n";
    failed++;
  }
  if (calc("lc", "46", 22, "*") == "4416") {
    passed++;
  } else {
    std::cout << "Test lc * 46 (22) is failed: must return 4416, returns: " << calc("lc", "46", 22, "*") << "\n";
    failed++;
  }
  if (calc("249", "1ce", 15, "+") == "428") {
    passed++;
  } else {
    std::cout << "Test 249 + 1ce (15) is failed: must return 428, returns: " << calc("249", "1ce", 15, "+") << "\n";
    failed++;
  }
  if (calc("1id", "60", 20, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test 1id / 60 (20) is failed: must return 6, returns: " << calc("1id", "60", 20, "/") << "\n";
    failed++;
  }
  if (calc("201202", "2210", 3, "*") == "2000211120") {
    passed++;
  } else {
    std::cout << "Test 201202 * 2210 (3) is failed: must return 2000211120, returns: " << calc("201202", "2210", 3, "*") << "\n";
    failed++;
  }
  if (calc("7g", "3e", 17, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 7g / 3e (17) is failed: must return 2, returns: " << calc("7g", "3e", 17, "/") << "\n";
    failed++;
  }
  if (calc("qe", "l1", 31, "-") == "5d") {
    passed++;
  } else {
    std::cout << "Test qe - l1 (31) is failed: must return 5d, returns: " << calc("qe", "l1", 31, "-") << "\n";
    failed++;
  }
  if (calc("10313", "3200", 4, "+") == "20113") {
    passed++;
  } else {
    std::cout << "Test 10313 + 3200 (4) is failed: must return 20113, returns: " << calc("10313", "3200", 4, "+") << "\n";
    failed++;
  }
  if (calc("458", "12a", 11, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 458 / 12a (11) is failed: must return 3, returns: " << calc("458", "12a", 11, "/") << "\n";
    failed++;
  }
  if (calc("280", "154", 10, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 280 / 154 (10) is failed: must return 1, returns: " << calc("280", "154", 10, "/") << "\n";
    failed++;
  }
  if (calc("2012", "1665", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 2012 / 1665 (7) is failed: must return 1, returns: " << calc("2012", "1665", 7, "/") << "\n";
    failed++;
  }
  if (calc("1026", "756", 8, "+") == "2004") {
    passed++;
  } else {
    std::cout << "Test 1026 + 756 (8) is failed: must return 2004, returns: " << calc("1026", "756", 8, "+") << "\n";
    failed++;
  }
  if (calc("qy", "m4", 35, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test qy / m4 (35) is failed: must return 1, returns: " << calc("qy", "m4", 35, "/") << "\n";
    failed++;
  }
  if (calc("1537", "677", 8, "+") == "2436") {
    passed++;
  } else {
    std::cout << "Test 1537 + 677 (8) is failed: must return 2436, returns: " << calc("1537", "677", 8, "+") << "\n";
    failed++;
  }
  if (calc("d1", "ai", 28, "*") == "4qki") {
    passed++;
  } else {
    std::cout << "Test d1 * ai (28) is failed: must return 4qki, returns: " << calc("d1", "ai", 28, "*") << "\n";
    failed++;
  }
  if (calc("123", "iq", 28, "-") == "b5") {
    passed++;
  } else {
    std::cout << "Test 123 - iq (28) is failed: must return b5, returns: " << calc("123", "iq", 28, "-") << "\n";
    failed++;
  }
  if (calc("1010121", "1000101", 3, "*") == "1011000022221") {
    passed++;
  } else {
    std::cout << "Test 1010121 * 1000101 (3) is failed: must return 1011000022221, returns: " << calc("1010121", "1000101", 3, "*") << "\n";
    failed++;
  }
  if (calc("15g", "6r", 28, "-") == "qh") {
    passed++;
  } else {
    std::cout << "Test 15g - 6r (28) is failed: must return qh, returns: " << calc("15g", "6r", 28, "-") << "\n";
    failed++;
  }
  if (calc("c8", "61", 26, "+") == "i9") {
    passed++;
  } else {
    std::cout << "Test c8 + 61 (26) is failed: must return i9, returns: " << calc("c8", "61", 26, "+") << "\n";
    failed++;
  }
  if (calc("319", "63", 16, "-") == "2b6") {
    passed++;
  } else {
    std::cout << "Test 319 - 63 (16) is failed: must return 2b6, returns: " << calc("319", "63", 16, "-") << "\n";
    failed++;
  }
  if (calc("4053", "421", 6, "-") == "3232") {
    passed++;
  } else {
    std::cout << "Test 4053 - 421 (6) is failed: must return 3232, returns: " << calc("4053", "421", 6, "-") << "\n";
    failed++;
  }
  if (calc("41a", "40c", 13, "*") == "13ab83") {
    passed++;
  } else {
    std::cout << "Test 41a * 40c (13) is failed: must return 13ab83, returns: " << calc("41a", "40c", 13, "*") << "\n";
    failed++;
  }
  if (calc("10b", "f4", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 10b / f4 (26) is failed: must return 1, returns: " << calc("10b", "f4", 26, "/") << "\n";
    failed++;
  }
  if (calc("pa", "13", 33, "/") == "n") {
    passed++;
  } else {
    std::cout << "Test pa / 13 (33) is failed: must return n, returns: " << calc("pa", "13", 33, "/") << "\n";
    failed++;
  }
  if (calc("1027", "113", 8, "/") == "7") {
    passed++;
  } else {
    std::cout << "Test 1027 / 113 (8) is failed: must return 7, returns: " << calc("1027", "113", 8, "/") << "\n";
    failed++;
  }
  if (calc("2cd", "29b", 17, "-") == "32") {
    passed++;
  } else {
    std::cout << "Test 2cd - 29b (17) is failed: must return 32, returns: " << calc("2cd", "29b", 17, "-") << "\n";
    failed++;
  }
  if (calc("4e", "2", 18, "+") == "4g") {
    passed++;
  } else {
    std::cout << "Test 4e + 2 (18) is failed: must return 4g, returns: " << calc("4e", "2", 18, "+") << "\n";
    failed++;
  }
  if (calc("1115", "543", 8, "+") == "1660") {
    passed++;
  } else {
    std::cout << "Test 1115 + 543 (8) is failed: must return 1660, returns: " << calc("1115", "543", 8, "+") << "\n";
    failed++;
  }
  if (calc("147", "73", 19, "+") == "1ba") {
    passed++;
  } else {
    std::cout << "Test 147 + 73 (19) is failed: must return 1ba, returns: " << calc("147", "73", 19, "+") << "\n";
    failed++;
  }
  if (calc("12j", "4b", 25, "+") == "175") {
    passed++;
  } else {
    std::cout << "Test 12j + 4b (25) is failed: must return 175, returns: " << calc("12j", "4b", 25, "+") << "\n";
    failed++;
  }
  if (calc("172", "eh", 21, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 172 / eh (21) is failed: must return 1, returns: " << calc("172", "eh", 21, "/") << "\n";
    failed++;
  }
  if (calc("1e1", "j8", 20, "-") == "ed") {
    passed++;
  } else {
    std::cout << "Test 1e1 - j8 (20) is failed: must return ed, returns: " << calc("1e1", "j8", 20, "-") << "\n";
    failed++;
  }
  if (calc("2c7", "242", 16, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 2c7 / 242 (16) is failed: must return 1, returns: " << calc("2c7", "242", 16, "/") << "\n";
    failed++;
  }
  if (calc("1ib", "h6", 21, "*") == "1bb43") {
    passed++;
  } else {
    std::cout << "Test 1ib * h6 (21) is failed: must return 1bb43, returns: " << calc("1ib", "h6", 21, "*") << "\n";
    failed++;
  }
  if (calc("11412", "3041", 5, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 11412 / 3041 (5) is failed: must return 2, returns: " << calc("11412", "3041", 5, "/") << "\n";
    failed++;
  }
  if (calc("1021010", "211202", 3, "*") == "1001201201020") {
    passed++;
  } else {
    std::cout << "Test 1021010 * 211202 (3) is failed: must return 1001201201020, returns: " << calc("1021010", "211202", 3, "*") << "\n";
    failed++;
  }
  if (calc("ha", "d2", 22, "+") == "18c") {
    passed++;
  } else {
    std::cout << "Test ha + d2 (22) is failed: must return 18c, returns: " << calc("ha", "d2", 22, "+") << "\n";
    failed++;
  }
  if (calc("327", "85", 11, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test 327 / 85 (11) is failed: must return 4, returns: " << calc("327", "85", 11, "/") << "\n";
    failed++;
  }
  if (calc("14", "9", 31, "*") == "a5") {
    passed++;
  } else {
    std::cout << "Test 14 * 9 (31) is failed: must return a5, returns: " << calc("14", "9", 31, "*") << "\n";
    failed++;
  }
  if (calc("1001222", "220110", 3, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1001222 / 220110 (3) is failed: must return 1, returns: " << calc("1001222", "220110", 3, "/") << "\n";
    failed++;
  }
  if (calc("3db", "18", 15, "*") == "600d") {
    passed++;
  } else {
    std::cout << "Test 3db * 18 (15) is failed: must return 600d, returns: " << calc("3db", "18", 15, "*") << "\n";
    failed++;
  }
  if (calc("104", "86", 9, "+") == "201") {
    passed++;
  } else {
    std::cout << "Test 104 + 86 (9) is failed: must return 201, returns: " << calc("104", "86", 9, "+") << "\n";
    failed++;
  }
  if (calc("1012100", "220210", 3, "-") == "21120") {
    passed++;
  } else {
    std::cout << "Test 1012100 - 220210 (3) is failed: must return 21120, returns: " << calc("1012100", "220210", 3, "-") << "\n";
    failed++;
  }
  if (calc("13033", "22", 4, "+") == "13121") {
    passed++;
  } else {
    std::cout << "Test 13033 + 22 (4) is failed: must return 13121, returns: " << calc("13033", "22", 4, "+") << "\n";
    failed++;
  }
  if (calc("1165", "1134", 7, "*") == "1361466") {
    passed++;
  } else {
    std::cout << "Test 1165 * 1134 (7) is failed: must return 1361466, returns: " << calc("1165", "1134", 7, "*") << "\n";
    failed++;
  }
  if (calc("3551", "1041", 6, "+") == "5032") {
    passed++;
  } else {
    std::cout << "Test 3551 + 1041 (6) is failed: must return 5032, returns: " << calc("3551", "1041", 6, "+") << "\n";
    failed++;
  }
  if (calc("i", "c", 34, "-") == "6") {
    passed++;
  } else {
    std::cout << "Test i - c (34) is failed: must return 6, returns: " << calc("i", "c", 34, "-") << "\n";
    failed++;
  }
  if (calc("ko", "33", 32, "/") == "6") {
    passed++;
  } else {
    std::cout << "Test ko / 33 (32) is failed: must return 6, returns: " << calc("ko", "33", 32, "/") << "\n";
    failed++;
  }
  if (calc("371", "19", 11, "/") == "20") {
    passed++;
  } else {
    std::cout << "Test 371 / 19 (11) is failed: must return 20, returns: " << calc("371", "19", 11, "/") << "\n";
    failed++;
  }
  if (calc("1bb", "cl", 25, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1bb / cl (25) is failed: must return 2, returns: " << calc("1bb", "cl", 25, "/") << "\n";
    failed++;
  }
  if (calc("2e5", "1fc", 19, "*") == "504i3") {
    passed++;
  } else {
    std::cout << "Test 2e5 * 1fc (19) is failed: must return 504i3, returns: " << calc("2e5", "1fc", 19, "*") << "\n";
    failed++;
  }
  if (calc("213", "139", 15, "-") == "c9") {
    passed++;
  } else {
    std::cout << "Test 213 - 139 (15) is failed: must return c9, returns: " << calc("213", "139", 15, "-") << "\n";
    failed++;
  }
  if (calc("hh", "5e", 21, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test hh / 5e (21) is failed: must return 3, returns: " << calc("hh", "5e", 21, "/") << "\n";
    failed++;
  }
  if (calc("3c8", "141", 15, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 3c8 / 141 (15) is failed: must return 3, returns: " << calc("3c8", "141", 15, "/") << "\n";
    failed++;
  }
  if (calc("1jb", "e0", 20, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 1jb / e0 (20) is failed: must return 2, returns: " << calc("1jb", "e0", 20, "/") << "\n";
    failed++;
  }
  if (calc("182", "137", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 182 / 137 (13) is failed: must return 1, returns: " << calc("182", "137", 13, "/") << "\n";
    failed++;
  }
  if (calc("fi", "af", 25, "-") == "53") {
    passed++;
  } else {
    std::cout << "Test fi - af (25) is failed: must return 53, returns: " << calc("fi", "af", 25, "-") << "\n";
    failed++;
  }
  if (calc("19i", "k7", 25, "*") == "13431") {
    passed++;
  } else {
    std::cout << "Test 19i * k7 (25) is failed: must return 13431, returns: " << calc("19i", "k7", 25, "*") << "\n";
    failed++;
  }
  if (calc("6a", "48", 33, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 6a / 48 (33) is failed: must return 1, returns: " << calc("6a", "48", 33, "/") << "\n";
    failed++;
  }
  if (calc("pr", "fn", 31, "*") == "d481") {
    passed++;
  } else {
    std::cout << "Test pr * fn (31) is failed: must return d481, returns: " << calc("pr", "fn", 31, "*") << "\n";
    failed++;
  }
  if (calc("189", "157", 15, "*") == "22303") {
    passed++;
  } else {
    std::cout << "Test 189 * 157 (15) is failed: must return 22303, returns: " << calc("189", "157", 15, "*") << "\n";
    failed++;
  }
  if (calc("1101", "111", 2, "+") == "10100") {
    passed++;
  } else {
    std::cout << "Test 1101 + 111 (2) is failed: must return 10100, returns: " << calc("1101", "111", 2, "+") << "\n";
    failed++;
  }
  if (calc("2042", "601", 7, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 2042 / 601 (7) is failed: must return 2, returns: " << calc("2042", "601", 7, "/") << "\n";
    failed++;
  }
  if (calc("in", "hf", 27, "+") == "19b") {
    passed++;
  } else {
    std::cout << "Test in + hf (27) is failed: must return 19b, returns: " << calc("in", "hf", 27, "+") << "\n";
    failed++;
  }
  if (calc("224", "c3", 18, "*") == "17f0c") {
    passed++;
  } else {
    std::cout << "Test 224 * c3 (18) is failed: must return 17f0c, returns: " << calc("224", "c3", 18, "*") << "\n";
    failed++;
  }
  if (calc("616", "531", 10, "-") == "85") {
    passed++;
  } else {
    std::cout << "Test 616 - 531 (10) is failed: must return 85, returns: " << calc("616", "531", 10, "-") << "\n";
    failed++;
  }
  if (calc("294", "c8", 20, "*") == "1aa1c") {
    passed++;
  } else {
    std::cout << "Test 294 * c8 (20) is failed: must return 1aa1c, returns: " << calc("294", "c8", 20, "*") << "\n";
    failed++;
  }
  if (calc("144", "j3", 20, "+") == "237") {
    passed++;
  } else {
    std::cout << "Test 144 + j3 (20) is failed: must return 237, returns: " << calc("144", "j3", 20, "+") << "\n";
    failed++;
  }
  if (calc("p2", "n6", 35, "-") == "1v") {
    passed++;
  } else {
    std::cout << "Test p2 - n6 (35) is failed: must return 1v, returns: " << calc("p2", "n6", 35, "-") << "\n";
    failed++;
  }
  if (calc("21321", "13122", 4, "-") == "2133") {
    passed++;
  } else {
    std::cout << "Test 21321 - 13122 (4) is failed: must return 2133, returns: " << calc("21321", "13122", 4, "-") << "\n";
    failed++;
  }
  if (calc("115", "af", 31, "+") == "1bk") {
    passed++;
  } else {
    std::cout << "Test 115 + af (31) is failed: must return 1bk, returns: " << calc("115", "af", 31, "+") << "\n";
    failed++;
  }
  if (calc("ig", "g4", 22, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ig / g4 (22) is failed: must return 1, returns: " << calc("ig", "g4", 22, "/") << "\n";
    failed++;
  }
  if (calc("cp", "ai", 27, "*") == "52ni") {
    passed++;
  } else {
    std::cout << "Test cp * ai (27) is failed: must return 52ni, returns: " << calc("cp", "ai", 27, "*") << "\n";
    failed++;
  }
  if (calc("3c0", "117", 16, "-") == "2a9") {
    passed++;
  } else {
    std::cout << "Test 3c0 - 117 (16) is failed: must return 2a9, returns: " << calc("3c0", "117", 16, "-") << "\n";
    failed++;
  }
  if (calc("17e", "10k", 24, "-") == "6i") {
    passed++;
  } else {
    std::cout << "Test 17e - 10k (24) is failed: must return 6i, returns: " << calc("17e", "10k", 24, "-") << "\n";
    failed++;
  }
  if (calc("3011", "1403", 5, "*") == "10234433") {
    passed++;
  } else {
    std::cout << "Test 3011 * 1403 (5) is failed: must return 10234433, returns: " << calc("3011", "1403", 5, "*") << "\n";
    failed++;
  }
  if (calc("od", "3r", 29, "-") == "kf") {
    passed++;
  } else {
    std::cout << "Test od - 3r (29) is failed: must return kf, returns: " << calc("od", "3r", 29, "-") << "\n";
    failed++;
  }
  if (calc("3b2", "129", 13, "+") == "50b") {
    passed++;
  } else {
    std::cout << "Test 3b2 + 129 (13) is failed: must return 50b, returns: " << calc("3b2", "129", 13, "+") << "\n";
    failed++;
  }
  if (calc("292", "256", 13, "-") == "39") {
    passed++;
  } else {
    std::cout << "Test 292 - 256 (13) is failed: must return 39, returns: " << calc("292", "256", 13, "-") << "\n";
    failed++;
  }
  if (calc("117", "15", 13, "*") == "1719") {
    passed++;
  } else {
    std::cout << "Test 117 * 15 (13) is failed: must return 1719, returns: " << calc("117", "15", 13, "*") << "\n";
    failed++;
  }
  if (calc("5a5", "377", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 5a5 / 377 (13) is failed: must return 1, returns: " << calc("5a5", "377", 13, "/") << "\n";
    failed++;
  }
  if (calc("21213", "1132", 4, "*") == "32013102") {
    passed++;
  } else {
    std::cout << "Test 21213 * 1132 (4) is failed: must return 32013102, returns: " << calc("21213", "1132", 4, "*") << "\n";
    failed++;
  }
  if (calc("4i", "1f", 23, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 4i / 1f (23) is failed: must return 2, returns: " << calc("4i", "1f", 23, "/") << "\n";
    failed++;
  }
  if (calc("212", "1e0", 20, "*") == "39h80") {
    passed++;
  } else {
    std::cout << "Test 212 * 1e0 (20) is failed: must return 39h80, returns: " << calc("212", "1e0", 20, "*") << "\n";
    failed++;
  }
  if (calc("ih", "6j", 27, "+") == "p9") {
    passed++;
  } else {
    std::cout << "Test ih + 6j (27) is failed: must return p9, returns: " << calc("ih", "6j", 27, "+") << "\n";
    failed++;
  }
  if (calc("ri", "iq", 28, "-") == "8k") {
    passed++;
  } else {
    std::cout << "Test ri - iq (28) is failed: must return 8k, returns: " << calc("ri", "iq", 28, "-") << "\n";
    failed++;
  }
  if (calc("1ki", "1jj", 22, "-") == "l") {
    passed++;
  } else {
    std::cout << "Test 1ki - 1jj (22) is failed: must return l, returns: " << calc("1ki", "1jj", 22, "-") << "\n";
    failed++;
  }
  if (calc("n2", "dd", 33, "-") == "9m") {
    passed++;
  } else {
    std::cout << "Test n2 - dd (33) is failed: must return 9m, returns: " << calc("n2", "dd", 33, "-") << "\n";
    failed++;
  }
  if (calc("73", "1l", 34, "*") == "bft") {
    passed++;
  } else {
    std::cout << "Test 73 * 1l (34) is failed: must return bft, returns: " << calc("73", "1l", 34, "*") << "\n";
    failed++;
  }
  if (calc("pp", "1h", 32, "*") == "17f9") {
    passed++;
  } else {
    std::cout << "Test pp * 1h (32) is failed: must return 17f9, returns: " << calc("pp", "1h", 32, "*") << "\n";
    failed++;
  }
  if (calc("1000010000", "110101110", 2, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1000010000 / 110101110 (2) is failed: must return 1, returns: " << calc("1000010000", "110101110", 2, "/") << "\n";
    failed++;
  }
  if (calc("nt", "cl", 34, "-") == "b8") {
    passed++;
  } else {
    std::cout << "Test nt - cl (34) is failed: must return b8, returns: " << calc("nt", "cl", 34, "-") << "\n";
    failed++;
  }
  if (calc("1000100100", "101100", 2, "+") == "1001010000") {
    passed++;
  } else {
    std::cout << "Test 1000100100 + 101100 (2) is failed: must return 1001010000, returns: " << calc("1000100100", "101100", 2, "+") << "\n";
    failed++;
  }
  if (calc("11g", "nl", 25, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 11g / nl (25) is failed: must return 1, returns: " << calc("11g", "nl", 25, "/") << "\n";
    failed++;
  }
  if (calc("3204", "44", 6, "-") == "3120") {
    passed++;
  } else {
    std::cout << "Test 3204 - 44 (6) is failed: must return 3120, returns: " << calc("3204", "44", 6, "-") << "\n";
    failed++;
  }
  if (calc("2240", "2044", 5, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 2240 / 2044 (5) is failed: must return 1, returns: " << calc("2240", "2044", 5, "/") << "\n";
    failed++;
  }
  if (calc("5d", "23", 29, "+") == "7g") {
    passed++;
  } else {
    std::cout << "Test 5d + 23 (29) is failed: must return 7g, returns: " << calc("5d", "23", 29, "+") << "\n";
    failed++;
  }
  if (calc("t2", "e1", 33, "-") == "f1") {
    passed++;
  } else {
    std::cout << "Test t2 - e1 (33) is failed: must return f1, returns: " << calc("t2", "e1", 33, "-") << "\n";
    failed++;
  }
  if (calc("d4", "9d", 32, "*") == "3rek") {
    passed++;
  } else {
    std::cout << "Test d4 * 9d (32) is failed: must return 3rek, returns: " << calc("d4", "9d", 32, "*") << "\n";
    failed++;
  }
  if (calc("10103", "3221", 4, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 10103 / 3221 (4) is failed: must return 1, returns: " << calc("10103", "3221", 4, "/") << "\n";
    failed++;
  }
  if (calc("79", "47", 22, "*") == "19lj") {
    passed++;
  } else {
    std::cout << "Test 79 * 47 (22) is failed: must return 19lj, returns: " << calc("79", "47", 22, "*") << "\n";
    failed++;
  }
  if (calc("a", "9", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test a / 9 (26) is failed: must return 1, returns: " << calc("a", "9", 26, "/") << "\n";
    failed++;
  }
  if (calc("707", "414", 9, "-") == "283") {
    passed++;
  } else {
    std::cout << "Test 707 - 414 (9) is failed: must return 283, returns: " << calc("707", "414", 9, "-") << "\n";
    failed++;
  }
  if (calc("26", "23", 9, "-") == "3") {
    passed++;
  } else {
    std::cout << "Test 26 - 23 (9) is failed: must return 3, returns: " << calc("26", "23", 9, "-") << "\n";
    failed++;
  }
  if (calc("87", "25", 30, "-") == "62") {
    passed++;
  } else {
    std::cout << "Test 87 - 25 (30) is failed: must return 62, returns: " << calc("87", "25", 30, "-") << "\n";
    failed++;
  }
  if (calc("m7", "hi", 31, "-") == "4k") {
    passed++;
  } else {
    std::cout << "Test m7 - hi (31) is failed: must return 4k, returns: " << calc("m7", "hi", 31, "-") << "\n";
    failed++;
  }
  if (calc("1a9", "3d", 25, "-") == "16l") {
    passed++;
  } else {
    std::cout << "Test 1a9 - 3d (25) is failed: must return 16l, returns: " << calc("1a9", "3d", 25, "-") << "\n";
    failed++;
  }
  if (calc("2204", "1311", 6, "+") == "3515") {
    passed++;
  } else {
    std::cout << "Test 2204 + 1311 (6) is failed: must return 3515, returns: " << calc("2204", "1311", 6, "+") << "\n";
    failed++;
  }
  if (calc("7s", "7d", 35, "-") == "f") {
    passed++;
  } else {
    std::cout << "Test 7s - 7d (35) is failed: must return f, returns: " << calc("7s", "7d", 35, "-") << "\n";
    failed++;
  }
  if (calc("b6", "91", 22, "+") == "k7") {
    passed++;
  } else {
    std::cout << "Test b6 + 91 (22) is failed: must return k7, returns: " << calc("b6", "91", 22, "+") << "\n";
    failed++;
  }
  if (calc("559", "111", 10, "+") == "670") {
    passed++;
  } else {
    std::cout << "Test 559 + 111 (10) is failed: must return 670, returns: " << calc("559", "111", 10, "+") << "\n";
    failed++;
  }
  if (calc("8j", "3s", 30, "-") == "4l") {
    passed++;
  } else {
    std::cout << "Test 8j - 3s (30) is failed: must return 4l, returns: " << calc("8j", "3s", 30, "-") << "\n";
    failed++;
  }
  if (calc("4215", "2021", 6, "-") == "2154") {
    passed++;
  } else {
    std::cout << "Test 4215 - 2021 (6) is failed: must return 2154, returns: " << calc("4215", "2021", 6, "-") << "\n";
    failed++;
  }
  if (calc("26d", "1a4", 15, "-") == "b9") {
    passed++;
  } else {
    std::cout << "Test 26d - 1a4 (15) is failed: must return b9, returns: " << calc("26d", "1a4", 15, "-") << "\n";
    failed++;
  }
  if (calc("288", "176", 12, "*") == "45100") {
    passed++;
  } else {
    std::cout << "Test 288 * 176 (12) is failed: must return 45100, returns: " << calc("288", "176", 12, "*") << "\n";
    failed++;
  }
  if (calc("335", "300", 7, "*") == "134100") {
    passed++;
  } else {
    std::cout << "Test 335 * 300 (7) is failed: must return 134100, returns: " << calc("335", "300", 7, "*") << "\n";
    failed++;
  }
  if (calc("180", "h5", 19, "-") == "9e") {
    passed++;
  } else {
    std::cout << "Test 180 - h5 (19) is failed: must return 9e, returns: " << calc("180", "h5", 19, "-") << "\n";
    failed++;
  }
  if (calc("ac", "7e", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test ac / 7e (30) is failed: must return 1, returns: " << calc("ac", "7e", 30, "/") << "\n";
    failed++;
  }
  if (calc("84", "3p", 30, "*") == "115a") {
    passed++;
  } else {
    std::cout << "Test 84 * 3p (30) is failed: must return 115a, returns: " << calc("84", "3p", 30, "*") << "\n";
    failed++;
  }
  if (calc("210", "10c", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 210 / 10c (13) is failed: must return 1, returns: " << calc("210", "10c", 13, "/") << "\n";
    failed++;
  }
  if (calc("87", "y", 35, "*") == "7xs") {
    passed++;
  } else {
    std::cout << "Test 87 * y (35) is failed: must return 7xs, returns: " << calc("87", "y", 35, "*") << "\n";
    failed++;
  }
  if (calc("500", "114", 6, "-") == "342") {
    passed++;
  } else {
    std::cout << "Test 500 - 114 (6) is failed: must return 342, returns: " << calc("500", "114", 6, "-") << "\n";
    failed++;
  }
  if (calc("1721", "426", 8, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test 1721 / 426 (8) is failed: must return 3, returns: " << calc("1721", "426", 8, "/") << "\n";
    failed++;
  }
  if (calc("nt", "lr", 30, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test nt / lr (30) is failed: must return 1, returns: " << calc("nt", "lr", 30, "/") << "\n";
    failed++;
  }
  if (calc("271", "134", 11, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 271 / 134 (11) is failed: must return 2, returns: " << calc("271", "134", 11, "/") << "\n";
    failed++;
  }
  if (calc("13d", "6i", 24, "-") == "kj") {
    passed++;
  } else {
    std::cout << "Test 13d - 6i (24) is failed: must return kj, returns: " << calc("13d", "6i", 24, "-") << "\n";
    failed++;
  }
  if (calc("j5", "hb", 24, "*") == "dn87") {
    passed++;
  } else {
    std::cout << "Test j5 * hb (24) is failed: must return dn87, returns: " << calc("j5", "hb", 24, "*") << "\n";
    failed++;
  }
  if (calc("2202", "464", 7, "+") == "2666") {
    passed++;
  } else {
    std::cout << "Test 2202 + 464 (7) is failed: must return 2666, returns: " << calc("2202", "464", 7, "+") << "\n";
    failed++;
  }
  if (calc("357", "22c", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 357 / 22c (14) is failed: must return 1, returns: " << calc("357", "22c", 14, "/") << "\n";
    failed++;
  }
  if (calc("i", "6", 23, "/") == "3") {
    passed++;
  } else {
    std::cout << "Test i / 6 (23) is failed: must return 3, returns: " << calc("i", "6", 23, "/") << "\n";
    failed++;
  }
  if (calc("3155", "1424", 6, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 3155 / 1424 (6) is failed: must return 1, returns: " << calc("3155", "1424", 6, "/") << "\n";
    failed++;
  }
  if (calc("3u", "2s", 32, "-") == "12") {
    passed++;
  } else {
    std::cout << "Test 3u - 2s (32) is failed: must return 12, returns: " << calc("3u", "2s", 32, "-") << "\n";
    failed++;
  }
  if (calc("u3", "t7", 32, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test u3 / t7 (32) is failed: must return 1, returns: " << calc("u3", "t7", 32, "/") << "\n";
    failed++;
  }
  if (calc("r1", "6", 34, "/") == "4h") {
    passed++;
  } else {
    std::cout << "Test r1 / 6 (34) is failed: must return 4h, returns: " << calc("r1", "6", 34, "/") << "\n";
    failed++;
  }
  if (calc("ig", "7m", 26, "*") == "5g1e") {
    passed++;
  } else {
    std::cout << "Test ig * 7m (26) is failed: must return 5g1e, returns: " << calc("ig", "7m", 26, "*") << "\n";
    failed++;
  }
  if (calc("2c", "n", 27, "*") == "226") {
    passed++;
  } else {
    std::cout << "Test 2c * n (27) is failed: must return 226, returns: " << calc("2c", "n", 27, "*") << "\n";
    failed++;
  }
  if (calc("111", "3d", 16, "+") == "14e") {
    passed++;
  } else {
    std::cout << "Test 111 + 3d (16) is failed: must return 14e, returns: " << calc("111", "3d", 16, "+") << "\n";
    failed++;
  }
  if (calc("20021", "213", 4, "*") == "10331133") {
    passed++;
  } else {
    std::cout << "Test 20021 * 213 (4) is failed: must return 10331133, returns: " << calc("20021", "213", 4, "*") << "\n";
    failed++;
  }
  if (calc("50", "22", 8, "/") == "2") {
    passed++;
  } else {
    std::cout << "Test 50 / 22 (8) is failed: must return 2, returns: " << calc("50", "22", 8, "/") << "\n";
    failed++;
  }
  if (calc("2l", "a", 22, "*") == "17c") {
    passed++;
  } else {
    std::cout << "Test 2l * a (22) is failed: must return 17c, returns: " << calc("2l", "a", 22, "*") << "\n";
    failed++;
  }
  if (calc("135", "d5", 17, "*") == "fed8") {
    passed++;
  } else {
    std::cout << "Test 135 * d5 (17) is failed: must return fed8, returns: " << calc("135", "d5", 17, "*") << "\n";
    failed++;
  }
  if (calc("dj", "8b", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test dj / 8b (26) is failed: must return 1, returns: " << calc("dj", "8b", 26, "/") << "\n";
    failed++;
  }
  if (calc("24a", "3d", 15, "*") == "8e0a") {
    passed++;
  } else {
    std::cout << "Test 24a * 3d (15) is failed: must return 8e0a, returns: " << calc("24a", "3d", 15, "*") << "\n";
    failed++;
  }
  if (calc("bn", "a8", 26, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test bn / a8 (26) is failed: must return 1, returns: " << calc("bn", "a8", 26, "/") << "\n";
    failed++;
  }
  if (calc("101", "da", 23, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 101 / da (23) is failed: must return 1, returns: " << calc("101", "da", 23, "/") << "\n";
    failed++;
  }
  if (calc("16e", "f5", 21, "*") == "k1c7") {
    passed++;
  } else {
    std::cout << "Test 16e * f5 (21) is failed: must return k1c7, returns: " << calc("16e", "f5", 21, "*") << "\n";
    failed++;
  }
  if (calc("138", "64", 13, "*") == "80a6") {
    passed++;
  } else {
    std::cout << "Test 138 * 64 (13) is failed: must return 80a6, returns: " << calc("138", "64", 13, "*") << "\n";
    failed++;
  }
  if (calc("gm", "fm", 26, "+") == "16i") {
    passed++;
  } else {
    std::cout << "Test gm + fm (26) is failed: must return 16i, returns: " << calc("gm", "fm", 26, "+") << "\n";
    failed++;
  }
  if (calc("q8", "jo", 33, "+") == "1cw") {
    passed++;
  } else {
    std::cout << "Test q8 + jo (33) is failed: must return 1cw, returns: " << calc("q8", "jo", 33, "+") << "\n";
    failed++;
  }
  if (calc("3e2", "1e1", 16, "-") == "201") {
    passed++;
  } else {
    std::cout << "Test 3e2 - 1e1 (16) is failed: must return 201, returns: " << calc("3e2", "1e1", 16, "-") << "\n";
    failed++;
  }
  if (calc("gw", "1v", 35, "/") == "8") {
    passed++;
  } else {
    std::cout << "Test gw / 1v (35) is failed: must return 8, returns: " << calc("gw", "1v", 35, "/") << "\n";
    failed++;
  }
  if (calc("54", "31", 16, "*") == "1014") {
    passed++;
  } else {
    std::cout << "Test 54 * 31 (16) is failed: must return 1014, returns: " << calc("54", "31", 16, "*") << "\n";
    failed++;
  }
  if (calc("2210", "2141", 7, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 2210 / 2141 (7) is failed: must return 1, returns: " << calc("2210", "2141", 7, "/") << "\n";
    failed++;
  }
  if (calc("1214", "33", 6, "/") == "22") {
    passed++;
  } else {
    std::cout << "Test 1214 / 33 (6) is failed: must return 22, returns: " << calc("1214", "33", 6, "/") << "\n";
    failed++;
  }
  if (calc("d7", "33", 23, "/") == "4") {
    passed++;
  } else {
    std::cout << "Test d7 / 33 (23) is failed: must return 4, returns: " << calc("d7", "33", 23, "/") << "\n";
    failed++;
  }
  if (calc("69", "4e", 22, "*") == "17fg") {
    passed++;
  } else {
    std::cout << "Test 69 * 4e (22) is failed: must return 17fg, returns: " << calc("69", "4e", 22, "*") << "\n";
    failed++;
  }
  if (calc("1551", "1075", 8, "-") == "454") {
    passed++;
  } else {
    std::cout << "Test 1551 - 1075 (8) is failed: must return 454, returns: " << calc("1551", "1075", 8, "-") << "\n";
    failed++;
  }
  if (calc("101011101", "100000", 2, "+") == "101111101") {
    passed++;
  } else {
    std::cout << "Test 101011101 + 100000 (2) is failed: must return 101111101, returns: " << calc("101011101", "100000", 2, "+") << "\n";
    failed++;
  }
  if (calc("h5", "2b", 24, "*") == "1i77") {
    passed++;
  } else {
    std::cout << "Test h5 * 2b (24) is failed: must return 1i77, returns: " << calc("h5", "2b", 24, "*") << "\n";
    failed++;
  }
  if (calc("167", "151", 11, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 167 / 151 (11) is failed: must return 1, returns: " << calc("167", "151", 11, "/") << "\n";
    failed++;
  }
  if (calc("ki", "gg", 26, "-") == "42") {
    passed++;
  } else {
    std::cout << "Test ki - gg (26) is failed: must return 42, returns: " << calc("ki", "gg", 26, "-") << "\n";
    failed++;
  }
  if (calc("bm", "5b", 23, "*") == "2jbc") {
    passed++;
  } else {
    std::cout << "Test bm * 5b (23) is failed: must return 2jbc, returns: " << calc("bm", "5b", 23, "*") << "\n";
    failed++;
  }
  if (calc("163", "dc", 26, "+") == "1jf") {
    passed++;
  } else {
    std::cout << "Test 163 + dc (26) is failed: must return 1jf, returns: " << calc("163", "dc", 26, "+") << "\n";
    failed++;
  }
  if (calc("7f", "5k", 21, "+") == "de") {
    passed++;
  } else {
    std::cout << "Test 7f + 5k (21) is failed: must return de, returns: " << calc("7f", "5k", 21, "+") << "\n";
    failed++;
  }
  if (calc("cj", "6k", 33, "-") == "5w") {
    passed++;
  } else {
    std::cout << "Test cj - 6k (33) is failed: must return 5w, returns: " << calc("cj", "6k", 33, "-") << "\n";
    failed++;
  }
  if (calc("cc", "4", 15, "*") == "363") {
    passed++;
  } else {
    std::cout << "Test cc * 4 (15) is failed: must return 363, returns: " << calc("cc", "4", 15, "*") << "\n";
    failed++;
  }
  if (calc("1f9", "28", 17, "*") == "4c64") {
    passed++;
  } else {
    std::cout << "Test 1f9 * 28 (17) is failed: must return 4c64, returns: " << calc("1f9", "28", 17, "*") << "\n";
    failed++;
  }
  if (calc("239", "158", 10, "-") == "81") {
    passed++;
  } else {
    std::cout << "Test 239 - 158 (10) is failed: must return 81, returns: " << calc("239", "158", 10, "-") << "\n";
    failed++;
  }
  if (calc("1c", "h", 27, "-") == "m") {
    passed++;
  } else {
    std::cout << "Test 1c - h (27) is failed: must return m, returns: " << calc("1c", "h", 27, "-") << "\n";
    failed++;
  }
  if (calc("6e", "2m", 32, "-") == "3o") {
    passed++;
  } else {
    std::cout << "Test 6e - 2m (32) is failed: must return 3o, returns: " << calc("6e", "2m", 32, "-") << "\n";
    failed++;
  }
  if (calc("325", "221", 11, "-") == "104") {
    passed++;
  } else {
    std::cout << "Test 325 - 221 (11) is failed: must return 104, returns: " << calc("325", "221", 11, "-") << "\n";
    failed++;
  }
  if (calc("223", "39", 16, "-") == "1ea") {
    passed++;
  } else {
    std::cout << "Test 223 - 39 (16) is failed: must return 1ea, returns: " << calc("223", "39", 16, "-") << "\n";
    failed++;
  }
  if (calc("hh", "47", 21, "*") == "3e3e") {
    passed++;
  } else {
    std::cout << "Test hh * 47 (21) is failed: must return 3e3e, returns: " << calc("hh", "47", 21, "*") << "\n";
    failed++;
  }
  if (calc("234", "20", 6, "-") == "214") {
    passed++;
  } else {
    std::cout << "Test 234 - 20 (6) is failed: must return 214, returns: " << calc("234", "20", 6, "-") << "\n";
    failed++;
  }
  if (calc("ts", "6g", 30, "*") == "6fgs") {
    passed++;
  } else {
    std::cout << "Test ts * 6g (30) is failed: must return 6fgs, returns: " << calc("ts", "6g", 30, "*") << "\n";
    failed++;
  }
  if (calc("d3", "43", 18, "-") == "90") {
    passed++;
  } else {
    std::cout << "Test d3 - 43 (18) is failed: must return 90, returns: " << calc("d3", "43", 18, "-") << "\n";
    failed++;
  }
  if (calc("314", "25c", 14, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 314 / 25c (14) is failed: must return 1, returns: " << calc("314", "25c", 14, "/") << "\n";
    failed++;
  }
  if (calc("1026", "451", 8, "-") == "355") {
    passed++;
  } else {
    std::cout << "Test 1026 - 451 (8) is failed: must return 355, returns: " << calc("1026", "451", 8, "-") << "\n";
    failed++;
  }
  if (calc("8b", "c", 26, "+") == "8n") {
    passed++;
  } else {
    std::cout << "Test 8b + c (26) is failed: must return 8n, returns: " << calc("8b", "c", 26, "+") << "\n";
    failed++;
  }
  if (calc("19a", "4d", 15, "*") == "800a") {
    passed++;
  } else {
    std::cout << "Test 19a * 4d (15) is failed: must return 800a, returns: " << calc("19a", "4d", 15, "*") << "\n";
    failed++;
  }
  if (calc("6l", "5p", 30, "-") == "q") {
    passed++;
  } else {
    std::cout << "Test 6l - 5p (30) is failed: must return q, returns: " << calc("6l", "5p", 30, "-") << "\n";
    failed++;
  }
  if (calc("419", "39a", 12, "+") == "7b7") {
    passed++;
  } else {
    std::cout << "Test 419 + 39a (12) is failed: must return 7b7, returns: " << calc("419", "39a", 12, "+") << "\n";
    failed++;
  }
  if (calc("299", "131", 14, "+") == "3ca") {
    passed++;
  } else {
    std::cout << "Test 299 + 131 (14) is failed: must return 3ca, returns: " << calc("299", "131", 14, "+") << "\n";
    failed++;
  }
  if (calc("8h", "1e", 29, "-") == "73") {
    passed++;
  } else {
    std::cout << "Test 8h - 1e (29) is failed: must return 73, returns: " << calc("8h", "1e", 29, "-") << "\n";
    failed++;
  }
  if (calc("pd", "dl", 33, "*") == "ag99") {
    passed++;
  } else {
    std::cout << "Test pd * dl (33) is failed: must return ag99, returns: " << calc("pd", "dl", 33, "*") << "\n";
    failed++;
  }
  if (calc("708", "597", 10, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 708 / 597 (10) is failed: must return 1, returns: " << calc("708", "597", 10, "/") << "\n";
    failed++;
  }
  if (calc("1000001001", "111111100", 2, "*") == "1000000100111011100") {
    passed++;
  } else {
    std::cout << "Test 1000001001 * 111111100 (2) is failed: must return 1000000100111011100, returns: " << calc("1000001001", "111111100", 2, "*") << "\n";
    failed++;
  }
  if (calc("320", "2f4", 16, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 320 / 2f4 (16) is failed: must return 1, returns: " << calc("320", "2f4", 16, "/") << "\n";
    failed++;
  }
  if (calc("1kg", "19c", 21, "*") == "2igf3") {
    passed++;
  } else {
    std::cout << "Test 1kg * 19c (21) is failed: must return 2igf3, returns: " << calc("1kg", "19c", 21, "*") << "\n";
    failed++;
  }
  if (calc("0", "0", 10, "/") == "infinity") {
    passed++;
  } else {
    std::cout << "Test 0 / 0 (10) is failed: must return infinity, returns: " << calc("0", "0", 10, "/") << "\n";
    failed++;
  }
  if (calc("1g3", "13f", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 1g3 / 13f (24) is failed: must return 1, returns: " << calc("1g3", "13f", 24, "/") << "\n";
    failed++;
  }
  if (calc("647", "288", 9, "+") == "1046") {
    passed++;
  } else {
    std::cout << "Test 647 + 288 (9) is failed: must return 1046, returns: " << calc("647", "288", 9, "+") << "\n";
    failed++;
  }
  if (calc("85", "5q", 31, "-") == "2a") {
    passed++;
  } else {
    std::cout << "Test 85 - 5q (31) is failed: must return 2a, returns: " << calc("85", "5q", 31, "-") << "\n";
    failed++;
  }
  if (calc("1366", "1254", 7, "+") == "2653") {
    passed++;
  } else {
    std::cout << "Test 1366 + 1254 (7) is failed: must return 2653, returns: " << calc("1366", "1254", 7, "+") << "\n";
    failed++;
  }
  if (calc("775", "231", 9, "-") == "544") {
    passed++;
  } else {
    std::cout << "Test 775 - 231 (9) is failed: must return 544, returns: " << calc("775", "231", 9, "-") << "\n";
    failed++;
  }
  if (calc("i2", "5m", 24, "*") == "4ank") {
    passed++;
  } else {
    std::cout << "Test i2 * 5m (24) is failed: must return 4ank, returns: " << calc("i2", "5m", 24, "*") << "\n";
    failed++;
  }
  if (calc("10d", "19", 17, "-") == "g4") {
    passed++;
  } else {
    std::cout << "Test 10d - 19 (17) is failed: must return g4, returns: " << calc("10d", "19", 17, "-") << "\n";
    failed++;
  }
  if (calc("11i", "46", 22, "-") == "jc") {
    passed++;
  } else {
    std::cout << "Test 11i - 46 (22) is failed: must return jc, returns: " << calc("11i", "46", 22, "-") << "\n";
    failed++;
  }
  if (calc("20112", "2012", 3, "+") == "22201") {
    passed++;
  } else {
    std::cout << "Test 20112 + 2012 (3) is failed: must return 22201, returns: " << calc("20112", "2012", 3, "+") << "\n";
    failed++;
  }
  if (calc("180", "b6", 13, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 180 / b6 (13) is failed: must return 1, returns: " << calc("180", "b6", 13, "/") << "\n";
    failed++;
  }
  if (calc("162", "148", 13, "-") == "17") {
    passed++;
  } else {
    std::cout << "Test 162 - 148 (13) is failed: must return 17, returns: " << calc("162", "148", 13, "-") << "\n";
    failed++;
  }
  if (calc("1011011011", "110110111", 2, "+") == "10010010010") {
    passed++;
  } else {
    std::cout << "Test 1011011011 + 110110111 (2) is failed: must return 10010010010, returns: " << calc("1011011011", "110110111", 2, "+") << "\n";
    failed++;
  }
  if (calc("554", "525", 12, "+") == "a79") {
    passed++;
  } else {
    std::cout << "Test 554 + 525 (12) is failed: must return a79, returns: " << calc("554", "525", 12, "+") << "\n";
    failed++;
  }
  if (calc("8c", "4e", 24, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test 8c / 4e (24) is failed: must return 1, returns: " << calc("8c", "4e", 24, "/") << "\n";
    failed++;
  }
  if (calc("p1", "3f", 32, "+") == "sg") {
    passed++;
  } else {
    std::cout << "Test p1 + 3f (32) is failed: must return sg, returns: " << calc("p1", "3f", 32, "+") << "\n";
    failed++;
  }
  if (calc("bb", "63", 32, "/") == "1") {
    passed++;
  } else {
    std::cout << "Test bb / 63 (32) is failed: must return 1, returns: " << calc("bb", "63", 32, "/") << "\n";
    failed++;
  }
  if (calc("4e", "1t", 31, "-") == "2g") {
    passed++;
  } else {
    std::cout << "Test 4e - 1t (31) is failed: must return 2g, returns: " << calc("4e", "1t", 31, "-") << "\n";
    failed++;
  }

  std::cout << passed << " tests passed\n";
  std::cout << failed << " tests failed\n";
  /*
let result = 'int passed = 0, failed = 0;\n'
const operations = '+/-*'
for (let i = 0; i < 200; i++) {
  const first = Math.round(Math.random() * 1000);
  let second = Math.round(Math.random() * 1000);
  while (second > first)
    second = Math.round(Math.random() * 1000);
  const operation = operations[Math.floor(operations.length * Math.random())]
  const basis = Math.floor(Math.random() * 34) + 2
  const calcResult = Math.abs((operation !== '/' ? eval(first + operation + second) : ((first - first % second) / second))).toString(basis)
  result += `if (calc("${first.toString(basis)}", "${second.toString(basis)}", ${basis}, "${operation}") == "${calcResult}") {
  passed++;
} else {
  std::cout << "Test ${first.toString(basis)} ${operation} ${second.toString(basis)} (${basis}) is failed: must return ${calcResult}, returns: " << calc("${first.toString(basis)}", "${second.toString(basis)}", ${basis}, "${operation}") << "\\n";
  failed++;
}
`
}
result += `
std::cout << passed << " tests passed\\n";
std::cout << failed << " tests failed\\n";
`
document.querySelector('body').innerHTML = `<pre>${result}</pre>`
  */

}
