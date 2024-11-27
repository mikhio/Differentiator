#include "cli_colors.h"
#include "iog_assert.h"

#include <stdio.h>

struct Context {
  char s[100];
  int p;
};

int GetG (Context *cont);
int GetE (Context *cont);
int GetT (Context *cont);
int GetP (Context *cont);
int GetN (Context *cont);

int GetG (Context *cont) {
  int val = GetE(cont);

  if (cont->s[cont->p] != '\0') {
    IOG_ASSERT(!"SyntaxError: No \0 symbols in the end");
  }
  (cont->p)++;

  return val;
}

int GetE (Context *cont) {
  int val = GetT(cont);

  while ((cont->s[cont->p] == '+') ||(cont->s[cont->p] == '-')) {
    char op = cont->s[cont->p];

    (cont->p)++;

    int val2 = GetT(cont);

    if (op == '+')
      val += val2;
    else
      val -= val2;
  }

  return val;
}

int GetT (Context *cont) {
  int val = GetP(cont);

  while ((cont->s[cont->p] == '*') ||(cont->s[cont->p] == '/')) {
    char op = cont->s[cont->p];

    (cont->p)++;

    int val2 = GetP(cont);

    if (op == '*')
      val *= val2;
    else
      val /= val2;
  }

  return val;
}

int GetP (Context *cont) {
  int val = 0;

  if (cont->s[cont->p] == '(') {
    (cont->p)++;
    val = GetE(cont);

    if (cont->s[cont->p] != ')')
      IOG_ASSERT(!"SyntaxError: No closing braket");

    (cont->p)++;
  } else {
    val = GetN(cont);
  }

  return val;
}

int GetN (Context *cont) {
  int val = 0;
  int old_p = cont->p;

  while (('0' <= cont->s[cont->p]) && (cont->s[cont->p] <= '9')) {
    val = val * 10 + cont->s[cont->p] - '0';
    (cont->p)++;
  }

  if (old_p == cont->p) {
    IOG_ASSERT(!"SynataxError: No num symbols");
  }

  return val;
}



int main(const int argc, const char *argv[]) {
  while (1) {
    Context cont = {"", 0};
    printf("Expr: ");
    scanf("%s", cont.s);
    printf(BLACK("Result: %d\n"), GetG(&cont));
  }

  return 0;
}
