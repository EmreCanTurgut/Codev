#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Sipariş yapısı
typedef struct
{
    int id;
    char urun[50];
    float fiyat;
    int miktar;
    char kullanici[50];
} Siparis;

int main(int argc, char const *argv[])
{
    char kullanici[50];
    printf("Kullanici isminizi girin: ");
    scanf("%s", kullanici);

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
                strcpy(siparisler[yemek_sayisi].kullanici, kullanici);

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
    fprintf(dosya1, "%d\t%s\t%s\t%.2f\t%d\n",
            rand(),
            secilen_siparis.kullanici,
            secilen_siparis.urun,
            secilen_siparis.fiyat,
            secilen_siparis.miktar);

    printf("Siparisiniz kaydedildi: ID=%d, Urun=%s\n",
           secilen_siparis.id,
           secilen_siparis.urun);

    fclose(dosya1);

    // Kullanıcıya ait siparişleri göster
    FILE *dosya2 = fopen("siparislerim.txt", "r");
    if (dosya2 == NULL)
    {
        perror("Dosya açma hatası");
        return 1;
    }

    printf("\n%s kullanicisina ait siparisler:\n", kullanici);
    printf("ID   YEMEK   FIYAT   MIKTAR\n");

    int id;
    char sip_kullanici[50];
    char urun[50];
    float fiyat;
    int miktar;

    while (fscanf(dosya, "%d %s %s %f %d", &id, &sip_kullanici, urun, &fiyat, &miktar) != EOF)
    {
        printf("%d %s %s %f %d", &id, &sip_kullanici, urun, &fiyat, &miktar);
    }

    fclose(dosya2);
    getchar();
    return 0;
}
