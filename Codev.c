#include <stdio.h>
int main(int argc, char const *argv[])
{
    
    FILE *dosya = fopen("yemeklistesi.txt", "a");
  
  fprintf(dosya, "Merhaba ben Yusuf SEZER\n");

  fclose(dosya);



    getchar();
    return 0;
}
