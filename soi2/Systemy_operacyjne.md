# Systemy operacyjne - SOI 
## Laboratorium 2 

### Raport
 Prowadzący: mgr. inż. Aleksander Pruszkowski

 Autor: Jakub Sikora

 Termin: wtorek 14:00 - 16:00

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
### 1. Treść zadania 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Algorytm szeregowania dzielący procesy użytkownika na grupy: A, B. Zakłada się, że proces jest umieszczony w grupie: A - gdy jego identyfikator procesu jest podzielny bez reszty przez 2, B - gdy jego identyfikator procesu nie jest podzielny bez reszty przez 2. Należy dokonać niezbędnych modyfikacji funkcji systemowych umożliwiającą przenoszenie procesów pomiędzy powyższymi grupami. Proszę także wykonać usługę systemową o prototypie: int set_scheduler(int x); Która ustala proporcje czasowe w jakich scheduler ma wybierać do wykonania między zadaniami z grupy A i B. Argument x może przyjmować wartości 0...100, i oznacza on ile procent czasu dostanie zadanie A (zadanie B dostanie odpowiednio 100-x procent czasu). Usługa ma zwracać 0 gdy udało się wykonać zmianę proporcji czasowych, gdy zwróci -1, oznaczać będzie, że proces wywołujący tę usługę został przydzielony do klasy B i procesom tej klasy nie wolno zmieniać proporcji. Opracować również klarowną i powtarzalną metodę weryfikacji poprawności rozwiązania (np.: za pomocą aplikacji testowych)	
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;


### 2. Planowane rozwiązanie
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Każdy proces grupy `USER` będzie miał przypisaną grupę priorytetu `pri_group` określającą czy należy do grupy A czy do grupy B. Grupa priorytetu będzie ustalana w trakcie tworzenia procesu i będzie inicjalizowana na podstawie przypisanego identyfikatora procesu. W późniejszej fazie życia procesu, grupa priorytetu nie będzie w żaden sposób związana z numerem pid. Pozwoli to na przenoszenie procesów między powyższymi grupami. Zmiana grupy priorytetu będzie realizowana za pomocą nowego wywołania systemowego `void set_pri_group(void)`. Procesy będą szeregowane w ramach pojedynczej kolejki a ich długość wykonywania będzie się różniła w zależności od grupy. Czas wykonywania procesu będzie mógł zostać zmieniony za pomocą wywołania systemowego `int set_scheduler(int x)`. Będzie ona ustalać procent kwantu czasu jaki będzie przyznawany procesom grupy A (procesy grupy B będą otrzymywać odpowiednio 100 - procent_grupy_A procent czasu). Bazowy kwant czasu będzie stały. 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;







### 3. Rzeczywiste rozwiązanie

Rzeczywista realizacja rozwiązania nie odbiega znacząco od planowanego. Zasadnicza różnica nastąpiła w miejscu przyznawania procesowi czasu procesora. W zaimplementowanym przeze mnie rozwiązaniu czas procesora jest odpowiednio wydłużany i przyznawany w funkcji `sched()`. Aby cała procedura mogła przyznawać zmienny kwant czasu procesora, dodałem do struktury procesu `proc` pole informujące ile jeszcze cykli powinno zostać przyznanych temu procesowi, zanim odda on sterowanie do następnego procesu z kolejki. W funkcji `sched()`, wartość ta jest dekrementowana a następnie sprawdzana.  Jeśli osiagnie ona zero, to czas procesora otrzymuje kolejny proces a liczba przyznanych cykli zależy od zmiennej ustawianej za pomocą wywołania systemowego `set_scheduler`. Aby procesor nie został przyznawany na za długi okres czasu pojedynczemu procesowi, skróciłem stałą `MILISECONDS` znajdującą się w pliku `usr/src/kernel/clock.c`. 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

### 4. Realizacja rozwiązania 
#### Modyfikacje funkcjonalne w plikach źródłowych modułu file system
- W pliku `table.c` dodałem odpowiednie pola `no_sys` odpowiadające dodanym przeze mnie wywołaniom systemowym (numery od 78 do 81).

#### Modyfikacje funkcjonalne w plikach źródłowych modułu memory manager
- W pliku `table.c` dodałem odpowiednie pola (kolejno `do_getprigroup`, `do_changeprigroup`, `set_scheduler`, `get_usagetime` odpowiadające dodanym przeze mnie wywołaniom systemowym (numery od 78 do 81).
- W pliku `proto.h` dodałem prototypy nowych wywołań systemowych
- W pliku `main.c` dodałem ciała wywołań systemowych `do_getprigroup`, `do_changeprigroup`, `set_scheduler` oraz `get_usagetime`. Wywołania te są interfejsem do wywołań mikrojądra, przekazują sterowanie, argumenty i pid oryginalnego wywołującego do kernela. 

#### Opis dodanych wywołań systemowych 
- `do_getprigroup` zwraca grupę priorytetu procesu wywołującego. W przypadku błędu zwraca -1.
- `do_changeprigroup` zmienia grupę priorytetu na przeciwną. W przypadku błędu zwraca -1.
- `set_scheduler` zmienia wartość procentu czasu na zadaną. Usługa jest dostępna tylko dla procesów typu A. Jeśli operacja powiodła się, zwraca 0. W przypadku wywołania przez proces typu B, zwraca -1.
- `get_usagetime` zwraca wartość pola `user_time` w polu struktury `proc`.

#### Modyfikacje funkcjonalne w plikach źródłowych modułu kernel
- W pliku nagłówkowym `proc.h` dodałem pola informujące o grupie priorytetu `pri_group` oraz drugie informujące o ilości cykli które zostały jeszcze temu procesowi zanim zostanie mu odebrany procesor.
- W pliku `proc.c` zmieniłem postać funkcji `sched()` tak aby oddawała procesor następnemu procesowi tylko gdy zakończy się wymagana liczba cykli.
- W pliku `system.c` zamieściłem procedury wykonujące wywołania kernela. Odwołują się one do faktycznej tablicy procesów oraz ustawiają zmienne globalne kernela takie jak współczynnik podziału. 

#### Modyfikacje dodatkowych plików nagłówkowych
- W pliku `com.h` dodałem informacje o nowych wywołaniach systemowych jądra systemu operacyjnego.
- W pliku callnr.h dodałem informacje o nowych wywołaniach systemowych modułu `MM`, pełniących rolę interfejsu do wywołań jądra.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
### 5. Testowanie rozwiązania
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Testowanie rozwiązania odbywa się za pomocą programu `soi2`. Uruchamia on 8 procesów za pomocą polecenia `fork()`. Każdy proces "symuluje" wykonywanie długich obliczeń poprzez dekrementację dużej zmiennej typu `unsigned long int`. Długość obliczeń może być regulowana poprzez zmianę predefiniowanej stałej `COUNTERVALUE`. Po zakończeniu wirtualnych obliczeń, proces zwraca ile czasu zajęła mu ta czynność oraz aktualną wartość `user_time` uzyskaną za pomocą dodanego wywołania systemowego. Testowanie algorytmu szeregowania odbywa się poprzez porównanie czasów wykonania próby kontrolnej (tylko jeden proces aktywny) oraz procesów testowych (osiem procesów aktywnych).  Nowo utworzone procesy nie kończą się na wypisaniu komunikatu, tylko dalej pracują w nieskończonej pętli, tak aby nie wpływać na wyniki pomiarów.


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Dodatkowo, za pomocą polecenia `scheduler <arg>` można podać nowy stosunek podziału priorytetu grup A i B.

















### 6. Wyniki testów

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Program testowy uruchomiłem w dwóch konfiguracjach podziału czasu procesora:
- A:B -> (80:20) 
- A:B -> (30:70)

Dodatkowo, uruchamiałem testy przy różnych długościach stałej `COUNTERVALUE`:
- `COUNTERVALUE = 20`
- `COUNTERVALUE = 60` 

Łącznie, uzyskałem 4 wyniki testowe, które znajdują się poniżej.

Rzeczywisty stosunek czasów wykonywania procesora obliczałem sumując wszystkie czasy wykonywania procesów oraz osobno czasy procesów należących do odpowiednich grup a następnie obliczałem procentowy udział każdej z grup w sumie.

#### Podział A:B -> (80:20)  oraz `COUNTERVALUE = 20`

| pri_group | Czas wykonywania | user_time |
| :-------: | :--------------: | :-------: |
|     A     |        14        |    204    |
|     A     |        14        |    207    |
|     A     |        15        |    206    |
|     A     |        15        |    208    |
|     B     |        63        |    201    |
|     B     |        63        |    205    |
|     B     |        63        |    204    |
|     B     |        69        |    221    |

Rzeczywisty stosunek wykorzystania procesora przez odpowiednie grupy 

##### A = 18%

##### B = 82%



#### Podział A:B -> (80:20)  oraz `COUNTERVALUE = 60`

| pri_group | Czas wykonywania | user_time |
| :-------: | :--------------: | :-------: |
|     A     |        50        |    632    |
|     A     |        50        |    644    |
|     A     |        56        |    670    |
|     A     |        55        |    665    |
|     B     |       201        |    646    |
|     B     |       208        |    665    |
|     B     |       208        |    660    |
|     B     |       208        |    650    |

Rzeczywisty stosunek wykorzystania procesora przez odpowiednie grupy 

##### A = 21%

##### B = 79%





#### Podział A:B -> (30:70)  oraz `COUNTERVALUE = 20`

| pri_group | Czas wykonywania | user_time |
| :-------: | :--------------: | :-------: |
|     B     |        15        |    205    |
|     B     |        16        |    209    |
|     B     |        15        |    205    |
|     B     |        15        |    210    |
|     A     |        42        |    193    |
|     A     |        42        |    203    |
|     A     |        42        |    212    |
|     A     |        42        |    215    |

Rzeczywisty stosunek wykorzystania procesora przez odpowiednie grupy 

##### A = 27%

##### B = 73%





#### Podział A:B -> (30:70)  oraz `COUNTERVALUE = 60`

| pri_group | Czas wykonywania | user_time |
| :-------: | :--------------: | :-------: |
|     B     |        62        |    657    |
|     B     |        63        |    667    |
|     B     |        63        |    670    |
|     B     |        63        |    680    |
|     A     |       146        |    664    |
|     A     |       145        |    670    |
|     A     |       145        |    682    |
|     A     |       152        |    668    |

Rzeczywisty stosunek wykorzystania procesora przez odpowiednie grupy 

##### A = 30%

##### B = 70%



### 6. Wnioski

Uzyskane rzeczywiste stosunki czasów wykorzystania procesora przez odpowiednie grupy nieznacznie odbiegają od wartości zadanej. Dodatkowo, rzeczywiste czasy wykonywania się procesu na procesorze zapisane w polu `user_time` są stałe i zależą tylko od stałej `COUNTERVALUE`, a w szczególności nie zależą od grupy priorytetowej i od liczby sekund wykonywania się procesu.

Długość wykonania zadania na procesorze jest stała dla każdego zadania a pomimo tego procesy o różnych grupach priorytetowych kończą się w różnych interwałach. Dzieje się tak ponieważ algorytm szeregowania przyznaje więcej czasu procesom o odpowiedniej grupie. 

Przedstawione wnioski jednoznacznie wskazują że zadany algorytm szeregowania został zaimplementowany poprawnie i działa zgodnie z założeniami. 