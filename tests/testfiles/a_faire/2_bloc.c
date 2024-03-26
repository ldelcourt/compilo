int main() {

  int a =1;
  int b = 2;
  int c;
  {
    int a = 3;
    {
      int c = 3;
      int a;
    }
    {
      int c = 3;
      int a;
    }
    b = a;
    c = b;
  }
  c = c +b;
  return c;

}
