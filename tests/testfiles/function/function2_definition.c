#include <stdio.h>


int fonction_int_sans_params() {
  return 4;
}

int fonction_int_1_params( int a) {

  a = a+1;
  return a;
}

int fonction_int_3_params(int a, int b, int c) {

  int e = a+b+c;
  return e;
}

void fonction_void_sans_params() {

  putchar('a');

}
  
void fonction_void_1_params(int a) {

  putchar('A'+a);

}
  
void fonction_void_3_params(int a, int b, int c) {

  putchar('A'+a+b+c);
}

int main() {

  int a = fonction_int_sans_params();
  int b = fonction_int_1_params(4);
  int c = fonction_int_3_params(4, 5, 6);

  fonction_void_sans_params();
  fonction_void_1_params(a);
  fonction_void_3_params(a, b, c);

  return 4;

}
