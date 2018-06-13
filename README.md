# RoboKelner

Robokelner jest próbą rozwiązania problemu robotyzacji dostarczania posilków w środowisku o dużym zagęszczeniu ludzi. Glównymi założeniami projektu było:
  - Środowiskiem pracy jest pomieszczenie zamknięte zawierające stałe oświetlenie
  - Jednostka robota mobilnego porusza sie wyznaczonymi trasami
  - Trasa jest wyznaczona pomiedzy obiektami, których detekcja nie jest trywialnym zadaniem
  - Trasa jest stała w czasie, z możliwością szybkiej adaptacji robota do nowego układu 
  - Jednostka mobilna nie może narazić innych obiektów stałych i ruchomych na uszkodzenie
  
  ##  Moduły programu
 Projekt sklada sie z podmodułów zawierające programy wykonujące poszczególne zadania. Schemat połączeń pomiędzy modułami załączono poniżej.
 <p align="center">
<img src="https://user-images.githubusercontent.com/40024757/41286333-53787c0c-6e3f-11e8-8d29-308ffc590575.png" width="50%">
</p>

### Moduł: Line Tracking
Moduł odpowiedzialny jest za rozpoznawanie lini. Sygnałem wejściowym jest obraz pochodzący z kamery. Sygnałem wyjściowym jest chmura punktów,z których złożona jest linia. Obraz wejściowy jest kolejno poddawany następującym operacją:
  - Konwersja przestrzeni barw z RGB na HSV
  - Podział obrazu na poszczególne warstwy przestrzeni barw HSV
  - Operacja progowania wybranej, optymalnej warstwy Saturation
  - Zamiana układu współrzędnych na układ bazowy robota
  - Wyslanie chmury punktów
  
Moduł wykorzystuje biblioteke OpenCV do przetwarzania obrazu. Uzyskany obraz po progowaniu można poddać operacji dylatacji lub erozji w zależności od uzyskanego obrazu wynikowego. 
<p align="center">
<img src="https://user-images.githubusercontent.com/40024757/41373313-44d1f2ac-6f50-11e8-810f-1c65980839e5.png" width="50%">
</p>

### Moduł: Control
TODO
<p align="center">
<img src="https://user-images.githubusercontent.com/40024757/41373316-47219972-6f50-11e8-946f-1d12f12c05a8.png" width="50%">
</p>

### Moduł: Barcode Scanner
TODO
<p align="center">
<img src="https://user-images.githubusercontent.com/40024757/41373319-47730f1e-6f50-11e8-9e5d-89d5b09fa37d.png" width="50%">
</p>

### Moduł: Object Detector
TODO
<p align="center">
<img src="https://user-images.githubusercontent.com/40024757/41373317-4743b50c-6f50-11e8-8e97-fd1200c36bf7.png" width="50%">
</p>

### Route Planning
TODO
<p align="center">
<img src="https://user-images.githubusercontent.com/40024757/41373455-b65909d8-6f50-11e8-95e6-cbb01e0ded25.png" width="50%">
</p>
 

## Środowisko pracy
W projekcie wykorzystano program symulujący założenia projektowe. Do tego celu zastosowano następujące narzędzia:
  - ROS(Robot Operating System) wykorzystany jako system w , którym będą komunikowały sie moduły skladowe, działające na rzeczywistym robocie mobilnym. Wersja: ROS Kinetic Kame 
  - V-REP wykorzystany do symulacji działania robota mobilnego. Wersja: V-REP PRO EDU V3.5.0 rev4
  
  ### Instalacja
  ```sh
$ TODO
$ 
```
