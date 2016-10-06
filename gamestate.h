#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdlib>
#include <cmath>
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

	// uint64_t hashKey;
	// uint64_t**** zobrist;

	int myFlatstones, myCapstones;
	int otherFlatstones, otherCapstones;
	gamestate(int, int, int);
	void update_board(string, int);
	void undo_move(string, int);
	void getNeighbours(int, int, int, vector<pair<int,int> >&);
	bool dfs(int, int, int, int);
	bool road(int);
	int over();
	void print_board();
};