#include <stdio.h>
#include <stdbool.h>
bool printable_buf(const void * buf, int len){
    bool printable=true;
    const char * buffor=buf;

    for(int i=0;i<len;i++){
        if(buffor[i]<32 || buffor[i]>126){
            return false;
        }
    }

    return true;
}

int main(){

    const char buff[19]="testowanko numer 1\0";

    if(printable_buf(buff,18)){
        printf("Zawiera same drukowalne znaki");
    }else{
        printf("Zawiera jakieś niedrukowalne znaki");
    }

}