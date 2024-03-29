#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

#define DATAGRAM_T 65536
#define PORT 2020
int sock=-1;

void closeCon (void) {
    if(sock!=-1){
        if (close(sock) == -1){
                perror("close error");
                exit(1);
        }
    }    
}

bool isPalindrome(char* word) {
    int length = strlen(word);
    for (int i = 0; i < length / 2; i++) {
        if (tolower(word[i]) != tolower(word[length - i - 1])) {
            return false;
        }
    }
    return true;
}

bool buforCorrect(char* buf, int leng){
    if(buf[0]==' ' || buf[leng-1]==' '){
        return false;
    }
    for(int i=0;i<leng;i++){
        if(!((buf[i]>64 && buf[i]<91) || (buf[i]>96 && buf[i]<123) || buf[i]==' ')){
            return false;
        }

        if(i<leng-2)if(buf[i]==' ' && buf[i+1]==' '){
            return false;
        }
    }
    return true;
}

/// trzeba najpierw sprawdzic czy ciag bajtow ma dobre wartosci a potem dodac \0 na koncu bo przeciez dane z internetu nie koniecznie posiadaja ten terminator
//gcc -std=c99 -pedantic -Wall udp_server.c -lm -lpthread -o /tmp/zadanie/program

int main(int argc, char *argv[]){
    atexit (closeCon);
	
	struct sockaddr_in adres;
	adres.sin_family = AF_INET;
	adres.sin_port = htons(PORT);
	adres.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror ("ERROR socket()");
		exit (EXIT_FAILURE);
	}
	
	if (bind (sock, (struct sockaddr*)&adres, sizeof(struct sockaddr_in)) == -1) {
		perror ("ERROR bind()");
		exit (EXIT_FAILURE);
	}
    while(true){
        struct sockaddr_in client;
		socklen_t clientSize = sizeof(client);

		char bufor[DATAGRAM_T+1] = {0};
        int received=-1;
		
		if ((received = recvfrom(sock, bufor, DATAGRAM_T, 0, (struct sockaddr *)&client, &clientSize)) == -1) {
			perror("ERROR recvfrom()");
			continue;
		} else if (received > 1024) {
			const char* errorMsg = "ERROR";
			sendto(sock, errorMsg, strlen(errorMsg), 0, (struct sockaddr *)&client, clientSize);
			continue;
		}
        if(!buforCorrect(bufor,received)){
            const char* errorMsg = "ERROR";
			sendto(sock, errorMsg, strlen(errorMsg), 0, (struct sockaddr *)&client, clientSize);
			continue;
        }
        bufor[received]='\0';

        char *response;
        int words = 0, palindromes = 0;
        

        char *token = strtok(bufor, " ");
        while (token) {
            if (isPalindrome(token)) {
                palindromes++;    
            }
            words++;
            token = strtok(NULL, " ");
        }
        if (words == 0 || received==0) {
            response = "0/0";
        } else {
            char *res = malloc(32);
            sprintf(res, "%d/%d", palindromes, words);
            response = res;
        }
        

        if (sendto(sock, response, strlen(response), 0, (struct sockaddr *)&client, clientSize) == -1) {
            perror("ERROR sendto()");
            exit(EXIT_FAILURE);
        }

        if (words != 0) free(response);
    }
}
