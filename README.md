# Számítógépi Grafika Féléves Feladat - Kovácsműhely

Ez a projekt a Számítógépi grafika tantárgy féléves beadandó feladata. A program egy középkori kovácsműhelyt mutat be 3D-ben, melyet a felhasználó szabadon, belső nézetből (FPS kamera) bejárhat.

A program C nyelven íródott, az ablakkezelést és a bemeneteket az **SDL2** könyvtár, a 3D-s megjelenítést pedig az **OpenGL** végzi.

## Specifikáció és Követelmények

A projekt az alábbi kötelező és extra funkciókat valósítja meg a maximális pontszám elérése érdekében:

### Alapkövetelmények (Elégséges szint)
- [x] **Kamerakezelés:** A tér egérrel és billentyűzettel (WASD) szabadon bejárható.
- [x] **Textúrák:** A 3D modellekre és a környezetre textúrák vannak feszítve (pl. kőpadló).
- [x] **Objektumok:** Térbeli 3D modellek (obj fájlok) betöltése és megjelenítése (pl. üllő, kard, fáklya).
- [x] **Fények:** A színtér bevilágítása, a fényerő interaktív állítása a `+` és `-` gombokkal.
- [x] **Animáció:** Interaktív, időalapú (Delta time) mozgás (pl. lebegő/forgó kard).
- [x] **Használati útmutató:** Az `F1` gomb lenyomására megjelenő 2D-s overlay súgó.

### Tervezett Többletfunkciók (+3 jegy)
- [x] **Ütközésvizsgálat (Bounding-box):** A kamerával nem lehet átmenni a falakon és a tárgyakon.
- [x] **Köd hatás (Fog):** Dinamikus, távolságfüggő köd/füst a műhely levegőjében a hangulat fokozására.
- [ ] **Részecskerendszer (Particle system):** Felszálló szikrák vagy tűz a fáklya/üllő körül.

---

## Irányítás (Kezelés)

| Gomb / Egér | Funkció |
| :--- | :--- |
| **W, A, S, D** | Mozgás előre, hátra, balra, jobbra |
| **Egér mozgatása** | Nézelődés (Kamera forgatása) |
| **Numpad +** | Fényerősség növelése |
| **Numpad -** | Fényerősség csökkentése |
| **F / G** | Köd sűrűségének növelése / csökkentése |
| **TAB** | Egér megjelenítése |
| **F1** | Súgó megjelenítése / elrejtése |
| **ESC** | Kilépés a programból |

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
   blacksmith.exe

---

## Felhasznált Források (Assets)

A projektben felhasznált 3D modellek és textúrák külső forrásból származnak:

* **Üllő 3D modell (Anvil):** [Free3D.com - Anvil by printable_models](https://free3d.com/3d-model/anvil-342094.html) (A modell motorhoz igazítva, triangulálva lett).
* **Üllő fém textúra:** [Freepik - Metallic background with grunge scratched effect](https://www.freepik.com/free-photo/metallic-background-with-grunge-scratched-effect_13839423.htm)
* **Padló textúra (Cobblestone):** [OpenGameArt - Seamless Cobblestone Texture by pzUH](https://opengameart.org/content/seamless-cobblestone-texture)
* **Kard textúra és 3D modell (Sword):** [Free3D.com - Sting Sword](https://free3d.com/3d-model/sting-sword-128810.html) (A modell motorhoz igazítva, triangulálva lett).
* **Fal textúra (Wall):** [Pixabay.com - Brick Wall](https://pixabay.com/photos/brick-wall-red-structure-masonry-1916752)
* **Plafon textúra (Ceiling):** [Pixabay.com - Wooden Boards](https://pixabay.com/photos/wood-wooden-boards-wooden-planks-411643)

---

**Készítette:** Dudás Dániel  
**Neptun kód:** ADVLIC