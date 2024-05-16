#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *dosya = fopen("Mutfak.txt", "r");

    if (dosya == NULL) {
        printf("Dosya açma hatası!\n");
        return 1;
    }

    char siparis[20]; // Siparişi tutmak için bir karakter dizisi
    char tarih[20]; // Tarihi tutmak için bir karakter dizisi
    char saat_str[20]; // Saati tutmak için bir karakter dizisi

    fscanf(dosya, "%s %s %s", siparis, tarih, saat_str); // Dosyadan siparişi, tarihi ve saati oku
    fclose(dosya); // Dosyayı kapat

    int saat, dakika;
    sscanf(saat_str, "%d.%d", &saat, &dakika); // Saati ve dakikayı al

    // Siparişe göre süre ekle
    if (strcmp(siparis, "hamburger") == 0) {
        dakika += 40;
    }

    // Dakikaya göre saati güncelle
    if (dakika >= 60) {
        saat += dakika / 60;
        dakika %= 60;
    }

    printf("Yeni saat: %s - %02d.%02d\n", tarih, saat, dakika);
getchar();
    return 0;
}
