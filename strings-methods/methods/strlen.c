int strlen(const char * string) {
  for (int i = 0; i < 10000; i++) {
    if (string[i] == '\0') {
      return i;
    }
  }
  return 0;
}