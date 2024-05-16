#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Yemek
{
    char ad[50];
    float fiyat;
    int hazirlanma_suresi;
    char durum[20];
    bool onay;
};

struct Siparis
{
    int id;
    char ad[50];
    float fiyat;
    char tarih[20];
    char bitisTarih[20];
    char kullanici[50];
    char masa[10];
    int onay; // 1: onaylandı, 0: red edildi
};

void siparisOnay(FILE *dosya)
{
    FILE *mutfakDosya = fopen("mutfak.txt", "a");
    fseek(dosya, 0, SEEK_SET);       // Dosyanın başına git
    fseek(mutfakDosya, 0, SEEK_END); // Dosyanın sonuna git
    struct Siparis siparis;
    printf("Onaylanacak siparis numarasini giriniz:\n");
    int siparisNo;
    scanf("%d", &siparisNo);

    while (fscanf(dosya, "%d %s %f %s %s %s %s", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa) != EOF)
    {

        if (siparis.id == siparisNo)
        {
            siparis.onay = 1;
            printf("Siparis onaylandi!\n %d", siparisNo);
            fprintf(mutfakDosya, "%d %s %f %s %s %s %s\n", siparis.id, siparis.ad, siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa);
        }
    }
}

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
    if (geciciDosya == NULL)
    {
        perror("Gecici dosya olusturulamadi");
        return;
    }

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
    if (geciciDosya == NULL)
    {
        perror("Gecici dosya olusturulamadi");
        return;
    }

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
    FILE *siparisDosya = fopen("siparisler.txt", "r+");
    if (dosya == NULL)
    {
        printf("Dosya açma hatasi!\n");
        exit(1); // Programı sonlandır
    }

    if (siparisDosya == NULL)
    {
        printf("Siparis dosyasi acilamadi!\n");
        exit(1);
    }

    char secim;
    printf("1. Yemek Ekle\n");
    printf("2. Yemek Guncelle\n");
    printf("3. Yemek Sil\n");
    printf("4. Siparis Onayla\n");
    printf("Seciminizi yapin (1/2/3/4): ");
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
    case '4':
        siparisOnay(siparisDosya);
        break;
    default:
        printf("Gecersiz secim!\n");
        break;
    }

    fclose(dosya);
    return 0;
}
