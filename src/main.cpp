/*
 *	main.cpp
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

#include <iostream>
#include "game.h"

using namespace std;

int main(int argc, char * argv[]) {

	game *gm = new game();

	cout << "memoryGame is free software: you can redistribute it and/or modify" <<
			"it under the terms of the GNU General Public License as published by" <<
			"the Free Software Foundation, either version 3 of the License, or" <<
			"(at your option) any later version.\n" <<
			"Source code available at https://github.com/udovicic\n\n";

	cout << "Initializing....";
	if (gm->initGame()) {
		cout << "DONE" << endl;
	} else {
		cout << "FAILED" << endl;
		delete gm;
		return 1;
	}

	cout << "Loading resources...";
	if (gm->loadResources()) {
		cout << "DONE" << endl;
	} else {
		cout << "FAILED" << endl;
		delete gm;
		return 1;
	}

	cout << "Running game :)" << endl;

	while (gm->doLogic()) {
		if (!gm->redraw()) {
			cerr << "Unable to draw on surface" << endl;
			break;
		} 
		if (!gm->grabInput()) {
			break;
		}
	}

	cerr << "Bye!" << endl;
	delete gm;

	return 0;
}