#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *dosya = fopen("yemeklistesi.txt", "r");
    char yemek[100];
    while (fgets(yemek, 100, dosya) != NULL)
    {
        printf("%s", yemek);
    }
    return 0;
}
