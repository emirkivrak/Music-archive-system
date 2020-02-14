#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SENTINEL -100000000
/**
 * ANADOSYA YAPISI   --> kayitkayitkayit
 * INDEX_DOSYASI YAPISI -> INDEX DOSYASI 10 A BÖLÜNMÜŞ HALDE ISBN NUMARASININ İLK HANESİNE GÖRE İNDEKSLİYOR
 * ERASE_LINKED_LIST YAPISI -> SILINEN KAYIDIN BASLANGIC ADRESI BIR SONRAKI KAYIT
 * */
struct erase_linked_list *Create_node(int data);
void erase_linked_list_EKLE(struct erase_linked_list **root,int delete_location);
int erase_linked_list_SEEK(struct erase_linked_list **root);
int erase_linked_list_bosmu(struct erase_linked_list *root);
void IF_NO_FILE();
void EKLE();
int index_dosyasinda_ARA(char aranacak_isbn[14]);
void INDEXLE(char isbn[14],int location);
void DELETE(char isbn[13],struct erase_linked_list ** root);
void pasif_hale_getir(char isbn[14]);
void kaydi_bastir(int location);
void ana_dosya_pasif_hale_getir(char isbn[14]);
/*-------------------------------------------------------------*/
// STRUCTLAR VE ERASE LINKED LIST FUNCLARI
typedef struct { //ANA KAYDI TUTACAK STRUCT anadosya.txt de bulunacak.
    char isbn[14];
    char albumismi[20];
    char sarkici_adi[20];
}kayit;

typedef struct { // INDEX KAYDINI TUTACAK STRUCT index_dosyasi.txt de bulunacak.
    char isbn[14];
    int location;
}index_kayit;

struct erase_linked_list { // silme linked listi sadece silinmis bir kayıt varsa o kaydın veya kayıtların baslangic
    int rewrite_location;  // konumlarını tutar.
    struct erase_linked_list *next;
};
/*-------------------------------------------------------------*/
// bir veri silindiğinde bu fonksiyon yardımcılığıyla node olusturulacak.
struct erase_linked_list *Create_node(int data){
    struct erase_linked_list *new ;
    new = (struct erase_linked_list *)malloc(sizeof(struct erase_linked_list));
    new->next = NULL;
    new->rewrite_location = data;
}
/*-------------------------------------------------------------*/
// BOŞSA SILINMIS ELEMAN YOKTUR
int erase_linked_list_bosmu(struct erase_linked_list *root){
    if(root==NULL) return 1;
    return 0;
}
/*-------------------------------------------------------------*/
// SILME ISLEMI YAPILDIGI VAKİT SİLİNEN KAYDIN ADRESI EKLENECEK
void erase_linked_list_EKLE(struct erase_linked_list **root,int delete_location) {
    struct erase_linked_list *new = Create_node(delete_location);
    if (*root == NULL) {
        new->next = *root;
        *root = new;
    } else {
        struct erase_linked_list *x = *root;
        while (x->next != NULL) {
            x = x->next;
        }
        x->next = new;
    }
    }
/*-------------------------------------------------------------*/
    int erase_linked_list_SEEK(struct erase_linked_list **root) { // silinmis eleman yoksa sentinel dondurecek
        if (*root == NULL) return SENTINEL;                 // silinmis eleman varsa ilk nodenin datasını döndürüp,
        struct erase_linked_list *silinecek;
        int return_data = (*root)->rewrite_location;
        silinecek = *root;
        *root = (*root)->next;
        free(silinecek);
        return return_data;
    }
    void linked_list_bastir(struct erase_linked_list *root) {
        if (root == NULL) {
            printf("LINKED LIST BOS! SILINMIS KAYIT YOK \n ");
        }
        while (root != NULL) {
            printf(" SILINMIS KONUM %d \n", root->rewrite_location);
            root = root->next;
        }
    }
/*-------------------------------------------------------------*/
// EGER DOSYALAR YOKSA DOSYALARI OLUSTURMAK ICIN KULLANILAN FONKSIYON
// INDEX DOSYASI '*' LAR ILE DOLU
    void IF_NO_FILE() {
        FILE *fp = fopen("anadosya.txt", "w");
        fclose(fp);
        FILE *fp2 = fopen("index_dosyasi.txt", "w");
        for (int i = 0; i < 100 * sizeof(index_kayit); i++) {
            fputs("*", fp);
        }
        fclose(fp2);
        fflush(stdout);
    }
/*-------------------------------------------------------------*/
// ISBN NUMARASINI ZATEN EKLI MI DIYE INDEX LISTESINDE KONTROL EDIYOR
// SILINMIS KAYIT VARSA UZERINE YAZMAK ICIN LINKED LISTE BAKIYOR
//YAZDIKTAN SONRA INDEXLIYOR..
//EKLENEN KAYDI BASTIRIYOR.
    void EKLE(struct erase_linked_list **root) { // LINKED LISTI PARAMETRE ALIYORUZ..
        kayit yeni_kayit;
        printf("Eklemek istediğiniz ISBN numarasını girin...");
        scanf("%s", yeni_kayit.isbn);
        fflush(stdin);

        if (index_dosyasinda_ARA(yeni_kayit.isbn) != SENTINEL) { // sentinel donmezse onceden ekli demektir.
            printf("Eklemek istediğiniz kayıt zaten ekli");
            return;
        }

        printf("Eklemek istediğiniz Sarkici Adini Girin..");
        scanf("%s", yeni_kayit.sarkici_adi);
        fflush(stdin);
        printf("Eklemek istediginiz Albumun ismini girin..");
        scanf("%s", yeni_kayit.albumismi);
        fflush(stdin);
        FILE *fp;
        fp = fopen("anadosya.txt", "r+");
        if (erase_linked_list_bosmu(*root)) {// SİLİNMİS KAYIT YOK
            printf("HIC SILINMIS KAYIT YOK .. \n ");
            fseek(fp, 0, SEEK_END);
            int yazilan_konum = ftell(fp); // yazilan konumu index dosyasinda saklayacağım icin alıyorum..
            printf("ANA DOSYAYA YAZILDI KONUM %d \n ",yazilan_konum);
            fwrite(&yeni_kayit, sizeof(kayit), 1, fp);
            fclose(fp);
            INDEXLE(yeni_kayit.isbn, yazilan_konum);
        } else {
            printf("SILINMIS KAYIT VAR .. UZERINE YAZILIYOR.");
            linked_list_bastir(*root);
            int rewrite_location = erase_linked_list_SEEK(root);  // erase linked listin ilk nodesindeki adresi aldı
            fseek(fp, rewrite_location, SEEK_SET);
            printf("ANA DOSYAYA YAZILDI KONUM %d \n ",rewrite_location);
            fwrite(&yeni_kayit, sizeof(kayit), 1, fp);
            fclose(fp);
            INDEXLE(yeni_kayit.isbn, rewrite_location);
        }
        printf("ASSAGIDA BELIRTILEN KAYIT SISTEME EKLENDI\n");
        kaydi_bastir(index_dosyasinda_ARA(yeni_kayit.isbn));


    }
/*-------------------------------------------------------------*/
//INDEX VE YARDIMCI FONKSIYONLARI
//tombstone finder -> isbn numarasının ilk hanesini int olarak döndürüyor.
    int tombstone_finder(char isbn[14]) {
        char ilk_hane = isbn[0];
        return atoi(&ilk_hane);
    }
/*-------------------------------------------------------------*/
// İNDEX DOSYASINDA KAYDI ARAYIP KAYDIN ANADOSYADAKI KONUMUNU DONDURUYOR
    int index_dosyasinda_ARA(char aranacak_isbn[14]) {
        int tombstone = tombstone_finder(aranacak_isbn); // bknz.tombstone_finder
        //TOMBSTONENIN SOYLEDIGI YERE ILK SEEK IMIZI YAPIYORUZ
        index_kayit bulunan_kayit;
        FILE *fp;
        fp = fopen("index_dosyasi.txt", "r+");
        char index_kontrol = '*';
        fseek(fp, tombstone * sizeof(index_kayit) * 10, SEEK_SET);
        while (fgetc(fp) != index_kontrol) {
            fseek(fp, -1, SEEK_CUR);
            fread(&bulunan_kayit, sizeof(index_kayit), 1, fp);
            if (strcmp(bulunan_kayit.isbn, aranacak_isbn) == 0) {
                return bulunan_kayit.location;
            }
        }
        return SENTINEL;
    }
/*-------------------------------------------------------------*/
    int WHERE_IS_RECORD_IN_INDEX(char *aranacak_isbn) {
        int tombstone = tombstone_finder(aranacak_isbn); // bknz.tombstone_finder
        //TOMBSTONENIN SOYLEDIGI YERE ILK SEEK IMIZI YAPIYORUZ
        //ISBN NUMARASI ALIP INDEX DOSYASINDAKI PARAMETRE OLARAK GELEN ISBN NUMARALI KAYDIN YERINI DONDURUR.
        index_kayit bulunan_kayit;
        FILE *fp;
        fp = fopen("index_dosyasi.txt", "r+");
        char index_kontrol = '*';
        fseek(fp, tombstone * sizeof(index_kayit) * 10, SEEK_SET);
        while (fgetc(fp) != index_kontrol) {
            fseek(fp, -1, SEEK_CUR);
            fread(&bulunan_kayit, sizeof(index_kayit), 1, fp);
            if (strcmp(bulunan_kayit.isbn, aranacak_isbn) == 0) {
                return ftell(fp);
            }
        }
        return SENTINEL;
    }
/*-------------------------------------------------------------*/
/*
 * Index dosyası yardımcılıgı ile kaydın baslangıc konumu almalı.
 * bulursa kaydı yazdırmalı
 *
 */
    void kaydi_bastir(int location) {
        kayit okunan_kayit;
        FILE *fp;
        fp = fopen("anadosya.txt", "r+");
        fseek(fp, location, SEEK_SET);
        fread(&okunan_kayit, sizeof(kayit), 1, fp);
        printf("Kaydin ISBN numarasi : %s \n", okunan_kayit.isbn);
        printf("Sanatcinin  adi : %s  \n", okunan_kayit.sarkici_adi);
        printf("Album adi : %s \n", okunan_kayit.albumismi);
        fclose(fp);
    }
/*-------------------------------------------------------------*/
    void butun_kayitlari_yazdir(void) {
        kayit okunan_kayit;
        FILE *fp;
        fp = fopen("anadosya.txt", "r+");
        int sira = 1;
        fseek(fp, 0, SEEK_SET);
        while (fread(&okunan_kayit, sizeof(kayit), 1, fp)) {
            printf("/*-------------------------------------------------------------*/\n");
            printf("Kayit numarasi %d \n", sira);
            printf("Kaydin ISBN numarasi : %s \n", okunan_kayit.isbn);
            printf("Sanatcinin  adi : %s  \n", okunan_kayit.sarkici_adi);
            printf("Album adi : %s \n", okunan_kayit.albumismi);
            sira += 1;
        }

    }

/*-------------------------------------------------------------*/
    void INDEXLE(char isbn[14], int location) {

        index_kayit yeni_kayit;
        strcpy(yeni_kayit.isbn, isbn);
        yeni_kayit.location = location;
        int tombstone = tombstone_finder(isbn);

        FILE *fp; //yazma yapmak için
        fp = fopen("index_dosyasi.txt", "r+");
        fseek(fp, sizeof(index_kayit) * tombstone * 10, SEEK_SET);
        char index_kontrol = '*';

        if (index_kontrol == fgetc(fp)) {
            fseek(fp, -1, SEEK_CUR);
            long int location = ftell(fp);
            printf("INDEXLENDI INDEX_DOSYASI KONU : %ld \n ",location);
            fwrite(&yeni_kayit, sizeof(index_kayit), 1, fp);
            fclose(fp);
        } else {
            fseek(fp, sizeof(index_kayit) * tombstone * 10, SEEK_SET);
            while (fgetc(fp) != index_kontrol);
            fseek(fp, -1, SEEK_CUR);
            long int location = ftell(fp);
            printf("INDEXLENDI INDEX_DOSYASI KONU : %ld \n",location);
            fwrite(&yeni_kayit, sizeof(index_kayit), 1, fp);
            fclose(fp);
        }
    }
/*-------------------------------------------------------------*/
/*
 ->SILINMEK ISTENEN KAYDIN ISBN NUMARASINI VE LINK LISTIN ADRESINI ALIR
 ->SILINEN KAYIT ARAMALARDA CIKMASIN DIYE KAYDI 00000 OLARAK DEĞİŞTİRİR.
 ->SILINEN KAYDI GOSTERIR
 -> KAYDI İNDEX DOSYASINDA ARAR, EĞER VARSA LINKED LISTE KAYDIN YERINI EKLER
 */
    void DELETE(char isbn[13], struct erase_linked_list **root) {
        int delete_location = index_dosyasinda_ARA(isbn);
        if (delete_location == SENTINEL) printf("aranılan isbn numaralı bir kayıt bulunamadı..");
        erase_linked_list_EKLE(root, delete_location);
        printf(" BIR DAHAKI YAZILACAK KONUM : %d \n", delete_location);
        printf("ERASE LINKED LISTTEKI KONUMLAR : \n");
        linked_list_bastir(*root);
        kaydi_bastir(index_dosyasinda_ARA(isbn));
        printf("Üstte bilgileri verilen Kayıt silindi..");
        pasif_hale_getir(isbn);
        ana_dosya_pasif_hale_getir(isbn);

    }
/*-------------------------------------------------------------*/
    void pasif_hale_getir(char isbn[14]) {
        // ISTENEN KAYDI ARAMALARDA BULUNMAMASI ICIN 00 SEKLINE GETIRIR.
        // KAYDIN ARAMALARDA HALA ANADOSYAYI ISARET EDEBILMESI ICIN LOCATION DEĞİŞKENİNE DOKUNMAZ.
        int passivation_location = WHERE_IS_RECORD_IN_INDEX(isbn);
        index_kayit passive;
        strcpy(passive.isbn, "0000000000000");
        FILE *fp;
        fp = fopen("index_dosyasi.txt", "r+");
        fseek(fp, passivation_location, SEEK_SET);
        fwrite(&passive, sizeof(index_kayit), 1, fp);
        fclose(fp);
    }
/*-------------------------------------------------------------*/
    void ana_dosya_pasif_hale_getir(char isbn[14]) {
        int passivation_location = index_dosyasinda_ARA(isbn);
        kayit passive;
        strcpy(passive.isbn, "0000000000000");
        strcpy(passive.sarkici_adi, "DELETED");
        strcpy(passive.albumismi, "DELETED");
        FILE *fp;
        fp = fopen("anadosya.txt", "r+");
        fseek(fp, passivation_location, SEEK_SET);
        fwrite(&passive, sizeof(kayit), 1, fp);
        fclose(fp);
    }
/*-------------------------------------------------------------*/
    void GUNCELLE(char isbn[14], struct erase_linked_list **root) {
        kayit guncellenecek_kayit;
        int location_index = WHERE_IS_RECORD_IN_INDEX(isbn);
        int location_anadosya = index_dosyasinda_ARA(isbn);
        FILE *fp;
        fp = fopen("anadosya.txt", "r+");

        if (location_anadosya == SENTINEL) {
            printf("guncellenmek istenen kayıt bulunamadı ..\n ");
            return;
        }
        printf("HANGISINI GUNCELLEMEK ISTIYORSUNUZ\n");
        printf("1-> SARKICI ADI GUNCELLE \n");
        printf("2-> ALBUM ADI GUNCELLE \n");
        printf("3-> KAYDI SİL VE YENIDEN EKLE\n");
        printf("4-> İPTAL\n");
        int choice;
        scanf("%d", &choice);
        yanlis_secim:
        switch (choice) {
            case 1: {
                fseek(fp, location_anadosya, SEEK_SET);
                fread(&guncellenecek_kayit, sizeof(kayit), 1, fp);
                printf("SARKICI ADINI GIRINIZ");
                scanf("%s", guncellenecek_kayit.sarkici_adi);
                fflush(stdin);
                fseek(fp, location_anadosya, SEEK_SET);
                fwrite(&guncellenecek_kayit, sizeof(kayit), 1, fp);
                break;
            }
            case 2: {
                fseek(fp, location_anadosya, SEEK_SET);
                fread(&guncellenecek_kayit, sizeof(kayit), 1, fp);
                fflush(stdin);
                printf("album ADINI GIRINIZ");
                scanf("%s", guncellenecek_kayit.albumismi);
                fseek(fp, location_anadosya, SEEK_SET);
                fwrite(&guncellenecek_kayit, sizeof(kayit), 1, fp);
                break;
            }
            case 3: {
                DELETE(isbn, root);
                EKLE(root);
                break;
            }
            case 4:
                break;
            default: {
                printf("Yanlis secim..\n");
                goto yanlis_secim;
            }
        }
        fclose(fp);
    }
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
    void indexleme_main() {
        tekrarbasla:
        if (access("anadosya.txt", F_OK) != -1) { // if file exist
            printf("Dosyalar Mevcut .. hangi işlemi yapmak istersiniz\n");
            struct erase_linked_list *root = NULL;
            menu_yenile:
            printf("LINKED LIST DURUMU : ");
            linked_list_bastir(root);
            int menu;

            printf("/**********************************/\n");
            printf("LUTFEN ISLEM NUMARASI SECINIZ\n");
            printf("1 ) KAYIT BUL (ISBN NUMARASI ILE)\n");
            printf("2 ) KAYIT SIL (ISBN NUMARASI ILE) \n");
            printf("3 ) KAYIT LISTELE (DETAYLI)\n");
            printf("4 ) KAYIT EKLE\n");
            printf("5 ) KAYIT GUNCELLE(ISBN ILE)\n");
            printf("9 ) CIKIS\n");
            while (1) {
                printf("/**********************************/\n");
                printf("/ISLEMI GIRINIZ /\n");
                scanf("%d", &menu);
                fflush(stdin);
                switch (menu) {
                    case 1: {
                        char isbn[14];
                        printf("BULMAK İSTEDİGİNİZ KAYDIN ISBN NUMARASINI GIRINIZ \n");
                        scanf("%s", isbn);
                        int konum = index_dosyasinda_ARA(isbn);
                        if (konum == SENTINEL) {
                            printf("BU KAYIT BULUNAMADI .. ");
                            break;
                        }
                        kaydi_bastir(konum);
                        break;
                    }
                    case 2: {
                        char isbn[14];
                        printf("SILMEK ISTEDIGINIZ KAYDI GİRİNİZ");
                        scanf("%s", isbn);
                        fflush(stdin);
                        DELETE(isbn, &root);
                        break;
                    }
                    case 3:
                        butun_kayitlari_yazdir();
                        break;
                    case 4:
                        EKLE(&root);
                        break;
                    case 5: {
                        char isbn[14];
                        printf("GUNCELLEMEK İSTEDİGİNİZ KAYDI GİRİNİZ");
                        scanf("%s", isbn);
                        fflush(stdin);
                        GUNCELLE(isbn, &root);
                        break;
                    }
                    case 6:
                        exit(1);
                    default:
                        printf("yanlis secim .. ");
                        break;
                }
                goto menu_yenile;
            }


        } else {
            // file doesn't exist
            // dosyalar olusturulmamıssa istenilen boyutta txt dosyalarını olusturup bastan basliyor
            printf("index ve ana dosya bulunamadı .. oluşturulup tekrar başlatılıyor..\n");
            IF_NO_FILE();
            goto tekrarbasla;
        }
    }