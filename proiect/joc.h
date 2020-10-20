#pragma once
#include "utile.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include "nava.h"
#include "proiectil.h"

/*
in metoda ini sunt incarcate resursele
metoda loop este bucla jocului
*/

class joc {
	int width, height;												// rezolutia
	long ultimul_frame;												// timpul la care a fost randat ultimul frame, pentru limitare framerate
	std::vector<nava*> player;										// toti jucatorii, inclusiv AI
	int playeri_activi;												// numarul de playeri ramasi in joc
	proiectil* proiectile;											// un array cu toate proiectilele existente in joc la un moment dat
	int max_proiectile;												// numarul de proiectile existente in joc la un moment dat
	SDL_Window* fereastra = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* textura[id_imagine::numar_imagini] = { NULL };		// nu e nici un failsafe mechanism; deocamdata contorul e id_imagine::numar_imagini din utile.h
	SDL_Event event;
	const Uint8* stare;												// array stare KEYDOWN (tastatura)
	bool creare_fereastra();
	bool incarca_resurse();
	void setup_playeri(int);
	void evaluare_player_input();
	void evaluare_AI();
	void evaluare_coliziuni();
	void limitare_framerate();
	void creaza_proiectil(int p_playerid, int p_slotarma);
	void randare();
	void draw_nave();
	void draw_proiectile();
	void draw_gui();
	void update();
	int selectie_nava();
	bool coliziune_AABB(int p_idproiectil, int p_idnava);
	bool inside_box(int p_playerid);
	bool AI_directie(int curent);
	punct intersectie_segmente(punct, punct, punct, punct);
	void deseneaza_sageti(int&, int&);
	void deseneaza_nave_selectie(int);
public:
	joc() : width(1366), height(768) {}
	joc(int p_width, int p_height) : width(p_width), height(p_height) {}
	~joc();
	void ini();
	void loop();
};