1. Dokończ pisanie par klient-serwer dla TCP/IPv4 oraz UDP/IPv4 (co razem daje cztery programy). Przetestuj czy działają poprawnie gdy klient i serwer są uruchomione na dwóch różnych komputerach w SPK. Wymaga to znajomości adresu IP przydzielonego komputerowi, na którym uruchamiany jest serwer — można go znaleźć w wynikach polecenia ip address show.

2. Sprawdź co się dzieje, gdy podasz zły adres IP albo zły numer portu serwera. Czy jądro systemu operacyjnego daje nam w jakiś sposób o tym znać? Jeśli tak, to jak długo trzeba czekać, aż jądro poinformuje nasz proces o wystąpieniu błędu?

Pamiętaj, że protokoły sieciowe z korekcją błędów wykonują wielokrotne retransmisje pakietów w zwiększających się odstępach czasu. Może to zająć nawet kilkadziesiąt minut. Nie pomyl sytuacji „proces zawiesza się na pięć minut zanim jądro zwróci -1” z sytuacją „zawiesza się na stałe”.

Jeśli któryś z klientów może się zawiesić czekając w nieskończoność na odpowiedź z nieistniejącego serwera, to popraw jego kod aby tego nie robił. W slajdach z wykładu są pokazane funkcje, które pozwalają na wykonywanie operacji we-wy z timeoutem (możecie go ustawić np. na 10 sekund).

3. Przeanalizuj niniejszą specyfikację protokołu sprawdzania, czy wyrazy są palindromami. Czy jest ona jednoznaczna, czy też może zostawia pewne rzeczy niedopowiedziane?

Komunikacja pomiędzy klientem a serwerem odbywa się przy pomocy datagramów. Klient wysyła datagram zawierający wyrazy do sprawdzenia. Serwer odpowiada datagramem zawierającym albo ułamek z liczbą otrzymanych wyrazów w mianowniku i liczbą wyrazów-palindromów w liczniku, albo komunikat o błędzie.

Zawartość datagramów interpretujemy jako tekst w ASCII. Datagramy wysyłane przez klienta mogą zawierać litery i spacje. Datagramy wysyłane przez serwer mogą zawierać albo cyfry i znak /, albo pięć liter składających się na słowo „ERROR”. Żadne inne znaki nie są dozwolone (ale patrz następny akapit).

Aby ułatwić ręczne testowanie serwera przy pomocy ncat, serwer może również akceptować datagramy mające na końcu dodatkowy znak \n (czyli bajt o wartości 10) albo dwa znaki \r\n (bajty 13, 10). Serwer może wtedy, ale nie musi, dodać \r\n do zwracanej odpowiedzi.

4. Napisz serwer UDP/IPv4 nasłuchujący na porcie nr 2020 i implementujący powyższy protokół.

Serwer musi weryfikować odebrane dane i zwracać komunikat o błędzie jeśli są one nieprawidłowe w sensie zgodności ze specyfikacją protokołu.

To zadanie jest ważne — zaimplementowane rozwiązanie trzeba oddać najpóźniej we wtorek 19 / środę 20 marca, nawet jeśli nie jest jeszcze w pełni gotowe.

