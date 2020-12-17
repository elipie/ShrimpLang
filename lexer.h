#ifndef lexer_h
#define lexer_h
struct Token {
  enum TokenType {

  } type;

  char* value;

  int line;
};
struct Token scan(void);

#endif
