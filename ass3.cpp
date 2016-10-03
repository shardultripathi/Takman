#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "gamestate.h"
#include "getmoves.h"
#include "minimax.h"

using namespace std;

int player_id = 0;
int n = 0;
int tl = 0;

// int ***board; //+-1 for flatstone, +-2 for wall, +-3 for capstone
// int **height;
vector<string> moves;
vector<string> *movesptr;


int main() {

	srand(time(0));
	string str;
	int temp,i,j,k;
	char c;

	//player no., board size, time in seconds/////
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

	// player_id = 1;
	// n = 5;
	// tl = 120;


	gamestate *game = new gamestate(n,tl,player_id);
	movesptr = &moves;


	// flatstones = 20;
	// capstones = 1;

	// board = new int**[n];
	// height = new int*[n];
	// for(i=0;i<n;i++) {
	// 	board[i] = new int*[n];
	// 	height[i] = new int[n];
	// 	for(j=0;j<n;j++) {
	// 		height[i][j] = 0;
	// 		board[i][j] = new int[2*(flatstones)+capstones];
	// 		for(k=0;k<n;k++) {
	// 			board[i][j][k] = 0;
	// 		}
	// 	}
	// }
	///////////////////////////////////////////
	
	if(player_id==1) {
		moves.clear();

		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) {
				if(game->height[i][j] == 0) {
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
		game->update_board(str,2);
		cerr<<"player 1 first move"<<endl<<endl;
		game->print_board();
		// my_move(str);

		getline(cin,str);
		// cerr<<"getline after\n";
		game->update_board(str,1);
		cerr<<"opponent_move"<<endl<<endl;
		game->print_board();
		// opponent_move(str);
		// cerr<<"opponent_move after\n";

		// generate_moves(moves,game);
		// if(moves.size()==0)
		// 	return 0;
		// temp = rand()%moves.size();
		// str = moves[temp];

		str =  value(game,4,0,-65536,65536,true).second;
			
		// cerr<<str<<"\n";
		cout<<str<<endl;
		game->update_board(str,player_id);
		cerr<<"my move"<<endl<<endl;
		game->print_board();
	} else {
		getline(cin,str);
		game->update_board(str,2);
		// opponent_move(str);
		moves.clear();

		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) {
				if(game->height[i][j] == 0) {
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
		game->update_board(str,1);
		cerr<<"player 2 first move"<<endl<<endl;
		game->print_board();
		// my_move(str);
	}

	int opp = (player_id==1)?2:1; 
	
	while(true) {
		// cerr<<"getline before\n";
		getline(cin,str);
		// cerr<<"getline after\n";
		game->update_board(str,opp);
		cerr<<"opponent_move"<<endl<<endl;
		game->print_board();
		// opponent_move(str);
		// cerr<<"opponent_move after\n";

		// generate_moves(moves,game);
		// if(moves.size()==0)
		// 	return 0;
		// temp = rand()%moves.size();
		// str = moves[temp];

		str =  value(game,4,0,-65536,65536,true).second;
			
		// cerr<<str<<"\n";
		cout<<str<<endl;
		game->update_board(str,player_id);
		cerr<<"my move"<<endl<<endl;
		game->print_board();
		// my_move(str);
	}
	// str = "Fe2";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();

	// str = "Fe1";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "Se3";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "Se4";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "Fb2";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "Fc2";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "Fa2";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "Sb3";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "Fb1";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "Fb5";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "1a2>1";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "Fa5";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "1b1+1";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "1a5>1";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "Cc5";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "1c2<1";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "1c5<1";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	
	// str = "Cd1";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();

	// str = "2b5-11";
	// cout<<str<<endl;
	// game->update_board(str,1);
	// game->print_board();
	// bool b1 = game->wallToFlat;

	// str = "4b2>211";
	// cout<<str<<endl;
	// game->update_board(str,2);
	// game->print_board();
	// bool b2 = game->wallToFlat;
	

	// string arr[] = {"4b2>211","2b5-11","Cd1","1c5<1","1c2<1","Cc5","1a5>1","1b1+1","Fa5","1a2>1","Fb5","Fb1","Sb3","Fa2","Fc2","Fb2","Se4","Se3","Fe1","Fe2"};

	// for(int bla=0;bla<20;bla++) {
	// 	if(bla==0)
	// 		game->wallToFlat = b2;
	// 	else if(bla==1)
	// 		game->wallToFlat = b1;
	// 	str = arr[bla];
	// 	cout<<"Undo "<<str<<endl;
	// 	game->undo_move(str,2 - (bla%2));
	// 	game->print_board();

	// }
	


	return 0;
}