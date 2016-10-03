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
			case 'F': 
			// cerr<<move<<" oF\n";
					  i = move[1] - 'a';
					  j = move[2] - '1';
					  board[i][j][height[i][j]] = sign*1;
					  height[i][j]++;
					  if (sign==1) myFlatstones--; else otherFlatstones--;
					  break;
			case 'S': 
			// cerr<<move<<" oS\n";
					  i = move[1] - 'a';
					  j = move[2] - '1';
					  board[i][j][height[i][j]] = sign*2;
					  height[i][j]++;
					  if (sign==1) myFlatstones--; else otherFlatstones--;
					  break;
			case 'C': 
			// cerr<<move<<" oC\n";
					  i = move[1] - 'a';
					  j = move[2] - '1';
					  board[i][j][height[i][j]] = sign*3;
					  height[i][j]++;
					  if (sign==1) myCapstones--; else otherCapstones--;
					  break;
			default: 
			// cerr<<move<<" oStack\n";
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
				case '>': for(it=move.length()-1;it>=4;it--) {
					 	  	step = move[it] - '0';
					 		it2 = step;
							while(it2>0) {
		 						board[i][j][height[i][j] - it2] = board[i+it-3][j][height[i+it-3][j] - it2];
		 						board[i+it-3][j][height[i+it-3][j] - it2] = 0;
					 			it2--;
							}
					 		height[i][j] -= step;
					 		height[i+it-3][j] -= step;
		 				  }
		 				  if(wallToFlat)
		 				  	board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1] *= 2;
					 	  break;
					 			  
				case '<': for(it=move.length()-1;it>=4;it--) {
			 				step = move[it] - '0';
			 				it2 = step;
					 		while(it2>0) {
					 			board[i][j][height[i][j] - it2] = board[i-it+3][j][height[i-it+3][j] - it2];
								board[i-it+3][j][height[i-it+3][j] - it2] = 0;
			 					it2--;
				 			}
					 		height[i][j] -= step;
							height[i-it+3][j] -= step;
			 			  }
			 			  if(wallToFlat)
			 			  	board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1] *= 2;
			 			  break;

				case '+': for(it=move.length()-1;it>=4;it--) {
					 		step = move[it] - '0';
					 		it2 = step;
			 				while(it2>0) {
								board[i][j][height[i][j] - it2] = board[i][j+it-3][height[i][j+it-3] - it2];
					 			board[i][j+it-3][height[i][j+it-3] - it2] = 0;
					 			it2--;
							}
			 				height[i][j] -= step;
			 				height[i][j+it-3] -= step;
					 	  }
					 	  if(wallToFlat)
					 	  	board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1] *= 2;
						  break;
					 			  
			 	default: for(it=move.length()-1;it>=4;it--) {
					 		step = move[it] - '0';
					 		it2 = step;
							while(it2>0) {
			 					board[i][j][height[i][j] - it2] = board[i][j-it+3][height[i][j-it+3] - it2];
					 			board[i][j-it+3][height[i][j-it+3] - it2] = 0;
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

	bool gamestate::roadUtil(int direction, int sign, int** visited, int i, int j)
	{
		if (direction==1)
		{
			visited[i][j] = 1;
			if (j==n-1)
			{
				if (board[i][j][height[i][j]-1]==sign*1 || board[i][j][height[i][j]-1]==sign*3)
					return true;
				else return false;
			}
			if (i==0)
			{
				if (visited[i+1][j]==0 && (board[i+1][j][height[i+1][j]-1]==sign*1 || board[i+1][j][height[i+1][j]-1]==sign*3))
					return roadUtil(direction,sign,visited,i+1,j);
				else if (visited[i][j+1]==0 && (board[i][j+1][height[i][j+1]-1]==sign*1 || board[i][j+1][height[i][j+1]-1]==sign*3))
					return roadUtil(direction,sign,visited,i,j+1);
				else return false;
			}
			else if(i==n-1)
			{
				if (visited[i-1][j]==0 && (board[i-1][j][height[i-1][j]-1]==sign*1 || board[i-1][j][height[i-1][j]-1]==sign*3))
					return roadUtil(direction,sign,visited,i-1,j);
				else if (visited[i][j+1]==0 && (board[i][j+1][height[i][j+1]-1]==sign*1 || board[i][j+1][height[i][j+1]-1]==sign*3))
					return roadUtil(direction,sign,visited,i,j+1);
				else return false;
			}
			else
			{
				if (visited[i-1][j]==0 && (board[i-1][j][height[i-1][j]-1]==sign*1 || board[i-1][j][height[i-1][j]-1]==sign*3))
					return roadUtil(direction,sign,visited,i-1,j);
				else if (visited[i+1][j]==0 && (board[i+1][j][height[i+1][j]-1]==sign*1 || board[i+1][j][height[i+1][j]-1]==sign*3))
					return roadUtil(direction,sign,visited,i+1,j);
				else if (visited[i][j+1]==0 && (board[i][j+1][height[i][j+1]-1]==sign*1 || board[i][j+1][height[i][j+1]-1]==sign*3))
					return roadUtil(direction,sign,visited,i,j+1);
				else return false;	
			}
		}
		else
		{
			visited[i][j] = 1;
			if (i==n-1)
			{
				if (board[i][j][height[i][j]-1]==sign*1 || board[i][j][height[i][j]-1]==sign*3)
					return true;
				else return false;
			}
			if (j==0)
			{
				if (visited[i+1][j]==0 && (board[i+1][j][height[i+1][j]-1]==sign*1 || board[i+1][j][height[i+1][j]-1]==sign*3))
					return roadUtil(direction,sign,visited,i+1,j);
				else if (visited[i][j+1]==0 && (board[i][j+1][height[i][j+1]-1]==sign*1 || board[i][j+1][height[i][j+1]-1]==sign*3))
					return roadUtil(direction,sign,visited,i,j+1);
				else return false;
			}
			else if(j==n-1)
			{
				if (visited[i][j-1]==0 && (board[i][j-1][height[i][j-1]-1]==sign*1 || board[i][j-1][height[i][j-1]-1]==sign*3))
					return roadUtil(direction,sign,visited,i,j-1);
				else if (visited[i+1][j]==0 && (board[i+1][j][height[i+1][j]-1]==sign*1 || board[i+1][j][height[i+1][j]-1]==sign*3))
					return roadUtil(direction,sign,visited,i+1,j);
				else return false;
			}
			else
			{
				if (visited[i][j-1]==0 && (board[i][j-1][height[i][j-1]-1]==sign*1 || board[i][j-1][height[i][j-1]-1]==sign*3))
					return roadUtil(direction,sign,visited,i,j-1);
				else if (visited[i+1][j]==0 && (board[i+1][j][height[i+1][j]-1]==sign*1 || board[i+1][j][height[i+1][j]-1]==sign*3))
					return roadUtil(direction,sign,visited,i+1,j);
				else if (visited[i][j+1]==0 && (board[i][j+1][height[i][j+1]-1]==sign*1 || board[i][j+1][height[i][j+1]-1]==sign*3))
					return roadUtil(direction,sign,visited,i,j+1);
				else return false;	
			}
		}
	}

	bool gamestate::road(int player)
	{
		int** visited;
		visited = new int*[n];
		for (int i=0; i<n; i++)
		{
			visited[i] = new int[n];
			for (int j=0; j<n; j++)
				visited[i][j] = 0;
		}

		bool temp = false;
		int sign = (player==player_id)?1:-1; 
		for (int i=0; i<n; i++)
		{
			if (visited[i][0]==0 && (board[i][0][height[i][0]-1]==sign*1 || board[i][0][height[i][0]-1]==sign*3))
			{
				temp = roadUtil(1,sign,visited,i,0);
				if (temp) return true;
			}
		}

		for (int i=0; i<n; i++)
			for (int j=0; j<n; j++)
				visited[i][j] = 0;

		for (int j=0; j<n; j++)
		{
			if (visited[0][j]==0 && (board[0][j][height[0][j]-1]==sign*1 || board[0][j][height[0][j]-1]==sign*3))
			{
				temp = roadUtil(-1,sign,visited,0,j);
				if (temp) return true;
			}
		}
		return false;

		// return roadUtil(1,(player==player_id)?true:false,visited,0,0) || roadUtil(-1,(player==player_id)?true:false,visited,0,0);
		//// direction 1 for left to right road, -1 for top to bottom road
	}

	void gamestate::getNeighbours(int i, int j, int len, vector<pair<int,int> > &neighbours)
	{
		// vector<pair<int,int> > neighbours;
		// pair<int,int> temp;

		for (int l=1; l<=len; l++)
		{
			if (i+l<n) neighbours.push_back(make_pair(i+l,j));
			if (j+l<n) neighbours.push_back(make_pair(i,j+l));
			if (i-l>-1) neighbours.push_back(make_pair(i-l,j));
			if (j-l>-1) neighbours.push_back(make_pair(i,j-l));
		}

		// if (i==0)
		// {
		// 	if (j==0)
		// 	{
		// 		neighbours.push_back(make_pair(i+1,j));
		// 		neighbours.push_back(make_pair(i,j+1));
		// 		goto hell;
		// 	}
		// 	else if (j==n-1)
		// 	{
		// 		neighbours.push_back(make_pair(i+1,j));
		// 		neighbours.push_back(make_pair(i,j-1));
		// 		goto hell;
		// 	}
		// 	else
		// 	{
		// 		neighbours.push_back(make_pair(i+1,j));
		// 		neighbours.push_back(make_pair(i,j+1));
		// 		neighbours.push_back(make_pair(i,j-1));
		// 		goto hell;
		// 	}
		// }
		// if (j==0)
		// {
		// 	if (i==n-1)
		// 	{
		// 		neighbours.push_back(make_pair(i-1,j));
		// 		neighbours.push_back(make_pair(i,j+1));
		// 		goto hell;
		// 	}
		// 	else
		// 	{
		// 		neighbours.push_back(make_pair(i-1,j));
		// 		neighbours.push_back(make_pair(i+1,j));
		// 		neighbours.push_back(make_pair(i,j+1));
		// 		goto hell;
		// 	}
		// }
		// if (i==n-1)
		// {
		// 	if (j==n-1)
		// 	{
		// 		neighbours.push_back(make_pair(i-1,j));
		// 		neighbours.push_back(make_pair(i,j-1));
		// 		goto hell;	
		// 	}
		// 	else
		// 	{
		// 		neighbours.push_back(make_pair(i-1,j));
		// 		neighbours.push_back(make_pair(i,j-1));
		// 		neighbours.push_back(make_pair(i,j+1));
		// 		goto hell;
		// 	}
		// }
		// if (j==n-1)
		// {
		// 	neighbours.push_back(make_pair(i-1,j));
		// 	neighbours.push_back(make_pair(i+1,j));
		// 	neighbours.push_back(make_pair(i,j-1));
		// 	goto hell;
		// }
		// neighbours.push_back(make_pair(i-1,j));
		// neighbours.push_back(make_pair(i+1,j));
		// neighbours.push_back(make_pair(i,j-1));
		// neighbours.push_back(make_pair(i,j+1));
		// hell: return neighbours;
	}

	int gamestate::over()
	{
		if (road(player_id))
			return 1;
		if (road(other_player))
			return -1;
		if ((myFlatstones==0 && myCapstones==0) || (otherFlatstones==0 && otherCapstones==0))
			return 2;
		for (int i=0; i<n; i++)
			for (int j=0; j<n; j++)
				if (height[i][j]==0) return 4;
		return 3;
	}

	void gamestate::print_board() {
		int i,j,k;
		for(j=n-1;j>=0;j--) {
			for(i=0;i<n;i++) {
				if(height[i][j]==0)
					cerr<<"-\t";
				else {
					for(k=0;k<height[i][j];k++) {
						switch(board[i][j][k]) {
							case -3: cerr<<"c";
									break;
							case -2: cerr<<"s";
									break;
							case -1: cerr<<"f";
									break;
							case 1: cerr<<"F";
									break;
							case 2: cerr<<"S";
									break;
							case 3: cerr<<"C";
									break;
							default: cerr<<"-";
						}
					}
					cerr<<"\t";
				}
			}
			cerr<<"\n";
		}
	}


// };