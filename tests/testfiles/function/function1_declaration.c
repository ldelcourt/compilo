
int fonction_int_sans_params();
int fonction_int_1_params( int a);
int fonction_int_3_params(int a, int b, int c);

void fonction_void_sans_params();
void fonction_void_1_params(int a);
void fonction_void_3_params(int a, int b, int c);

int main() {

  int a = fonction_int_sans_params();
  int b = fonction_int_1_params(4);
  int c = fonction_int_3_params(4, 5, 6);

  fonction_void_sans_params();
  fonction_void_1_params(a);
  fonction_void_3_params(a, b, c);

  return 4;

}
