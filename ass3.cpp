#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

int player_id = 0;
int n = 0;
int tl = 0;

int ***board; //+-1 for flatstone, +-2 for wall, +-3 for capstone
int **height;
vector<string> moves;

int flatstones, capstones; //no. of unused pieces

void opponent_move(string move) {
	int i,j;
	int it,it2,stack_size,step;
	switch(move[0]) {
		case 'F': cerr<<move<<" oF\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = -1;
				  height[i][j]++;
				  break;
		case 'S': cerr<<move<<" oS\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = -2;
				  height[i][j]++;
				  break;
		case 'C': cerr<<move<<" oC\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = -3;
				  height[i][j]++;
				  break;
		default: cerr<<move<<" oStack\n";
				 stack_size = move[0] - '0';
				 i = move[1] - 'a';
				 j = move[2] - '1';
				 height[i][j] -= stack_size;
				 switch(move[3]) {
				 	case '>': for(it=4;it<move.length();it++) {
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
				 			  
				 	case '<': for(it=4;it<move.length();it++) {
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

				 	case '+': for(it=4;it<move.length();it++) {
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
				 			  
				 	default: for(it=4;it<move.length();it++) {
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

void my_move(string move) {
	int i,j;
	int it,it2,stack_size,step;
	switch(move[0]) {
		case 'F': cerr<<move<<" mF\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = 1;
				  height[i][j]++;
				  flatstones--;
				  break;
		case 'S': cerr<<move<<" mS\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = 2;
				  height[i][j]++;
				  flatstones--;
				  break;
		case 'C': cerr<<move<<" mC\n";
				  i = move[1] - 'a';
				  j = move[2] - '1';
				  board[i][j][height[i][j]] = 3;
				  height[i][j]++;
				  capstones--;
				  break;
		default: cerr<<move<<" mStack\n";
				 stack_size = move[0] - '0';
				 i = move[1] - 'a';
				 j = move[2] - '1';
				 height[i][j] -= stack_size;
				 switch(move[3]) {
				 	case '>': for(it=4;it<move.length();it++) {
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
				 			  
				 	case '<': for(it=4;it<move.length();it++) {
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

				 	case '+': for(it=4;it<move.length();it++) {
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
				 			  
				 	default: for(it=4;it<move.length();it++) {
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

void place_new() {
	int i,j;
	string str;
	char c;
	for(i=0;i<n;i++) {
		for(j=0;j<n;j++) {
			if(height[i][j] == 0) {
				str = "";
				c = i + 'a';
				str += c;
				str += to_string(j+1);
				if(flatstones>0) {
					moves.push_back("F"+str);
					moves.push_back("S"+str);
				}
				if(capstones>0)
					moves.push_back("C"+str);
			}
		}
	}
}

void move_stack(int i, int j, int total, int direction, string str, int top) { //direction is 0 for >, 1 for <, 2 for +, 3 for - /////top is 1 for flatstone, 2 for wall, 3 for capstone
	cerr<<str<<"\n";
	if(total==0) {
		moves.push_back(str);
		return;
	}
	int it1;
	switch(direction) {
		case 0: if(i==n-1) 
					return;
				if(abs(board[i+1][j][height[i+1][j] - 1] ) == 3)
					return;
				if(abs(board[i+1][j][height[i+1][j] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i+1,j,0,direction,str+to_string(total),top);					
					}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i+1,j,total-it1,direction,str+to_string(it1),top);
				}
				break;

		case 1: if(i==0) 
					return;
				if(abs(board[i-1][j][height[i-1][j] - 1] ) == 3)
					return;
				if(abs(board[i-1][j][height[i-1][j] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i-1,j,0,direction,str+to_string(total),top);					
					}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i-1,j,total-it1,direction,str+to_string(it1),top);
				}
				break;

		case 2: if(j==n-1) 
					return;
				if(abs(board[i][j+1][height[i][j+1] - 1] ) == 3)
					return;
				if(abs(board[i][j+1][height[i][j+1] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i,j+1,0,direction,str+to_string(total),top);					
					}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i,j+1,total-it1,direction,str+to_string(it1),top);
				}
				break;

		default: if(j==0) 
					return;
				if(abs(board[i][j-1][height[i][j-1] - 1] ) == 3)
					return;
				if(abs(board[i][j-1][height[i][j-1] - 1] ) == 2) {
					if(top==3 && total==1) {
						move_stack(i,j-1,0,direction,str+to_string(total),top);					
					}
					return;
				}
				for(it1=1;it1<=total;it1++) {
					move_stack(i,j-1,total-it1,direction,str+to_string(it1),top);
				}
	}

}

void generate_moves() {
	moves.clear();
	place_new();
	int i,j,k,max_stack_move;
	string str,str1;
	char c;
	for(i=0;i<n;i++) {
		for(j=0;j<n;j++) {
			if(height[i][j]>0 && board[i][j][height[i][j] - 1] > 0) {
				max_stack_move = min(n,height[i][j]);
				c = i + 'a';
				str = "";
				str += c;
				str += to_string(j+1);
				for(k=1;k<=max_stack_move;k++) {
					str1 = to_string(k) + str;
					move_stack(i,j,k, 0, str1+">",board[i][j][height[i][j] - 1]);
					move_stack(i,j,k, 1, str1+"<",board[i][j][height[i][j] - 1]);
					move_stack(i,j,k, 2, str1+"+",board[i][j][height[i][j] - 1]);
					move_stack(i,j,k, 3, str1+"-",board[i][j][height[i][j] - 1]);
				}
			}
		}
	}
}

int main() {

	srand(time(0));
	string str;
	int temp,i,j,k;
	char c;
	srand(time(0));

	////player no., board size, time in seconds/////
	getline(cin,str);
	i = 0;
	while(str[i]!=' ') {
		player_id = player_id*10 + (str[i] - '0');
		i++;
	}
	i++;
	while(str[i]!=' ') {
		n = n*10 + (str[i] - '0');
		i++;
	}
	i++;
	while(i<str.length()) {
		tl = tl*10 + (str[i] - '0');
		i++;
	}

	flatstones = 20;
	capstones = 1;

	board = new int**[n];
	height = new int*[n];
	for(i=0;i<n;i++) {
		board[i] = new int*[n];
		height[i] = new int[n];
		for(j=0;j<n;j++) {
			height[i][j] = 0;
			board[i][j] = new int[2*(flatstones)+capstones];
			for(k=0;k<n;k++) {
				board[i][j][k] = 0;
			}
		}
	}
	///////////////////////////////////////////
	
	if(player_id==1) {
		moves.clear();

		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) {
				if(height[i][j] == 0) {
					str = "";
					c = i + 'a';
					str += c;
					str += to_string(j+1);
					moves.push_back("F"+str);
				}
			}

		}


		temp = rand()%moves.size();
		str = moves[temp];
		cout<<str<<endl;
		my_move(str);
	}

	
	while(true) {
		cerr<<"getline before\n";
		getline(cin,str);
		cerr<<"getline after\n";
		opponent_move(str);
		cerr<<"opponent_move after\n";

		generate_moves();
		if(moves.size()==0)
			return 0;
		temp = rand()%moves.size();
		str = moves[temp];
			
		
		cout<<str<<endl;
		my_move(str);
	}
	return 0;
}