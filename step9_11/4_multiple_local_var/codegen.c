#include "9cc.h"

// Pushes the given node's address to the stack.
void gen_addr(Node *node)
{
  if (node->kind == ND_VAR)
  {
    printf("  sub x0, x29, #%d\n", node->var->offset);
    printf("  str x0, [sp, -16]!\n");
    return;
  }

  error("not an lvalue");
}

void load()
{
  printf("  ldr x0, [sp], 16\n");
  printf("  ldr x0, [x0]\n");
  printf("  str x0, [sp, -16]!\n");
}

void store()
{
  printf("  ldr x1, [sp], 16\n");
  printf("  ldr x0, [sp], 16\n");
  printf("  str x1, [x0]\n");
  printf("  str x1, [sp, -16]!\n");
}

void gen(Node *node)
{
  switch (node->kind)
  {
  case ND_NUM:
    printf("  mov x0,  #%d\n", node->val);
    printf("  str x0, [sp, -16]!\n");
    return;
  case ND_EXPR_STMT:
    gen(node->lhs);
    printf("  add sp, sp, #16\n");
    return;
  case ND_VAR:
    gen_addr(node);
    load();
    return;
  case ND_ASSIGN:
    gen_addr(node->lhs);
    gen(node->rhs);
    store();
    return;

  case ND_RETURN:
    gen(node->lhs);
    printf("  ldr x0, [sp], 16\n");
    printf("  ret\n");
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  ldr x1, [sp], 16\n");
  printf("  ldr x0, [sp], 16\n");

  switch (node->kind)
  {
  case ND_ADD:
    printf("  add x0, x0, x1\n");
    break;
  case ND_SUB:
    printf("  sub x0, x0, x1\n");
    break;
  case ND_MUL:
    printf("  mul x0, x0, x1\n");
    break;
  case ND_DIV:
    printf("  sdiv x0, x0, x1\n");
    break;
  case ND_EQ:
    printf("  cmp x0, x1\n");
    printf("  cset x0, eq\n");
    break;
  case ND_NE:
    printf("  cmp x0, x1\n");
    printf("  cset x0, ne\n");
    break;
  case ND_LT:
    printf("  cmp x0, x1\n");
    printf("  cset x0, lt\n");
    break;
  case ND_LE:
    printf("  cmp x0, x1\n");
    printf("  cset x0, le\n");
    break;
  }

  printf("  str x0, [sp, -16]!\n");
}

void codegen(Program *prog)
{
  // アセンブリの前半部分を出力
  printf(".globl main\n");
  printf("main:\n");

  // Prologue
  printf("  stp x29, x30, [sp, -16]!\n");
  printf("  mov x29, sp\n");
  printf("  sub sp, sp, #%d\n", prog->stack_size);

  // Emit code
  for (Node *node = prog->node; node; node = node->next)
    gen(node);

  // Epilogue
  printf("  .Lreturn:\n");
  printf("  mov sp, x29\n");
  printf("  ldp x29, x30, [sp], #16\n");
  printf("  ret\n");
}