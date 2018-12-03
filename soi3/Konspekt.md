# Systemy operacyjne - SOI 
## Laboratorium 3 

### Konspekt rozwiązania
Prowadzący: mgr. inż. Aleksander Pruszkowski

Autor: Jakub Sikora

Termin: wtorek 14:00 - 16:00

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
### 1. Treść zadania 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Zgodnie z wytycznymi podanymi na stronie "Ćwiczenie 3 Synchronizacja procesów z wykorzystaniem semaforów" [http://www.ia.pw.edu.pl/~tkruk/edu/soi.b/lab/t3.txt](http://www.ia.pw.edu.pl/~tkruk/edu/soi.b/lab/t3.txt), napisz usługę „chat” dającą użytkownikom możliwość komunikacji między sobą (w obrębie jednej maszyny). 

Załóż, że niektórzy wśród użytkowników mają podwyższone prawa i ich wiadomości „wskakują” na początek kolejki rozsyłania wiadomości do pozostałych użytkowników. 

Załóż, że wszyscy użytkownicy komunikują się poprzez system w wyłącznie jednym temacie. 

Przemyśl metodę testowania powstałego systemu, w szczególności zwróć uwagę na pokazanie w działaniu równoczesnego „chatowania” obu grup użytkowników – w tym udowodnienia, że system faktycznie obsługuje wiadomości uwzględniając priorytety. 

Załóż, że wytworzone przez Ciebie programy testowe będą działały automatycznie generując testowe fikcyjne wiadomości a jeden z nich będzie te wiadomości pokazywał na konsoli. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

### 2. Planowane rozwiązanie

​	Chat systemowy zostanie zrealizowany za pomocą bufora blokowanego semaforem dostępnym z poziomu systemu operacyjnego Linux za pomocą funkcji `semget()`. Każdy z użytkowników będzie mógł wejść do sesji krytycznej, poprzez podniesienia semafora i zablokowanie dostępu pozostałym użytkownikom. W przypadku podwyższonych uprawnień (na przykład dla procesów o numerze `pid` podzielnym przez 2/3/4/5, konto root'a - do decyzji implementacyjnej) komunikaty będą wrzucane na początek bufora a w każdym innym przypadku na jego koniec. 	

​	Aby umożliwić przechowywanie więcej niż jednego komunikatu, bufor zostanie zrealizowany jako lista jednokierunkowa. Umożliwi to proste wstawianie komunikatów na koniec i początek listy oraz pobranie pierwszego elementu.

​	Jeden proces będący serwerem zostanie oddelegowany do wysyłania komunikatów do aktywnych użytkowników.  Każdemu procesowi zostanie przydzielony swój semafor do którego "serwer" będzie wrzucał nowe komunikaty do wyświetlenia. W trakcie wysyłania komunikatów do klientów, możliwość dodawania nowych komunikatów powinna zostać zablokowana.



### 3. Testowanie rozwiązania

​	Każdemu użytkownikowi zostaną udostępnione dwa programy: jeden do nadawania `chatsend` oraz konsola do odbierania komunikatów `chatreceive`.

​	Każdy nowy użytkownik, będzie wysyłał nową wiadomość co określony kwant czasu za pomocą procesu uruchomionego w tle. Każdy z użytkowników będzie wysyłał wiadomości z różnymi częstotliowściami. 

​	W ramach testów przygotuje scenariusze testowe wraz z oczekiwaną kolejnością wypisywania komunikatów.

​	Ponadto, scenariusze powinny być przygotowane również dla wersji w której nadaje proces uprzywilejowany.

​	W celu ułatwienia testowania, komunikaty będą opatrzone datą i godziną wysłania, nazwą użytkownika oraz specjalnym licznikiem generowanym programowo celem stwierdzenia ile wiadomości wysłał już dany proces.



