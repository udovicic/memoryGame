/*
 *	game.cpp
 * 
 *	Copyright 2012 Stjepan Udovičić <udovicic.stjepan@gmail.com>
 *	      
 *	This file is part of memoryGame.
 *
 *	memoryGame is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	memoryGame is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with memoryGame.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <time.h>
#include <iostream>
#include "game.h"

game::~game() {
// free all alocated surfaces
	if (disp != NULL) SDL_FreeSurface(disp);
	if (back != NULL) SDL_FreeSurface(back);
	if (background != NULL) SDL_FreeSurface(background);
	for (int i=0; i<12; i++)
		if (tiles[i] != NULL) SDL_FreeSurface(tiles[i]);
}

bool game::initGame() {
	int i;

	pauseDelay = 3000; // pause 3s
	clicks = 0; // click counter 

// initial game field
	for (i=0; i<12; i++) {
		flower_field[i][0] = 0; // state = 0
		flower_field[i][1] = i; // fill with all tiles
	}

//randomize tiles order
	srand(time(NULL));
	for (i=0; i<11; i++) {
		int r = i + (rand() % (12-i)); // Random remaining position.
		int temp = flower_field[r][1];
		flower_field[r][1] = flower_field[i][1];
		flower_field[i][1] = temp;
	}

// set rect
	blit_rect.x = 0;
	blit_rect.y = 0;
	blit_rect.w = 200;
	blit_rect.h = 200;

// set SDL_Surface pointers to NULL
	disp = NULL;
	back = NULL;
	background = NULL;
	for (i=0; i<12; i++) tiles[i] = NULL;

	disp = SDL_SetVideoMode(800,600,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (disp == NULL) return false;

	return true;
}

bool game::loadResources() {

	background = IMG_Load("img/background.jpg");
	if (background == NULL) return false;

	back = IMG_Load("img/back.jpg");
	if ( back == NULL) return false;

	tiles[0] = IMG_Load("img/a1.jpg");
	tiles[1] = IMG_Load("img/a2.jpg");
	tiles[2] = IMG_Load("img/b1.jpg");
	tiles[3] = IMG_Load("img/b2.jpg");
	tiles[4] = IMG_Load("img/c1.jpg");
	tiles[5] = IMG_Load("img/c2.jpg");
	tiles[6] = IMG_Load("img/d1.jpg");
	tiles[7] = IMG_Load("img/d2.jpg");
	tiles[8] = IMG_Load("img/e1.jpg");
	tiles[9] = IMG_Load("img/e2.jpg");
	tiles[10] = IMG_Load("img/f1.jpg");
	tiles[11] = IMG_Load("img/f2.jpg");
	for (int i=0; i<12; i++) if (tiles[i] == NULL) return false;

	return true;
}

bool game::redraw() {
	int i,j;
	SDL_Surface * temp;

// paint background
	SDL_BlitSurface(background, NULL, disp, NULL);

// paint tiles
	for (i=0; i<4; i++) for (j=0; j<3; j++) {
		// set position
		blit_rect.x = 200 * i;
		blit_rect.y = 200 * j;

		// what should be displayed?
		if (flower_field[i+j*4][0] == 0) { // back
			temp = back;
		} else if (flower_field[i+j*4][0] == 1) { // front
			temp = tiles[flower_field[i+j*4][1]];
		} else { // nothing
			temp = NULL;
		}

		// finally pait to surface
		if (temp != NULL) {
			SDL_BlitSurface(temp, NULL, disp, &blit_rect);
		}
	}

//finally flip buffer
	SDL_Flip(disp);

	return true;
}

bool game::doLogic() {
	bool alive;
	int i;

// check for match
	if (clicks == 3) {
		int a,b;
			a = flower_field[clicked[0]][1];
			b = flower_field[clicked[1]][1];
		if ((a%2 == 1 && b == a - 1) || (a%2 == 0 && b == a + 1)) {
				flower_field[clicked[0]][0] = 2;
				flower_field[clicked[1]][0] = 2;
				clicks = 4;
		}
	}
// check win condition
	alive = false;
	for (i=0; i<12; i++)
		if (flower_field[i][0] != 2) alive = true;

	return alive;
}

bool game::grabInput() {
	if (clicks == 2) {
		oldTime = SDL_GetTicks(); // start counting ticks
		clicks = 3;
	} else if (clicks == 3 or clicks == 4) {
		if (oldTime + pauseDelay > SDL_GetTicks() && clicks != 4) { // game is blocking input
			while (SDL_PollEvent(&ev)) {  // remove pending events except quit call
				switch (ev.type) {
				case SDL_QUIT:
					return false;
					break;
				case SDL_KEYDOWN:
					if (ev.key.keysym.sym == SDLK_ESCAPE)
					return false;
					break;
				}
			}
			return true;
		}

		// unpause action
		if (flower_field[clicked[0]][0] == 1) {
			flower_field[clicked[0]][0] = 0;
			flower_field[clicked[1]][0] = 0;
		}
		clicks = 0;	// reset click counter
	}

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			return false;
			break;
		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				return false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.button == SDL_BUTTON_LEFT && clicks < 2) {
				int x,y;
				x = (int)ev.button.x/200;
				y = (int)ev.button.y/200;

				if (flower_field[x+4*y][0] == 0) { // check if already clicked or opened
					flower_field[x+4*y][0]=1;
					clicked[clicks] = x+4*y;
					clicks++;
				}
			}
			break;
		}
	}

	return true;
}

void game::controlCpuUsage() {
	SDL_Delay(100);
	return;
}