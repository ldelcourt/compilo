int toto(int a, int b, int c, int d, int e, int f, int g, int h) {
  int t1, t2;
  t1 = g;
  t2 = h;
  return a+b+c+d+e+f+g+h;
}

int main() {

    int a,b,c,d,e;
    a = 1;
    b= 2;
    c = 3;
    int f = toto(1,2,3,4,5,6,a,8);
    d = 4;
    e = 5;

    

    return f;
}
