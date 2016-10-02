#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "gamestate.h"
#include "getmoves.h"

using namespace std;

int eval(gamestate* game, int player)
{
	return 1;
}

pair<int,string> value(gamestate* game, int cutoff, int depth, int alpha, int beta, bool maxNode)
{
	if (cutoff==depth)
		return make_pair(eval(game),"");
	if (game->over())
		return make_pair(maxNode?neg:pos,"");

	if (maxNode)
	{
		int max = neg;
		vector<string> moves; generate_moves(moves,game);
		int moves_size = moves.size();
		int max_index;
		int temp;
		for (int i=0; i<moves_size; i++)
		{
			game->update_board(moves[i],game->player_id);
			temp = value(game,cutoff,depth+1,alpha,beta,false);
			if (temp>max)
			{
				max = temp;
				max_index = i;
			}
			game->undo_move();
			if (max >= beta)
				return make_pair(max,moves[i]);
			alpha = max(alpha,max);
		}
		return make_pair(max,max_index);
	}
	else
	{
		return minValue(game,cutoff,depth,alpha,beta);
	}
	
}