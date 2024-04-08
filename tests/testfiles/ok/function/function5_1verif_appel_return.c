#include <stdio.h>

void max (int a, int b) {

  if (a < b) {
    putchar('A' + a);
  }
  else {
    putchar('A' + b);
  }

}

int main() {

  int vmax = max(5, 8);

  return 0;

}
