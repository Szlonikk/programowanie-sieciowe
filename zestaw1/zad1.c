#include <stdio.h>

void drukuj(int *tab, int i){
    int j=0;
    for(;j<i;j++){
        if(tab[j]>10 && tab[j]<100){
            printf("%d, ",tab[j]);
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
    
    drukuj(tab,i);
    return 0;
}