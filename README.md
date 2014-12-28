DATA VAULT - readme



Krótka instrukcja, jak odpalić program w Codeblocksie w środowisku Linux:
- pobieracie boost i boost-asio, jeśli ich nie macie
sudo apt-get install libboost-all-dev
sudo apt-get install libasio-dev
- otwieracie oba projekty (DataVaultServer.cbp i DataVaultClient.cbp) w Codeblocksie
- budujecie oba 
- jak oba się zbudują, odpalacie - jeden w Codeblocksie, 2-gi w terminalu bądź oba w osobnych terminalach 
- aby odpalić np. serwer w terminalu, przechodzicie do DataVaultServer/bin/Debug i piszecie w konsoli: 
./DataVaultServer 21000 21001 - dla serwera, 
./DataVaultClient localhost 21000 21001 - dla klienta
(porty możecie sobie dobrać jak chcecie, byle były te same dla serwera i klienta).



Szybki kurs gita:

TWORZENIE LOKALNEGO REPOZYTORIUM PROJEKTU: 
git clone https://github.com/Macielos/tin-project.git

PRACA W BRANCHACH:
Jeśli dodajecie coś dużego, to nie róbcie tego w masterze, tylko załóżcie swojego brancha. Do swojego brancha możecie swobodnie wrzucać, co chcecie. Dopiero gdy wszystko jest skończone, tworzycie pull requesta, patrzycie czy wszystko ok, jak chcecie, możecie dać komuś innemu do ocenienia. Na koniec mergujecie zmiany do mastera. 

Żeby stworzyć brancha:
git checkout -b nazwa_waszego_brancha

Żeby przełączyć się pomiędzy branchami:
git checkout branch_docelowy

Żeby wylistować wszystkie branche (i zobaczyć, w którym aktualnie jesteśmy)
git branch

WRZUCANIE ZMIAN DO GŁÓWNEGO REPOZYTORIUM: 

git add plik1 plik2... albo git add .
git commit -m "komentarz do commita"
git push origin twój_branch

POBIERANIE ZMIAN Z GŁÓWNEGO REPOZYTORIUM

git fetch - uaktualnia wszystkie lokalne branche
git merge origin/nazwa_brancha (albo origin/master)

albo 
git pull

Jeśli merge'ujecie brancha z innym branchem i w obydwu były zmieniane te same pliki, będziecie mieli konflikty. W plikach będziecie mieli obie wersje określonych fragmentów oddzielone liniami <<<<<<, pierwsza wasza, druga w branchu, który chcecie domerge'ować. Musicie ręcznie przeedytować pliki, zostawić którąś z wersji lub je jakoś połączyć, a następnie:
git add plik1 plik2 ...
git commit (bez komentarza)
Otworzy wam się plik z opisem commita. Zostawiacie wszystko jak jest, zapisujecie i wychodzicie:
ctrl-o, enter, ctrl-x
