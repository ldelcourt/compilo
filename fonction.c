int test_decla(int a, int b);

void fonction_void () {
  putchar('t');
}


int toto(int a, int b, int c) {

  if (a + b + c > 81) {
    return a+b+c;
  }
  else {

    return toto(a+1, b+1, c+1);
  }
  
}


int main() {

  test_decla(4, 3);


  toto(1,2,3);
  
  int a = toto(6, 4, 3);
  int d = a;

  return d;
}


