#include "joc.h"
#include<windows.h>
#include<sysinfoapi.h>
#include<WinUser.h>

#pragma warning (disable : 4083 4838 4244)


void joc::loop() {
	while (true)
	{
		evaluare_player_input();
		evaluare_AI();
		update();
		randare();
		//SDL_Delay(1000/60);				// o sa fie inlocuit de vsync
		limitare_framerate();				// vsync v1.0 
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT || !player[0]->get_stare())
			return;
	}
}

/*-----------------*/


void joc::evaluare_player_input()
{
	if (stare[tasta::D] && !stare[tasta::A])
		player[0]->viraj_dreapta();
	if (stare[tasta::A] && !stare[tasta::D])
		player[0]->viraj_stanga();
	if (stare[tasta::SPACE])	// foloseste arma 1
	{
		if (player[0]->arma1->trage())
			creaza_proiectil(0, 1);
	}
	if (stare[tasta::S])		// foloseste arma 2
	{
		if (player[0]->arma2->trage())
			creaza_proiectil(0, 2);
	}
	if (stare[tasta::ESC])
		player[0]->distruge();
}

/*-----------------*/

void joc::evaluare_AI()
{
	for (int i = 1; i < playeri_activi; i++)
	{
		int status = player[i]->get_stare();
		if (status == 200)
		{
			bool set_directie = AI_directie(i);
		}
	}
}

/*-----------------*/

void joc::update()
{
	//updateaza playerii
	for (int i = 0; i < playeri_activi; i++)
		// daca mai e in joc
		if (player[i]->get_stare() > 0)
		{
			player[i]->update();

			// daca iese de pe harta, atunci e distrus
			if (!inside_box(i) && player[i]->get_stare() == 200)
				player[i]->distruge();
		}

	/*
	facem collision check inainte de update la proiectile pentru ca check-ul se face intre
	segmentul definit de poz curenta si urmatoare a proiectilului si AABB-ul navei
	*/
	evaluare_coliziuni();

	for (int i = 0; i < max_proiectile; i++)
		if (proiectile[i].get_stare())
			proiectile[i].update();
}

/*-----------------*/

void joc::evaluare_coliziuni()
{
	/*
	pana la urma am pus toate proiectilele intr-un singur array, prin care iteram
	si daca sunt active, verificam coliziunele cu fiecare player/nava
	*/
	for (int i = 0; i < max_proiectile; i++)
		if (proiectile[i].get_stare())
			for (int j = 0; j < playeri_activi; j++)
				if (coliziune_AABB(i, j))
				{
					player[j]->ia_damage(proiectile[i].get_damage(), 0);
					proiectile[i].deactiveaza();
				}
}

/*-----------------*/

void joc::randare()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, textura[id_imagine::background], NULL, NULL);
	draw_nave();
	draw_proiectile();
	draw_gui();
	SDL_RenderCopy(renderer, textura[id_imagine::bg_border], NULL, NULL);
	SDL_RenderPresent(renderer);
}

/*-----------------*/

void joc::limitare_framerate() {
	// framerate = 60fps, deci asteaptam sa treaca ~16.7ms de la ultimul frame
	while (SDL_GetTicks() - ultimul_frame < 1000 / 60)
		;
	ultimul_frame = SDL_GetTicks();
}