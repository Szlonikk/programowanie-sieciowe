#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define DATAGRAM_T 65507
#define PORT 2020
int socket=-1;

void closeCon (void) {
    if(socket!=-1){
        if (close(socket) == -1){
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


int main(int argc, char *argv[]){
    atexit (closeCon);
	
	struct sockaddr_in adres;
	adres.sin_family = AF_INET;
	adres.sin_port = htons(PORT);
	adres.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ((socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror ("ERROR socket()");
		exit (EXIT_FAILURE);
	}
	
	if (bind (socket, (struct sockaddr*)&adres, sizeof(struct sockaddr_in)) == -1) {
		perror ("ERROR bind()");
		exit (EXIT_FAILURE);
	}
    while(true){
        struct sockaddr_in client;
		socklen_t clientSize = sizeof(client);

		char bufor[DATAGRAM_T + 1] = {0};
        int received=-1;
		
		if (received = recvfrom(socket, bufor, DATAGRAM_T, 0, (struct sockaddr *)&client, &clientSize) == -1) {
			perror("ERROR recvfrom()");
			continue;
		} else if (received > 1024) {
			const char* errorMsg = "ERROR";
			sendto(socket, errorMsg, strlen(errorMsg), 0, (struct sockaddr *)&client, clientSize);
			continue;
		}

        char *response;
        int words = 0, palindromes = 0;
        bool error = false;

    }
}