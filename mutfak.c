#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *dosya = fopen("Mutfak.txt", "r");

    if (dosya == NULL)
    {
        printf("Dosya açma hatası!\n");
        return 1;
    }

    char siparis[20]; // Siparişi tutmak için bir karakter dizisi
    char tarih[20];   // Tarihi tutmak için bir karakter dizisi
    float saat;       // Saati tutmak için bir float
    char line[100];   // Dosyadan okunan satırı tutmak için bir karakter dizisi

    // Dosyadan satırı oku
    fgets(line, sizeof(line), dosya);

    // Okunan satırdan siparişi, tarihi ve saati ayır
    sscanf(line, "%s %s %f", siparis, tarih, &saat);

    fclose(dosya); // Dosyayı kapat

    // Saati ve dakikayı ayrı değişkenlerde sakla
    int saat_int = (int)saat;
    int dakika = (int)((saat - saat_int) * 100);

    // Siparişe göre süre ekle
    if (strcmp(siparis, "hamburger") == 0)
    {
        dakika += 40;
    }

    // Dakikaya göre saati güncelle
    saat_int += dakika / 60;
    dakika %= 60;

    printf("Yeni saat: %s - %02d.%02d\n", tarih, saat_int, dakika);
    return 0;
}
