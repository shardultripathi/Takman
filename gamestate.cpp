#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include "gamestate.h"
#include "getmoves.h"

using namespace std;

// class gamestate
// {
// public:
// 	int n;
// 	int tl;
// 	int player_id;
// 	int*** board;
// 	int** height;

// 	int myFlatstones, myCapstones;
// 	int otherFlatstones, otherCapstones;

	gamestate::gamestate(int N, int time, int player)
	{
		n = N;
		tl = time;
		player_id = player;
		other_player = (player==1)?2:1;

		myFlatstones = otherFlatstones = 20; /////// do wrt n finally
		myCapstones = otherCapstones = 1;

		board = new int**[n];
		height = new int*[n];
		for(int i=0;i<n;i++) {
			board[i] = new int*[n];
			height[i] = new int[n];
			for(int j=0;j<n;j++) {
				height[i][j] = 0;
				board[i][j] = new int[2*(myFlatstones)+myCapstones];
				for(int k=0;k<n;k++) {
					board[i][j][k] = 0;
				}
			}
		}
	}

	void gamestate::update_board(string move, int player)
	{
		int i,j;
		int it,it2,stack_size,step;
		int sign = (player==player_id)? 1:-1;
		wallToFlat = false;
		switch(move[0]) 
		{
			case 'F': cerr<<move<<" oF\n";
					  i = move[1] - 'a';
					  j = move[2] - '1';
					  board[i][j][height[i][j]] = sign*1;
					  height[i][j]++;
					  if (sign==1) myFlatstones--; else otherFlatstones--;
					  break;
			case 'S': cerr<<move<<" oS\n";
					  i = move[1] - 'a';
					  j = move[2] - '1';
					  board[i][j][height[i][j]] = sign*2;
					  height[i][j]++;
					  if (sign==1) myFlatstones--; else otherFlatstones--;
					  break;
			case 'C': cerr<<move<<" oC\n";
					  i = move[1] - 'a';
					  j = move[2] - '1';
					  board[i][j][height[i][j]] = sign*3;
					  height[i][j]++;
					  if (sign==1) myCapstones--; else otherCapstones--;
					  break;
			default: cerr<<move<<" oStack\n";
					 stack_size = move[0] - '0';
					 i = move[1] - 'a';
					 j = move[2] - '1';
					 height[i][j] -= stack_size;
					 switch(move[3]) {
					 	case '>': if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1])==2) {
					 				wallToFlat = true;
					 				board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1] /= 2;
					 			  }
					 			  for(it=4;it<move.length();it++) {
					 				step = move[it] - '0';
					 				it2 = step;
					 				while(it2>0) {
					 					board[i+it-3][j][height[i+it-3][j] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
					 					board[i][j][height[i][j] + it2 - 1] = 0;
					 					it2--;
					 				}
					 				height[i][j] += step;
					 				height[i+it-3][j] += step;
					 			  }
					 			  break;
					 			  
					 	case '<': if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1])==2) {
					 				wallToFlat = true;
					 				board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1] /= 2;
					 			  }
					 			  for(it=4;it<move.length();it++) {
					 				step = move[it] - '0';
					 				it2 = step;
					 				while(it2>0) {
					 					board[i-it+3][j][height[i-it+3][j] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
					 					board[i][j][height[i][j] + it2 - 1] = 0;
					 					it2--;
					 				}
					 				height[i][j] += step;
					 				height[i-it+3][j] += step;
					 			  }
					 			  break;

					 	case '+': if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1])==2) {
					 				wallToFlat = true;
					 				board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1] /= 2;
					 			  }
					 			  for(it=4;it<move.length();it++) {
					 				step = move[it] - '0';
					 				it2 = step;
					 				while(it2>0) {
					 					board[i][j+it-3][height[i][j+it-3] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
					 					board[i][j][height[i][j] + it2 - 1] = 0;
					 					it2--;
					 				}
					 				height[i][j] += step;
					 				height[i][j+it-3] += step;
					 			  }
					 			  break;
					 			  
					 	default: if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1])==2) {
					 				wallToFlat = true;
					 				board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1] /= 2;
					 			  }
					 			  for(it=4;it<move.length();it++) {
					 				step = move[it] - '0';
					 				it2 = step;
					 				while(it2>0) {
					 					board[i][j-it+3][height[i][j-it+3] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
					 					board[i][j][height[i][j] + it2 - 1] = 0;
					 					it2--;
					 				}
					 				height[i][j] += step;
					 				height[i][j-it+3] += step;
					 			  }
					 			  
					 }
					 height[i][j] -= stack_size;
		}
	}


	void gamestate::undo_move(string move, int player)
	{
		int i,j;
		int it,it2,stack_size,step;
		int sign = (player==player_id)? 1:-1;
		if(isalpha(move[0])) {
			i = move[1] - 'a';
			j = move[2] - '1';
			board[i][j][height[i][j]] = 0;
			height[i][j]--;
			if(move[0]=='C')
				if (sign==1) myCapstones++; else otherCapstones++;
			else 
				if (sign==1) myFlatstones++; else otherFlatstones++;
					  
		} else {
			stack_size = move[0] - '0';
			i = move[1] - 'a';
			j = move[2] - '1';
			height[i][j] += stack_size;
			switch(move[3]) {
				case '>': for(it=4;it<move.length();it++) {
					 	  	step = move[it] - '0';
					 		it2 = step;
							while(it2>0) {
		 						board[i][j][height[i][j] + it2 - 1] = board[i+it-3][j][height[i+it-3][j] + it2 - 1];
		 						board[i+it-3][j][height[i+it-3][j] + it2 - 1] = 0;
					 			it2--;
							}
					 		height[i][j] -= step;
					 		height[i+it-3][j] -= step;
		 				  }
		 				  if(wallToFlat)
		 				  	board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1] *= 2;
					 	  break;
					 			  
				case '<': for(it=4;it<move.length();it++) {
			 				step = move[it] - '0';
			 				it2 = step;
					 		while(it2>0) {
					 			board[i][j][height[i][j] + it2 - 1] = board[i-it+3][j][height[i-it+3][j] + it2 - 1];
								board[i-it+3][j][height[i-it+3][j] + it2 - 1] = 0;
			 					it2--;
				 			}
					 		height[i][j] -= step;
							height[i-it+3][j] -= step;
			 			  }
			 			  if(wallToFlat)
			 			  	board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1] *= 2;
			 			  break;

				case '+': for(it=4;it<move.length();it++) {
					 		step = move[it] - '0';
					 		it2 = step;
			 				while(it2>0) {
								board[i][j][height[i][j] + it2 - 1] = board[i][j+it-3][height[i][j+it-3] + it2 - 1];
					 			board[i][j+it-3][height[i][j+it-3] + it2 - 1] = 0;
					 			it2--;
							}
			 				height[i][j] -= step;
			 				height[i][j+it-3] -= step;
					 	  }
					 	  if(wallToFlat)
					 	  	board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1] *= 2;
						  break;
					 			  
			 	default: for(it=4;it<move.length();it++) {
					 		step = move[it] - '0';
					 		it2 = step;
							while(it2>0) {
			 					board[i][j][height[i][j] + it2 - 1] = board[i][j-it+3][height[i][j-it+3] + it2 - 1];
					 			board[i][j-it+3][height[i][j-it+3] + it2 - 1] = 0;
					 			it2--;
							}
			 				height[i][j] -= step;
			 				height[i][j-it+3] -= step;
					 	  }
					 	  if(wallToFlat)
					 	  	board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1] *= 2;
					 			  
			}
			height[i][j] += stack_size;
		}
	}

	bool gamestate::over()
	{
		return false;
	}


// };