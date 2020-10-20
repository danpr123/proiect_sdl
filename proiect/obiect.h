#pragma once
#include "utile.h"

/*
orice are sprite si se misca
*/

class obiect {
protected:
	float x, y;						// coordonate
	float rotatie;					// directia spre care se misca, in grade
	float viteza, viteza_maxima;	// pixeli pe frame
	int spriteID;
	int stare;						// daca obiectul respectiv este activ sau nu
public:
	obiect() {}
	obiect(int p_x, int p_y, int p_rot, int p_sprite_id);
	void deplasare();
	void deplasare(int);
	punct deplasare_return(int);
	int getx();
	int gety();
	int get_id();
	int get_rotatie();
	int get_stare();
	float get_viteza();
};