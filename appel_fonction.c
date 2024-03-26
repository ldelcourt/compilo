#include <stdio.h>

int main() {

  int c = (4*5+6)*getchar()+9;

  putchar(c);
  int d = getchar();
  putchar(d);

  c=c-d;

  putchar(c);

  return c;

}
