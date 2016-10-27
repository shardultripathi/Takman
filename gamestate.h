#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdlib>
#include <cmath>
#include <random>
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

	uint64_t hash;
	uint64_t**** zobrist;

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
	uint64_t getHash();
	// bool operator==(const gamestate&b) {
 //            if (hash != b.hash)
 //                return false;
 //            for (int i = 0; i < n; i++)
 //                for (int j = 0; j < n; j++) {
 //                    if (height[i][j] != b.height[i][j])
 //                        return false;
 //                    for (int k = 0; k < height[i][j]; k++)
 //                        if (board[i][j][k] != b.board[i][j][k])
 //                            return false;
 //                }
 //            return true;
 //        }
	// uint64_t getHash() const { return hash; }
};