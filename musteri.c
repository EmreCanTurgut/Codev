#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Maksimum yemek sayısı ve maksimum string uzunluğu sabitleri
#define MAX_YEMEK 10
#define MAX_STR 50

// Sipariş yapısı
typedef struct
{
    int id;                // Sipariş ID'si
    char urun[MAX_STR];    // Ürün adı
    float fiyat;           // Ürün fiyatı
    int hazirlamasure;            // Ürün hazirlamasureı
    char kullanici[MAX_STR]; // Kullanıcı adı
} Siparis;

int yemekListesiniYazdirVeSiparisleriOku(FILE *dosya, Siparis siparisler[MAX_YEMEK], char kullanici[MAX_STR]);
int kullaniciSecimYap(int yemek_sayisi);
void kullaniciSiparisleriniGoster(FILE *dosya, const char kullanici[MAX_STR]);
void siparisKaydet(FILE *dosya, Siparis secilen_siparis);




FILE* dosyaAc(const char *dosyaAdi, const char *mod) {
    FILE *dosya = fopen(dosyaAdi, mod);
    if (dosya == NULL) {
        perror("Dosya açma hatası");
        exit(1); // programı kapatır
    }
    return dosya;
}



int main() {

    char kullanici[MAX_STR];
     printf("Kullanici isminizi girin: ");
     scanf("%s", kullanici);

    FILE *dosya = dosyaAc("yemeklistesi.txt", "r"); // Yemek listesi dosyasını aç


    Siparis siparisler[MAX_YEMEK];

    int yemek_sayisi = yemekListesiniYazdirVeSiparisleriOku(dosya, siparisler, kullanici); 
    fclose(dosya); // Dosyayı kapat

    int secim = kullaniciSecimYap(yemek_sayisi); 

    FILE *dosya1 = dosyaAc("siparislerim.txt", "a"); // Sipariş dosyasını aç
    Siparis secilen_siparis = siparisler[secim - 1]; // Seçilen siparişi al
    siparisKaydet(dosya1, secilen_siparis); 
    fclose(dosya1); 

    FILE *dosya2 = dosyaAc("siparislerim.txt", "r"); 
    kullaniciSiparisleriniGoster(dosya2, kullanici); 
    fclose(dosya2); 


    return 0;
}

// Yemek listesini yazdıran ve siparişleri okuyan fonksiyon
int yemekListesiniYazdirVeSiparisleriOku(FILE *dosya, Siparis siparisler[MAX_YEMEK], char kullanici[MAX_STR]) {
    printf("ID  YEMEK  FIYAT  SURE\n");

    char urun[MAX_STR];
    float fiyat;
    int hazirlamasure;
    char mevcut[10];
    int yemek_sayisi = 0;

    // Dosyadan satır satır okuma işlemi
    while (fscanf(dosya, "%s %f %d %s", urun, &fiyat, &hazirlamasure, mevcut) == 4 && yemek_sayisi < MAX_YEMEK) {
        // "Mevcut" alanı "True" ise kontrol et
        if (strcmp(mevcut, "True") == 0) {
            // Sipariş yapısını doldur
            siparisler[yemek_sayisi].id = yemek_sayisi + 1;
            strcpy(siparisler[yemek_sayisi].urun, urun); 
            siparisler[yemek_sayisi].fiyat = fiyat;
            siparisler[yemek_sayisi].hazirlamasure = hazirlamasure;
            strcpy(siparisler[yemek_sayisi].kullanici, kullanici);

            // Ekrana yazdır
            printf("%d %s %.2ftl %ddk\n",
                   siparisler[yemek_sayisi].id,
                   siparisler[yemek_sayisi].urun,
                   siparisler[yemek_sayisi].fiyat,
                   siparisler[yemek_sayisi].hazirlamasure);

            yemek_sayisi++;
        }
    }

    return yemek_sayisi;
}

// Kullanıcının yemek seçimini alan fonksiyon
int kullaniciSecimYap(int yemek_sayisi) {
    printf("Siparisinizi yapin (1-%d): ", yemek_sayisi);
    int secim;
    scanf("%d", &secim);

    // Geçersiz seçim kontrolü
    if (secim < 1 || secim > yemek_sayisi) {
        printf("Gecersiz secim!\n");
        exit(1); // programı sonlandırır
    }

    return secim;
}

// Kullanıcıya ait siparişleri gösteren fonksiyon
void kullaniciSiparisleriniGoster(FILE *dosya, const char kullanici[MAX_STR]) {
    printf("\n%s kullanicisina ait siparisler:\n", kullanici);
    printf("ID  KISI   YEMEK   FIYAT   SURE\n");

    int id;
    char sip_kullanici[MAX_STR];
    char urun[MAX_STR];
    float fiyat;
    int hazirlamasure;

    // Dosyadan satır satır okuma işlemi
    while (fscanf(dosya, "%d %49s %49s %f %d", &id, sip_kullanici, urun, &fiyat, &hazirlamasure) != EOF) {
        // Kullanıcı adı kontrolü
        if (strcmp(kullanici, sip_kullanici) == 0) {
            // Ekrana yazdırma işlemi
            printf("%d %s %s %.2f %d\n", id, sip_kullanici, urun, fiyat, hazirlamasure);
        }
    }
}



// Siparişi dosyaya kaydeden fonksiyon
void siparisKaydet(FILE *dosya, Siparis secilen_siparis) {
    srand(time(NULL)); // Rastgele sayı üreteci için tohum
    fprintf(dosya, "%d\t%s\t%s\t%.2f\t%d\n",
            rand(), // Rastgele bir ID oluşturur
            secilen_siparis.kullanici,
            secilen_siparis.urun,
            secilen_siparis.fiyat,
            secilen_siparis.hazirlamasure);

    printf("Siparisiniz kaydedildi: ID=%d, Urun=%s\n",
           secilen_siparis.id,
           secilen_siparis.urun);
}
