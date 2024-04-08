int min(int a, int b) {
  if (a < b) {
    return a;
  }
  else {
    return b;
  }
}

int main() {
  int a = 2;
  int b = 4;
  return min(a * 2, b ^ 1);
}