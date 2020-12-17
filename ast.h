#ifndef ast_h
#define ast_h
struct ExprNode {
  enum {
    NUM_LITERAL,
    VAR_NAME,
    BINARY_OP,
    UNARY_OP
  } type;

  union {
    struct {
      long int num;
    } num_literal;

    struct {
      char* name;
    } var_name;

    struct {
      struct ExprNode* left;
      struct ExprNode* right;
      char binop_type;
    } binary_op;

    struct {
      struct ExprNode* left;
      char unop_type;
    } unary_op;
  } as;
};
struct Program {
  struct StmtNode** statements;
  int stmtnum;
};
struct StmtNode {
  enum {
    VAR_DECL,
    WHILE_STMT,
    PRINT_STMT,
    EXPR_STMT
  } type;

  union {
    struct {
      char* name;
    } var_decl;

    struct {
      struct ExprNode* expression;
      struct StmtNode** statements;
      int num;
    } while_stmt;

    struct {
      struct ExprNode* expression;
    } print_stmt;

    struct {
      struct ExprNode* expression;
    } expr_stmt;
  } as;
};


#endif
