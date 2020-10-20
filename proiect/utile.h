#pragma once
#include <SDL.h>
#include <cmath>
#include <algorithm>
#include <string>

#define _USE_MATH_DEFINES
#define grade(unghi) ((unghi) * M_PI / 180.0)

namespace tasta {
	enum {
		A = SDL_SCANCODE_A,
		S = SDL_SCANCODE_S,
		D = SDL_SCANCODE_D,
		W = SDL_SCANCODE_W,
		Q = SDL_SCANCODE_Q,
		E = SDL_SCANCODE_E,
		M = SDL_SCANCODE_M,
		SPACE = SDL_SCANCODE_SPACE,
		ESC = SDL_SCANCODE_ESCAPE,
		ENTER = SDL_SCANCODE_RETURN
	};
}

namespace id_imagine {
	const int numar_imagini = 22;
	enum sprites
	{
		iar80, la5, me109, il2, ju87,			// done
		mm127, mm127v2, mm127v3,				// done
		mm200, mm200v2, mm200v3,				// done
		racheta,								// done
		background, bg_border,					// done
		sageata_selectie, intro,				// done
		mm127_gui_bg, mm127_gui_status,			// done
		mm200_gui_bg, mm200_gui_status,			// done
		fum, explozie, debris,					// 
		la5_dmg, me109_dmg, il2_dmg, ju87_dmg	//
	};
}

struct punct {
	int x, y;
};

template <typename T>
T min(T x, T y)
{
	if (x < y)
		return x;
	else
		return y;
}

template <typename T>
T max(T x, T y)
{
	if (x > y)
		return x;
	else
		return y;
}