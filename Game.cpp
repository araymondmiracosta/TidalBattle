/*-----------------------------------------------------------------------------
  Program Name: Tidal Battle
  Programmer Name: Alastair Raymond
  Program Description/Purpose: Naval strategy war game
  Date: 29 November 2022
-----------------------------------------------------------------------------*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Game.h"

using namespace std;

/*-----------------------------------------------------------------------------
  Function Name: Game
  Function Return Type: None
  Parameters: None
  Function Description/Purpose: Primary constructor, initializes random seed
-----------------------------------------------------------------------------*/
Game::Game() {
	debug = false;
	srand(time(NULL));
}

/*-----------------------------------------------------------------------------
  Function Name: Game
  Function Return Type: None
  Parameters: int debug
  Function Description/Purpose: Constructor to select debug mode
-----------------------------------------------------------------------------*/
Game::Game(int debug) {
	this->debug = debug;
	Game();
}

/*-----------------------------------------------------------------------------
  Function Name: Game
  Function Return Type: None
  Parameters: None
  Function Description/Purpose: Destructor, closes this object
-----------------------------------------------------------------------------*/
Game::~Game() {
	free(this);
}

/*-----------------------------------------------------------------------------
  Function Name: run
  Function Return Type: void
  Parameters: None
  Function Description/Purpose: Main method to run game
-----------------------------------------------------------------------------*/
void Game::run() {
	menu(); 	// Call menu

	// Initialize boards and place ships on them
	initializeBoards();

	placeShipsRandom(playerBoard);
	placeShipsRandom(enemyBoard);

	// Ensure loop keeps running
	bool running = true;

	// Define variables to hold enemy and player states
	int row, sunkStatus, enemyRow, enemyCol, enemySunkStatus;
	char col;
	bool playerShipsSunk[5], enemyShipsSunk[5];

	// Set initial ship states to not sunk
	for (int i = carrier; i < SHIPS_AMOUNT; i++) {
		playerShipsSunk[i] = false;
		enemyShipsSunk[i] = false;
	}
	do { 	// main loop
		// Player guess
		// Print information for current turn
		cout << "------------ Your turn -----------" << endl;
		cout << "\nGuess board" << endl;
		printBoard(playerGuessBoard, true);
		cout << "\nYour board" << endl;
		printBoard(playerBoard, false);
		// Input loop
		do {
			cout << "\n";
			cout << "Enter a position (a #): ";
			cin >> col >> row;
			cin.ignore();
			col = (col - 97); 	// Convert human input to machine input
			row = (row - 1);
			if (row < 0 || row > 10 || col < 0 || col > 10) { 	// Input validation
				cout << "Invalid entry." << endl;
				waitForInput();
			}
		} while (row < 0 || row > 10 || col < 0 || col > 10);
		cout << (char)(col + 97) << (row + 1) << "!" << endl;
		if (check(enemyBoard, row, col)) { 	// If the guess is a hit
			cout << "It's a hit!" << endl;
			waitForInput();
			hitOrMiss(true, row, col, true); 	// Mark respective boards with hit markers
			sunkStatus = checkSunk(enemyBoard, enemyShipsSunk); 	// Update enemy sunk ships array
			if (sunkStatus != -1) { 	// If a ship is sunk
				cout << "You sunk the enemy's " << ships[sunkStatus] << "!" << endl;
				waitForInput();
			}
			sunkStatus = true;
			for (int i = carrier; i < SHIPS_AMOUNT; i++) { 	// Check if all enemy ships are sunk
				if (!(enemyShipsSunk[i])) {
					sunkStatus = false;
				}
			}
			if (sunkStatus) { 	// If all ships sunk
				cout << "You sunk the enemy fleet!" << endl;
				cout << "Thanks for playing!" << endl;
				running = false;
				continue;
			}
		}
		else {
			cout << "It's a miss..." << endl;
			hitOrMiss(true, row, col, false); 	// Mark respective boards with miss markers
		}
		cout << "-------- End of your turn --------" << endl;

		// Enemy guess

		cout << "\n---------- Enemy's turn ----------" << endl;
		// Random move
		enemyRow = rand() % BOARD_Y_SIZE; 	// 0 - 9
		enemyCol = rand() % BOARD_X_SIZE; 	// 0 - 9
		cout << (char)(enemyCol + 97) << (enemyRow + 1) << "!" << endl;
		if (check(playerBoard, enemyRow, enemyCol)) { 	// Hit
			cout << "The enemy hit your ";
			switch (getCellStatus(playerBoard, enemyRow, enemyCol)) {
				case carrier:
					cout << "carrier";
					break;
				case battleship:
					cout << "battleship";
					break;
				case cruiser:
					cout << "cruiser";
					break;
				case submarine:
					cout << "submarine";
					break;
				case destroyer:
					cout << "destroyer";
					break;
			}
			cout << "!" << endl;
			hitOrMiss(false, enemyRow, enemyCol, true);
			sunkStatus = checkSunk(playerBoard, playerShipsSunk);
			if (sunkStatus != -1) {
				cout << "The enemy sunk your " << ships[sunkStatus] << "!" << endl;
			}
			sunkStatus = true; 	// Assume ships are sunk by default
			for (int i = carrier; i < SHIPS_AMOUNT; i++) { 	// Check if each ship is still present
				if (!(playerShipsSunk[i])) { 	// If this ship is not sunk
					sunkStatus = false;
				}
			}
			if (sunkStatus) {
				cout << "The enemy sunk your fleet!" << endl;
				cout << "Game over." << endl;
				running = false;
				continue;
			}

		}
		else {
			cout << "The enemy missed!" << endl;
			hitOrMiss(false, enemyRow, enemyCol, false);
		}
		cout << "------ End of Enemy's turn ------\n" << endl;
		waitForInput();
	} while (running);
}

/*-----------------------------------------------------------------------------
  Function Name: waitForInput
  Function Return Type: void
  Parameters: None
  Function Description/Purpose: Wait for input from user
-----------------------------------------------------------------------------*/
void Game::waitForInput() {
	cout << "Press Enter to continue..." << endl;
	cin.ignore();
}

/*-----------------------------------------------------------------------------
  Function Name: menu
  Function Return Type: void
  Parameters: None
  Function Description/Purpose: Shows a menu and waits for user input
-----------------------------------------------------------------------------*/
void Game::menu() {
	cout << "░▀█▀░▀█▀░█▀▄░█▀█░█░░░░░█▀▄░█▀█░▀█▀░▀█▀░█░░░█▀▀" << endl;
	cout << "░░█░░░█░░█░█░█▀█░█░░░░░█▀▄░█▀█░░█░░░█░░█░░░█▀▀" << endl;
	cout << "░░▀░░▀▀▀░▀▀░░▀░▀░▀▀▀░░░▀▀░░▀░▀░░▀░░░▀░░▀▀▀░▀▀▀" << endl;
	cout << "\nAlastair Raymond (@) 2022" << endl;
	waitForInput();
}

/*-----------------------------------------------------------------------------
  Function Name: initializeBoards
  Function Return Type: void
  Parameters: None
  Function Description/Purpose: Initializes boards to be empty
-----------------------------------------------------------------------------*/
void Game::initializeBoards() {
	for (int i = 0; i < BOARD_Y_SIZE; i++) {
		for (int j = 0; j < BOARD_X_SIZE; j++) {
			mark(playerBoard, i, j, 0);
			mark(playerGuessBoard, i, j, 0);
			mark(enemyBoard, i, j, 0);
			mark(enemyGuessBoard, i, j, 0);
		}
	}
}

/*-----------------------------------------------------------------------------
  Function Name: mark
  Function Return Type: void
  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col, int value
  Function Description/Purpose: Sets the given position on the given board with the given value
-----------------------------------------------------------------------------*/
void Game::mark(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col, int value) {
	board[row][col] = value;
}

/*-----------------------------------------------------------------------------
  Function Name: check
  Function Return Type: bool
  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col
  Function Description/Purpose: Check if the guess was a hit
-----------------------------------------------------------------------------*/
bool Game::check(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col) {
	if (getCellStatus(board, row, col) > hit) {
		return true;
	}
	else {
		return false;
	}
}

/*-----------------------------------------------------------------------------
  Function Name: checkSunk
  Function Return Type: int
  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool shipsSunk[]
  Function Description/Purpose: Update ships sunk and return last ship sunk
-----------------------------------------------------------------------------*/
int Game::checkSunk(int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool shipsSunk[]) {
	bool carrierPresent = false;
	bool battleshipPresent = false;
	bool cruiserPresent = false;
	bool submarinePresent = false;
	bool destroyerPresent = false;
	for (int i = 0; i < BOARD_Y_SIZE; i++) { 	// Head count for ships on given board
		for (int j = 0; j < BOARD_X_SIZE; j++) {
	  		switch(getCellStatus(board, i, j)) {
				case carrier: carrierPresent = true; break;
				case battleship: battleshipPresent = true; break;
				case cruiser: cruiserPresent = true; break;
				case submarine: submarinePresent = true; break;
				case destroyer: destroyerPresent = true; break;
			}
		}
	}
	if ((carrierPresent == false) && (shipsSunk[carrier] == false)) { 	// If carrier was not in head count AND it is NOT ALREADY marked as sunk, then mark as sunk
		shipsSunk[carrier] = true;
		return carrier;
	}
	if ((!(battleshipPresent)) && (!(shipsSunk[battleship]))) {
		shipsSunk[battleship] = true;
		return battleship;
	}
	if ((!(cruiserPresent)) && (!(shipsSunk[cruiser]))) {
		shipsSunk[cruiser] = true;
		return cruiser;
	}
	if ((!(submarinePresent)) && (!(shipsSunk[submarine]))) {
		shipsSunk[submarine] = true;
		return submarine;
	}
	if ((!(destroyerPresent)) && (!(shipsSunk[destroyer]))) {
		shipsSunk[destroyer] = true;
		return destroyer;
	}
	return -1;
}

/*-----------------------------------------------------------------------------
  Function Name: hitOrMiss
  Function Return Type: void
  Parameters: bool player, int row, int col, bool hit
  Function Description/Purpose: Mark the respective boards with hit or miss
-----------------------------------------------------------------------------*/
void Game::hitOrMiss(bool player, int row, int col, bool hit) {
	int peg;
	if (hit) {
		peg = ::boardKey::hit;
	}
	else {
		peg = ::boardKey::miss;
	}
	if (player) { 	// If player misses or hits then mark playerGuessBoard and enemyBoard
		mark(playerGuessBoard, row, col, peg);
		mark(enemyBoard, row, col, peg);
	}
	else { 	// If enemy misses or hits then mark enemyGuessBoard and playerBoard
		mark(enemyGuessBoard, row, col, peg);
		if (peg == ::boardKey::hit) {
			mark(playerBoard, row, col, peg);
		}
	}
}

/*-----------------------------------------------------------------------------
  Function Name: placeShipsRandom
  Function Return Type: void
  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE]
  Function Description/Purpose: Place ships randomly on the given board
-----------------------------------------------------------------------------*/
void Game::placeShipsRandom(int board[BOARD_Y_SIZE][BOARD_X_SIZE]) {
	/*
	* Ship lengths:
	* 	Carrier: 5
	* 	Battleship: 4
	* 	Cruiser: 3
	* 	Submarine: 3
	* 	Destroyer: 2
	*/
	int col, row, direction;
	bool carrierPlaced = false, battleshipPlaced = false, cruiserPlaced = false, submarinePlaced = false, destroyerPlaced = false;
	while (!(carrierPlaced)) {
		col = rand() % BOARD_X_SIZE; 	// 0 - 9
		row = rand() % BOARD_Y_SIZE; 	// 0 - 9
		direction = rand() % 4; 	// 0 - 4
		if (placeShip(board, row, col, direction, 5, carrier)) {
			carrierPlaced = true;
		}
	}
	while (!(battleshipPlaced)) {
		col = rand() % BOARD_X_SIZE; 	// 0 - 9
		row = rand() % BOARD_Y_SIZE; 	// 0 - 9
		direction = rand() % 4; 	// 0 - 4
		if (placeShip(board, row, col, direction, 4, battleship)) {
			battleshipPlaced = true;
		}
	}
	while (!(cruiserPlaced)) {
		col = rand() % BOARD_X_SIZE; 	// 0 - 9
		row = rand() % BOARD_Y_SIZE; 	// 0 - 9
		direction = rand() % 4; 	// 0 - 4
		if (placeShip(board, row, col, direction, 3, cruiser)) {
			cruiserPlaced = true;
		}
	}
	while (!(submarinePlaced)) {
		col = rand() % BOARD_X_SIZE; 	// 0 - 9
		row = rand() % BOARD_Y_SIZE; 	// 0 - 9
		direction = rand() % 4; 	// 0 - 4
		if (placeShip(board, row, col, direction, 3, submarine)) {
			submarinePlaced = true;
		}
	}
	while (!(destroyerPlaced)) {
		col = rand() % BOARD_X_SIZE; 	// 0 - 9
		row = rand() % BOARD_Y_SIZE; 	// 0 - 9
		direction = rand() % 4; 	// 0 - 4
		if (placeShip(board, row, col, direction, 2, destroyer)) {
			destroyerPlaced = true;
		}
	}
}

/*-----------------------------------------------------------------------------
  Function Name: placeShip
  Function Return Type: bool
  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int rowOrigin int colOrigin, int direction, int length, int type
  Function Description/Purpose: Place ships in the given position and direction
-----------------------------------------------------------------------------*/
bool Game::placeShip(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int rowOrigin, int colOrigin, int direction, int length, int type) {
	// Direction key: 0 = up, 1 = right, 2 = down, 3 = left
	// Board key: 0 = empty, 1 = miss, 2 = hit, 3 = carrier, 4 = battleship, 5 = cruiser, 6 = submarine, 7 = destroyer
	switch (direction) {
		case 0: 	// up (north)
			if ((rowOrigin - length) < 0) {
				return false;
			}
			for (int i = rowOrigin; i > (rowOrigin - length); i--) {
				if (getCellStatus(board, i, colOrigin) != 0) {
					return false;
				}
			}
			for (int i = rowOrigin; i > (rowOrigin - length); i--) {
				board[i][colOrigin] = type;
			}
			break;

		case 1: 	// right (east)
			if ((colOrigin + length) > 9) {
				return false;
			}
			for (int i = colOrigin; i < (colOrigin + length); i++) {
				if (getCellStatus(board, rowOrigin, i) != 0) {
					return false;
				}
			}
			for (int i = colOrigin; i < (colOrigin + length); i++) {
				board[rowOrigin][i] = type;
			}
			break;
		case 2: 	// down (south)
			if ((rowOrigin + length) > 9) {
				return false;
			}
			for (int i = rowOrigin; i < (rowOrigin + length); i++) { 	// Start at given point and move down
				if (getCellStatus(board, i, colOrigin) != 0) {
					return false;
				}
			}
			for (int i = rowOrigin; i < (rowOrigin + length); i++) {
				board[i][colOrigin] = type;
			}
			break;

		case 3: 	// left (west)
			if ((colOrigin - length) < 0) {
				return false;
			}
			for (int i = colOrigin; i > (colOrigin - length); i--) {
				if (getCellStatus(board, rowOrigin, i) != 0) {
					return false;
				}
			}
			for(int i = colOrigin; i > (colOrigin - length); i--) {
				board[rowOrigin][i] = type;
			}
			break;
	}
	return true;
}

/*-----------------------------------------------------------------------------
  Function Name: getCellStatus
  Function Return Type: int
  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col
  Function Description/Purpose: Returns the status of the position on the given board
-----------------------------------------------------------------------------*/
int Game::getCellStatus(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col) {
	// Board key: 0 = empty, 1 = miss, 2 = hit, 3 = carrier, 4 = battleship, 5 = cruiser, 6 = submarine, 7 = destroyer
	return (board[row][col]);
}

/*-----------------------------------------------------------------------------
  Function Name: printKey()
  Function Return Type: void
  Parameters: None
  Function Description/Purpose: Print map key
-----------------------------------------------------------------------------*/
void Game::printKey() {
	cout << "\tMap key\n" << endl;
	cout << "~ - Ocean" << "\t C - Carrier (length 5)" << endl;
	cout << "M - Miss" << "\t B - Battleship (length 4)" << endl;
	cout << "H - Hit" << "\t\t R - Cruiser (length 3)" << endl;
	cout << "\t\t S - Submarine (length 3)" << endl;
	cout << "\t\t D - Destroyer (length 2)" << endl;
}

/*-----------------------------------------------------------------------------
  Function Name: printBoard
  Function Return Type: void
  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool key
  Function Description/Purpose: Print the given board with or without the key
-----------------------------------------------------------------------------*/
void Game::printBoard(int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool key) {
	int cellStatus = 0;
	cout << "\n   A B C D E F G H I J" << endl;
	for (int i = 0; i < BOARD_Y_SIZE; i++) { 	// Iterate over each row
		cout << (i + 1);
		if ((i + 1) == 10) {
			cout << " ";
		}
		else {
			cout << "  ";
		}
		for (int j = 0; j < BOARD_X_SIZE; j++) { 	// Iterate over each column
			cellStatus = getCellStatus(board, i, j);
			switch (cellStatus) {
				case 0:
					cout << "~ ";
					break;
				case 1:
					cout << "M ";
					break;
				case 2:
					cout << "H ";
					break;
				case carrier: 	// Carrier
					cout << "C ";
					break;
				case battleship: 	// Battleship
					cout << "B ";
					break;
				case cruiser: 	// Cruiser
					cout << "R ";
					break;
				case submarine: 	// Submarine
					cout << "S ";
					break;
				case destroyer: 	// Destroyer
					cout << "D ";
					break;
			}	
		}
		if (key) { 	// Print map key if requested
			switch (i) {
				case 1:
					cout << "\t\t\tMap key";
					break;
				case 3:
					cout << "\t\t~ - Ocean" << "\tC - Carrier (length 5)";
					break;
				case 4:
					cout << "\t\tM - Miss" << "\tB - Battleship (length 4)";
					break;
				case 5:
					cout << "\t\tH - Hit" << "\t\tR - Cruiser (length 3)";
					break;
				case 6:
					cout << "\t\t\t\tS - Submarine (length 3)";
					break;
				case 7:
					cout << "\t\t\t\tD - Destroyer (length 2)";
					break;
			}
		}
		cout << "\n";
	}
}
