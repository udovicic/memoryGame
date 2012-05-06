/*
 *	game.h
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
#ifndef H_GAME
#define H_GAME


#ifdef DEBUG
#define dout cout << "DEBUG: "
#else
#define dout 0 && cout
#endif

#include "SDL/SDL.h"

class game {

private:
// internal game logic helpers
	// game filed [position][state/tile]
	int flower_field[12][2],
		clicks,clicked[2], // used for mouse clicks
		remaining;

// SDL stuff
	SDL_Surface *disp, *back, *background,
				*tiles[12];
	SDL_Rect blit_rect;
	SDL_Event ev;

public:
	~game();
	bool loadResources(); // load images
	bool initGame(); // init display and variables
	bool redraw(); // draw on surface and flip buffer
	bool grabInput(); //grab input and perform click tasks; false on user quit
	bool doLogic(); // check win condition
};

#endif