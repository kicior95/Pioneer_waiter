# RoboKelner

Robokelner jest próbą rozwiązania problemu robotyzacji dostarczania posilków w środowisku o dużym zagęszczeniu ludzi. Glównymi założeniami projektu było:
  - Środowiskiem pracy jest pomieszczenie zamknięte zawierające stałe oświetlenie
  - Jednostka robota mobilnego porusza sie wyznaczonymi trasami
  - Trasa jest wyznaczona pomiedzy obiektami, których detekcja nie jest trywialnym zadaniem
  - Trasa jest stała w czasie, z możliwością szybkiej adaptacji robota do nowego układu 
  - Jednostka mobilna nie może narazić innych obiektów stałych i ruchomych na uszkodzenie
  
  ##  Moduły programu
 Projekt sklada sie z podmodułów zawierające programy wykonujące poszczególne zadania. Schemat połączeń pomiędzy modułami załączono poniżej.
 ![1](https://user-images.githubusercontent.com/40024757/41286333-53787c0c-6e3f-11e8-8d29-308ffc590575.png)

## Środowisko pracy
W projekcie wykorzystano program symulujący założenia projektowe. Do tego celu zastosowano następujące narzędzia:
  - ROS(Robot Operating System) wykorzystany jako system w , którym będą komunikowały sie moduły skladowe, działające na rzeczywistym robocie mobilnym. Wersja: ROS Kinetic Kame 
  - V-REP wykorzystany do symulacji działania robota mobilnego. Wersja: V-REP PRO EDU V3.5.0 rev4
  
  ### Instalacja
  ```sh
$ TODO
$ 
```
