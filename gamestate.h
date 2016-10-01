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
	int*** board;
	int** height;

	int myFlatstones, myCapstones;
	int otherFlatstones, otherCapstones;
	gamestate(int, int, int);
	void update_board(std::string, int);
};