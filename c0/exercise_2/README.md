# POSIX and Unix signals

## Treść:  
Sygnały są podstawowym mechanizmem informowania procesów o zdarzeniach, na które mogą zareagować. Pełna lista sygnałów w systemie Unix dostępna jest po wpisaniu man kill, lub kill -l. Przykładem są często demony działające w sytemie UNIX/Linux, które są w stanie odczytać ponownie konfigurajcę bez potrzeby uruchamiania się na nowo, lub dowolny program, który może zwolnić zarezerwowane zasoby w momencie wystąpienia błędu naruszenia ochrony pamięci (przykład cleanup.c, przykładowymi zasobami mogą być np. pliki tymczasowe).  

## Zadanie do przećwiczenia:  
Napisać program, który tworzy N procesów potomnych. Każdy z tych procesów obsługuje sygnał HUP (N potomnych oraz proces pierwotny). Po uruchomieniu procesy potomne wykonują czasochłonne zadanie (np. poszukują kolejnych liczb pierwszych wypisując je na ekran – wraz ze swoim PID). Proces potomny po otrzymaniu sygnału HUP rozpoczyna pracę od nowa. Gdy proces nadrzędny otrzyma sygnał HUP, przekazuje go do wszystkich potomków.  

## Po wykonaniu ćwiczenia powinieneś wiedzieć:  
1. co to są sygnały, do czego służą?
2 .jak wysyłać je z terminala (kill)?
3. jak wysyłać je z C?
4. jak można maskować/blokować sygnały?
5. czy wszystkie sygnały da się zablokować?
6. czemu sleep(10) może skończyć się przed upływem 10s?  
