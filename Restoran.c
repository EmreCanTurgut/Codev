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

void gunlukRapor(FILE *siparisDosya)
{
    FILE *raporDosya;
    char tarih[11]; // "dd-mm-yyyy\0" için 11 karakterlik alan
    char raporDosyaAdi[50];

    printf("Rapor almak istediginiz tarihi girin (dd.mm.yyyy): ");
    scanf("%s", tarih);

    // Rapor dosyasının adını oluştur
    sprintf(raporDosyaAdi, "gunluk_rapor_%s.txt", tarih);

    // Rapor dosyasını oluştur veya aç
    raporDosya = fopen(raporDosyaAdi, "w");
    if (raporDosya == NULL)
    {
        perror("Rapor dosyasi olusturulamadi");
        return;
    }

    // Siparişler dosyasından ilgili tarihe ait siparişleri oku ve rapor dosyasına yaz
    struct Siparis siparis;
    char line[256]; // Satırı okumak için kullanılacak karakter dizisi
    while (fgets(line, sizeof(line), siparisDosya) != NULL)
    {
        // Satırdan gerekli bilgileri almak için sscanf kullanabiliriz
        sscanf(line, "%d %s %f %10[^-] %s %s %s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay);

        printf("Siparis tarihi: %s\n %s", siparis.tarih, tarih);
        // Siparişin tarih bilgisini kontrol et
        if (strcmp(tarih, siparis.tarih) == 0)
        {
            // İlgili tarih bilgisini içeren sipariş, rapor dosyasına yaz
            fprintf(raporDosya, "%d %s %.2f %s %s %s %s %d\n", siparis.id, siparis.ad, siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, siparis.onay);
        }
    }

    printf("Günlük rapor oluşturuldu: %s\n", raporDosyaAdi);

    // Dosyaları kapat
    fclose(raporDosya);
}

void gunlukKazanc(FILE *siparisDosya)
{
    float toplamKazanc = 0.0;
    char tarih[11]; // "dd.mm.yyyy\0" için 11 karakterlik alan

    printf("Kazancini ogrenmek istediginiz tarihi girin (dd.mm.yyyy): ");
    scanf("%10s", tarih);

    // Siparişler dosyasından ilgili tarihe ait siparişleri oku ve kazancı hesapla
    struct Siparis siparis;
    char line[256]; // Satırı okumak için kullanılacak karakter dizisi
    while (fgets(line, sizeof(line), siparisDosya) != NULL)
    {
        // Satırdan gerekli bilgileri almak için sscanf kullanabiliriz
        sscanf(line, "%d %49s %f %19s %19s %49s %9s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay);

        // Siparişin tarih bilgisini kontrol et
        if (strncmp(tarih, siparis.tarih, 10) == 0)
        {
            // İlgili tarih bilgisini içeren siparişin fiyatını toplam kazanca ekle
            toplamKazanc += siparis.fiyat;
        }
    }

    printf("Gunluk Kazanc: %.2f TL\n", toplamKazanc);
}

void aylikKazanc(FILE *siparisDosya)
{
    char ay[3], yil[5]; // Ay için 2 karakter, yıl için 4 karakter
    printf("Kazancini gormek istediginiz ayi girin (mm-yyyy formatinda): ");
    scanf("%2s-%4s", ay, yil); // Ay ve yıl olarak ayrılmış olarak oku

    struct Siparis siparis;
    float toplamKazanc = 0.0;

    // Dosyanın başına git
    fseek(siparisDosya, 0, SEEK_SET);

    while (fscanf(siparisDosya, "%d %s %f %s %s %s %s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay) != EOF)
    {
        int siparisAy, siparisYil;
        // Tarihi ay ve yıl olarak ayır
        sscanf(siparis.tarih, "%*d.%2d.%*d", &siparisAy);
        sscanf(siparis.tarih, "%*d.%*d.%4d", &siparisYil);
        // Ay ve yıl eşleşiyorsa, toplam kazanca ekle
        printf("%d %d", siparisAy, siparisYil);
        printf("%d %d", atoi(ay), atoi(yil));

        if (siparisAy == atoi(ay) && siparisYil == atoi(yil))
        {
            toplamKazanc += siparis.fiyat;
        }
    }

    printf("Belirtilen aydaki toplam kazanc: %.2f\n", toplamKazanc);
}

void gunlukRaporAl(char *tarih)
{
    char dosyaAdi[50];
    sprintf(dosyaAdi, "gunluk_rapor_%s.txt", tarih); // Tarihle ilişkilendirilmiş dosya adı oluştur

    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL)
    {
        printf("Gunluk rapor bulunamadi!\n");
        return;
    }

    struct Siparis siparis;
    printf("Tarih: %s\n", tarih);
    printf("Siparisler:\n");
    while (fscanf(dosya, "%d %s %f %s %s %s %s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay) != EOF)
    {
        printf("ID: %d, Ad: %s, Fiyat: %.2f, Tarih: %s, Bitis Tarihi: %s, Kullanici: %s, Masa: %s, Onay: %s\n", siparis.id, siparis.ad, siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, siparis.onay ? "Onaylandi" : "Red Edildi");
    }

    fclose(dosya);
}

int siparisOnaylanmisMi(FILE *mutfakDosya, int siparisNo)
{
    struct Siparis siparis;
    fseek(mutfakDosya, 0, SEEK_SET); // Dosyanın başına git

    while (fscanf(mutfakDosya, "%d %49s %f %19s %19s %49s %9s", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa) != EOF)
    {
        if (siparis.id == siparisNo)
        {
            return 1; // Sipariş zaten onaylanmış
        }
    }
    return 0; // Sipariş onaylanmamış
}

void siparisOnay(FILE *dosya)
{
    FILE *mutfakDosya = fopen("mutfak.txt", "a+"); // "a+" modunu kullanarak dosyayı hem okuyabilir hem yazabiliriz
    if (mutfakDosya == NULL)
    {
        perror("mutfak.txt dosyası açılamadı");
        return;
    }

    fseek(dosya, 0, SEEK_SET); // Dosyanın başına git
    struct Siparis siparis;
    printf("Onaylanacak siparis numarasini giriniz:\n");
    int siparisNo;
    scanf("%d", &siparisNo);

    if (siparisOnaylanmisMi(mutfakDosya, siparisNo))
    {
        printf("Bu siparis zaten onaylanmis.\n");
        fclose(mutfakDosya);
        return;
    }

    int siparisBulundu = 0; // Sipariş bulunduğunda döngüden çıkmak için

    while (fscanf(dosya, "%d %49s %f %19s %19s %49s %9s", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa) != EOF)
    {
        if (siparis.id == siparisNo)
        {
            siparis.onay = 1;
            printf("Siparis onaylandi! Siparis No: %d\n", siparisNo);
            fseek(mutfakDosya, 0, SEEK_END); // Dosyanın sonuna git
            fprintf(mutfakDosya, "%d %s %.6f %s %s %s %s\n", siparis.id, siparis.ad, siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa);
            siparisBulundu = 1;
            break; // Sipariş bulundu ve onaylandı, döngüden çık
        }
    }

    if (!siparisBulundu)
    {
        printf("Siparis bulunamadi! Siparis No: %d\n", siparisNo);
    }

    fclose(mutfakDosya);
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
        exit(1);
    }

    if (siparisDosya == NULL)
    {
        printf("Siparis dosyasi acilamadi!\n");
        exit(1);
    }
    char kazancSecim;
    char secim;
    printf("1. Yemek Ekle\n");
    printf("2. Yemek Guncelle\n");
    printf("3. Yemek Sil\n");
    printf("4. Siparis Onayla\n");
    printf("5. Gunluk Rapor Olustur\n");
    printf("6. Gunluk Rapor Oku\n");
    printf("7. Analizler Bolumu \n");
    printf("Seciminizi yapin (1/2/3/4/5/6/7): ");
    scanf(" %c", &secim);

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
    case '5':
        gunlukRapor(siparisDosya);
        break;
    case '6':
        printf("Goruntulemek istediginiz tarihi girin (dd.mm.yyyy formatinda): ");
        char tarih[20];
        scanf("%s", tarih);
        gunlukRaporAl(tarih);
        break;
    case '7':
        printf("\033[2J\033[H");
        printf("1. Gunluk Kazanc \n");
        printf("2. Aylik Kazanc \n");
        printf("3. Donemsel Kazanc \n");
        printf("Seciminizi Yapin (1/2/3) \n");
        scanf(" %c", &kazancSecim);
        switch (kazancSecim)
        {
        case '1':
            gunlukKazanc(siparisDosya);
            break;
        case '2':
            aylikKazanc(siparisDosya);
            break;
        default:
            printf("Geçersiz Seçim");
            break;
        }
    default:
        printf("Gecersiz secim!\n");
        break;
    }

    fclose(dosya);
    getchar();
    return 0;
}
