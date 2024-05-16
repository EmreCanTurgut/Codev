#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Sipariş yapısı
typedef struct {
    int id;
    char urun[50];
    float fiyat;
    int miktar;
} Siparis;

int main(int argc, char const *argv[])
{
    FILE *dosya = fopen("yemeklistesi.txt", "r");
    if (dosya == NULL)
    {
        perror("Dosya açma hatası");
        return 1;
    }

    printf("ID   YEMEK  FIYAT       SURE\n");

    char satir[256];
    Siparis siparisler[10];
    int yemek_sayisi = 0;

    while (fgets(satir, sizeof(satir), dosya))
    {
        char urun[50];
        float fiyat;
        int miktar;
        char mevcut[10];

        // Satırı ayrıştır
        if (sscanf(satir, "%s %f %d %s", urun, &fiyat, &miktar, mevcut) == 4)
        {
            // Mevcut alanı "True" ise kontrol et
            if (strcmp(mevcut, "True") == 0)
            {
                // Mevcut "True" ise satırı yazdır ve sipariş yapısına ekle
                siparisler[yemek_sayisi].id = yemek_sayisi + 1;
                strcpy(siparisler[yemek_sayisi].urun, urun);
                siparisler[yemek_sayisi].fiyat = fiyat;
                siparisler[yemek_sayisi].miktar = miktar;

                printf("%2d %5s %10.2f %15d\n", 
                    siparisler[yemek_sayisi].id, 
                    siparisler[yemek_sayisi].urun, 
                    siparisler[yemek_sayisi].fiyat, 
                    siparisler[yemek_sayisi].miktar);

                yemek_sayisi++;
            }
        }
    }

    fclose(dosya);

    // Kullanıcının yemek seçimi
    printf("Siparisinizi yapin (1-%d): ", yemek_sayisi);
    int secim;
    scanf("%d", &secim);

    if (secim < 1 || secim > yemek_sayisi)
    {
        printf("Gecersiz secim!\n");
        return 1;
    }

    FILE *dosya1 = fopen("siparislerim.txt", "a");
    if (dosya1 == NULL)
    {
        perror("Dosya açma hatası");
        return 1;
    }
    srand(time(NULL));

    Siparis secilen_siparis = siparisler[secim - 1];
   // fprintf(dosya1, "SIPARIS ID: %d", secilen_siparis.id);
    fprintf(dosya1,"%d\t",rand()); // rastegele id atama
    fprintf(dosya1, " %s\t", secilen_siparis.urun);
    fprintf(dosya1, " %.2f\t", secilen_siparis.fiyat);
    fprintf(dosya1, " %d\t", secilen_siparis.miktar);
    fprintf(dosya1, "\n\n");
    printf("Siparisiniz kaydedildi: ID=%d, Urun=%s\n", 
        secilen_siparis.id, 
        secilen_siparis.urun);

    fclose(dosya1);

    getchar(); // Programın sonlanmasını geciktirmek için
    return 0;
}
