1. Spróbuj napisać program wyszukujący w bazie Discogs wszystkie albumy danego wykonawcy (np. Budki Suflera, czyli zespołu nr 359282). Sprawdź czy to, co drukuje program zgadza się z tym, co można ręcznie wyszukać na witrynie.

Może się okazać, że Discogs odrzuca zapytania, jeśli są one całkowicie anonimowe. Sprawdź wtedy w dokumentacji kim /czym trzeba być, aby uzyskać odpowiedź — zalogowanym użytkownikiem witryny, aplikacją napisaną przez zarejestrowanego dewelopera?

2. Napisz program, który w oparciu o informacje z Discogs sprawdza, czy dani muzycy grali razem w jakichś zespołach. Wyświetl nazwy tych zespołów oraz imiona i nazwiska występujących w nich muzyków. Załóż, że numeryczne identyfikatory muzyków podawane są w argv.

Dla przykładu: 516820 to Tomasz Zeliszewski, 532854 Marek Stefankiewicz, a 702387 Mieczysław Jurecki. Wszyscy trzej grali w Budce Suflera (id 359282), do tego Zeliszewski i Jurecki występowali w zespole Wieko (id 4751291), a Stefankiewicz i Jurecki — w Perfekcie (id 669348).

Zadbaj, aby drukowane wyniki były posortowane po nazwie zespołu. Nie zapomnij o weryfikowaniu poprawności odpowiedzi zwracanych przez serwer. Dobrze by było, gdyby w razie przekroczenia limitu zapytań na minutę program chwilę czekał i ponawiał zapytanie, ale nie jest to obowiązkowe (program może taką sytuację traktować jak każdy inny błąd i kończyć działanie).