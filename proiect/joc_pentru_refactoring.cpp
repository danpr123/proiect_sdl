#include "joc.h"

/*-----------------
daca o sa mai lucrez la proiectul asta, o sa trebuiasca sa fac refactoring la clasa joc,
insa deocamdata celelalte module (main menu, renderer, AI) sunt foarte simpliste si sunt incluse aici
*/


// menu-ul din care selectezi nava
int joc::selectie_nava()
{
	// incarca textura pentru sageata de selectie
	SDL_SetTextureColorMod(textura[id_imagine::sageata_selectie], 0, 0, 0);

	// seteaza selectia default pentru avioane
	int selectie = id_imagine::iar80;

	// glowul pentru sagetile de selectie; dezactivat by default
	int glowS = 0, glowD = 0;

	// delay input 333ms la apasarea tastelor sa poti selecta avionul
	int delay_input = 0;

	// same purpose as above, doar ca inregistreaza o singura apasare pe tasta
	int k_record = 0;

	// loop-ul menu-ului
	while (true)
	{
		if (delay_input > 0)
			delay_input--;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, textura[id_imagine::background], NULL, NULL);
		SDL_RenderCopy(renderer, textura[id_imagine::intro], NULL, NULL);
		deseneaza_sageti(glowS, glowD);		// deseneaza sagetile, cu sau fara glow
		deseneaza_nave_selectie(selectie);	// deseneaza nava selectata
		SDL_RenderCopy(renderer, textura[id_imagine::bg_border], NULL, NULL);
		SDL_RenderPresent(renderer);
		limitare_framerate();

		// daca ai apasat tasta A
		if (stare[tasta::A] && !delay_input)
		{
			delay_input = 20;					// ai delay input 20frames (333ms)
			glowS = 10;							// glow la sageata stanga 10farmes (167ms)
			if (selectie == id_imagine::ju87)	// daca avionul curent are cel mai mare id, revenim la avion cu id=0
				selectie = id_imagine::iar80;
			else
				selectie += 1;
		}

		// daca tasta D nu era apasata inainte
		if (!k_record)
		{
			// si o apesi acum
			if (stare[tasta::D])
			{
				glowD = 10;	// glow sageata dreapta

				// schimba avionul selectat
				if (selectie == id_imagine::iar80)
					selectie = id_imagine::ju87;
				else
					selectie -= 1;

				// si seteaza flag ca tasta D este apasata
				k_record = 1;
			}
		}
		// altfel, daca tasta D nu era apasata inainte
		else
			// si acum nu e apasata
			if (!stare[tasta::D])
				// resetam flag-ul k_record
				k_record = 0;

		// apasand tasta ENTER selectam nava
		if (stare[tasta::ENTER])
			return selectie;

		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			exit(1);
	}
}

/*-----------------
deseneaza sagetile de selectie a navelor, si glow-ul cand sunt activate
*/

void joc::deseneaza_sageti(int& glow1, int& glow2)
{
	SDL_Rect coord;
	coord.h = 244;
	coord.w = 201;
	coord.x = 200;
	coord.y = 264;
	if (glow1)
	{
		glow1--;
		int marire = rand() % 20;
		coord.h += marire;
		coord.w += marire;
		coord.x -= marire / 2;
		coord.y -= marire / 2;
		SDL_SetTextureColorMod(textura[id_imagine::sageata_selectie], rand() % 50 + 105, 0, 0);
		SDL_RenderCopy(renderer, textura[id_imagine::sageata_selectie], NULL, &coord);
		SDL_SetTextureColorMod(textura[id_imagine::sageata_selectie], 0, 0, 0);
		coord.h = 244;
		coord.w = 201;
		coord.x = 200;
		coord.y = 264;
	}
	else
		SDL_RenderCopy(renderer, textura[id_imagine::sageata_selectie], NULL, &coord);

	coord.x += 610;
	if (glow2)
	{
		glow2--;
		int marire = rand() % 20;
		coord.h += marire;
		coord.w += marire;
		coord.x -= marire / 2;
		coord.y -= marire / 2;
		SDL_SetTextureColorMod(textura[id_imagine::sageata_selectie], rand() % 50 + 105, 0, 0);
		SDL_RenderCopyEx(renderer, textura[id_imagine::sageata_selectie], 0, &coord, NULL, NULL, SDL_FLIP_HORIZONTAL);
		SDL_SetTextureColorMod(textura[id_imagine::sageata_selectie], 0, 0, 0);
	}
	else
		SDL_RenderCopyEx(renderer, textura[id_imagine::sageata_selectie], 0, &coord, NULL, NULL, SDL_FLIP_HORIZONTAL);

}

/*-----------------
deseneaza nava selectat pentru menu
*/

void joc::deseneaza_nave_selectie(int selectie)
{
	SDL_Rect coord;
	coord.h = 305;
	coord.w = 410;
	coord.x = 400;
	coord.y = 210;
	SDL_RenderCopy(renderer, textura[selectie], NULL, &coord);
}

/*-----------------
urmatoarele metode sunt apelate de joc::randare() si folosite in bucla principala
*/

void joc::draw_nave()
{
	SDL_Rect coordonate;
	SDL_Point centru;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	for (int i = 0; i < playeri_activi; i++)
	{
		// daca mai e in joc
		if (player[i]->get_stare())
		{
			// daca e in viata
			if (player[i]->get_stare() == 200)
			{
				coordonate.h = player[i]->get_lungime();
				coordonate.w = player[i]->get_latime();
				coordonate.x = player[i]->getx();
				coordonate.y = player[i]->gety();
				centru.x = player[i]->get_latime() / 2;
				centru.y = player[i]->get_lungime() / 2;
				SDL_RenderCopyEx(renderer, textura[player[i]->get_id()], 0, &coordonate, player[i]->get_rotatie(), &centru, flip);
			}

			// daca se prabuseste dar nu este inca distrusa, se micsoreaza invers proportional cu valoarea starii + explozie
			else
			{
				float dimensiune = player[i]->get_stare() / 200.0;
				if (dimensiune < 0.45f)
					dimensiune = 0.45f;
				coordonate.h = player[i]->get_lungime() * dimensiune;
				coordonate.w = player[i]->get_latime() * dimensiune;
				coordonate.x = player[i]->getx() + (player[i]->get_lungime() - coordonate.h) / 2;
				coordonate.y = player[i]->gety() + (player[i]->get_latime() - coordonate.w) / 2;
				centru.x = coordonate.h / 2;
				centru.y = coordonate.w / 2;
				SDL_RenderCopyEx(renderer, textura[player[i]->get_id()], 0, &coordonate, player[i]->get_rotatie(), &centru, flip);
				int status = player[i]->get_stare();
				if (status > 180)
					dimensiune = 0.35f;
				else
					if (status > 170)
					{
						dimensiune = 0.35f;
						if (status % 2)
							dimensiune += 0.15 * (180 - status);
					}
					else
						if (status > 50)
						{
							dimensiune = 0.65 + (rand() % 51 - 25) / 100.0;
						}
						else
						{
							dimensiune = 0.55 * status / 50 + (rand() % 51 - 25) / 100.0;
						}
				coordonate.h = player[i]->get_lungime() * dimensiune;
				coordonate.w = player[i]->get_latime() * dimensiune;
				coordonate.x = player[i]->getx() + (player[i]->get_lungime() - coordonate.h) / 2 + rand() % 7 - 3;
				coordonate.y = player[i]->gety() + (player[i]->get_latime() - coordonate.w) / 2 + rand() % 7 - 3;
				centru.x = coordonate.h / 2;
				centru.y = coordonate.w / 2;
				SDL_RenderCopyEx(renderer, textura[id_imagine::explozie], 0, &coordonate, player[i]->get_rotatie(), &centru, flip);
			}
		}
	}
}

/*-----------------*/

void joc::draw_proiectile()
{
	SDL_Rect coordonate;
	SDL_Point centru;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	for (int i = 0; i < max_proiectile; i++)
		if (proiectile[i].get_stare())
		{
			flip = rand() % 2 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL;
			coordonate.h = proiectile[i].get_lungime();
			coordonate.w = proiectile[i].get_latime();
			coordonate.x = proiectile[i].getx();
			coordonate.y = proiectile[i].gety();
			centru.x = proiectile[i].get_latime() / 2;
			centru.y = proiectile[i].get_lungime() / 2;

			// unele proiectile au mai multe sprite-uri, asa ca alegem cu rand() pe care sa-l folosim
			int versiune = proiectile[i].get_id();
			if (versiune == id_imagine::mm127 || versiune == id_imagine::mm200)
				versiune += rand() % 3;

			SDL_RenderCopyEx(renderer, textura[versiune], 0, &coordonate, proiectile[i].get_rotatie(), &centru, flip);
		}
}

/*-----------------
deseneaza statusul munitiei
cu cat ai mai putina munitie, cu atat glontele din dreapta jos o sa fie mai putin colorat
*/

void joc::draw_gui()
{
	SDL_Rect coordonate;
	SDL_Rect crop;
	if (player[0]->arma1->id == id_imagine::mm127)
	{
		coordonate.h = 70;
		coordonate.w = 40;
		coordonate.x = width - 225 - coordonate.w;
		coordonate.y = height - 19 - coordonate.h;
		SDL_RenderCopy(renderer, textura[id_imagine::mm127_gui_bg], NULL, &coordonate);
		crop.h = 70 * player[0]->arma1->munitie / 250.0;
		crop.w = 40;
		crop.x = 0;
		crop.y = 0;
		coordonate.h = crop.h;
		SDL_RenderCopy(renderer, textura[id_imagine::mm127_gui_status], &crop, &coordonate);
	}

	if (player[0]->arma1->id == id_imagine::mm200 || player[0]->arma2->id == id_imagine::mm200)
	{
		coordonate.h = 100;
		coordonate.w = 40;
		coordonate.x = width - 175 - coordonate.w;
		coordonate.y = height - 15 - coordonate.h;
		SDL_RenderCopy(renderer, textura[id_imagine::mm200_gui_bg], NULL, &coordonate);
		if (player[0]->arma1->id == id_imagine::mm200)
			crop.h = 100 * player[0]->arma1->munitie / 100.0;
		else
			if (player[0]->arma2->id == id_imagine::mm200)
				crop.h = 100 * player[0]->arma2->munitie / 100.0;
		crop.w = 40;
		crop.x = 0;
		crop.y = 0;
		coordonate.h = crop.h;
		SDL_RenderCopy(renderer, textura[id_imagine::mm200_gui_status], &crop, &coordonate);
	}
}

/*-----------------
returneaza daca un player a iesit de pe ecran sau nu
*/

bool joc::inside_box(int p_playerid)
{
	int x = player[p_playerid]->getx();
	int y = player[p_playerid]->gety();
	return x > -80 && x < width&& y > -80 && y < height;
}

/*-----------------
functia care se ocupa de navigatia AI-ului
*/

bool joc::AI_directie(int curent) {
	// flag spune daca trebuie schimbata directia sau nu
	bool flag = false;
	int unghi = player[curent]->get_rotatie();
	int x = player[curent]->getx();
	int y = player[curent]->gety();

	// daca e in partea stanga a hartii
	if (x < 300)
	{
		if (unghi < 5 || unghi > 270)
		{
			player[curent]->viraj_dreapta();
			flag = true;
		}
		else
			if (unghi <= 270 && unghi > 175)
			{
				player[curent]->viraj_stanga();
				flag = true;
			}
		// daca e aproape de marginea de sus si merge prost
		if (y < 300 && unghi >= 5 && unghi < 95)
		{
			player[curent]->viraj_dreapta();
			flag = true;
		}
		else	//daca e aproape de marginea de jos si merge prost
			if (y > height - 300 && unghi <= 175 && unghi > 85)
			{
				player[curent]->viraj_stanga();
				flag = true;
			}
		return flag;
	}

	// daca e in partea dreapta a hartii
	if (x > width - 300)
	{
		if (unghi < 185 && unghi > 90)
		{
			player[curent]->viraj_dreapta();
			flag = true;
		}
		else
			if (unghi <= 90 || unghi > 355)
			{
				player[curent]->viraj_stanga();
				flag = true;
			}
		// daca e aproape de marginea de sus si merge prost
		if (y < 300 && unghi <= 355 && unghi > 265)
		{
			player[curent]->viraj_stanga();
			flag = true;
		}
		else	//daca e aproape de marginea de jos si merge prost
			if (y > height - 300 && unghi >= 185 && unghi < 275)
			{
				player[curent]->viraj_dreapta();
				flag = true;
			}
		return flag;
	}

	// daca e sus
	if (y < 300)
		if (unghi < 360 && unghi > 265)
		{
			player[curent]->viraj_stanga();
			return true;
		}
		else
			if (unghi >= 0 && unghi < 95)
			{
				player[curent]->viraj_dreapta();
				return true;
			}

	// daca e jos
	if (y > height - 300)
		if (unghi > 180 && unghi < 275)
		{
			player[curent]->viraj_dreapta();
			return true;
		}
		else
			if (unghi <= 180 && unghi > 85)
			{
				player[curent]->viraj_stanga();
				return true;
			}
	return false;
}

/*-----------------*/

punct joc::intersectie_segmente(punct A, punct B, punct C, punct D)
{
	punct rez;
	double a1 = B.y - A.y;
	double b1 = A.x - B.x;
	double c1 = a1 * (A.x) + b1 * (A.y);

	double a2 = D.y - C.y;
	double b2 = C.x - D.x;
	double c2 = a2 * (C.x) + b2 * (C.y);

	double determinant = a1 * b2 - a2 * b1;

	if (!determinant)
	{
		rez.x = -1;
		rez.y = -1;
	}
	else
	{
		rez.x = (b2 * c1 - b1 * c2) / determinant;
		rez.y = (a1 * c2 - a2 * c1) / determinant;
		int minAB = min(A.x, B.x);
		int minCD = min(C.x, D.x);
		int maxAB = max(A.x, B.x);
		int maxCD = max(C.x, D.x);
		if (rez.x > maxAB || rez.x > maxCD || rez.x < minAB || rez.x < minCD || rez.y > maxAB || rez.y >maxCD || rez.y < minAB || rez.y < minCD)
		{
			rez.x = -1;
			rez.y = -1;
		}
	}
	return rez;
}

/*-----------------*/

bool joc::coliziune_AABB(int p_idproiectil, int p_idnava)
{
	SDL_Rect avion;
	avion.x = player[p_idnava]->getx();
	avion.y = player[p_idnava]->gety();
	avion.w = player[p_idnava]->get_latime();
	avion.h = player[p_idnava]->get_lungime();
	int x1 = proiectile[p_idproiectil].getx();
	int y1 = proiectile[p_idproiectil].gety();
	punct A = proiectile[p_idproiectil].deplasare_return(proiectile[p_idproiectil].get_viteza());
	int x2 = A.x, y2 = A.y;
	return SDL_IntersectRectAndLine(&avion, &x1, &y1, &x2, &y2);
}

/*-----------------
am schimbat structura de date pentru proiectile pentru ca lifetime-ul lor e variabil
acum e doar un array care tine numarul maxim posibil de proiectile; fiecare proiectil poate fi activ sau inactiv
functia cauta primul proiectil inactiv din array, il activeaza si ii da valori in functie de tip
*/
void joc::creaza_proiectil(int p_playerid, int p_slotarma) {
	nava* curent = player[p_playerid];	// playerul care trage
	int i = 0;

	while (proiectile[i].get_stare())
		i++;
	int v_deviatie, v_unghi_deviatie, armaid;
	if (p_slotarma == 1)
	{
		v_deviatie = curent->arma1->spread;
		v_deviatie = rand() % v_deviatie - v_deviatie / 2;
		v_unghi_deviatie = curent->arma1->spread_angle;
		v_unghi_deviatie = v_unghi_deviatie - v_unghi_deviatie / 2;
		armaid = curent->arma1->id;
	}
	else
	{
		armaid = curent->arma2->id;
		if (armaid == id_imagine::racheta)
		{
			v_deviatie = (rand() % 2 ? -1 : 1) * curent->get_latime() * 0.33;
			v_unghi_deviatie = 0;
		}
		else
		{
			v_deviatie = curent->arma2->spread;
			v_deviatie = rand() % v_deviatie - v_deviatie / 2;
			v_unghi_deviatie = curent->arma2->spread_angle;
			v_unghi_deviatie = v_unghi_deviatie - v_unghi_deviatie / 2;
		}
	}

	// mutam proiectilul in centrul navei, cu deviatie in functie de spread
	int x = curent->getx() + curent->get_latime() / 2;
	int y = curent->gety() + curent->get_lungime() / 2 + v_deviatie;
	int rotatie = curent->get_rotatie() + v_unghi_deviatie;
	proiectile[i].set(x, y, rotatie, armaid);
	proiectile[i].activeaza();

	// miscam proiectilul in fata navei
	proiectile[i].deplasare(curent->get_lungime() + 5);
}