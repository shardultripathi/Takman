#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "gamestate.h"
#include "getmoves.h"
#define neg	-65536
#define pos	65536
using namespace std;

int eval(gamestate* game, int player)
{
	return 1;
}

pair<int,string> value(gamestate* game, int cutoff, int depth, int alpha, int beta, bool maxNode)
{
	if (cutoff==depth)
		return make_pair(eval(game,maxNode?(game->other_player):(game->player_id)),""); //// if it is maxnode =>
																						//// came from a move of minnode => evaluate wrt to other player
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
			temp = value(game,cutoff,depth+1,alpha,beta,false).first;
			if (temp>max)
			{
				max = temp;
				max_index = i;
			}
			game->undo_move(moves[i],game->player_id);
			if (max >= beta)
				return make_pair(max,moves[i]);
			alpha = max(alpha,max);
		}
		return make_pair(max,moves[max_index]);
	}
	else
	{
		int min = pos;
		vector<string> moves; generate_moves(moves,game);
		int moves_size = moves.size();
		int min_index;
		int temp;
		for (int i=0; i<moves_size; i++)
		{
			game->update_board(moves[i],game->other_player);
			temp = value(game,cutoff,depth+1,alpha,beta,true).first;
			if (temp<min)
			{
				min = temp;
				min_index = i;
			}
			game->undo_move(moves[i],game->other_player);
			if (min <= alpha) 
				return make_pair(min,moves[i]);
			beta = min(beta,min);
		}
		return make_pair(min,moves[min_index]);
	}
	
}