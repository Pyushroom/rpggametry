🗡️ RPG Prototype (C++ / Raylib)

Prosty prototyp gry RPG 2D (top-down) napisany w C++ z użyciem Raylib.
Projekt rozwijany jako sandbox do nauki architektury gier i systemów RPG.

🎮 Aktualne funkcjonalności
🌍 Overworld
ruch postaci (WASD / strzałki)
system scen (mapy łączone siatką)
przejścia między scenami
obiekty środowiskowe (ściany, drzewa, skały, drabiny)
💬 NPC i dialogi
NPC z danymi (NpcData)
wielostronicowe dialogi
wybory dialogowe
dialogi zależne od stanu gry (GameState)
📜 Questy
system questów oparty o flagi
wiele questów jednocześnie
quest journal (J)
dwa taby:
aktywne
ukończone
możliwość przypięcia (tracked quest)
HUD pokazujący aktualne zadanie
⚔️ Walka (prototype)
wejście do walki po kontakcie z przeciwnikiem
system turowy:
gracz / przeciwnik
akcje:
atak
ucieczka
prosty system obrażeń
🧠 Architektura

Projekt jest podzielony na moduły:

game/        - główna pętla gry
scene/       - sceny i obiekty świata
world/       - zarządzanie mapą
player/      - ruch i statystyki gracza
npc/         - dane NPC i logika dialogów
dialogue/    - system dialogów
quest/       - questy, journal, tracker
enemy/       - dane przeciwników
battle/      - system walki
Kluczowe systemy:
GameState
Przechowuje flagi świata (np. questy, interakcje)
NpcDatabase
Decyduje jaki dialog wybrać na podstawie stanu gry
QuestSystem
Oblicza stan questów na podstawie flag
BattleController
Obsługuje walkę turową
🛠️ Build
Wymagania
CMake 3.20+
kompilator C++20
vcpkg (zainstalowany raylib)
Build (Windows / MinGW)
cmake --preset mingw-debug
cmake --build --preset mingw-debug
🎯 Sterowanie
Overworld
ruch: WASD / strzałki
interakcja: E
journal: J
Dialogi
wybór: W/S
zatwierdzenie: E / Enter
Journal
J / Esc – otwórz/zamknij
W/S – wybór questa
A/D – zmiana zakładki
Enter – przypnij quest
Walka
W/S – wybór akcji
E / Enter – zatwierdzenie