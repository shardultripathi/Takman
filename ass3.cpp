#include "gamestate.h"
#include "getmoves.h"
#include "minimax.h"

// #define neg	std::numeric_limits<int>::min()
// #define pos	std::numeric_limits<int>::max()
#define neg -100000
#define pos 100000

using namespace std;

int player_id = 0;
int n = 0;
int tl = 0;

int main() {
	
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

	auto start = std::chrono::system_clock::now();

	gamestate *game = new gamestate(n,tl,player_id);

	/////////////////////////////////////////
	
	if(player_id==1) 
	{
		srand(time(NULL));
		int a = rand()%5;
		int b = rand()%5;
		switch(a)
		{
			case 0: str = "Fa" + to_string(b+1); break;
			case 1: str = "Fb" + to_string(b+1); break;
			case 2: str = "Fc" + to_string(b+1); break;
			case 3: str = "Fd" + to_string(b+1); break;
			case 4: str = "Fe" + to_string(b+1); break;
			default: break;
		}
		
		
		// str = "Fa1";
		cout<<str<<endl;
		game->update_board(str,2);

		getline(cin,str);
		game->update_board(str,1);

		str = ids(game).second;
		// SearchData data = ids(game);
			
		cout<<str<<endl;
		game->update_board(str,player_id);
	} 
	else 
	{
		getline(cin,str);
		game->update_board(str,2);
		if (game->height[0][0]==0)
			str = "Fa1";
		else
			str = "Fa5";
		cout<<str<<endl;
		game->update_board(str,1);
	}

	int opp = (player_id==1)?2:1; 
	pair<int,string> t;
	
	while(true) {

		getline(cin,str); count++;
		game->update_board(str,opp);

		t = ids(game);
		// SearchData data = ids(game);

		str = t.second;
		cout<<str<<endl;
		game->update_board(str,player_id);
	}

	// {"e1", "a5", "Fb5", "Cb4", "Fa2", "Fe3", "Fa3", "Fa4", "Fc4", "Fc5", "Fd5", "Fd4", "Fe5", "1b4>1", "1a5-1", "Fe2", "1b5>1", "2c4+2", "Ca5"}

	// string strs[] = {"Fa1","Fa5","Fd4","Fd3","Fc4","Fd2","Fb4","Fc2","Fb5","Fe4","Fc5","1e4<1","Fd5","Fe5","Ce4","1e5<1"};
	// int bla;
	// str = strs[0];
	// game->update_board(str,2);
	// game->print_board();
	// str = strs[1];
	// game->update_board(str,1);
	// game->print_board();
	// for(bla=2;bla<16;bla++) {
	// 	str = strs[bla];
	// 	game->update_board(str,1+bla%2);
	// 	game->print_board();
	// 	cout<<endl;
	// }
	// // cout<<value(game,3,0,-65536,65536,false).first<<endl;
	// // // cout<<eval(game)<<endl;
	// // vector<string> moves;
	// // generate_moves(moves,game,game->other_player);
	// // for(bla=0;bla<moves.size();bla++) {
	// // 	cout<<moves[bla]<<" ";
	// // }
	// pair<int,string> t;
	// t =  value(game,5,0,-655360,655360,true);
	// str = t.second;
	// cout<<str<<endl;
	// game->update_board(str,player_id);



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