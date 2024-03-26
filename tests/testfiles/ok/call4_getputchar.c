#include <stdio.h>

int main() {

  int z = 'a';
  putchar(z+1);

  int c = getchar();

  putchar(c);
  putchar(c+1);

  int d = getchar();

  return d;

}
