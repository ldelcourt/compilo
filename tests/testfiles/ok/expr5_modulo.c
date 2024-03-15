int main() {

  int a = 2;
  int b = 3;

  int c;
  c = 255 % 2;

  c = 46;
  c = c % b;

  c = c % a % b;

  c = 47;
  c = c % 7;

  return c;

}
