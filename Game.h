/*-----------------------------------------------------------------------------
  Program Name: Tidal Battle
  Programmer Name: Alastair Raymond
  Program Description/Purpose: Naval strategy war game
  Date: 29 November 2022
-----------------------------------------------------------------------------*/

#include <iostream>

using namespace std;

//enum natoAlphabetAbreviated {a, b, c, d, e, f, g, h, i, j};
enum boardKey {empty, miss, hit, carrier, battleship, cruiser, submarine, destroyer};

const string ships[] = {"", "", "", "carrier", "battleship", "cruiser", "submarine", "destroyer"};

const int BOARD_X_SIZE = 10;
const int BOARD_Y_SIZE = 10;
const int SHIPS_AMOUNT = destroyer + 1;

class Game {
	private:
		int debug;
		// Variables for boards
		int playerBoard[BOARD_Y_SIZE][BOARD_X_SIZE]; 	// Holds player's naval ships
		int playerGuessBoard[BOARD_Y_SIZE][BOARD_X_SIZE]; 	// Holds player's guesses of where enemy's ships are
		int enemyBoard[BOARD_Y_SIZE][BOARD_X_SIZE]; 	// Holds enemy's naval ships
		int enemyGuessBoard[BOARD_Y_SIZE][BOARD_X_SIZE]; 	// Holds guesses of where player's ships are

		/*-----------------------------------------------------------------------------
		  Function Name: printBoard
		  Function Return Type: void
		  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool key
		  Function Description/Purpose: Print the given board with or without the key
		-----------------------------------------------------------------------------*/
		void printBoard(int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool key);

		/*-----------------------------------------------------------------------------
		  Function Name: placeShipsRandom
		  Function Return Type: void
		  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE]
		  Function Description/Purpose: Place ships randomly on the given board
		-----------------------------------------------------------------------------*/
		void placeShipsRandom(int board[BOARD_Y_SIZE][BOARD_X_SIZE]);

		/*-----------------------------------------------------------------------------
		  Function Name: initializeBoards
		  Function Return Type: void
		  Parameters: None
		  Function Description/Purpose: Initializes boards to be empty
		-----------------------------------------------------------------------------*/
		void initializeBoards();

		/*-----------------------------------------------------------------------------
		  Function Name: placeShip
		  Function Return Type: bool
		  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int rowOrigin int colOrigin, int direction, int length, int type
		  Function Description/Purpose: Place ships in the given position and direction
		-----------------------------------------------------------------------------*/
		bool placeShip(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int rowOrigin, int colOrigin, int direction, int length, int type);

		/*-----------------------------------------------------------------------------
		  Function Name: getCellStatus
		  Function Return Type: int
		  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col
		  Function Description/Purpose: Returns the status of the position on the given board
		-----------------------------------------------------------------------------*/
		int getCellStatus(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col);

		/*-----------------------------------------------------------------------------
		  Function Name: printKey()
		  Function Return Type: void
		  Parameters: None
		  Function Description/Purpose: Print map key
		-----------------------------------------------------------------------------*/
		void printKey();

		/*-----------------------------------------------------------------------------
		  Function Name: mark
		  Function Return Type: void
		  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col, int value
		  Function Description/Purpose: Sets the given position on the given board with the given value
		-----------------------------------------------------------------------------*/
		void mark(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col, int value);

		/*-----------------------------------------------------------------------------
		  Function Name: hitOrMiss
		  Function Return Type: void
		  Parameters: bool player, int row, int col, bool hit
		  Function Description/Purpose: Mark the respective boards with hit or miss
		-----------------------------------------------------------------------------*/
		void hitOrMiss(bool player, int row, int col, bool hit);

		/*-----------------------------------------------------------------------------
		  Function Name: check
		  Function Return Type: bool
		  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col
		  Function Description/Purpose: Check if the guess was a hit
		-----------------------------------------------------------------------------*/
		bool check(int board[BOARD_Y_SIZE][BOARD_X_SIZE], int row, int col);

		/*-----------------------------------------------------------------------------
		  Function Name: checkSunk
		  Function Return Type: int
		  Parameters: int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool shipsSunk[]
		  Function Description/Purpose: Update ships sunk and return last ship sunk
		-----------------------------------------------------------------------------*/
		int checkSunk(int board[BOARD_Y_SIZE][BOARD_X_SIZE], bool shipsSunk[]);

		/*-----------------------------------------------------------------------------
		  Function Name: menu
		  Function Return Type: void
		  Parameters: None
		  Function Description/Purpose: Shows a menu and waits for user input
		-----------------------------------------------------------------------------*/
		void menu();

		/*-----------------------------------------------------------------------------
		  Function Name: waitForInput
		  Function Return Type: void
		  Parameters: None
		  Function Description/Purpose: Wait for input from user
		-----------------------------------------------------------------------------*/
		void waitForInput();

	public:
		/*-----------------------------------------------------------------------------
		  Function Name: run
		  Function Return Type: void
		  Parameters: None
		  Function Description/Purpose: Main method to run game
		-----------------------------------------------------------------------------*/
		void run();

		/*-----------------------------------------------------------------------------
		  Function Name: Game
		  Function Return Type: None
		  Parameters: None
		  Function Description/Purpose: Primary constructor, initializes random seed
		-----------------------------------------------------------------------------*/
		Game();

		/*-----------------------------------------------------------------------------
		  Function Name: Game
		  Function Return Type: None
		  Parameters: int debug
		  Function Description/Purpose: Constructor to select debug mode
		-----------------------------------------------------------------------------*/
		Game(int debug);

		/*-----------------------------------------------------------------------------
		  Function Name: Game
		  Function Return Type: None
		  Parameters: None
		  Function Description/Purpose: Destructor, closes this object
		-----------------------------------------------------------------------------*/
		~Game();
};
