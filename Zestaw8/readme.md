1. Weź przykładowy kod rest_webapp.py.

2. Rozszerz funkcjonalność kodu tak, aby można było wyszukiwać osoby o zadanym imieniu i / lub nazwisku, tzn. zaimplementuj obsługę URL-i postaci /osoby/search?imie=Jan, /osoby/search?imie=Adam&nazwisko=Nowak itp.

3. Dodaj do bazy tabelę psy. Każdy pies ma imię, rasę (kundel to też rasa) oraz może, ale nie musi mieć właściciela (czyli kogoś z tabeli osoby). Zaprojektuj i zaimplementuj dodatkowe REST-owe endpointy pozwalające wykonywać operacje na tej tabeli.

4. Nie zapomnij o rozważeniu spójności danych. Co się dzieje, gdy klient wyśle żądanie usunięcia z bazy Adama Nowaka, który jest właścicielem psa?

