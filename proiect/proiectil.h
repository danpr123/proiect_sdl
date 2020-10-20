#pragma once
#include "obiect.h"

#pragma warning (disable : 4244)

class proiectil : public obiect {
	int lifetime;					// numarul de frameuri cat e activ (decide distanta maxima)
	int damage;
	int lungime, latime;			// dimensiune pentru AABB collision check
public:
	proiectil();
	proiectil(int p_x, int p_y, int p_rot, int p_sprite_id);
	void update();
	void activeaza() { stare = 1; }
	void deactiveaza() { stare = 0; }
	void set(int p_x, int p_y, int p_rot, int p_sprite_id);
	int get_lungime() { return lungime; }
	int get_latime() { return latime; }
	int get_damage() { return damage; }
};