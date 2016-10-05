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
	int count=0;

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

	/////////////////////////////////////////
	
	if(player_id==1) {

		str = "Fa1";
		cout<<str<<endl;
		game->update_board(str,2);
		// cerr<<"player 1 first move"<<endl<<endl;
		// game->print_board();
		// my_move(str);

		getline(cin,str);
		// cerr<<"getline after\n";
		game->update_board(str,1);
		// cerr<<"opponent_move"<<endl<<endl;
		// game->print_board();

		str =  value(game,3,0,-655360,655360,true).second;
			
		// cerr<<str<<"\n";
		cout<<str<<endl;
		game->update_board(str,player_id);
		// cerr<<"my move"<<endl<<endl;
		// game->print_board();
	} else {
		getline(cin,str);
		game->update_board(str,2);
		if (game->height[0][0]==0)
			str = "Fa1";
		else
			str = "Fa5";
		cout<<str<<endl;
		game->update_board(str,1);
		// cerr<<"player 2 first move"<<endl<<endl;
		// game->print_board();
		// my_move(str);
	}

	int opp = (player_id==1)?2:1; 
	pair<int,string> t;
	
	while(true) {
		// cerr<<"getline before\n";
		getline(cin,str); count++;
		// cerr<<"getline after\n";
		game->update_board(str,opp);
		// cerr<<"opponent_move"<<endl<<endl;
		// game->print_board();
		// cerr<<endl<<endl;
		// if (game->over() != 4)
		// 	break;

		
		if (count<7)
			t =  value(game,4,0,-65536,65536,true);
		else 
			t =  value(game,5,0,-65536,65536,true);
		// cerr<<"My move evaluation: "<<t.first<<endl;
			
		// cerr<<str<<"\n";
		str = t.second;
		cout<<str<<endl;
		// cerr<<endl;
		game->update_board(str,player_id);
		// game->print_board();
		// if (game->over() != 4)
		// 	break;

		// my_move(str);
	}

	// string strs[] = {"Fa1","Fa2","Fc3","1a1+1","Fc4","Fa1","Fd4","Fa3","Fd3","Fa4","Fa5","Cb4","Cb5","1b4<1","1b5<1"};
	// int bla;
	// str = strs[0];
	// game->update_board(str,2);
	// game->print_board();
	// str = strs[1];
	// game->update_board(str,1);
	// game->print_board();
	// for(bla=2;bla<15;bla++) {
	// 	str = strs[bla];
	// 	game->update_board(str,1+bla%2);
	// 	game->print_board();
	// 	cout<<endl;
	// }
	// cout<<value(game,3,0,-65536,65536,false).first<<endl;
	// // cout<<eval(game)<<endl;
	// vector<string> moves;
	// generate_moves(moves,game,game->other_player);
	// for(bla=0;bla<moves.size();bla++) {
	// 	cout<<moves[bla]<<" ";
	// }
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