#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
struct {
  char* position;
  int line;
} scanner;
struct Token scan(){ 
  for(;;) {
    switch(*scanner.position) {
      case ' ': case '\t': case '\r':
        break;

      case '/':
        if(*(scanner.position + 1) == '/') {
          while(*scanner.position != '\n') {
            scanner.position ++;
          }
          break;
        }
      
      default:
        goto done_checking_ws;
    }
  }
  done_checking_ws:
  switch(*scanner.position) {
    case '\0':
      return (struct Token){ .type = T_EOF, .value = "\0", .line = scanner.line };
    case '(':
      scanner.position++;
      return (struct Token){ .type = T_LPAR, .value = "(", .line = scanner.line };
    case ')':
      scanner.position++;
      return (struct Token){ .type = T_RPAR, .value = ")", .line = scanner.line };
    case '+':
      scanner.position++;
      return (struct Token){ .type = T_PLUS, .value = "+", .line = scanner.line };
    case '-':
      scanner.position++;
      return (struct Token){ .type = T_MINUS, .value = "-", .line = scanner.line };
    case '=':
      scanner.position ++;
      return (struct Token){ .type = T_EQ, .value = "=", .line = scanner.line};
    case '\n':
      scanner.position++;
      scanner.line ++;
      int line = scanner.line - 1;
      while(*scanner.position == '\n') {
        scanner.position ++;
        scanner.line ++;
      }
      return (struct Token){ .type = T_SEP, .value = "\n", .line = line };

    default:
      if(isalpha(*scanner.position) || *scanner.position == '_') {
        char* start = scanner.position;
        while(isalpha(*scanner.position) || *scanner.position == '_') {
          scanner.position++;
        }
        size_t len = sizeof(char) * (scanner.position - start);

        char* value = (char*)malloc(len + 1);
        memcpy(value, start, len);
        value[len] = '\0';

        if(strcmp(value, "var") == 0) {
          return (struct Token){ .type = T_VAR, .value = value, .line = scanner.line };
        }
        if(strcmp(value, "while") == 0) {
          return (struct Token){ .type = T_WHILE, .value = value, .line = scanner.line };
        }
        if(strcmp(value, "do") == 0) {
          return (struct Token){ .type = T_DO, .value = value, .line = scanner.line };
        }
        if(strcmp(value, "end") == 0) {
          return (struct Token){ .type = T_END, .value = value, .line = scanner.line };
        }
        if(strcmp(value, "println") == 0) {
          return (struct Token){ .type = T_PRINT, .value = value, .line = scanner.line };
        }

        return (struct Token){ .type = T_ID, .value = value, .line = scanner.line };
      }
      if(isdigit(*scanner.position)) {
        char* start = scanner.position;
        while(isdigit(*scanner.position)) {
          scanner.position++;
        }
        size_t len = sizeof(char) * (scanner.position - start);
        char* value = (char*)malloc(len + 1);
        memcpy(value, start, len);
        value[len] = '\0';
        return (struct Token){ .type = T_NUM, .value = value, .line = scanner.line };
      }

      printf("Unknown token %c at line %d\n", *scanner.position, scanner.line);
      exit(1);
  }
}
void initializeScanner(char* file) {
  scanner.position = file;
  scanner.line = 1;
}

void printToken(struct Token t) {
  static char* names[] = {"VAR", "WHILE", "DO", "END", "PRINT", "ID", "NUM", "LPAR", "RPAR", "PLUS", "MINUS", "EQUALS", "NEWLINE", "EOF"}; 
  printf("Token{ type=%s, value=%s, line=%d }\n", names[t.type], t.value, t.line);
}



