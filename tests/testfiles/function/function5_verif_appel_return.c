#include <stdio.h>
int min (int a, int b) {

  if (a < b) {
    return a;
  }
  else {
    return b;
  }
}

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

  vmin(5,8);

  return 0;

}
