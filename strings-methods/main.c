#include <iostream>
#include "./methods/strcmp.h"
#include "./methods/strlen.h"
#include "./methods/strcat.h"

int main() {
  char firstString[10000] = {'a', 'b', 'c', '\0'};
  char secondString[10000] = {'k', 'l', 'm', '\0'};
  scanf("%s", firstString); 
  scanf("%s", secondString);
  printf("Strcmp: %i\n", strcmp(firstString, secondString));
  printf("Strlen (first string): %i\n", strlen(firstString));
  printf("Strlen (second string): %i\n", strlen(secondString));
  printf("Strcat: %s\n", strcat(firstString, secondString));
}