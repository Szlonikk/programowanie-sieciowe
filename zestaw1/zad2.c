#include <stdio.h>

void drukuj_alt(int * tab, int i){
    int *ptr=tab;
    for(;ptr<tab+i;ptr++){
        if(*ptr>10 && *ptr<100){
            printf("%d, ",*ptr);
        }
    }
}

int main(){
    int tab[50];
    int i=0;
    int odczyt=-1;
    for(;i<50 && odczyt!=0;i++){
        scanf("%d",&odczyt);
        tab[i]=odczyt;
    }
    
    drukuj_alt(tab,i);
}