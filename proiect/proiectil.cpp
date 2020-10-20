#include "proiectil.h"

proiectil::proiectil() {
	x = 0;
	y = 0;
	rotatie = 0;
	viteza = 0;
	viteza_maxima = 0;
	spriteID = 0;
	stare = 0;
	lifetime = 0;
	damage = 0;
	lungime = 0;
	latime = 0;
}

/*-----------------*/

proiectil::proiectil(int p_x, int p_y, int p_rot, int p_sprite_id) : obiect(p_x, p_y, p_rot, p_sprite_id)
{
	switch (spriteID)
	{
	case id_imagine::mm127:
	{
		viteza = viteza_maxima = 35;
		lifetime = 10;
		damage = 7;
		latime = 3;
		lungime = 7;
		break;
	}

	case id_imagine::mm200:
	{
		viteza = viteza_maxima = 20;
		lifetime = 25;
		damage = 10;
		latime = 4;
		lungime = 10;
		break;
	}

	case id_imagine::racheta:
	{
		viteza = 7;
		viteza_maxima = 14;
		lifetime = 65;
		damage = 85;
		latime = 4;
		lungime = 20;
		break;
	}
	}
	stare = 0;
}

/*-----------------*/

void proiectil::update()
{
	deplasare();
	if (!lifetime)
		deactiveaza();
	--lifetime;
}

/*-----------------*/

void proiectil::set(int p_x, int p_y, int p_rot, int p_sprite_id)
{
	x = p_x;
	y = p_y;
	rotatie = p_rot;
	spriteID = p_sprite_id;
	switch (spriteID)
	{
	case id_imagine::mm127:
	{
		viteza = viteza_maxima = 35;
		lifetime = 10;
		damage = 7;
		latime = 3;
		lungime = 7;
		break;
	}

	case id_imagine::mm200:
	{
		viteza = viteza_maxima = 20;
		lifetime = 25;
		damage = 20;
		latime = 4;
		lungime = 10;
		break;
	}

	case id_imagine::racheta:
	{
		viteza = 5;
		viteza_maxima = 14;
		lifetime = 65;
		damage = 130;
		latime = 6;
		lungime = 26;
		break;
	}
	}
	stare = 0;
}