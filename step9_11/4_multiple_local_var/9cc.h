#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
// Tokenizer
//
typedef enum
{
  TK_RESERVED, // 記号
  TK_IDENT,    // Identifiers
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

// トークン型
typedef struct Token Token;
struct Token
{
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
char *strndup(char *p, int len);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize();

extern char *user_input;
extern Token *token;

//
// Parser
//

// Local variable
typedef struct Var Var;
struct Var
{
  Var *next;
  char *name; // Variable name
  int offset; // Offset from RBP
};

// AST node
typedef enum
{
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_ASSIGN,    // =
  ND_RETURN,    // "return"
  ND_EXPR_STMT, // Expression statement
  ND_VAR,       // Variable
  ND_NUM,       // 整数
} NodeKind;

// 抽象構文木のノードの型
typedef struct Node Node;
struct Node
{
  NodeKind kind; // ノードの型
  Node *next;    // 次のノード
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  Var *var;      // Used if kind == ND_VAR
  int val;       // kindがND_NUMの場合のみ使う
};

typedef struct
{
  Node *node;
  Var *locals;
  int stack_size;
} Program;

Program *program();

//
// Code generator
//
void codegen(Program *prog);