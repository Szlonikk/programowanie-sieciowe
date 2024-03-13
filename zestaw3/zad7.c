#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
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

int main(int argc, char* argv[])
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu
    int port;
    const char* ipv4;
    if(argc==3){
        ipv4 = argv[1];
	    port = atoi(argv[2]);
    }else{
        perror("pierwszy argumeny to IPv4, drugi to numer portu");
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    // Ustawienie limitu czasu na odczyt danych z gniazda
    struct timeval timeout;
    timeout.tv_sec = 10;  // 10 sekund
    timeout.tv_usec = 0;  // 0 milisekund

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = inet_addr(ipv4) },   // 127.0.0.1
        .sin_port = htons(port)
    };

    rc = connect(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("connect");
        return 1;
    }

    unsigned char buf[16];

    cnt = read(sock, buf, sizeof(buf)-1);
    if (cnt == -1) {
        perror("read");
        return 1;
    }
    buf[cnt] = '\0'; // Dodanie znaku końca linii dla bezpiecznego wypisywania
    printf("%s\n", buf);

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
