#include "gamestate.h"
#include "getmoves.h"
#include "minimax.h"

using namespace std;

int visited[5][5];

gamestate::gamestate(int N, int time, int player)
{
	n = N;
	tl = time;
	player_id = player;
	other_player = (player==1)?2:1;

	myFlatstones = otherFlatstones = 21; /////// do wrt n finally
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
	// hashKey ^= zobrist[i][j][k][l];

	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2,61)), std::llround(std::pow(2,62)));
	hash = 0;
	zobrist = new uint64_t***[n];
	for (int i =0;i<n;i++)
	{
	   	zobrist[i] = new uint64_t**[n];
		for (int j = 0;j<n;j++)
		{
			zobrist[i][j] = new uint64_t*[2*(myFlatstones)+myCapstones];
			for (int k = 0;k<2*(myFlatstones)+myCapstones;k++)
			{
				zobrist[i][j][k] = new uint64_t[6];
				for (int l = 0;l<6;l++)
				{ /////////// 0 my flat, 1 my wall, 2 my cap, 3 opp flat, 4 opp wall, 5 opp cap
					zobrist[i][j][k][l] = dist(e2);
				}
			}
		}
	}
}

void gamestate::update_board(string move, int player) //////// make changes for zobrist key
{

	int i,j,z;
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
				  hash ^= zobrist[i][j][0][-3*(sign-1)/2];
				  if (sign==1) myFlatstones--; else otherFlatstones--;
				  break;
		case 'S': 
		// cerr<<move<<" oS\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = sign*2;
				  height[i][j]++;
				  hash ^= zobrist[i][j][0][(5-3*sign)/2];
				  if (sign==1) myFlatstones--; else otherFlatstones--;
				  break;
		case 'C': 
		// cerr<<move<<" oC\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = sign*3;
				  height[i][j]++;
				  hash ^= zobrist[i][j][0][(7-3*sign)/2];
				  if (sign==1) myCapstones--; else otherCapstones--;
				  break;
		default: 
		// cerr<<move<<" oStack\n";
				// cerr<<"update "<<move<<endl;
				 stack_size = move[0] - '0';
				 i = move[1] - 'a';
				 j = move[2] - '1';
				 height[i][j] -= stack_size;
				 switch(move[3]) {
				 	case '>': if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1])==2) {
				 				wallToFlat = true;
				 				z = (board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1] > 0) ? 1:4;
				 				board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1] /= 2;
				 				hash ^= zobrist[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1][z];
				 				hash ^= zobrist[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1][z-1];
				 			  }
				 			  for(it=4;it<move.length();it++) {
				 				step = move[it] - '0';
				 				it2 = step;
				 				while(it2>0) {
				 					
				 					board[i+it-3][j][height[i+it-3][j] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
				 					z = (board[i][j][height[i][j] + it2 - 1] > 0) ? board[i][j][height[i][j] + it2 - 1] - 1 : 2 - board[i][j][height[i][j] + it2 - 1];
				 					hash ^= zobrist[i][j][height[i][j] + it2 - 1][z];
				 					hash ^= zobrist[i+it-3][j][height[i+it-3][j] + it2 - 1][z];
				 					board[i][j][height[i][j] + it2 - 1] = 0;
				 					it2--;
				 				}
				 				height[i][j] += step;
				 				height[i+it-3][j] += step;
				 			  }
				 			  break;
				 			  
				 	case '<': if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1])==2) {
				 				wallToFlat = true;
				 				z = (board[i-move.length()+4][j][height[i-move.length()+4][j] - 1] > 0) ? 1:4;
				 				board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1] /= 2;
				 				hash ^= zobrist[i-move.length()+4][j][height[i-move.length()+4][j] -1][z];
				 				hash ^= zobrist[i-move.length()+4][j][height[i-move.length()+4][j] -1][z-1];

				 			  }
				 			  for(it=4;it<move.length();it++) {
				 				step = move[it] - '0';
				 				it2 = step;
				 				while(it2>0) {
				 					board[i-it+3][j][height[i-it+3][j] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
				 					z = (board[i][j][height[i][j] + it2 - 1] > 0) ? board[i][j][height[i][j] + it2 - 1] - 1 : 2 - board[i][j][height[i][j] + it2 - 1];
				 					hash ^= zobrist[i][j][height[i][j] + it2 - 1][z];
				 					hash ^= zobrist[i-it+3][j][height[i-it+3][j] + it2 - 1][z];
				 					board[i][j][height[i][j] + it2 - 1] = 0;
				 					it2--;
				 				}
				 				height[i][j] += step;
				 				height[i-it+3][j] += step;
				 			  }
				 			  break;

				 	case '+': if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1])==2) {
				 				wallToFlat = true;
				 				z = (board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1] > 0)? 1:4;
				 				board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1] /= 2;
				 				hash ^= zobrist[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1][z];
				 				hash ^= zobrist[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1][z-1];
				 			  }
				 			  for(it=4;it<move.length();it++) {
				 				step = move[it] - '0';
				 				it2 = step;
				 				while(it2>0) {
				 					board[i][j+it-3][height[i][j+it-3] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
				 					z = (board[i][j][height[i][j] + it2 - 1] > 0) ? board[i][j][height[i][j] + it2 - 1] - 1 : 2 - board[i][j][height[i][j] + it2 - 1];
				 					hash ^= zobrist[i][j][height[i][j] + it2 - 1][z];
				 					hash ^= zobrist[i][j+it-3][height[i][j+it-3] + it2 - 1][z];
				 					board[i][j][height[i][j] + it2 - 1] = 0;
				 					it2--;
				 				}
				 				height[i][j] += step;
				 				height[i][j+it-3] += step;
				 			  }
				 			  break;
				 			  
				 	default: if(move[move.length() - 1]=='1' && abs(board[i][j][height[i][j] + stack_size - 1])==3 && abs(board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1])==2) {
				 				wallToFlat = true;
				 				z = (board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1] > 0) ? 1:4;
				 				board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1] /= 2;
				 				hash ^= zobrist[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1][z];
				 				hash ^= zobrist[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1][z-1];
				 			  }
				 			  for(it=4;it<move.length();it++) {
				 				step = move[it] - '0';
				 				it2 = step;
				 				while(it2>0) {
				 					board[i][j-it+3][height[i][j-it+3] + it2 - 1] = board[i][j][height[i][j] + it2 - 1];
				 					z = (board[i][j][height[i][j] + it2 - 1] > 0) ? board[i][j][height[i][j] + it2 - 1] - 1 : 2 - board[i][j][height[i][j] + it2 - 1];
				 					hash ^= zobrist[i][j][height[i][j] + it2 - 1][z];
				 					hash ^= zobrist[i][j-it+3][height[i][j-it+3] + it2 - 1][z];
				 					board[i][j][height[i][j] + it2 - 1] = 0;
				 					it2--;
				 				}
				 				height[i][j] += step;
				 				height[i][j-it+3] += step;
				 			  }
				 			  
				 }
				 height[i][j] -= stack_size;
	}
	// print_board();
}


void gamestate::undo_move(string move, int player) //////// make changes for zobrist key
{
	
	int i,j,z;
	int it,it2,stack_size,step;
	int sign = (player==player_id)? 1:-1;
	if(isalpha(move[0])) {
		i = move[1] - 'a';
		j = move[2] - '1';
		z = (board[i][j][height[i][j]-1]>0) ? (board[i][j][height[i][j] - 1] - 1):(2 - board[i][j][height[i][j] - 1]);
		board[i][j][height[i][j]-1] = 0;
		hash ^= zobrist[i][j][height[i][j]-1][z];
		height[i][j]--;
		if(move[0]=='C')
			if (sign==1) myCapstones++; else otherCapstones++;
		else 
			if (sign==1) myFlatstones++; else otherFlatstones++;
				  
	} else {
		// cerr<<"undo "<<move<<endl;
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
	 						z = (board[i][j][height[i][j] - it2] > 0) ? board[i][j][height[i][j] - it2] - 1 : 2 - board[i][j][height[i][j] - it2];
				 			hash ^= zobrist[i][j][height[i][j] - it2][z];
				 			hash ^= zobrist[i+it-3][j][height[i+it-3][j] - it2][z];
				 			board[i+it-3][j][height[i+it-3][j] - it2] = 0;
				 			it2--;
						}
				 		height[i][j] -= step;
				 		height[i+it-3][j] -= step;
	 				  }
	 				  if(wallToFlat) {
	 				  	board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1] *= 2;
	 				  	z = (board[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1] > 0)?1:4;
	 				  	hash ^= zobrist[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1][z];
	 				  	hash ^= zobrist[i+move.length() - 4][j][height[i+move.length() - 4][j] - 1][z-1];
	 				  }
				 	  break;
				 			  
			case '<': for(it=move.length()-1;it>=4;it--) {
		 				step = move[it] - '0';
		 				it2 = step;
				 		while(it2>0) {
				 			board[i][j][height[i][j] - it2] = board[i-it+3][j][height[i-it+3][j] - it2];
							z = (board[i][j][height[i][j] - it2] > 0) ? board[i][j][height[i][j] - it2] - 1 : 2 - board[i][j][height[i][j] - it2];
				 			hash ^= zobrist[i][j][height[i][j] - it2][z];
				 			hash ^= zobrist[i-it+3][j][height[i-it+3][j] - it2][z];
				 			board[i-it+3][j][height[i-it+3][j] - it2] = 0;
		 					it2--;
			 			}
				 		height[i][j] -= step;
						height[i-it+3][j] -= step;
		 			  }
		 			  if(wallToFlat) {
		 			  	board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1] *= 2;
		 			  	z = (board[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1] > 0)?1:4;
	 				  	hash ^= zobrist[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1][z];
	 				  	hash ^= zobrist[i-move.length() + 4][j][height[i-move.length() + 4][j] - 1][z-1];
		 			  }
		 			  break;

			case '+': for(it=move.length()-1;it>=4;it--) {
				 		step = move[it] - '0';
				 		it2 = step;
		 				while(it2>0) {
							board[i][j][height[i][j] - it2] = board[i][j+it-3][height[i][j+it-3] - it2];
				 			z = (board[i][j][height[i][j] - it2] > 0) ? board[i][j][height[i][j] - it2] - 1 : 2 - board[i][j][height[i][j] - it2];
				 			hash ^= zobrist[i][j][height[i][j] - it2][z];
				 			hash ^= zobrist[i][j+it-3][height[i][j+it-3] - it2][z];
				 			board[i][j+it-3][height[i][j+it-3] - it2] = 0;
				 			it2--;
						}
		 				height[i][j] -= step;
		 				height[i][j+it-3] -= step;
				 	  }
				 	  if(wallToFlat) {
				 	  	board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1] *= 2;
				 	  	z = (board[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1] > 0)?1:4;
	 				  	hash ^= zobrist[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1][z];
	 				  	hash ^= zobrist[i][j+move.length() - 4][height[i][j+move.length() - 4] - 1][z-1];
				 	  }
					  break;
				 			  
		 	default: for(it=move.length()-1;it>=4;it--) {
				 		step = move[it] - '0';
				 		it2 = step;
						while(it2>0) {
		 					board[i][j][height[i][j] - it2] = board[i][j-it+3][height[i][j-it+3] - it2];
				 			z = (board[i][j][height[i][j] - it2] > 0) ? board[i][j][height[i][j] - it2] - 1 : 2 - board[i][j][height[i][j] - it2];
				 			hash ^= zobrist[i][j][height[i][j] - it2][z];
				 			hash ^= zobrist[i][j-it+3][height[i][j-it+3] - it2][z];
				 			board[i][j-it+3][height[i][j-it+3] - it2] = 0;
				 			it2--;
						}
		 				height[i][j] -= step;
		 				height[i][j-it+3] -= step;
				 	  }
				 	  if(wallToFlat) {
				 	  	board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1] *= 2;
				 	  	z = (board[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1] > 0)?1:4;
	 				  	hash ^= zobrist[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1][z];
	 				  	hash ^= zobrist[i][j-move.length() + 4][height[i][j-move.length() + 4] - 1][z-1];
				 	  }
				 			  
		}
		height[i][j] += stack_size;
	}

	// print_board();
}

void gamestate::getNeighbours(int i, int j, int len, vector<pair<int,int> > &neighbours)
{
	neighbours.clear();

	for (int l=1; l<=len; l++)
	{
		if (i+l<n) neighbours.push_back(make_pair(i+l,j));
		if (j+l<n) neighbours.push_back(make_pair(i,j+l));
		if (i-l>-1) neighbours.push_back(make_pair(i-l,j));
		if (j-l>-1) neighbours.push_back(make_pair(i,j-l));
	}
}

bool gamestate::dfs (int direction, int sign, int i, int j)
{
	visited[i][j] = 1;
	int ngb_i, ngb_j, ngb_h;

	if (direction == 1)
	{
		if (j==n-1)
			return true;
	}
	else 
	{
		if(i==n-1)
			return true;
	}

	vector<pair<int,int> > dfs_v; getNeighbours(i,j,1,dfs_v);
		for (int k=0; k<dfs_v.size(); k++)
		{
			ngb_i = dfs_v[k].first;
			ngb_j = dfs_v[k].second;
			ngb_h = height[ngb_i][ngb_j];
			if (ngb_h > 0 && visited[ngb_i][ngb_j]==0 && (board[ngb_i][ngb_j][ngb_h-1]==sign*1 || board[ngb_i][ngb_j][ngb_h-1]==sign*3))
				if(dfs(direction,sign,ngb_i,ngb_j))
					return true;
		}
		return false;
}

bool gamestate::road(int player)
{
	int i,j;
	for (i=0; i<n; i++)
	{
		for (j=0; j<n; j++)
			visited[i][j] = 0;
	}

	int sign = (player==player_id)?1:-1;
	int h = 0;
	for (i=0; i<n; i++)
	{
		h = height[i][0];
		if (h>0)
		{
			if (visited[i][0] == 0 && (board[i][0][h-1]==sign*1 || board[i][0][h-1]==sign*3))
				if (dfs(1,sign,i,0))
					return true;
		}
	}

	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			visited[i][j] = 0;

	for (j=0; j<n; j++)
	{
		h = height[0][j];
		if (h>0)
		{
			if (visited[0][j] == 0 && (board[0][j][h-1]==sign*1 || board[0][j][h-1]==sign*3))
				if (dfs(-1,sign,0,j))
					return true;
		}
	}
	return false;
}

int gamestate::over() ////////// make changes
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

uint64_t gamestate::getHash()
{
	// hash2 = 0;
	// int z = 0;
	// for (int i=0; i<n; i++)
	// {
	// 	for (int j=0; j<n; j++)
	// 	{
	// 		for (int k=0; k<height[i][j]; k++)
	// 		{
	// 		 /////////// 0 my flat, 1 my wall, 2 my cap, 3 opp flat, 4 opp wall, 5 opp cap
	// 			// switch(board[i][j][k]){
	// 			// 	case 1: z = 0; break;
	// 			// 	case 2: z = 1; break;
	// 			// 	case 3: z = 2; break;
	// 			// 	case -1: z = 3; break;
	// 			// 	case -2: z = 4; break;
	// 			// 	case -3: z = 5; break;
	// 			// 	default: break;
	// 			// }
	// 			z = (board[i][j][k] > 0) ? board[i][j][k] - 1 : -board[i][j][k] + 2;
	// 			hash2 ^= zobrist[i][j][k][z];
	// 		}
	// 	}
	// }
	// if(hash2!=hash) {
	// 	cerr<<"FALSE!!!!!!!!!!"<<endl;
	// 	exit(0);
	// } 
	// // else
	// // cerr<<"TRUE!!!!!!!!!!"<<endl;
	return hash;
}
