[Engleska verzija](README.md).

Uvod
============
Prijatelj Zvonimir Lapov me upitao bi li mogao dizajnirati pločicu u THT tehnologiji za potrebe edukacije učenika osnovnih škola.
Svrha edukacijskog modula "Uhvati me BCL-2" je edukacija učenika. Pokazuje prednosti spajanja sedam dvobojnih svjetlećih dioda u spoj charlieplexing (multipleksiranje sa tri stanja).
Ova verzija je dizajnirana za slučaj kada dvobojna svjetleću dioda ima svjetliju diodu na pinu 3.
Na tržištu postoji mnogo varijacija dvobojnih svjetlećih dioda, pa ćete morati sami definirati otpornike.

![PCB application](images/attiny85_catch_me_bcl-2_pcb.jpg "Edukacijski modul Uhvati me BCL-2")
Fotografirao Robert Sedak

Pozor:
------------
Kako bi se program u cijelosti mogao izvršiti, na ATtiny pin RESET moramo deklarirati kao ulazno/izlazni pin. To se postiže aktiviranjem FUSE bita s nazivom RSTDISBL. Ukoliko nakon toga želimo ponovno programirati ATtiny, moramo isključiti FUSE bit s nazivom RSTDISBL pomoću tehnike programiranja pomoću visokog napona (12V).
Postupak programiranja FUSE bitova i tehniku programiranja pomoću visokog napona potražite u stručnoj literaturi. Novostećena znanja koristite tek kada potpuno razumijete što morate napraviti. U suptornom možete oštetiti mikroupravljač.


Autor
------------
- Robert Sedak — svojstva, programiranje, dizajn el. scheme, CAD dizajn, kučište
- Zvonimir Lapov - ideja


Programiranje
-----------
Robert Sedak je snimio video s primjerom programiranim u Arduino IDE s podrškom za ATtiny85:
[![PCB application](images/default.png)](https://youtu.be/r-Ip-7PmgAQ "Edukacijski modul Uhvati me BCL-2 - primjer programiranja")
Fotografirao Robert Sedak




Primjere za Arduino IDE možete preuzeti [ovdje](examples/).


Popis komponenti
-----------------
- 1 kom, PCB
- 5 kom, 100 ohm 1/4W 1% otpornik (R1, R2, R3, R4, R5)
- 7 kom, 1.5K ohm 1/4W 1% otpornik (R6, R7, R8, R9, R10, R11, R12)
- 7 kom, dvobojna LED 5mm (D1, D2, D3, D4, D5, D6, D7) za zajedničkom katodom
- 1 kom, 6x6x4.3mm SPST tipkalo (SW1)
- 1 kom, SDPT 1P2T pomični prekidač (SW2)
- 1 kom, 100nF 50V keramički kondenzator
- 1 kom, 8-Pin DIL IC podnožje
- 1 kom, ATtiny85 ili ATtiny13A
- 1 kom, kučište za bateriju Keystone 1060
- 1 kom, baterija 2032



Datoteke s dizajnom
------------
Ovaj semafor je dizajniran pomoću programa [KiCad](http://kicad.org/). Datoteke se nalaze u mapi [design_files](design_files/). Elekroničku schemu možete pogledati [ovdje](images/attiny85_catch_me_bcl-2_schematic.pdf).


Gerber datoteke
------------
Gerber datoteke možete preuzeti [ovdje](gerber/attiny85_catch_me_bcl-2.zip).


Firmware
--------
Ovo učilo može biti progrmairano pomoću [Arduino IDE](https://www.arduino.cc/).
[ATTinyCore by Spence Konde](https://github.com/SpenceKonde/ATTinyCore) je korišten za programiranje ATtiny85 u Arduino IDE.
[MicroCore by MCUdude](https://github.com/MCUdude/MicroCore) je korišten za programiranje ATtiny13A u Arduino IDE.
Primjeri za Arduino IDE se nalaze u mapi [examples](examples/).


Kučište
-------
Robert Sedak je dizajnirao kučište za pločicu. Možete preuzeti datoteke u STL i FCStd obliku iz mape [case](case/).
Ovo kučište je dizajnirano pomoću programa [FreeCAD](https://www.freecad.org/).

**Popis spojnih materijala**
- 4 kom, matica M3
- 4 kom, M3x5mm + 4mm plastični odstojnik sa unutarnjim i vanjskim navojem
- 4 kom, M3x5mm plastični vijak

![PCB kučište](images/attiny85_catch_me_bcl_case1.jpg "Catch me educational module")
Fotografirao Robert Sedak

![PCB kučište](images/attiny85_catch_me_bcl_case2.jpg "Catch me educational module")
Fotografirao Robert Sedak


Licenca
-------
[Imenovanje-Dijeli pod istim uvjetima 4.0 međunarodna (CC BY-SA 4.0)](https://creativecommons.org/licenses/by-sa/4.0/deed.hr)

Slobodno možete:
- Dijelite dalje — možete umnažati i redistribuirati materijal u bilo kojem mediju ili formatu
-  Stvarajte prerade — možete remiksirati, mijenjati i prerađivati djelo u bilo koju svrhu, pa i komercijalnu

Davatelj licence ne može opozvati slobode korištenja koje Vam je ponudio dokle god se pridržavate uvjeta licence.

Pod sljedećim uvjetima:
- Imenovanje — Morate adekvatno navesti autora, uvrstiti link na licencu i naznačiti eventualne izmjene. Možete to učiniti na bilo koji razuman način, ali ne smijete sugerirati da davatelj licence izravno podupire Vas ili Vaše korištenje djela.
- Dijeli pod istim uvjetima — Ako remiksirate, mijenjate ili prerađujete materijal, Vaše prerade morate distribuirati pod istom licencom pod kojom je bio izvornik.

Bez daljnjih ograničenja — Ne smijete dodavati pravne uvjete ili tehnološke mjere zaštite koji će druge pravno ograničiti da čine ono što im licenca dopušta.

Odricanje od odgovornosti
-------------------------
NAPOMENA - Dizajn je objavljen u dobroj namjeri i "takav kakav je" ("AS IS"), te sa mogućim greškama. Autor je kreirao učilo, slike, video i programski kod. Autoru je radilo i uspjelo raditi na način kako je opisano, što ne znaći da će raditi i Vama. Ne odgovaram ni za kakve posljedice koje mogu nastati prilikom korištenja učila, uputa ili programskog koda. Odričem se sve odgovorosti i jamstva- sami ste odgovorni za ono što radite i sami ste odgovorni što ste pristali pratiti i koristiti naše upute i informacije.

Autor može mijenjati specifikacije i opise proizvoda u bilo kojem trenutku, bez prethodne obavijesti. Korisnik se ne smije osloniti na odsutnost ili karakteristike bilo kojih značajki ili uputa označenih kao "rezervirano" ili "nedefinirano".
Autor zadržava pravo budućih izmjena definicija i neće snositi nikakvu odgovornost za sukobe ili nekompatibilnosti koje proizlaze iz budućih promjena. Podaci o proizvodu na web stranici ili materijalima podložni su promjenama bez prethodne obavijesti. Nemojte finalizirati dizajn s ovim podacima.