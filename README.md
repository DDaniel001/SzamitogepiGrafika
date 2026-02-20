# Számítógépi Grafika Féléves Feladat - Kovácsműhely

Ez a projekt a Számítógépi grafika tantárgy féléves beadandó feladata. A program egy középkori kovácsműhelyt mutat be 3D-ben, melyet a felhasználó szabadon, belső nézetből (FPS kamera) bejárhat.

A program C nyelven íródott, az ablakkezelést és a bemeneteket az **SDL2** könyvtár, a 3D-s megjelenítést pedig az **OpenGL** végzi.

## Specifikáció és Követelmények

A projekt az alábbi kötelező és extra funkciókat valósítja meg a maximális pontszám elérése érdekében:

### Alapkövetelmények (Elégséges szint)
- [x] **Kamerakezelés:** A tér egérrel és billentyűzettel (WASD) szabadon bejárható.
- [x] **Textúrák:** A 3D modellekre és a környezetre textúrák vannak feszítve (pl. kőpadló).
- [ ] **Objektumok:** Térbeli 3D modellek (obj fájlok) betöltése és megjelenítése (pl. üllő, kard, fáklya).
- [ ] **Fények:** A színtér bevilágítása, a fényerő interaktív állítása a `+` és `-` gombokkal.
- [ ] **Animáció:** Interaktív, időalapú (Delta time) mozgás (pl. lebegő/forgó kard).
- [ ] **Használati útmutató:** Az `F1` gomb lenyomására megjelenő 2D-s overlay súgó.

### Tervezett Többletfunkciók (+3 jegy)
- [ ] **Ütközésvizsgálat (Bounding-box):** A kamerával nem lehet átmenni a falakon és a tárgyakon.
- [ ] **Köd hatás (Fog):** Dinamikus, távolságfüggő köd/füst a műhely levegőjében a hangulat fokozására.
- [ ] **Részecskerendszer (Particle system):** Felszálló szikrák vagy tűz a fáklya/üllő körül.

---

## Irányítás (Kezelés)

| Gomb / Egér | Funkció |
| :--- | :--- |
| **W, A, S, D** | Mozgás előre, hátra, balra, jobbra |
| **Egér mozgatása** | Nézelődés (Kamera forgatása) |
| **ESC** | Kilépés a programból |
| **F1** | Súgó megjelenítése / elrejtése *(fejlesztés alatt)* |
| **Numpad + / -** | Fényerősség növelése és csökkentése *(fejlesztés alatt)* |

---

## Fordítás és Futtatás

### Rendszerkövetelmények
A program fordításához Windows rendszeren a **MinGW** környezet szükséges az alábbi telepített csomagokkal:
- `gcc` fordító és `make`
- `SDL2` és `SDL2_image` fejlesztői könyvtárak
- `opengl32` és `glu32`
- A tárgyhoz biztosított statikus `libobj.a` modellbetöltő könyvtár.

### Telepítés és Futtatás
1. Klónozza vagy töltse le a repository-t.
2. Nyissa egy terminált a projekt gyökerében (ahol a `Makefile` található).
3. Addja ki a fordítás parancsát:
   ```bash
   make