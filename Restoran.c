#include <stdio.h>
#include <stdlib.h> // exit fonksiyonu için

struct Yemek
{
    char ad[50];
    float fiyat;
    int hazirlanma_suresi;
    char durum[20];
};

void yemekEkle(FILE *dosya)
{
    fseek(dosya, 0, SEEK_END); // Dosyanın sonuna git
    struct Yemek yeniYemek;
    printf("Yeni yemek bilgilerini girin:\n");
    printf("Ad: ");
    scanf("%s", yeniYemek.ad);
    printf("Fiyat: ");
    scanf("%f", &yeniYemek.fiyat);
    printf("Hazirlanma suresi (dakika): ");
    scanf("%d", &yeniYemek.hazirlanma_suresi);
    printf("Durum (Hazir/Mevcut/Yok): ");
    scanf("%s", yeniYemek.durum);

    // Dosyaya yazma
    fprintf(dosya, "%s %.2f %d %s\n", yeniYemek.ad, yeniYemek.fiyat, yeniYemek.hazirlanma_suresi, yeniYemek.durum);
    printf("Yeni yemek eklendi!\n");
}

void yemekGuncelle(FILE *dosya)
{
    fseek(dosya, 0, SEEK_SET); // Dosyanın başına git
    char hedefAd[50];
    printf("Guncellenecek yemegin adini girin: ");
    scanf("%s", hedefAd);

    // Geçici dosya oluştur
    FILE *geciciDosya = fopen("gecici.txt", "w");

    // Yemekleri geçici dosyaya kopyala, hedef yemeği güncelle
    struct Yemek yemek;
    while (fscanf(dosya, "%s %f %d %s", yemek.ad, &yemek.fiyat, &yemek.hazirlanma_suresi, yemek.durum) != EOF)
    {
        if (strcmp(yemek.ad, hedefAd) == 0)
        {
            printf("Yeni bilgileri girin:\n");
            printf("Fiyat: ");
            scanf("%f", &yemek.fiyat);
            printf("Hazirlanma suresi (dakika): ");
            scanf("%d", &yemek.hazirlanma_suresi);
            printf("Durum (Hazir/Mevcut/Yok): ");
            scanf("%s", yemek.durum);
        }
        fprintf(geciciDosya, "%s %.2f %d %s\n", yemek.ad, yemek.fiyat, yemek.hazirlanma_suresi, yemek.durum);
    }

    fclose(dosya);
    fclose(geciciDosya);

    // Geçici dosyayı asıl dosya ile değiştir
    remove("yemeklistesi.txt");
    rename("gecici.txt", "yemeklistesi.txt");

    printf("Yemek guncellendi!\n");
}

void yemekSil(FILE *dosya)
{
    fseek(dosya, 0, SEEK_SET); // Dosyanın başına git
    char hedefAd[50];
    printf("Silinecek yemegin adini girin: ");
    scanf("%s", hedefAd);

    // Geçici dosya oluştur
    FILE *geciciDosya = fopen("gecici.txt", "w");

    // Yemekleri geçici dosyaya kopyala, hedef yemeği sil
    struct Yemek yemek;
    while (fscanf(dosya, "%s %f %d %s", yemek.ad, &yemek.fiyat, &yemek.hazirlanma_suresi, yemek.durum) != EOF)
    {
        if (strcmp(yemek.ad, hedefAd) != 0)
        {
            fprintf(geciciDosya, "%s %.2f %d %s\n", yemek.ad, yemek.fiyat, yemek.hazirlanma_suresi, yemek.durum);
        }
    }

    fclose(dosya);
    fclose(geciciDosya);

    // Geçici dosyayı asıl dosya ile değiştir
    remove("yemeklistesi.txt");
    rename("gecici.txt", "yemeklistesi.txt");

    printf("Yemek silindi!\n");
}

int main()
{
    FILE *dosya = fopen("yemeklistesi.txt", "r+");
    if (dosya == NULL)
    {
        printf("Dosya açma hatasi!\n");
        exit(1); // Programı sonlandır
    }

    char secim;
    printf("1. Yemek Ekle\n");
    printf("2. Yemek Guncelle\n");
    printf("3. Yemek Sil\n");
    printf("Seciminizi yapin (1/2/3): ");
    scanf(" %c", &secim); // Boşluk karakteri ekleyerek scanf'in tamponunu temizle

    switch (secim)
    {
    case '1':
        yemekEkle(dosya);
        break;
    case '2':
        yemekGuncelle(dosya);
        break;
    case '3':
        yemekSil(dosya);
        break;
    default:
        printf("Gecersiz secim!\n");
        break;
    }

    fclose(dosya);
    return 0;
}
