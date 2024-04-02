int main() {
  int a = 0;
  int i = 0;
  int b = 20;

  while(i<10){
    i = i + 1;
    while(a<b){
      a = a+i; 
    }
    a = 0;  
  }
  return a+i;
}