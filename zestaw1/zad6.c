#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main (int argc, char* argv[]) {
	
	int odczyt;
	
	if ((odczyt = open (argv[1], O_RDONLY, 0644)) == -1) {
		printf ("Blad otwarcia pliku %s do odczytu", argv[1]);
		perror ("Blad pliku");
		exit (EXIT_FAILURE);
	}
	
	int zapis;
	
	if ((zapis = open (argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
		printf ("Blad otwarcia pliku %s do zapisu", argv[2]);
		perror ("Blad pliku");
		exit (EXIT_FAILURE);
	}
	
	size_t odczytano;
	size_t zapisano;
	char bufor[64];
	
	while (1) {
		
		odczytano = read(odczyt, &bufor, sizeof(bufor));
		if (odczytano == -1) {
			perror("Blad odczytu z pliku\n");
			exit(EXIT_FAILURE);
		} else if (odczytano == 0) {
			break;
		} else {
			zapisano = write (zapis, &bufor, odczytano);
			if (zapisano == -1) {
				perror("Blad zapisu do pliku\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	if (close (odczyt) == -1) {
		perror("Blad zamkniecia pliku");
		exit (EXIT_FAILURE);
	}
	
	if (close (zapis) == -1) {
		perror("Blad zamkniecia pliku");
		exit (EXIT_FAILURE);
	}
	
}