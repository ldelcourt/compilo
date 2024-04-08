int vmin(int a, int b) {
  if (a < b) {
    return a;
  }
  else {
    return b;
  }
}

int main() {
  vmin(5,8);
  return 0;
}
