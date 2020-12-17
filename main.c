#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"


char* read_file(char* fname) {
  FILE* f = fopen(fname, "r");
  char* buffer = NULL;
  int string_size, read_size;

  if(f) {
    fseek(f, 0, SEEK_END);
    string_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(sizeof(char) * (string_size + 1));
    buffer[string_size] = '\0';

    read_size = fread(buffer, sizeof(char), string_size, f);

    if(string_size != read_size) {
      free(buffer);
      printf("Unable to allocate file memory!\n");
      exit(1);
    }
    
    fclose(f);

    return buffer;
  } else {
    printf("Unable to read file!\n");
    exit(1);
  }
}

int main() {
  char* filename = "main.shr";
  char* contents = read_file(filename);
  initializeScanner(contents);
  printToken(scan());
  printToken(scan());
  printToken(scan());
  // todo — everything

  return 0;
} 
