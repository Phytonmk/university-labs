int strcmp(const char * firstString, const char * secondString) {
  for (int i = 0; i < 10000; i++) {
    if (firstString[i] != secondString[i])
      return firstString[i] - secondString[i];
  }
  return 0;
}