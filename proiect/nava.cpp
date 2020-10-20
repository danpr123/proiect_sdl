#include "nava.h"


nava::nava(int p_x, int p_y, int p_rot, int p_sprite_id) : obiect(p_x, p_y, p_rot, p_sprite_id)
{
	/*
	stare = 200 -> avionul e in viata
	stare = x (< 200), avionul e distrus si in x frame-uri dispare
	stare = 0, avionul nu mai e in joc
	*/
	stare = 200;

	/*
	partea de mai jos o sa fie inlocuita cu clase pentru fiecare avion care extind clasa nava
	dar deocamdata sunt doar 5 modele si e mai usor de urmarit / ajustat stats aici
	*/
	switch (spriteID)
	{
	case id_imagine::iar80:
	{
		lungime = 90;
		latime = 110;
		viteza = 1.4f;
		viteza_maxima = 5.5f;
		viteaza_viraj = 2.5f;
		arma1 = new arma(id_imagine::mm127);
		arma2 = new arma(id_imagine::mm200);
		hp_aripa_dreapta = hp_aripa_stanga = 95;
		hp_fuselaj = 110;
		break;
	}
	case id_imagine::la5:
	{
		lungime = 86;
		latime = 98;
		viteza = 1.4f;
		viteza_maxima = 6.5f;
		viteaza_viraj = 2.3f;
		arma1 = new arma(id_imagine::mm200);
		arma2 = new arma(id_imagine::racheta);
		hp_aripa_dreapta = hp_aripa_stanga = 95;
		hp_fuselaj = 110;
		break;
	}
	case id_imagine::me109:
	{
		lungime = 89;
		latime = 99;
		viteza = 1.4f;
		viteza_maxima = 6.2f;
		viteaza_viraj = 2.5f;
		arma1 = new arma(id_imagine::mm127);
		arma2 = new arma(id_imagine::mm200);
		hp_aripa_dreapta = hp_aripa_stanga = 105;
		hp_fuselaj = 90;
		break;
	}
	case id_imagine::il2:
	{
		lungime = 115;
		latime = 146;
		viteza = 1.4f;
		viteza_maxima = 4.1f;
		viteaza_viraj = 1.8f;
		arma1 = new arma(id_imagine::mm200);
		arma2 = new arma(id_imagine::racheta);
		hp_aripa_dreapta = hp_aripa_stanga = 130;
		hp_fuselaj = 155;
		break;
	}
	case id_imagine::ju87:
	{
		lungime = 110;
		latime = 138;
		viteza = 1.4f;
		viteza_maxima = 3.9f;
		viteaza_viraj = 1.7f;
		arma1 = new arma(id_imagine::mm200);
		arma2 = new arma(id_imagine::racheta);
		hp_aripa_dreapta = hp_aripa_stanga = 145;
		hp_fuselaj = 115;
		break;
	}
	}
}

/*-----------------*/

void nava::viraj_dreapta()
{
	rotatie += viteaza_viraj;
	incetinire();				// scade viteza cand vireaza
}

/*-----------------*/

void nava::viraj_stanga()
{
	rotatie -= viteaza_viraj;
	incetinire();				// scade viteza cand vireaza
}

/*-----------------*/

void nava::incetinire()
{
	if (viteza > viteza_maxima * 0.6f && stare == 200)
		viteza *= 0.94f;
}

/*-----------------*/

void nava::abilitate()
{
	if (!cooldown_abilitate)
	{
		//do abilitate, neimplementat inca. Posibil sa devina o metoda virtuala daca navele au abilitati diferite.
		cooldown_abilitate = cooldown_max_abilitate;
	}
}

/*-----------------*/

void nava::update()
{
	// este apelata la fiecare frame al jocului
	deplasare();
	if (cooldown_abilitate)
		cooldown_abilitate--;
	arma1->update();
	arma2->update();
	if (stare < 200)
	{
		stare--;
		viteza *= 0.92f;
		rand() % 3 ? viraj_dreapta() : viraj_stanga();
	}
	if (rotatie >= 360)
		rotatie -= 360;
	if (rotatie <= 0)
		rotatie += 360;
}

/*-----------------*/

int nava::get_lungime()
{
	return lungime;
}

/*-----------------*/

int nava::get_latime()
{
	return latime;
}

/*-----------------*/

int nava::get_hp()
{
	return std::min({ hp_aripa_dreapta, hp_aripa_stanga, hp_fuselaj });
}

/*-----------------*/

void nava::distruge() {
	if (stare == 200)	// pt ca e posibil sa fie apelata metoda asta cand nava e deja distrusa => skip la niste frame-uri
		stare--;
}

/*-----------------*/

arma::arma(int p_id)
{
	id = p_id;
	heat = 0;
	switch (id)
	{
	case id_imagine::mm127:
	{
		heatmax = 12;
		heat_per_round = 3;
		munitie = 250;
		spread = 11;
		spread_angle = 8;
		break;
	}

	case id_imagine::mm200:
	{
		heatmax = 14;
		heat_per_round = 7;
		munitie = 100;
		spread = 15;
		spread_angle = 6;
		break;
	}

	case id_imagine::racheta:
	{
		heatmax = 420;
		heat_per_round = 420;
		munitie = 4;
		spread = 1;
		spread_angle = 1;
		break;
	}
	}
}

/*-----------------*/

bool arma::trage() {
	if (heat + heat_per_round <= heatmax && munitie > 0)
	{
		heat += heat_per_round;
		munitie--;
		return true;
	}
	else
		return false;
}

/*-----------------*/

void arma::update() {
	if (heat > 0)
		heat--;
}

/*-----------------*/

void nava::ia_damage(int dmg, int part)
{
	switch (part)
	{
	case 0:
		hp_fuselaj -= dmg;
		break;
	case 1:
		hp_aripa_stanga -= dmg;
		break;
	case 2:
		hp_aripa_dreapta -= dmg;
		break;
	}
	if (get_hp() < 0)
		distruge();
}