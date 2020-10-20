#include "obiect.h"

//definitie in utile.h
static const double t_90grade = grade(90);

/*-----------------*/

void obiect::deplasare()
{
	x += viteza * static_cast<float>(cos(grade(rotatie) - t_90grade));
	y += viteza * static_cast<float>(sin(grade(rotatie) - t_90grade));
	if (viteza < viteza_maxima)
	{
		viteza += viteza_maxima / 16;
		if (viteza > viteza_maxima)
			viteza = viteza_maxima;
	}
}

/*-----------------*/

void obiect::deplasare(int p_viteza) {
	x += p_viteza * static_cast<float>(cos(grade(rotatie) - t_90grade));
	y += p_viteza * static_cast<float>(sin(grade(rotatie) - t_90grade));
}

/*-----------------*/

punct obiect::deplasare_return(int distanta)
{
	punct A;
	A.x = static_cast<int>(x + distanta * static_cast<float>(cos(grade(rotatie) - t_90grade)));
	A.y = static_cast<int>(y + distanta * static_cast<float>(sin(grade(rotatie) - t_90grade)));
	return A;

}

/*-----------------*/

int obiect::getx()
{
	return static_cast<int>(x);
}

/*-----------------*/

int obiect::gety()
{
	return static_cast<int>(y);
}

/*-----------------*/

int obiect::get_id() {
	return spriteID;
}

/*-----------------*/

int obiect::get_rotatie() {
	return static_cast<int>(rotatie);
}

float obiect::get_viteza() {
	return viteza;
}

/*-----------------*/

int obiect::get_stare() {
	return stare;
}

/*-----------------*/

obiect::obiect(int p_x, int p_y, int p_rot, int p_sprite_id) {
	x = static_cast<float>(p_x);
	y = static_cast<float>(p_y);
	rotatie = static_cast<float>(p_rot);
	spriteID = p_sprite_id;
	stare = 0;
	viteza = 0;
	viteza_maxima = 0;
}