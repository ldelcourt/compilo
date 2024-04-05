int toto(int a, int b, int c) {

  if (a + b + c > 81) {
    return a+b+c;
  }
  else {

    return toto(a+1, b+1, c+1);
  }
  
}


int main() {

  int a = toto(6, 4, 3);
  int d = a;

  return d;
}


