# Matrix Project

# Cerinta

Trebuie sa realizam un joc pe o matrice de leduri 8x8, trebuie sa creasca in dificultate odata cu avansarea. Jocul va avea un meniu cu mai multe functionalitati:
  1. Mesaj introductiv
  2. Despre:
      - numele jocului
      - numele persoanei care a creat jocul
      - link-ul de la github
  3. Cum se joaca:
      - o scurta descriere
  4. Setari:
      - setarea intensitatii pentru LCD
      - setarea intensitatii pentru matrice
      - sunet (pornit/oprit)
      - numele jucatorului
  5. Podium:
      - salveaza primele trei cele mai bune scoruri inregistrate impreuna cu numele jucatorului
  6. Reseteaza podium:
      - reseteaza toate valorile care se afla pe podium
  7. Incepe jocul

# Realizarea Jocului

Jocul ales de mine este un Bomberman reinterpretat. Am ales acest joc, deoarece am putut sa continui progresul facut la tema 7 si am vrut sa vad daca il pot imbunatati si sa il fac sa ajunga intr-o forma finita.

Jocul se desfasoara astfel: 
  1. La inceput, jucatorul se afla pe pozitia [0,0] din matrice si se genereaza o harta de pereti. Scopul jocului este ca jucatorul sa plaseze bombe pentru a distruga toti peretii.
  2. Dificultatea este data de 3 nivele:
      - La PRIMUL nivel jucatorul are timp nelimitat de a distruge peretii si nu poate sa fie omorat de bombe daca se afla in raza lor cand explodeaza.
      - La AL DOILEA nivel jucatorul are timp nelimitat de a distruge peretii, dar poate sa fie omorat de bombe.
      - La AL TREILEA nivel jucatorul poate castiga jocul daca reuseste sa doboare toti peretii in mai putin de 60 de secunde si fara sa fie omorat de pereti.
  3. Ocupabilitatea hartii de pereti creste odata cu nivelul de dificultate.
  4. Podiumul este actualizat doar daca jucatorul a castigat jocul (a ajuns la ultimul nivel si nu a murit).
  5. Daca jucatorul nu isi seteaza numele inainte de a incepe jocul si are un scor de podium, numele care va aparea in dreptul scorului lui este "AAA".

# Componente folosite

- 1 Arduino Uno Board
- 1 joystick
- 1 8x8 LED Matrix
- MAX7219 Driver
- 1 rezistenta de 100K (pentru matrice)
- 1 rezistenta de 100Ω (pentru buzzer)
- 1 buzzer
- 1 LCD
- 1 rezistenta de 330Ω (pentru LCD)
- 1 capacitor electrolitic (10 µF)
- 1 capacitor ceramic (10p ρF)
- multe fire

# Poza
![WhatsApp Image 2023-12-19 at 21 19 59](https://github.com/SaicuCarina/matrix-project/assets/93483071/c7c4ed79-1d6b-4060-945a-b0a4c2082022)

# Link video
  

