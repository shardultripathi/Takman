#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "gamestate.h"
#include "getmoves.h"

using namespace std;


void place_new(vector<string> &moves, gamestate* game, int player) 
{
	// vector<string> &moves = *v;
	// gamestate &game = *g;
	int n = game->n;
	int i,j;
	string str;
	char c;
	for(i=0;i<n;i++) {
		for(j=0;j<n;j++) {
			if(game->height[i][j] == 0) {
				str = "";
				c = i + 'a';
				str += c;
				str += to_string(j+1);
				if(player==game->player_id) {
					if(game->myFlatstones>0) {
						moves.push_back("F"+str);
						moves.push_back("S"+str);
					}
					if(game->myCapstones>0)
						moves.push_back("C"+str);
				} else {
					if(game->otherFlatstones>0) {
						moves.push_back("F"+str);
						moves.push_back("S"+str);
					}
					if(game->otherCapstones>0)
						moves.push_back("C"+str);
				}
			}
		}
	}
}


void move_stack(int i, int j, int total, int direction, string str, int top,vector<string> &moves, gamestate* game) 
{ 
	//direction is 0 for >, 1 for <, 2 for +, 3 for - /////top is 1 for flatstone, 2 for wall, 3 for capstone
	// vector<string> &moves = *v;
	// gamestate &game = *g;
	int n = game->n;

	// cerr<<str<<"\n";
	if(total==0) {
		moves.push_back(str);
		return;
	}
	int it1;
	switch(direction) {
		case 0: if(i==n-1) 
					return;
				if(abs(game->board[i+1][j][game->height[i+1][j] - 1] ) == 3)
					return;
				if(abs(game->board[i+1][j][game->height[i+1][j] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i+1,j,0,direction,str+to_string(total),top,moves,game);					
					}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i+1,j,total-it1,direction,str+to_string(it1),top,moves,game);
				}
				break;

		case 1: if(i==0) 
					return;
				if(abs(game->board[i-1][j][game->height[i-1][j] - 1] ) == 3)
					return;
				if(abs(game->board[i-1][j][game->height[i-1][j] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i-1,j,0,direction,str+to_string(total),top,moves,game);					
					}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i-1,j,total-it1,direction,str+to_string(it1),top,moves,game);
				}
				break;

		case 2: if(j==n-1) 
					return;
				if(abs(game->board[i][j+1][game->height[i][j+1] - 1] ) == 3)
					return;
				if(abs(game->board[i][j+1][game->height[i][j+1] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i,j+1,0,direction,str+to_string(total),top,moves,game);					
				}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i,j+1,total-it1,direction,str+to_string(it1),top,moves,game);
				}
				break;

		default: if(j==0) 
					return;
				if(abs(game->board[i][j-1][game->height[i][j-1] - 1] ) == 3)
					return;
				if(abs(game->board[i][j-1][game->height[i][j-1] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i,j-1,0,direction,str+to_string(total),top,moves,game);					
					}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i,j-1,total-it1,direction,str+to_string(it1),top,moves,game);
				}
	}
}


void generate_moves(vector<string> &moves, gamestate* game, int player) 
{
	// vector<string> &moves = *v;
	// gamestate &game = *g;
	int n = game->n;

	moves.clear();
	place_new(moves,game,player);
	int i,j,k,max_stack_move;
	string str,str1;
	char c;
	int sign = (player==game->player_id)?1:-1;
	for(i=0;i<n;i++) {
		for(j=0;j<n;j++) {
			if(game->height[i][j]>0 && (game->board[i][j][game->height[i][j] - 1])*sign > 0) {
				max_stack_move = min(n,game->height[i][j]);
				c = i + 'a';
				str = "";
				str += c;
				str += to_string(j+1);
				for(k=1;k<=max_stack_move;k++) {
					str1 = to_string(k) + str;
					move_stack(i,j,k, 0, str1+">",abs(game->board[i][j][game->height[i][j] - 1]),moves,game);
					move_stack(i,j,k, 1, str1+"<",abs(game->board[i][j][game->height[i][j] - 1]),moves,game);
					move_stack(i,j,k, 2, str1+"+",abs(game->board[i][j][game->height[i][j] - 1]),moves,game);
					move_stack(i,j,k, 3, str1+"-",abs(game->board[i][j][game->height[i][j] - 1]),moves,game);
				}
			}
		}
	}
}

