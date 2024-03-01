#include <stdio.h>
#include <stdbool.h>

bool printable_str(const char * buf){
    const char *ptr=buf;
    while(*ptr!='\0'){
        if(*ptr<32 || *ptr>126){
            return false;
        }
        ++ptr;
    }
    return true;
    
}

int main(){

    const char buff[19]="testowanko numer 1\0";
    const char buff2[] = "niedrukowalny\7 znak";
    if(printable_str(buff)){
        printf("Zawiera same drukowalne znaki");
    }else{
        printf("Zawiera jakieś niedrukowalne znaki");
    }
    printf("\n");
    if(printable_str(buff2)){
        printf("Zawiera same drukowalne znaki");
    }else{
        printf("Zawiera jakies niedrukowalne znaki");
    }

}