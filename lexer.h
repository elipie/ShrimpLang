#ifndef lexer_h
#define lexer_h
struct Token {
enum TokenType {
  T_VAR, // var
  T_WHILE, // while 
  T_DO, // do 
  T_END, // end 
  T_PRINT, // println
  T_ID, // x, y, z
  T_NUM,  // 1, 2, 3
  T_LPAR, // (
  T_RPAR, //)
  T_PLUS, //+
  T_MINUS, // -
  T_EQ, //=
  T_SEP, // force newline
  T_EOF // end of file
} type;


  char* value;

  int line;
};
struct Token scan(void);

#endif
