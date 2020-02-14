/**
 * KOD LINUX ICIN TASARLANMISTIR. 
 * WINDOWSTA DUZGUN CALISMAZ.
 * */


/**
*HAZIRLAYANLAR
*ATAKAN TURKAY 170401009
*EGEMEN INCELER 170401027
*EMIR ALI KIVRAK 170401028
**/

#include <stdio.h>
#include <stdlib.h>
#include "Indexleme.h"
#include "AVL.h"
int main()
{
    int secim_yap;
    printf("AVL Agaclari ile cozum icin--1\nIndexleme icin--2\n");
    scanf("%d",&secim_yap);
    switch(secim_yap){
        case 1:
            hazirlik();
            menu_fonk();
            break;
        case 2:
            indexleme_main();
            break;
        default:
            exit(1);
    }
    return 0;

}
