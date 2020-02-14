#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 1-)DOSYA OKUNUYOR VE OKUNURKEN AVL AGACINA 
 * ISBNLERI KEY ,KONUMLARI VALUE OLARAK KOYULUYOR.
 * 2-)SILINENLERIN KONUMLARI LINKED LISTE EKLENIYOR
 * 3-)BIR KAYIT EKLENECEGI ZAMAN ONCE LINKED LISTTEN BASLANGIC KONUMUNU ISTIYOR
 * YOKSA DOSYANIN SONUNA KAYDEDIYOR KENDINI.
 * 4-)KAYIT-SILME ISLEMLERI OLURKEN AVL AGACI,LINKEDLIST,DOSYA KENDINI
 * HEP GUNCEL TUTUYOR.
 * 5-)SILME ISLEMI STRUCT ICERISINDE BULUNAN DURUM DEGISKENININ "F" YAPILMASI ILE OLUYOR.
 * NOTLAR -> 
 * 1-)AVL,FILEPOINTER,LINKEDLIST DISARIDA GLOBAL OLARAK TANIMLANMISTIR.
 * O YUZDEN FONKSIYONLARIN COGUNLUGU VOID VE DEGISKEN ALMIYOR.
 * 2-)KOD 5 BOLUMDEN OLUSUYOR
 *  -STRUCT TANIMLARI
 *  -AGAC KODLARI
 *  -LISTE KODLARI
 *  -DOSYA KODLARI(A.K.A BIRLESTIRICI KODLAR)
 *  -MAIN
 * 3-)DOSYANIN ADI testdosya.dat
 * 
 * 
 * */




/**********STRUCT TANIMLARI BASLANGIC***************/
struct node{
    int konum;
    int key;
    struct node *left;
    struct node *right;
    int height; // derinlik
};
struct kayit{

    long int isbn;
    char durum[2]; // null var sonunda
    char sarkici_ismi[30];
    char isim[30];
    char tur[15];
    char besteci[30];

};
struct silinecek_kayit{
        int konum;
        struct silinecek_kayit *sonraki;
};
/***********STRUCT TANIMLARI BITIS****************/


/***********FONKSIYONLAR************************/
void dosya_ac();
int max(int a, int b);
struct node *newNode(int key,int konum);
int height(struct node *node);
struct node *rightRotate(struct node *y);
struct node *leftRotate(struct node *x);
int getBalance(struct node *node);
struct node *insert(struct node *node, int key,int konum);
void preOrder_yardimci2(struct node *node);
void preOrder2(struct node *node);
void preOrder_yardimci(struct node *node);
void preOrder(struct node *node);
struct node *minValueNode(struct node *root);
struct node *deleteNode(struct node *root, int key);
void findNode(struct node *root, long int key,long int *konum);
struct silinecek_kayit* hucre_olustur(int icerik);
void liste_sirala(struct silinecek_kayit **liste_basi);
void liste_sonuna_ekle(int icerik, struct silinecek_kayit **liste_basi);
void liste_yaz(struct silinecek_kayit *liste_basi);
void liste_eleman_sil(int silinen, struct silinecek_kayit **liste_basi);
void dosya_ac();
int index_olusturucu();
void kayit_bulucu();
void kaydi_sil(long int isbn);
void yazdir(long int konum);
void kayit_ekle();
void kayit_ekle_yardimci(long int konum,struct kayit kayit_temp);
void duzenle();
void duzenle_yardimci(long int konum);
void random_kayit_ekle();
void hazirlik();
void menu_fonk();
/***********FONKSIYONLAR_END************************/


/******GLOBAL DEGISKEN TANIMLARI BASLANGIC*******/
struct silinecek_kayit *silinecekler_listesi = NULL;
struct node *root=NULL;
FILE * file_pointer;
/*******GLOBAL DEGISKEN TANIMLARI BITIS*********/




/***********AVL KODLARI  BASLANGIC**********/
int max(int a, int b){
    return a>b ? a:b;
}
struct node *newNode(int key,int konum){
    struct node *node=(struct node*)malloc(sizeof(struct node));
    node->key=key;
    node->konum=konum;
    node->left=node->right=NULL;
    node->height=1;
    return node;
}
int height(struct node *node){
    if(node==NULL) return 0;
    return node->height;
}

struct node *rightRotate(struct node *y){
    struct node *x=y->left, *T=x->right;
    x->right=y;
    y->left = T;
    
    y->height=max(height(y->left),height(y->right))+1;
    x->height=max(height(x->left),height(x->right))+1; 
    
    return x;
} // sol sol
struct node *leftRotate(struct node *x){
    struct node *y=x->right, *T=y->left; 
    y->left=x;
    x->right = T;
    
    x->height=max(height(x->left),height(x->right))+1;
    y->height=max(height(y->left),height(y->right))+1;
    
    return y;
} // sag sag
int getBalance(struct node *node){
    if(node==NULL) return 0;
    return height(node->left) - height(node->right);
}

struct node *insert(struct node *node, int key,int konum){
    int balance;
    if(node==NULL) return newNode(key,konum);
    if(key<node->key) node->left = insert(node->left,key,konum);
    else node->right = insert(node->right,key,konum);
    
    node->height = max(height(node->left),height(node->right)) +1;
    
    balance=getBalance(node);
    
    if(balance>1 && key < node->left->key) // sol sol
        return rightRotate(node);
    if(balance<-1 && key > node->right->key) // sag sag
        return leftRotate(node);
    if(balance>1 && key>node->left->key){  // sol sag
        node->left=leftRotate(node->left);
        return rightRotate(node);
    }
    if(balance<-1 && key<node->right->key){  // sag sdol
        node->right=rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

void preOrder_yardimci2(struct node *node) {
    if(node!=NULL){
        preOrder_yardimci2(node->left);
        yazdir(node->konum);
        preOrder_yardimci2(node->right);
    }
    
}

void preOrder2(struct node *node) {
    preOrder_yardimci2(node);
    printf("\n");
}
void preOrder_yardimci(struct node *node) {
    if(node!=NULL){
        preOrder_yardimci(node->left);
        printf("ISBN NUMARASI = %d  KAYIT KONUMU = %d \n",node->key,node->konum);
        preOrder_yardimci(node->right);
    }
    
}
void preOrder(struct node *node) {
    preOrder_yardimci(node);
    printf("\n");
}
struct node *minValueNode(struct node *root){
    struct node *current = root;
    if(current == NULL) return NULL;
    while(current->left) current=current->left;
    return current;
}
struct node *deleteNode(struct node *root, int key){
    if(root==NULL) return root;
    if(key <root->key) root->left=deleteNode(root->left,key);
    else if(key>root->key) root->right=deleteNode(root->right,key);
    else {
        if(root->left == NULL || root->right == NULL){
            struct node *temp=root->left ?root->left:root->right;
            if(temp==NULL){
                temp=root;
                root=NULL;
            }
            else *root=*temp; // root->key=temp->key; root->rigt = temp->rigt; root->left= 
            free(temp);
        }
        else {
            struct node *temp=minValueNode(root->right);
            root->key=temp->key;
            root->right=deleteNode(root->right,temp->key);
        }
    }
    
    if(root==NULL) return root;
    
    root->height=max(height(root->left),height(root->right))+1;
    int balance=getBalance(root);
    
    if(balance>1 && getBalance(root->left) >=0)
        return rightRotate(root);
    if(balance>1 && getBalance(root->left) < 0){
        root->left=leftRotate(root->left);
        return rightRotate(root);
    }
    if(balance <-1 && getBalance(root->right) <=0)
        return leftRotate(root);
    if(balance < -1 && getBalance(root->right)>0){
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
        
    return root;    
}
/**********************************************************/
void findNode(struct node *root, long int key,long int *konum){
    if(root==NULL) return;
    if(key <root->key) findNode(root->left,key,konum);
    else if(key>root->key) findNode(root->right,key,konum);
    else {
        //printf("%d BULUNDU \n",root->key);
        *konum=root->konum;

    }
    return;

}

/**********************AVL KODLARI  BITIS*****************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/


/**********SILINECEK KAYITLAR BAŞLANGIÇ.**********************/
struct silinecek_kayit* hucre_olustur(int icerik) {
        struct silinecek_kayit *a;
        a = (struct silinecek_kayit*)malloc(sizeof(struct silinecek_kayit));
        if (a==NULL) {
                printf("Heap alaninda yer ayrilamadi.\n");
                exit(1);
        }
        a->konum = icerik;
        a->sonraki = NULL;
        return a;
}
void liste_sirala(struct silinecek_kayit **liste_basi) {
        struct silinecek_kayit *a, *b, *c, *d;
        if (*liste_basi == NULL || (*liste_basi)->sonraki == NULL) {
                return;
        } //Listede bir elemandan fazla olmalı.
        d=(*liste_basi)->sonraki;
        (*liste_basi)->sonraki = NULL;
        while(d != NULL) {
                c = d;
                d = d->sonraki;
                b = *liste_basi;
                while(b != NULL && b->konum < c->konum) {
                        a = b;
                        b = b->sonraki;
                }
                if(b == *liste_basi) {
                        c->sonraki = *liste_basi;
                        *liste_basi = c;
                }
                else {
                        a->sonraki = c;
                        c->sonraki = b;
                }
        }
}
void liste_sonuna_ekle(int icerik, struct silinecek_kayit **liste_basi) {
        struct silinecek_kayit* a = hucre_olustur(icerik);
        if(*liste_basi == NULL) {
                a->sonraki = *liste_basi;
                *liste_basi = a;
        }
        else {
                struct silinecek_kayit* x = *liste_basi;
                while(x->sonraki != NULL) {
                        x = x->sonraki;
                }
                x->sonraki = a;
        }
}

void liste_yaz(struct silinecek_kayit *liste_basi) {
        while(liste_basi != NULL) {
                printf("%d  ", liste_basi->konum);
                liste_basi = liste_basi->sonraki;
        }
        printf("\n");
}
void liste_eleman_sil(int silinen, struct silinecek_kayit **liste_basi) {
        //değişiklik olasılığı olduğundan liste başının adresi gönderilir.**
        struct silinecek_kayit *temp = *liste_basi;
        struct silinecek_kayit *once;
        while((temp != NULL) && (temp->konum != silinen)) {
                once = temp;
                temp = temp->sonraki;
        }
        if (temp == NULL) {
                return;
        }
        else if (temp == *liste_basi) {
                *liste_basi =(*liste_basi)->sonraki;
        }
        else {
                once->sonraki = temp->sonraki;
        }
        free(temp);
}

/****************SILINECEK KAYITLAR BITIS.***********************/
/****************************************************************/
/****************************************************************/
/****************************************************************/
/****************************************************************/


void dosya_ac(){
    /**
     * 1-)DOSYA YOKSA  W+ MODUNDA AÇILIYOR.
     * 2-)DOSYA VARSA  R+ İLE AÇILIR.
     **/
    file_pointer = fopen("testdosya.dat","r+");
    if(file_pointer==NULL){
        printf("INFO***|Dosya bulunamadi o yuzden yeni dosya yaratiliyor.\n");
        file_pointer=fopen("testdosya.dat","w+");
        return;
    }
    
    return;
    }
/****************************************************************/
/**
 *  DOSYA SONUNA KADAR GIDER
 *  EGER DOSYA DURUMU T ISE AGACA EKLER
 *  EGER DOSYA DURUMU T DEGILSE LINKED LISTE EKLER
 * */
int index_olusturucu(){
    int kayit_sayisi=0;
    rewind(file_pointer);
    while(!feof(file_pointer)){
        
        struct kayit temp;
        if(fread(&temp,sizeof(temp),1,file_pointer)){
            if(temp.durum[0]=='T'){    
                root=insert(root,temp.isbn,ftell(file_pointer)-sizeof(temp));
                kayit_sayisi++;
            }
            else if(temp.durum[0]=='F'){
                liste_sonuna_ekle(ftell(file_pointer)-sizeof(temp),&silinecekler_listesi);
            }                
        }
    }
    printf("TOPLAM KAYIT SAYISI=%d\n",kayit_sayisi);
}

/****************************************************************/
void kayit_bulucu(){
    printf("LUTFEN BULMAK ISTEDIGINIZ SARKININ ISBN NUMARASINI GIRIN\n");
    long int isbn;
    scanf("%ld",&isbn);
    long int konum = -1;
    findNode(root,isbn,&konum);
    printf("KONUM %ld \n",konum);
    if(konum ==-1){
        printf("BOYLE BIR ISBN NUMARASI BULUNAMADI\n");
        return;
    }
    else{
        fseek(file_pointer,0,SEEK_SET);
        yazdir(konum);
    }
    //printf("YAPMAK ISTEDIĞINIZ ISLEMI SECIN LUTFEN\n");
}
/****************************************************************/
void kaydi_sil(long int isbn){
    long int konum = -1;
    findNode(root,isbn,&konum);
    if(konum == -1 ){
        printf("SILME ISLEMI TAMAMLANAMADI -> ARADIGINIZ KAYIT BULUNAMADI \n");
        return;
    }
    struct kayit temp;
    fseek(file_pointer,konum,SEEK_SET);
    fread(&temp,sizeof(temp),1,file_pointer);
    root=deleteNode(root,temp.isbn);
    strcpy(temp.durum,"F");
    strcpy(temp.isim,"TESTBETA");
    temp.isbn=9999;
    fseek(file_pointer,konum,SEEK_SET);
    if(fwrite(&temp,sizeof(temp),1,file_pointer)){
        printf("SILME ISLEMI BASARILI\n");
    }
    fflush(file_pointer);
    liste_sonuna_ekle(konum,&silinecekler_listesi);
}
/****************************************************************/
void yazdir(long int konum){
    struct kayit temp23;
    rewind(file_pointer);
    fseek(file_pointer,konum,SEEK_SET);
    if(fread(&temp23,sizeof(temp23),1,file_pointer)){
        printf("/ISBN NUMARASI = %ld/\n",temp23.isbn);
        printf("/SARKICI ISMI %s/\n",temp23.sarkici_ismi);
        printf("/SARKI ISMI %s/\n",temp23.isim);
        printf("/SARKI TURU %s/\n",temp23.tur);
        printf("/BESTECI ISMI %s/\n",temp23.besteci);
        printf("/***********************/\n");
    }
    return;
}
/****************************************************************/
void kayit_ekle(){
    long int isbn_temp;
    printf("/ISBN NUMARASINI GIRIN/\n");
    scanf("%ld",&isbn_temp);
    long int konum = -1;
    findNode(root,isbn_temp,&konum);
    if(konum != -1){
        printf("BU ISBN NUMARASINA AIT KAYIT BULUNMAKTADIR.ISLEM IPTAL EDILIYOR\n");
        return;
    }
    else
    {
        char sarkici_ismi[30];
        char isim[30];
        char tur[15];
        char besteci[30];
        printf("/SARKICI ISMINI GIRIN/\n");
        scanf("%s",sarkici_ismi);
        printf("/SARKI ISMINI GIRIN/\n");
        scanf("%s",isim);
        printf("/TURUNU GIRIN GIRIN/\n");
        scanf("%s",tur);
        printf("/BESTECIYI GIRIN/\n");
        scanf("%s",besteci);
        struct kayit kayit_temp;
        strcpy(kayit_temp.besteci,besteci);
        strcpy(kayit_temp.sarkici_ismi,sarkici_ismi);
        kayit_temp.isbn=isbn_temp;
        strcpy(kayit_temp.durum,"T");
        strcpy(kayit_temp.tur,tur);
        strcpy(kayit_temp.isim,isim);
        if(silinecekler_listesi==NULL){
            fseek(file_pointer,0,SEEK_END);
            kayit_ekle_yardimci(0,kayit_temp);
            root=insert(root,kayit_temp.isbn,ftell(file_pointer)-sizeof(kayit_temp));
            fflush(file_pointer);
            return;
        }
        else{
            liste_sirala(&silinecekler_listesi);
            kayit_ekle_yardimci(silinecekler_listesi->konum,kayit_temp);
            root=insert(root,kayit_temp.isbn,silinecekler_listesi->konum);
            liste_eleman_sil(silinecekler_listesi->konum,&silinecekler_listesi);
            fflush(file_pointer);
        }
    }
}
/****************************************************************/
void kayit_ekle_yardimci(long int konum,struct kayit kayit_temp){
    fseek(file_pointer,konum,SEEK_CUR);
    if(fwrite(&kayit_temp,sizeof(kayit_temp),1,file_pointer)){
        printf("KAYIT BASARI ILE TAMAMLANDI\n");    
    }
}
/****************************************************************/
void duzenle(){
    printf("DUZENLEMEK ISTEDIGINIZ KAYDIN ISBN NUMARASINI YAZINIZ\n");
    long int duzenlecenek_kayit;
    scanf("%ld",&duzenlecenek_kayit);
    long int kayit_konum=-1;
    findNode(root,duzenlecenek_kayit,&kayit_konum);
    if(kayit_konum==-1){
        printf("ARADIGINIZ KAYIT BULUNAMADI\n");
        return;
    }
    yazdir(kayit_konum);
    duzenle_yardimci(kayit_konum);
}
/****************************************************************/
void duzenle_yardimci(long int konum){
    fseek(file_pointer,konum,SEEK_SET);
    struct kayit kayit_temp;
    if(fread(&kayit_temp,sizeof(kayit_temp),1,file_pointer)){
        printf("SARKICI ISMI NEDIR ?\n");
        scanf("%s",kayit_temp.sarkici_ismi);
        printf("SARKI ISMI NEDIR ?\n");
        scanf("%s",kayit_temp.isim);
        printf("SARKI TURU NEDIR ?\n");
        scanf("%s",kayit_temp.tur);
        printf("BESTECI ISMI NEDIR ?\n");
        scanf("%s",kayit_temp.besteci);
        fseek(file_pointer,konum,SEEK_SET);
        if(fwrite(&kayit_temp,sizeof(kayit_temp),1,file_pointer)){
            printf("GUNCELLEME BASARILI\n");
            return;
        }
         printf("GUNCELLEME BASARISIZ\n");
            return;

        
    }
}
/****************************************************************/
void random_kayit_ekle(){
    int i = 0;
    fseek(file_pointer,0,SEEK_END);
    while(i<1000){
    struct kayit atakan;
    atakan.isbn=i;
    strcpy(atakan.isim,"CANAKKALEICINDE");
    strcpy(atakan.durum,"T");
    strcpy(atakan.tur,"RNB");
    strcpy(atakan.besteci,"ATAKANHR");
    strcpy(atakan.sarkici_ismi,"EMIRREYIZ");
    fwrite(&atakan,sizeof(atakan),1,file_pointer);
    printf("EKLENDI %d\n",i);
    i++;

    } //RANDOM ALBUM EKLEME STRES TESTI

    index_olusturucu();


}
/****************************************************************/
void hazirlik()
{
    
    printf("INFO***|DOSYA ACILIYOR LUTFEN BEKLEYINIZ\n");
    dosya_ac();
    printf("INFO***|2/1 |DOSYA ACMA BASARILI\n");
    printf("INFO***|INDEX OLUSTURULUYOR LUTFEN BEKLEYINIZ\n");
    index_olusturucu();
    printf("INFO***|2/2 |INDEX OLUSTURMA BASARILI\n");
    fflush(file_pointer);
}

void  menu_fonk(){
        
        while (1)
        {
            int menu=0;
        
        printf("/**********************************/\n");
        printf("LUTFEN ISLEM NUMARASI SECINIZ\n");
        printf("1 ) KAYIT BUL (ISBN NUMARASI ILE)\n");
        printf("2 ) KAYIT SIL (ISBN NUMARASI ILE) \n");
        printf("3 ) KAYIT LISTELE (OZET)\n");
        printf("4 ) KAYIT LISTELE (DETAYLI)\n");
        printf("5 ) KAYIT EKLE\n");
        printf("6 ) KAYIT GUNCELLE(ISBN ILE)\n");
        printf("7 ) SILINMIS KAYITLARI LISTELE\n");
        printf("8 ) RANDOM 1000 KAYIT EKLE(DOSYA BOS ISE YAPILACAK)\n");//ISBN NUMARASI CAKISMAMASI ICIN
        printf("10 ) CIKIS\n");
        printf("/**********************************/\n");
         printf("/ISLEMI GIRINIZ /\n");
        scanf("%d",&menu);
 
        switch (menu)
        {
        case 1:
            kayit_bulucu();
            break;
        case 2:
            printf("LUTFEN ISBN NUMARASINI YAZINIZ\n");
            long int isbn;
            scanf("%ld",&isbn);
            kaydi_sil(isbn);
            break;    
        case 3:
            printf("NORMAL KAYITLAR:\n");
            preOrder(root);
            printf("SILINMIS KAYITLAR:\n");
            liste_yaz(silinecekler_listesi);
            break;    
        case 4:
            printf(" \n\n\n/*******************************/\n");
            printf("/KAYIT_DETAYLI_START/\n");
            printf("/*******************************/\n");
            preOrder2(root);
            printf("/KAYIT_DETAYLI_END\n");
            break;    
        case 5:
            kayit_ekle();
            break;
        case 6:
            duzenle();
            break;    
        case 7:
            liste_yaz(silinecekler_listesi);
            break;
        case 8:
            random_kayit_ekle();
            break;
        case 10:
            fclose(file_pointer);
            return;        
        default:
            printf("LUTFEN DOGRU BIR ISLEM KODU YAZINIZ\n");
            break;
        }
    }
        
    

}
