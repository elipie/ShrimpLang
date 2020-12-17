#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"
#include "ast.h"
struct {
  struct Token next;
  struct Token current;
} parser;
void advance() {
  parser.current = parser.next;
  parser.next = scan();
}

void consume(enum TokenType type, char* errmessage) {
  if(parser.next.type == type)
    advance();
  else {
    fprintf(stderr, "[line %d] Error at ", parser.next.line);
    if(parser.next.type == T_EOF)
      fprintf(stderr, "end");
    else if(parser.next.type == T_SEP)
      fprintf(stderr, "<newline>");
    else
      fprintf(stderr, "\"%s\"", parser.next.value);
    fprintf(stderr, ": %s\n", errmessage);
    exit(1);
  }
}
struct ExprNode* parse_binop_expression() {
  struct ExprNode* expr = parse_unary_op_expression();
  while(parser.next.type == T_PLUS || parser.next.type == T_MINUS){
    char type = parser.next.type == T_PLUS ? '+' : '-';
    advance();
    struct ExprNode* left = expr;
    struct ExprNode* right = parse_unary_op_expression();
    
    expr = (struct ExprNode*)malloc(sizeof(struct ExprNode));
    *expr = (struct ExprNode){ .type = BINARY_OP, .as.binary_op = { .left = left, .right = right, .binop_type = type } };
  }
  return expr;
}
struct ExprNode* parse_expression() {
  struct ExprNode* expr = parse_binop_expression();
  if(parser.next.type == T_EQ){
    char type = '=';
    advance();
    struct ExprNode* left = expr;
    struct ExprNode* right = parse_expression();
    
    expr = (struct ExprNode*)malloc(sizeof(struct ExprNode));
    *expr = (struct ExprNode){ .type = BINARY_OP, .as.binary_op = { .left = left, .right = right, .binop_type = type } };
  }
  return expr;
}
struct StmtNode* parse_statement() {
  if(parser.next.type == T_VAR) {
    advance(); // VAR token

    consume(T_ID, "Expect identifier as variable name!");
    char* id = parser.current.value;
    consume(T_SEP, "Expect newline after statement!");

    struct StmtNode* varstmt = (struct StmtNode*)malloc(sizeof(struct StmtNode));
    *varstmt = (struct StmtNode){ .type = VAR_DECL, .as.var_decl = { .name = id } };
    return varstmt;
  }
  else if (parser.next.type == T_PRINT) {
    advance(); // PRINT token

    struct ExprNode* expression = parse_expression();
    consume(T_SEP, "Expect newline after statement!");
    
    struct StmtNode* printstmt = (struct StmtNode*)malloc(sizeof(struct StmtNode));
    *printstmt = (struct StmtNode){ .type = PRINT_STMT, .as.print_stmt = { .expression = expression } };
    return printstmt;
  }
  else if (parser.next.type == T_WHILE) {
    advance(); // WHILE token

    struct ExprNode* condition = parse_expression();
    consume(T_DO, "Expect DO after while statement condition!");
    if(parser.next.type == T_SEP) advance(); // optional newline

    struct StmtNode** statements = (struct StmtNode**)malloc(1 * sizeof(struct StmtNode*));
    int capacity = 1;
    int len = 0;

    while(parser.next.type != T_END) {
      statements[len] = parse_statement();
      len ++;

      if(len >= capacity) {
        capacity *= 2;
        statements = (struct StmtNode**)realloc(statements, capacity * sizeof(struct StmtNode*));
      }
    }

    statements = (struct StmtNode**)realloc(statements, len * sizeof(struct StmtNode*));

    advance(); // END
    consume(T_SEP, "Expect newline after while statement END!");

    struct StmtNode* whilestmt = (struct StmtNode*)malloc(sizeof(struct StmtNode));
    *whilestmt = (struct StmtNode){ .type = WHILE_STMT, .as.while_stmt = { .expression = condition, .statements = statements, .num = len } };
    return whilestmt;
  }
  else {
    struct ExprNode* expression = parse_expression();
    consume(T_SEP, "Expect newline after statement!");
    
    struct StmtNode* exprstmt = (struct StmtNode*)malloc(sizeof(struct StmtNode));
    *exprstmt = (struct StmtNode){ .type = EXPR_STMT, .as.expr_stmt = { .expression = expression } };
    return exprstmt;
  }
}
struct Program parse() {
  advance();

  struct Program prog = { .stmtnum = 0, .statements = (struct StmtNode**)malloc(1 * sizeof(struct StmtNode*)) };
  int initcap = 1;

  while(parser.next.type != T_EOF) {
    prog.statements[prog.stmtnum] = parse_statement();
    prog.stmtnum ++;

    if(prog.stmtnum >= initcap) {
      initcap *= 2;
      prog.statements = (struct StmtNode**)realloc(prog.statements, initcap * sizeof(struct StmtNode*));
    }
  }
  
  prog.statements = (struct StmtNode**)realloc(prog.statements, prog.stmtnum * sizeof(struct StmtNode*));

  return prog;
}



