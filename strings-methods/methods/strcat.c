const char* strcat(char * firstString, const char * secondString) {
  int resultStringLength = strlen(firstString) + strlen(secondString) + 1;
  int firstStringLength = strlen(firstString);
  for (int i = firstStringLength; i < resultStringLength; i++) {
    firstString[i] = secondString[i - firstStringLength];
  }
  return firstString;
}