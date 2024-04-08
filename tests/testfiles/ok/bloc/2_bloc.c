int main() {

  int a =1;
  int b = 2;
  int c;
  {
    int a = 3;
    {
      int c = 3;
      int a;
      a = 3 * c;
      c = a + c;
    }
    {
      int c = 3;
      int a;
      c = a;
      a = 3 * c;
    }
    b = a;
    c = b;
  }
  c = c +b + a;
  return c;

}
