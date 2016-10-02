#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

class gamestate
{
	public:
	int n;
	int tl;
	int player_id;
	int other_player;
	int*** board;
	int** height;
	bool wallToFlat;

	int myFlatstones, myCapstones;
	int otherFlatstones, otherCapstones;
	gamestate(int, int, int);
	void update_board(std::string, int);
	void undo_move(std::string, int);
	bool over();
};