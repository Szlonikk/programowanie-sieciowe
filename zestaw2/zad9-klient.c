#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int gniazdko = -1;

bool printable_buf(const void * buf, int len){
    const char * buffor=buf;

    for(int i=0;i<len;i++){
        if(buffor[i]<32 || buffor[i]>126){
            return false;
        }
    }

    return true;
}

void closeConnection () {
	if (gniazdko != -1)
		close (gniazdko);
}

int main (int argc, char const *argv[]) {
	
	const char* ipv4 = argv[1];
	int port = atoi (argv[2]);
	
	atexit (closeConnection);
	
	struct sockaddr_in adres;
    adres.sin_family = AF_INET;
    adres.sin_port = htons(port);
    adres.sin_addr.s_addr = inet_addr(ipv4);
	
	gniazdko = socket(AF_INET, SOCK_DGRAM, 0);
	if (gniazdko == -1) {
		perror ("Blad socket()");
		exit (EXIT_FAILURE);
	}
	
	if (sendto(gniazdko, "", 0, 0, (struct sockaddr *)&adres, sizeof(struct sockaddr_in)) == -1) {
        perror ("Blad sendto()");
        exit (EXIT_FAILURE);
    }
	
	char bufor[32];
	socklen_t adresRozmiar = sizeof(adres);
	
	if (recvfrom (gniazdko, bufor, 32, 0, (struct sockaddr *)&adres, &adresRozmiar) == -1) {
		perror ("Blad recvfrom()");
		exit (EXIT_FAILURE);
	}
	
	if (printable_buf (&bufor, 11)) {
		printf ("%s\n", bufor);
	} else {
		printf ("W buforze znalazly sie niedrukowalne znaki\n");
		exit (EXIT_FAILURE);
	}
	
	if (close(gniazdko) == -1) {
		perror ("Blad close()");
		exit (EXIT_FAILURE);
	}
}