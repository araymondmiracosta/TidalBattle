/*-----------------------------------------------------------------------------
  Program Name: Tidal Battle
  Programmer Name: Alastair Raymond
  Program Description/Purpose: Naval strategy war game
  Date: 29 November 2022
-----------------------------------------------------------------------------*/

#include <iostream>
#include "Game.h"

using namespace std;

/*-----------------------------------------------------------------------------
  Function Name: main
  Function Return Type: int
  Parameters: None
  Function Description/Purpose: Initialize new Game object, start game
-----------------------------------------------------------------------------*/
int main() {
	Game newGame;
	newGame.run();
	return 0;
}
