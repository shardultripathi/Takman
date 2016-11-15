#include "gamestate.h"
#include "getmoves.h"
#include "minimax.h"
#include <chrono>

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

	srand(time(NULL));

	int a = rand()%n;
	int b = rand()%n;
	
	update_start_time(n,tl*1.0);

	/////////////////////////////////////////
	
	if(player_id==1) 
	{
		c = a+'a';
		str = "F";
		str += c;
		str += to_string(b+1);
		// str = "Fa5";
		// cerr<<str<<endl;
		
		// str = "Fd4";
		cout<<str<<endl;
		game->update_board(str,2);
		// game->print_board();

		getline(cin,str);
		game->update_board(str,1);

		str = ids(game).second;
		// SearchData data = ids(game);
			
		cout<<str<<endl;
		game->update_board(str,player_id);
		// game->print_board();
	} 
	else 
	{
		getline(cin,str);
		game->update_board(str,2);
		// game->print_board();
		// if (game->height[a][b]==0)
		// 	str = "Fa1";
		// else
		// 	str = "Fa5";

		while(game->height[a][b]!=0) {
			a = rand()%n;
			b = rand()%n;
		}
		c = a+'a';
		str = "F";
		str += c;
		str += to_string(b+1);
		// cerr<<str<<endl;
		cout<<str<<endl;
		game->update_board(str,1);
		// game->print_board();
	}

	int opp = (player_id==1)?2:1; 
	pair<int,string> t;
	
	while(true) {

		getline(cin,str); count++;
		game->update_board(str,opp);
		// game->print_board();

		t = ids(game);
		// SearchData data = ids(game);

		str = t.second;
		cout<<str<<endl;
		game->update_board(str,player_id);
		// game->print_board();
	}

	
	
	return 0;
}