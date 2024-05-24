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

void enCokSiparisVerenKullanici(FILE *siparisDosya)
{
    struct Siparis siparis;
    char line[256];
    char kullanicilar[100][50];            // Maksimum 100 farklı kullanıcı varsayıyoruz
    int kullaniciSiparisSayisi[100] = {0}; // Bu kullanıcıların sipariş sayılarını saklayacak dizi
    int kullaniciSayisi = 0;               // Farklı kullanıcı sayısını tutacak değişken

    while (fgets(line, sizeof(line), siparisDosya) != NULL)
    {
        char *token = strtok(line, " "); // Satırı boşluk karakterlerine göre parçala
        if (token != NULL)
        {
            sscanf(token, "%d", &siparis.id); // İlk parçayı sipariş numarası olarak al
            token = strtok(NULL, " ");        // Bir sonraki parçaya geç
            if (token != NULL)
            {
                strcpy(siparis.ad, token); // İkinci parçayı yemek adı olarak al
                token = strtok(NULL, " "); // Bir sonraki parçaya geç
                if (token != NULL)
                {
                    sscanf(token, "%f", &siparis.fiyat); // Üçüncü parçayı fiyat olarak al
                    token = strtok(NULL, " ");           // Bir sonraki parçaya geç
                    if (token != NULL)
                    {
                        strncpy(siparis.tarih, token, 10); // Dördüncü parçayı tarih olarak al
                        siparis.tarih[10] = '\0';          // String sonunu işaretle
                        token = strtok(NULL, " ");         // Bir sonraki parçaya geç
                        if (token != NULL)
                        {
                            strcpy(siparis.bitisTarih, token); // Beşinci parçayı bitiş tarihi olarak al
                            token = strtok(NULL, " ");         // Bir sonraki parçaya geç
                            if (token != NULL)
                            {
                                strcpy(siparis.kullanici, token); // Altıncı parçayı kullanıcı adı olarak al
                                token = strtok(NULL, " ");        // Diğer parçalara geç
                                if (token != NULL)
                                {
                                    strcpy(siparis.masa, token); // Yedinci parçayı masa bilgisi olarak al
                                    token = strtok(NULL, " ");   // Diğer parçalara geç
                                    if (token != NULL)
                                    {
                                        sscanf(token, "%d", &siparis.onay); // Sekizinci parçayı onay durumu olarak al
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Kullanıcıyı kullanicilar dizisinde arayalım
        int found = -1;
        for (int i = 0; i < kullaniciSayisi; i++)
        {
            if (strcmp(kullanicilar[i], siparis.kullanici) == 0)
            {
                found = i;
                break;
            }
        }

        if (found != -1)
        {
            // Eğer kullanıcı bulunduysa, sipariş sayısını arttır
            kullaniciSiparisSayisi[found]++;
        }
        else
        {
            // Eğer kullanıcı bulunmadıysa, yeni bir kullanıcı ekle
            strcpy(kullanicilar[kullaniciSayisi], siparis.kullanici);
            kullaniciSiparisSayisi[kullaniciSayisi] = 1;
            kullaniciSayisi++;
        }
    }

    // En çok sipariş veren kullanıcıyı bul
    int maxIndex = 0;
    for (int i = 1; i < kullaniciSayisi; i++)
    {
        if (kullaniciSiparisSayisi[i] > kullaniciSiparisSayisi[maxIndex])
        {
            maxIndex = i;
        }
    }

    printf("En Cok Siparis Veren Kullanici: %s, Siparis Sayisi: %d\n", kullanicilar[maxIndex], kullaniciSiparisSayisi[maxIndex]);
}

void enKazancliGun(FILE *siparisDosya)
{
    struct Siparis siparis;
    char line[256];
    char gunler[100][20];         // Maksimum 100 farklı gün varsayıyoruz
    float gunKazanc[100] = {0.0}; // Bu günlerin kazançlarını saklayacak dizi
    int gunSayisi = 0;            // Farklı gün sayısını tutacak değişken

    while (fgets(line, sizeof(line), siparisDosya) != NULL)
    {
        sscanf(line, "%d %s %f %19[^-] %s %s %s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay);

        // Günü gunler dizisinde arayalım
        int found = -1;
        for (int i = 0; i < gunSayisi; i++)
        {
            if (strcmp(gunler[i], siparis.tarih) == 0)
            {
                found = i;
                break;
            }
        }

        if (found != -1)
        {
            // Eğer gün bulunduysa, kazancını ekle
            gunKazanc[found] += siparis.fiyat;
        }
        else
        {
            // Eğer gün bulunmadıysa, yeni bir gün ekle
            strcpy(gunler[gunSayisi], siparis.tarih);
            gunKazanc[gunSayisi] = siparis.fiyat;
            gunSayisi++;
        }
    }

    // En kazançlı günü bul
    int maxIndex = 0;
    for (int i = 1; i < gunSayisi; i++)
    {
        if (gunKazanc[i] > gunKazanc[maxIndex])
        {
            maxIndex = i;
        }
    }

    printf("En Kazancli Gun: %s, Kazanc: %.2f TL\n", gunler[maxIndex], gunKazanc[maxIndex]);
}

void enCokTuketilenYemek(FILE *siparisDosya)
{
    struct Siparis siparis;
    char line[256];
    char yemekAdlari[100][50];    // Maksimum 100 farklı yemek adı varsayıyoruz
    int yemekAdetleri[100] = {0}; // Bu yemeklerin adetlerini saklayacak dizi
    int yemekSayisi = 0;          // Farklı yemek sayısını tutacak değişken

    while (fgets(line, sizeof(line), siparisDosya) != NULL)
    {
        sscanf(line, "%d %s %f %19[^-] %s %s %s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay);

        // Yemek adını yemekAdlari dizisinde arayalım
        int found = -1;
        for (int i = 0; i < yemekSayisi; i++)
        {
            if (strcmp(yemekAdlari[i], siparis.ad) == 0)
            {
                found = i;
                break;
            }
        }

        if (found != -1)
        {
            // Eğer yemek adı bulunduysa, adetini bir artır
            yemekAdetleri[found]++;
        }
        else
        {
            // Eğer yemek adı bulunmadıysa, yeni bir yemek ekle
            strcpy(yemekAdlari[yemekSayisi], siparis.ad);
            yemekAdetleri[yemekSayisi] = 1;
            yemekSayisi++;
        }
    }

    // En çok tüketilen yemeği bul
    int maxIndex = 0;
    for (int i = 1; i < yemekSayisi; i++)
    {
        if (yemekAdetleri[i] > yemekAdetleri[maxIndex])
        {
            maxIndex = i;
        }
    }

    printf("En Cok Tuketilen Yemek: %s, Tuketim Miktari: %d\n", yemekAdlari[maxIndex], yemekAdetleri[maxIndex]);
}

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

    struct Siparis siparis;
    char line[256];
    while (fgets(line, sizeof(line), siparisDosya) != NULL)
    {
        sscanf(line, "%d %s %f %10[^-] %s %s %s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay);

        if (strcmp(tarih, siparis.tarih) == 0)
        {
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
    char tarih[11];

    printf("Kazancini ogrenmek istediginiz tarihi girin (dd.mm.yyyy): ");
    scanf("%10s", tarih);

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
    char restoranYil[8]; // Ay için 2 karakter, yıl için 4 karakter
    printf("Kazancini gormek istediginiz ayi girin (mm.yyyy formatinda): ");
    scanf("%s", restoranYil); // Ay ve yıl olarak ayrılmış olarak oku

    struct Siparis siparis;
    float toplamKazanc = 0.0;
    char buffer[256];

    // Dosyanın başına git
    fseek(siparisDosya, 0, SEEK_SET);

    while (fgets(buffer, sizeof(buffer), siparisDosya))
    {
        // Satırı ayrıştır
        int fieldCount = sscanf(buffer, "%d %49s %f %10s %19s %49s %9s",
                                &siparis.id, siparis.ad, &siparis.fiyat,
                                siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa);
        char ayYil[8];
        strncpy(ayYil, siparis.tarih + 3, 7);
        ayYil[7] = '\0';

        if (strcmp(ayYil, restoranYil) == 0)
        {
            printf("%s\n", ayYil); // Sadece test amaçlı, belirtilen ay ve yılın doğru şekilde alındığını gösterir
            toplamKazanc += siparis.fiyat;
        }
    }

    printf("Belirtilen aydaki toplam kazanc: %.2f TL\n", toplamKazanc);
}

void donemselKazanc(FILE *siparisDosya)
{
    char baslangicTarih[11]; // "dd.mm.yyyy\0" için 11 karakterlik alan
    char bitisTarih[11];     // "dd.mm.yyyy\0" için 11 karakterlik alan

    printf("Baslangic tarihini girin (dd.mm.yyyy): ");
    scanf("%s", baslangicTarih);
    printf("Bitis tarihini girin (dd.mm.yyyy): ");
    scanf("%s", bitisTarih);

    struct Siparis siparis;
    float toplamKazanc = 0.0;
    char line[256]; // Satırı okumak için kullanılacak karakter dizisi

    // Siparişler dosyasından belirtilen tarih aralığındaki siparişleri oku ve kazancı hesapla
    fseek(siparisDosya, 0, SEEK_SET);
    while (fgets(line, sizeof(line), siparisDosya) != NULL)
    {
        // Satırdan gerekli bilgileri almak için sscanf kullanabiliriz
        sscanf(line, "%d %49s %f %19s %19s %49s %9s %d", &siparis.id, siparis.ad, &siparis.fiyat, siparis.tarih, siparis.bitisTarih, siparis.kullanici, siparis.masa, &siparis.onay);

        // Siparişin tarih bilgisini kontrol et
        if (strcmp(siparis.tarih, baslangicTarih) >= 0 && strcmp(siparis.tarih, bitisTarih) <= 0)
        {
            // İlgili tarih aralığındaki siparişin fiyatını toplam kazanca ekle
            toplamKazanc += siparis.fiyat;
        }
    }

    printf("Belirtilen donemdeki toplam kazanc: %.2f TL\n", toplamKazanc);
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

void anaMenu(FILE *dosya, FILE *siparisDosya)
{
    char secim;
    printf("\033[2J\033[H");
    printf("\n1. Yemek Ekle\n");
    printf("2. Yemek Guncelle\n");
    printf("3. Yemek Sil\n");
    printf("4. Siparis Onayla\n");
    printf("5. Gunluk Rapor Olustur\n");
    printf("6. Gunluk Rapor Oku\n");
    printf("7. Analizler Bolumu\n");
    printf("8. Istatistikler\n");
    printf("9. Cikis\n");
    printf("Seciminizi yapin (1/2/3/4/5/6/7/8/9): ");
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
        char kazancSecim;
        scanf(" %c", &kazancSecim);
        switch (kazancSecim)
        {
        case '1':
            gunlukKazanc(siparisDosya);
            break;
        case '2':
            aylikKazanc(siparisDosya);
            break;
        case '3':
            donemselKazanc(siparisDosya);
            break;
        default:
            printf("Gecersiz Seçim\n");
            break;
        }
        break;
    case '8':
        printf("\033[2J\033[H");
        printf("1) En Cok Tuketilen Yemek \n");
        printf("2) En Kazancli Gun\n");
        printf("3) En Cok Siparis Veren Kullanici\n");
        printf("Seciminizi Yapin (1/2/3) \n");
        char istatistikSecim;
        scanf(" %c", &istatistikSecim);
        switch (istatistikSecim)
        {
        case '1':
            enCokTuketilenYemek(siparisDosya);
            break;
        case '2':
            enKazancliGun(siparisDosya);
            break;
        case '3':
            enCokSiparisVerenKullanici(siparisDosya);
            break;
        default:
            printf("Gecersiz Secim\n");
            break;
        }
        break;
    case '9':
        printf("Uygulama kapatiliyor...\n");
        fclose(dosya);
        fclose(siparisDosya);
        exit(EXIT_SUCCESS);
    default:
        printf("Gecersiz secim!\n");
        break;
    }
}

int main()
{
    FILE *dosya = fopen("yemeklistesi.txt", "r+");
    FILE *siparisDosya = fopen("siparisler.txt", "r+");
    if (dosya == NULL || siparisDosya == NULL)
    {
        printf("Dosya acma hatasi!\n");
        exit(EXIT_FAILURE);
    }

    char devam;
    do
    {
        anaMenu(dosya, siparisDosya);
        printf("\nAna menuye donmek icin bir tusa basiniz (q cikis yapar): ");
        scanf(" %c", &devam);
    } while (devam != 'q' && devam != 'Q');

    fclose(dosya);
    fclose(siparisDosya);
    return 0;
}
