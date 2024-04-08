#include <stdio.h>
int fonction_int_1_params(int a, int b, int c, int d, int e, int f, int g, int h, int i, int k, int j, int m, int l) {
  putchar('A');
  return a + b + c + d + e + f + g + h + i + k + j + m + l;
}

int main() {

  int b = fonction_int_1_params(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);

  return b;

}