#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

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
	void update_board(string, int);
	void undo_move(string, int);
	bool roadUtil(int, int, int**, int, int);
	bool road(int);
	void getNeighbours(int, int, int, vector<pair<int,int> >&);
	int over();
	void print_board();
};