DevTools

DevTools 1.0 – CLI tool per generare rapidamente file di sviluppo per C++, Python e Rust.
Supporta la creazione di classi, moduli e shader con flags personalizzabili.

📌 Funzionalità principali
Creazione classi: genera file .h, .cpp, .hpp, .c, .py o .rs a seconda della lingua.
Creazione moduli: genera directory con i file corrispondenti.
Creazione shader: genera file shader .vert, .frag, .geom, .comp, .glsl, .sha, .vs, .fs, .vsh, .fsh.
Gestione linguaggi: cambia lingua di default con set_lang.
Help integrato: visualizza comandi disponibili direttamente dal terminale.
⚡ Installazione

Clona il repository e compila:

git clone https://github.com/<username>/devtools.git
cd devtools
g++ src/main.cpp -o devtools -std=c++17

Esegui:

./devtools
🖥️ Uso
Comandi principali
class <name> [flags]

Crea una classe con file base.

Flags C++:

Flag	Descrizione
-h	Genera header .h
-hpp	Genera header .hpp
-cpp	Genera implementazione .cpp
-c	Genera implementazione .c

Esempi:

devtools> class Player
# Cpp: crea Player.h + Player.cpp
devtools> class Player -hpp
# Cpp: crea Player.hpp

Python e Rust: crea direttamente .py o .rs.

module <name> [flags]

Crea un modulo (directory + file).

Flags C++: stessi flag di class.
Esempio:

devtools> module Renderer -h -cpp
# Cpp: crea directory Renderer con Renderer.h e Renderer.cpp

Python e Rust: crea file all’interno della cartella.

shader <name> [flags]

Crea file shader con supporto a combinazioni:

Flag	Output
-vert	.vert
-frag	.frag
-geom	.geom
-comp	.comp
-glsl	.glsl
-sha	.sha
-xs	.vs + .fs
-xsh	.vsh + .fsh
-v	versione vertex
-f	versione fragment

Esempio:

devtools> shader Lighting -vert -frag
# Crea Lighting.vert e Lighting.frag
set_lang [flag]

Cambia lingua predefinita: C++, Python, Rust.

Flag	Linguaggio
-cpp	C++
-py	Python
-rs	Rust

Esempio:

devtools> set_lang -py
help

Mostra la guida rapida con link alla documentazione completa.

devtools> help
exit

Chiude l’applicazione.

📂 Struttura dei file generati
C++ class example
Player.h
--------
#pragma once

class Player
{
public:
    Player();
    ~Player();
};

Player.cpp
----------
#include "Player.h"

Player::Player() {}
Player::~Player() {}
Python class example
player.py
---------
class Player:
    def __init__(self):
        pass
Rust class example
player.rs
----------
pub struct Player;

impl Player {
    pub fn new() -> Self {
        Player
    }
}
📖 Documentazione completa

https://github.com/prbAsalad19/devtools-linux

MIT License – vedi LICENSE per i dettagli.