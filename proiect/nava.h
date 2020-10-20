#pragma once
#include"obiect.h"

struct arma {
	int id;
	int munitie;				// armele au munitie limitata
	int spread, spread_angle;	// proiectilele pot fi spawnate cu o mica deviatie fata de pozitia armei

	/*
	heat este un cooldown pe arma care penalizeaza fire rate cand tragi continuu / permite un burst initial
	are o valoare intre 0 si heatmax
	in fiecare frame: if (heat > 0) heat--;
	cand folosesti arma, heat += heat_per_round
	daca heat >= heatmax, arma nu mai poate fi folosita
	*/
	int heatmax, heat, heat_per_round;

	arma(int p_id);
	bool trage();
	void update();
};

class nava :public obiect {
private:
	float viteaza_viraj;								// in grade
	int lungime, latime;								// dimensiuni pentru AABB
	int hp_fuselaj, hp_aripa_stanga, hp_aripa_dreapta;	// pentru cand vor fi implementate hitboxurile
	int cooldown_max_abilitate, cooldown_abilitate;		// pentru cand vor fi implementate abilitatile

public:
	arma* arma1, * arma2;

	nava(int p_x, int p_y, int p_rot, int p_sprite_id);
	~nava() { delete arma1; delete arma2; }
	void viraj_dreapta();
	void viraj_stanga();
	void incetinire();
	void abilitate();
	void update();
	void ia_damage(int damage, int part);
	int get_lungime();
	int get_latime();
	int get_hp();
	void distruge();
};