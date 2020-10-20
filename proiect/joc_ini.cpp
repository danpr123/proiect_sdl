#include"joc.h"


void joc::ini()
{
	if (!creare_fereastra())
		return;
	if (!incarca_resurse())					// load sprites si audio
		return;
	srand(SDL_GetTicks());					// setare seed pentru random
	ultimul_frame = SDL_GetTicks();			// setare framerate
	stare = SDL_GetKeyboardState(NULL);		// setare taste
	int sel_nava_player = selectie_nava();	// menu-ul din care player-ul alege nava
	setup_playeri(sel_nava_player);			// creaza si plaseaza jucatorii
	loop();									// bucla principala
}

/*-----------------*/

bool joc::creare_fereastra()
{
	bool stare = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("err ini SDL: %s\n", SDL_GetError());
		stare = false;
	}

	fereastra = SDL_CreateWindow("Peunu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (fereastra == NULL)
	{
		printf("err creare fereastra: %s\n", SDL_GetError());
		stare = false;
	}

	renderer = SDL_CreateRenderer(fereastra, -1, 0);
	if (renderer == NULL)
	{
		printf("err creare renderer: %s\n", SDL_GetError());
		stare = false;
	}

	return stare;
}

/*-----------------*/

bool joc::incarca_resurse()
{
	bool stare = true;

	/*
	incarca imaginile sa poata crea sprites
	posibil sa inlocuiesc asta cu un manager de resurse ulterior
	*/
	SDL_Surface* imagine[id_imagine::numar_imagini] = { NULL };
	imagine[id_imagine::iar80] = SDL_LoadBMP("resource/iar80.bmp");
	imagine[id_imagine::la5] = SDL_LoadBMP("resource/la5.bmp");
	imagine[id_imagine::me109] = SDL_LoadBMP("resource/me109.bmp");
	imagine[id_imagine::il2] = SDL_LoadBMP("resource/il2.bmp");
	imagine[id_imagine::ju87] = SDL_LoadBMP("resource/ju87.bmp");
	imagine[id_imagine::mm127] = SDL_LoadBMP("resource/mm127v1.bmp");
	imagine[id_imagine::mm127v2] = SDL_LoadBMP("resource/mm127v2.bmp");
	imagine[id_imagine::mm127v3] = SDL_LoadBMP("resource/mm127v3.bmp");
	imagine[id_imagine::mm200] = SDL_LoadBMP("resource/mm200v1.bmp");
	imagine[id_imagine::mm200v2] = SDL_LoadBMP("resource/mm200v2.bmp");
	imagine[id_imagine::mm200v3] = SDL_LoadBMP("resource/mm200v3.bmp");
	imagine[id_imagine::racheta] = SDL_LoadBMP("resource/racheta.bmp");
	imagine[id_imagine::background] = SDL_LoadBMP("resource/bg_temp.bmp");
	imagine[id_imagine::bg_border] = SDL_LoadBMP("resource/bg_temp_border.bmp");
	imagine[id_imagine::sageata_selectie] = SDL_LoadBMP("resource/sageata_selectie.bmp");
	imagine[id_imagine::intro] = SDL_LoadBMP("resource/intro.bmp");
	imagine[id_imagine::fum] = SDL_LoadBMP("resource/fum.bmp");
	imagine[id_imagine::explozie] = SDL_LoadBMP("resource/explozie.bmp");
	imagine[id_imagine::mm127_gui_bg] = SDL_LoadBMP("resource/mm127_gui_bg.bmp");
	imagine[id_imagine::mm127_gui_status] = SDL_LoadBMP("resource/mm127_gui_status.bmp");
	imagine[id_imagine::mm200_gui_bg] = SDL_LoadBMP("resource/mm200_gui_bg.bmp");
	imagine[id_imagine::mm200_gui_status] = SDL_LoadBMP("resource/mm200_gui_status.bmp");
	for (int i = id_imagine::iar80; i < id_imagine::numar_imagini; i++)
	{
		if (!imagine[i])
		{
			printf("err incarcare imagine %d: %s\n", i, SDL_GetError());
			stare = false;
			break;
		}

		textura[i] = SDL_CreateTextureFromSurface(renderer, imagine[i]);
		if (!textura[i])
		{
			printf("err incarcare textura %d: %s\n", i, SDL_GetError());
			stare = false;
			break;
		}

		// nu mai e nevoie de surface dupa ce e creata textura
		SDL_FreeSurface(imagine[i]);
	}
	return stare;
}

/*-----------------*/

// plaseaza playerii pe harta si le aloca avioane
void joc::setup_playeri(int nava_player)
{
	playeri_activi = 4;

	// caracteristicile navelor ce vor fi create: coordonate, rotatie, tipul avionului
	int x = 251, y = 0;
	int rotatie = 90;
	int id_tip_nava;

	// prima data o sa fie creat playerul, apoi ceilalti 
	id_tip_nava = nava_player;
	for (int i = 0; i < playeri_activi; i++)
	{
		// pune player-ul curent in vectorul de playeri
		player.push_back(new nava(x, y, rotatie, id_tip_nava));

		// creaza caracteristicile navei urmatoare
		x += rand() % 50;
		y += 100 + rand() % 100;
		rotatie = rand() % 90 - 45;
		id_tip_nava = rand() % 2;
	}

	/*
	se pare ca structura cu proiectile e mai complicata decat credeam initial
	pentru ca nu toate proiectilele au acelasi lifetime.
	deocamdata o sa fie un array cu numarul maxim de proiectile posibile.
	*/
	max_proiectile = 26;
	proiectile = new proiectil[max_proiectile];
	for (int i = 0; i < max_proiectile; i++)
		proiectile[i].deactiveaza();
}

/*-----------------*/

joc::~joc() {
	for (int i = 0; i < id_imagine::numar_imagini; i++)
	{
		SDL_DestroyTexture(textura[i]);
	}
	delete[] proiectile;

	for (int i = 0; i < player.size(); i++)
		delete player[i];

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(fereastra);
	SDL_Quit();
}