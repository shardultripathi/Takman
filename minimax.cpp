#include "gamestate.h"
#include "getmoves.h"
#include "minimax.h"

// #define neg	std::numeric_limits<int>::min()
// #define pos	std::numeric_limits<int>::max()
#define neg -1000000
#define pos 1000000
#define ttsize 47901599
#define hsize 167923

// #define fc 100
// #define cs 85
// // #define w 40
// #define w 30
// #define c 12
// #define ccap 15
// #define s 15
// #define inf 10
// #define wallpenalty 10


int fc = 100;
int cs = 85;
int w = 30;
// int w = 40;
int c = 12;
int ccap = 15;
int s = 15;
int inf = 10;
int wallpenalty = 10;


using namespace std;

int height;
int i_ngb, j_ngb, h_ngb;
int i_lcrt,i_rcrt;
int j_lcrt,j_rcrt;
int peice, opp_peice;

vector<pair<int,int> > neighbours;	///////////////////////////////
vector<string> d_moves;
int dmoves_size;

int nodes = 0;
int flatcount = 0;
int centre_control = 0;
int stack_color = 0;
int stack = 0;
int total_influence = 0;
int wall_dis = 0;
int mycolor=0; int oppcolor=0;
int row = 0; int col = 0;
int row_num = 0;
int col_num = 0;
int composition = 0;
int row_influence = 0; int col_influence = 0;

int influence_table[7][7];	///////////////////////////////////

// int infl[7][7] =
//     {
// //    	  0  1  2  3  -1  -2  -3
// /* 0 */  {0, 0, 0, 0,  0,  0, 0},
// /* 1 */  {0, 1, 0, 1,  1,  0, 0},
// /* 2 */  {0, 1, 0, 0,  1,  0, 0},
// /* 3 */  {0, 1, 1, 1,  1,  1, 0},
// /* -1 */ {0,-1,0, 0, -1,  0, -1},
// /* -2 */ {0,-1,0, 0, -1,  0, 0},
// /* -3 */ {0,-1,-1,-1, -1, -1, 0}
//     };
//     //0,1,2,3 -> 0,1,2,3
//     //-1->4 , -2->5, -3->6
//     //p<0?-p+3:p

int infl[7][7] =
    {
//    	  0  1  2  3  -1  -2  -3
/* 0 */  {0, 0, 0, 0,  0,  0, 0},
/* 1 */  {0, 1, 0, 1,  1,  0, 0},
/* 2 */  {0, 0, 0, 0,  1,  0, 0},
/* 3 */  {0, 1, 1, 1,  1,  1, 0},
/* -1 */ {0,-1,0, 0, 0,  0, -1},
/* -2 */ {0,-1,0, 0, -1,  0, 0},
/* -3 */ {0,-1,-1,-1, -1, -1, 0}
    };
    //0,1,2,3 -> 0,1,2,3
    //-1->4 , -2->5, -3->6
    //p<0?-p+3:p



std::chrono::system_clock::time_point start_time;

double remaining_time;

void update_start_time(double tl)
{
	start_time = std::chrono::system_clock::now();
	remaining_time = tl*1000;
}

struct evalInfo 
{
	entry_type type;
	int value;
	int depth;
	string bmove;
	evalInfo() : type(exact), value(0), depth(0), bmove("") { }
	evalInfo(entry_type t, int v, int d, string bms) : type(t), value(v), depth(d), bmove(bms) { }
};

struct EvalInfo 
{
	// entry_type type;
	int lbound;
	int ubound;
	int depth;
	string bmove;
	EvalInfo() : lbound(neg), ubound(pos), depth(0), bmove("") { }
	EvalInfo(int l, int u, int d, string bms) : lbound(l), ubound(u), depth(d), bmove(bms) { }
};

// unordered_map<uint64_t,evalInfo> ttable(ttsize);
unordered_map<uint64_t,EvalInfo> ttable(ttsize);
unordered_map<string,int> history(hsize);
unordered_map<string,int> historyopp(hsize);

// unordered_map<string,int> myflatcap(257);
// unordered_map<string,int> mystack(7919);
// unordered_map<string,int> mystanding(257);
// unordered_map<string,int> oppflatcap(257);
// unordered_map<string,int> oppstack(7919);
// unordered_map<string,int> oppstanding(257);

int flatwin(gamestate* game) //// change in case of draw
{
	int delta = 1;
	int myflats = 0;
	int otherflats = 0;
	int n = game->n;
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
		{
			if (game->board[i][j][game->height[i][j]-1]==1||game->board[i][j][game->height[i][j]-1]==3)
				myflats++;
			if (game->board[i][j][game->height[i][j]-1]==-1||game->board[i][j][game->height[i][j]-1]==-3)
				otherflats++;
		}
	// if (maxNode)
	// 	return (myflats-otherflats-delta)*pos/(myflats+otherflats); ///////// also return difference of leftover flatstones (used in case of draw)
	// else
	// 	return (myflats-otherflats+delta)*pos/(myflats+otherflats);
		return (myflats-otherflats)*pos/(myflats+otherflats) + game->myFlatstones - game->otherFlatstones;
}

int eval(gamestate* game) ///// check and tune
{
	flatcount = 0;
 	centre_control = 0;
  	stack_color = 0;
  	stack = 0;
	total_influence = 0;
	wall_dis = 0;
  	mycolor=0;
  	oppcolor=0;
  	int row_num = 0;
	int col_num = 0;
	int composition = 0;
  
  	int n = game->n;
  	for (int i=0; i<n; i++)
 		for (int j=0; j<n; j++)
 			influence_table[i][j]=0;
 	
 	for (int i=0; i<n; i++)
 	{
 		for (int j=0; j<n; j++)
 		{
 			height = game->height[i][j];
 			if (!height) continue;
			peice = game->board[i][j][height-1];
			switch(peice)
			{
				case 1: flatcount += fc; if (i>0 && i<n-1 && j>0 && j<n-1) centre_control += c; break;
				case -1: flatcount -= fc; if (i>0 && i<n-1 && j>0 && j<n-1) centre_control -= c; break;
				case 2: flatcount += w; break;
				case -2: flatcount -= w; break;
				case 3: flatcount += cs; if (i>0 && i<n-1 && j>0 && j<n-1) centre_control += ccap; break;
				case -3: flatcount -= cs; if (i>0 && i<n-1 && j>0 && j<n-1) centre_control -= ccap; break;
				default: break;
			}
 
 			if (height > 1) //// stack
 			{
 				stack = 0;
 				mycolor = 0;
 				oppcolor = 0;
 				for (int h=0; h<height-1; h++)
 				{
 					if (game->board[i][j][h] == 1)
 						mycolor++;
 					else
 						oppcolor++;
 				}
 				stack = mycolor*s - oppcolor*s;
 				if (peice==1) stack += (stack>0)?2*s:-2*s;
 				else if (peice==-1) stack -= (stack>0)?-2*s:2*s;
 				else if (peice==3) stack += (stack>0)?s*5:-3*s;
 				else if (peice==-3) stack -= (stack>0)?-3*s:s*5;
 				else if (peice==2) stack += (stack>0)?3*s:-2*s;
 				else stack -= (stack>0)?-2*s:3*s;
 				stack_color += stack;
 
 				if (mycolor>0 && peice > 0)
 				{
 					game->getNeighbours(i,j,mycolor+1,neighbours);
 					for (int k=0; k<neighbours.size(); k++)
 					{
 						i_ngb = neighbours[k].first;
 						j_ngb = neighbours[k].second;
 						h_ngb = game->height[i_ngb][j_ngb];
 						opp_peice = h_ngb>0 ? game->board[i_ngb][j_ngb][game->height[i_ngb][j_ngb]-1] : 0;
 						if (peice==1 || peice==2)
 						{
 							if (opp_peice==0 || opp_peice==-1)
 							 	influence_table[i_ngb][j_ngb]++;
 							else if (abs(opp_peice)>1)
 							{
 								influence_table[i_ngb][j_ngb]--;
 							}
 						}
 						else
 						{
 							if (opp_peice==0 || opp_peice==-1 || (opp_peice==-2 && game->board[i][j][height-2]>0))
 							 	influence_table[i_ngb][j_ngb]++;
 							else if (abs(opp_peice)>1)
 							{
 								influence_table[i_ngb][j_ngb]--;
 							}
 						}
 						if (abs(i+j-i_ngb-j_ngb)==1)
 						{
 							if (opp_peice>=2)
 							 	wall_dis -= mycolor*wallpenalty;
 							else if (opp_peice <= -2)
 							{
 							 	wall_dis -= oppcolor*wallpenalty;
 							}
 						}
 					}
 					
 				}
 				else if (oppcolor>0 && peice < 0)
 				{
 					game->getNeighbours(i,j,oppcolor+1,neighbours);
 					for (int k=0; k<neighbours.size(); k++)
 					{
 						i_ngb = neighbours[k].first;
 						j_ngb = neighbours[k].second;
 						h_ngb = game->height[i_ngb][j_ngb];
 						opp_peice = h_ngb>0 ? game->board[i_ngb][j_ngb][game->height[i_ngb][j_ngb]-1] : 0;
 						if (peice==-1 || peice==-2)
 						{
 							if (opp_peice==0 && opp_peice==1)
 								influence_table[i_ngb][j_ngb]--;
 							else if (abs(opp_peice)>1)
 							{
 								influence_table[i_ngb][j_ngb]++;
 							}	
 						}
 							
 						else
 						{
 							if (opp_peice==0 || opp_peice==1 || (opp_peice==2 && game->board[i][j][height-2]<0))
 								influence_table[i_ngb][j_ngb]--;
 							else if (abs(opp_peice)>1)
 							{
 								influence_table[i_ngb][j_ngb]++;
 							}	
 						}
 							
 						if (abs(i+j-i_ngb-j_ngb)==1)
 						{
 							if (opp_peice>=2)
 								wall_dis += mycolor*wallpenalty;
 							else if (opp_peice <= -2)
 							{
 								wall_dis += oppcolor*wallpenalty;	
 							}
 						}
 							
 					}	
 				}
 
 			}
 			// game->getNeighbours(i,j,1,neighbours);
 			if (height==1)
			{
				game->getNeighbours(i,j,1,neighbours);
				for (int k=0; k<neighbours.size(); k++)
				{
					i_ngb = neighbours[k].first;
					j_ngb = neighbours[k].second;
					h_ngb = game->height[i_ngb][j_ngb];
					opp_peice = h_ngb>0?game->board[i_ngb][j_ngb][h_ngb-1]:0;

					influence_table[i_ngb][j_ngb] += infl[peice<0?-peice+3:peice][opp_peice<0?3-opp_peice:opp_peice];
				}			
			}
 		}
 	}
 
 	row = 0; col = 0;
 	row_influence = 0; col_influence = 0;
 
 	for (int i=0; i<n; i++)
 	{
 		row = 0;
 		row_num = 0;
 		for (int j=0; j<n; j++)
 		{
 			peice = (game->height[i][j]>0) ? game->board[i][j][game->height[i][j]-1] : 0;
 			if (peice == 1 || peice == 3)
 				row_num++;
 			else if (peice == -1 || peice == -3)
 				row_num--;
 			composition += row_num*row_num*row_num;
 			total_influence += influence_table[i][j]*inf;
 			row += influence_table[i][j];
 		}
 		if (3*row>=4*n)
 			row_influence += 2*inf*row;
 		if (3*row<=-4*n)
 			row_influence -= 2*inf*row;
 	}
 	for (int j=0; j<n; j++)
 	{
 		col = 0;
 		col_num = 0;
 		for (int i=0; i<n; i++)
 		{
 			peice = (game->height[i][j]>0) ? game->board[i][j][game->height[i][j]-1] : 0;
 			if (peice == 1 || peice == 3)
 				col_num++;
 			else if (peice == -1 || peice == -3)
 				col_num--;
 			composition += col_num*col_num*col_num;

 			col += influence_table[i][j];;
 		}
 		if (3*col>=4*n)
 			col_influence += 2*inf*col;
 		if (3*col<=-4*n)
 			col_influence -= 2*inf*col;
 	}
 
 	return (8*flatcount + 2*centre_control + 3*stack_color + 3*total_influence + 4*row_influence + 4*col_influence + wall_dis + 0.75*composition);
 	// return (16*flatcount + 5*centre_control + 2.2*stack_color + 1.9*total_influence + 5*row_influence + 5*col_influence + 0.3*wall_dis + 0.5*composition);
}

bool myComparison(const pair<string,int> &a,const pair<string,int> &b)
{
	return a.second>b.second;
}
bool myComparison2(const pair<string,int> &a,const pair<string,int> &b)
{
	return a.second<b.second;
}
bool moveComp(const string& a, const string& b)
{
	return history[a]>history[b];
}
bool moveCompopp(const string& a, const string& b)
{
	return historyopp[a]>historyopp[b];
}

// bool myplacecomp(const string& a, const string& b) ///// place
// {
// 	return myflatcap[a]>myflatcap[b];
// }
// bool mystackcomp(const string& a, const string& b) ///// stack
// {
// 	return mystack[a]>mystack[b];
// }
// bool mystandingcomp(const string& a, const string& b) ////// standing
// {
// 	return mystanding[a]>mystanding[b];
// }
// bool oppplacecomp(const string& a, const string& b)
// {
// 	return oppflatcap[a]>oppflatcap[b];
// }
// bool oppstackcomp(const string& a, const string& b)
// {
// 	return oppstack[a]>oppstack[b];
// }
// bool oppstandingcomp(const string& a, const string& b)
// {
// 	return oppstanding[a]>oppstanding[b];
// }

// int mtdf (gamestate* game, int f, int d)
// {
// 	int g = f;
// 	int beta;
// 	int u = pos;
// 	int l = neg;
// 	while (l < u)
// 	{
// 		if (g == l) beta = g + 1;
// 		else beta = g;
// 		// g = abpruning(game,beta-1,beta,d,true);
// 		g = negamax(game,beta-1,beta,d,true);
// 		if (g < beta) 
// 			u = g;
// 		else
// 			l = g;
// 	}
// 	return g;
// }

auto start_move = std::chrono::system_clock::now();
auto end_move = std::chrono::system_clock::now();

vector<string> moves56237;

pair<int,string> ids(gamestate* game)
{
	ttable.clear();
	// myflatcap.clear();
	// mystack.clear();
	// mystanding.clear();
	// oppflatcap.clear();
	// oppstack.clear();
	// oppstanding.clear();
	history.clear();
	historyopp.clear();
	static int count = 0;
	
	int guess = 0;
	double tm = 0.0;
	string move;
	double br = 0.0;

	static int dlimit = 6;
	// if (count<3) dlimit = 3; else dlimit = 6;
	static int rem_moves = 1.5*std::min(game->myFlatstones,game->otherFlatstones) + 3;
	static double aim = 1000 + remaining_time/rem_moves;
	if (std::min(game->myFlatstones,game->otherFlatstones) < 5)
	{
		fc = 150;
		if (remaining_time < 20000)
			aim -= 1000;
	}
	// auto present = std::chrono::system_clock::now();
	// remaining_time -= std::chrono::duration_cast<std::chrono::milliseconds>(present - start_time).count();
	

	start_move = std::chrono::system_clock::now();;
	for (int d=1; d<=dlimit; d++)
	{
		if (guess == pos)
			break;
		nodes = 0;
		if (tm*(br) > aim)
			break;
		auto start = std::chrono::system_clock::now();
		cerr<<"depth: "<<d;
		// guess = mtdf(game,guess,d);
		guess = abtt(game,neg,pos,d,true);
		// guess = abpruning(game,neg,pos,d,true);
		// guess = negamax(game,neg,pos,d,true);
		// guess = negatest(game,neg,pos,d,true);
		auto end = std::chrono::system_clock::now();
		tm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		move = (&(ttable.find(game->getHash())->second))->bmove;
		br = pow(nodes,1.0/(d+1));
		cerr<<" abttmove: "<<move;
		cerr<<" val: "<<guess/25;
		// cerr<<" ubound: "<<(&(ttable.find(game->getHash())->second))->ubound;
		// cerr<<" lbound: "<<(&(ttable.find(game->getHash())->second))->lbound;
		cerr<<" time: "<<tm/1000;
		cerr<<" branching: "<<br;
		cerr<<" nodes: "<<nodes<<endl;
		// string pv = "[ ";
		// getpv(pv,game,d,game->player_id);
		// cerr<<" pv: "<<pv<<"]"<<endl;
	}
	end_move = std::chrono::system_clock::now();
	// cerr<<"time taken: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end_move - start_move).count()/1000<<endl;
	remaining_time -= std::chrono::duration_cast<std::chrono::milliseconds>(end_move - start_move).count();
	// cerr<<"REMAINING: "<<remaining_time/1000<<endl;
	count++;
	return make_pair(guess,move);
}



void getpv(string& pv, gamestate* game, int depth, int turn)
{
	if (depth<1)
		return;
	bool localW2F;
	auto e = ttable.find(game->getHash());
	if (e == ttable.end())
	{
		pv += "mila ni bhencho";
		return;
	}
	string bm = (&(e->second))->bmove;
	pv += bm + " ";
	game->update_board(bm,turn);
	localW2F = game->wallToFlat;
	getpv(pv,game,depth-1,turn%2 + 1);
	game->wallToFlat = localW2F;
	game->undo_move(bm,turn);
}

// int negamax(gamestate* game, int alpha, int beta, int depth, bool maxNode)getH// {
// 	nodes++;
// 	int alphaOrig = alpha;
// 	auto entry = ttable.find(game->hash());
// 	evalInfo* info;
// 	string bestmove = "";

// 	if (entry != ttable.end())
// 	{
// 		info = &(entry->second);
// 		if (info->depth >= depth)
// 		{
// 			if (info->type == exact)
// 				return info->value;
// 			else if (info->type == lowerbound)
// 				alpha = std::max(alpha,info->value);
// 			else if (info->type == upperbound)
// 				beta = std::min(beta,info->value);
// 			if (alpha >= beta)
// 				return info->value;
// 		}
// 		bestmove = info->bmove;
// 	}
// 	int color = maxNode ? 1:-1;
// 	int game_over = game->over(maxNode);
// 	if (game_over == 1)
// 		return color*pos;
// 	else if (game_over == -1)
// 		return color*neg;
// 	else if (game_over == 2 || game_over == 3)
// 		return color*flatwin(game);
// 	else if (depth == 0)
// 		return color*eval(game);
	
// 	auto player = maxNode ? game->player_id : game->other_player;
// 	int bestValue = neg;
// 	bool localW2F;
// 	vector<string> moves; generate_moves(moves,game,player);
// 	int moves_size = moves.size();
// 	int v=0;

// 	vector<pair<string,int> > sorted_moves;
// 		for (int i=0; i<moves_size; i++)
// 		{
// 			v = (maxNode)?history[moves[i]]:historyopp[moves[i]];
// 			sorted_moves.push_back(make_pair(moves[i],v));
// 		}
// 		sort(sorted_moves.begin(),sorted_moves.end(),myComparison);

// 	if (bestmove == "")
// 		bestmove = sorted_moves[0].first;

// 	game->update_board(bestmove,player);
// 	localW2F = game->wallToFlat;
// 	v = -negamax(game,-beta,-alpha,depth-1,!maxNode);
// 	game->wallToFlat = localW2F;
// 	game->undo_move(bestmove,player);
// 	bestValue = std::max(bestValue,v);
// 	alpha = std::max(alpha,v);
// 	if (alpha >= beta)
// 		goto hell;

// 	for (int i=0; i<moves_size; i++)
// 	{
// 		if (sorted_moves[i].first == bestmove) continue;

// 		game->update_board(sorted_moves[i].first,player);
// 		localW2F = game->wallToFlat;

// 		// v = -negamax(game,-alpha-1,-alpha,depth-1,!maxNode);

// 		// if (v > alpha && v < beta && depth > 1) 
//   // 			v = std::max(v,-negamax(game,-beta,-v,depth-1,!maxNode));
		
// 		v = -negamax(game,-beta,-alpha,depth-1,!maxNode);
		
// 		game->wallToFlat = localW2F;
// 		game->undo_move(sorted_moves[i].first,player);

// 		if (v > bestValue)
// 		{
// 			bestValue = v;
// 			bestmove = sorted_moves[i].first;
// 		}
		
// 		if (bestValue >= beta)
// 			break;
		
// 		alpha = std::max(alpha,bestValue);
// 	}

// 	hell:
// 	// history[bestmove] += pow(2,depth);
// 	maxNode?history[bestmove]:historyopp[bestmove] += depth*depth;
// 	if (bestValue <= alphaOrig)
// 		storeEntry(game->hash(),bestValue,upperbound,depth,bestmove);
// 	else if (bestValue >= beta)
// 		storeEntry(game->hash(),bestValue,lowerbound,depth,bestmove);
// 	else
// 		storeEntry(game->hash(),bestValue,exact,depth,bestmove);
// 	return bestValue;
// }

// int negatest(gamestate* game, int alpha, int beta, int depth, bool maxNode)getH// {
// 	nodes++;
// 	int alphaOrig = alpha;
// 	auto entry = ttable.find(game->hash());
// 	evalInfo* info;
// 	string bestmove = "";

// 	if (entry != ttable.end())
// 	{
// 		info = &(entry->second);
// 		if (info->depth >= depth)
// 		{
// 			if (info->type == exact)
// 				return info->value;
// 			else if (info->type == lowerbound)
// 				alpha = std::max(alpha,info->value);
// 			else if (info->type == upperbound)
// 				beta = std::min(beta,info->value);
// 			if (alpha >= beta)
// 				return info->value;
// 		}
// 		bestmove = info->bmove;
// 	}
// 	int color = maxNode ? 1:-1;
// 	int game_over = game->over(maxNode);
// 	if (game_over == 1)
// 		return color*pos;
// 	else if (game_over == -1)
// 		return color*neg;
// 	else if (game_over == 2 || game_over == 3)
// 		return color*flatwin(game);
// 	else if (depth == 0)
// 		return color*eval(game);
	
// 	auto player = maxNode ? game->player_id : game->other_player;
// 	int bestValue = neg;
// 	bool localW2F;
// 	vector<string> moves; generate_moves(moves,game,player);
// 	int moves_size = moves.size();
// 	int v=0;
// 	sort(moves.begin(),moves.end(),moveComp);

// 	if (bestmove == "")
// 		bestmove = moves[0];

// 	game->update_board(bestmove,player);
// 	localW2F = game->wallToFlat;
// 	v = -negatest(game,-beta,-alpha,depth-1,!maxNode);
// 	game->wallToFlat = localW2F;
// 	game->undo_move(bestmove,player);
// 	bestValue = std::max(bestValue,v);
// 	alpha = std::max(alpha,v);
// 	if (alpha >= beta)
// 		goto hell;

// 	for (int i=0; i<moves_size; i++)
// 	{
// 		if (moves[i] == bestmove) continue;

// 		game->update_board(moves[i],player);
// 		localW2F = game->wallToFlat;

// 		v = -negatest(game,-alpha-1,-alpha,depth-1,!maxNode);

// 		if (v > alpha && v < beta && depth > 1) 
//   			v = std::max(v,-negatest(game,-beta,-v,depth-1,!maxNode));
		
// 		v = -negatest(game,-beta,-alpha,depth-1,!maxNode);
		
// 		game->wallToFlat = localW2F;
// 		game->undo_move(moves[i],player);

// 		if (v > bestValue)
// 		{
// 			bestValue = v;
// 			bestmove = moves[i];
// 		}
		
// 		if (bestValue >= beta)
// 			break;
		
// 		alpha = std::max(alpha,bestValue);
// 	}

// 	hell:
// 	// history[bestmove] += pow(2,depth);
// 	history[bestmove] += depth*depth;
// 	entry_type flag = exact;
// 	if (bestValue <= alphaOrig)
// 		flag = upperbound;
// 	else if (bestValue >= beta)
// 		flag = lowerbound;
// 	else
// 		flag = exact;
// 	auto e = &(ttable[game->hash()]);
// 	e->type = flag;
// 	e->value = bestValue;
// 	e->depth = depth;
// 	e->bmove = bestmove;
// 	return bestValue;
// }

void check(vector<string> &moves, string move) {
	for(int i=0;i<moves.size();i++) {
		if(moves[i]==move) {
			cerr<<"present"<<endl;
		}
	}
}

// int fakeabtt(gamestate* game, int alpha, int beta, int depth, bool maxNode) {
// 	int g;
// 	auto entry = ttable.find(game->getHash());
// 	EvalInfo* info;
// 	string bestmove = "";

// 	if (entry != ttable.end())
// 	{
// 		info = &(entry->second);
// 		if (info->depth >= depth)
// 		{
// 			if (info->lbound >= beta)
// 				return info->lbound;
// 			else if (info->ubound <= alpha || info->ubound == info->lbound)
// 				return info->ubound;
// 			alpha = std::max(alpha,info->lbound);
// 			beta = std::min(beta,info->ubound);
// 		}
// 		else
// 		{
// 			info->lbound = neg;
// 			info->ubound = pos;
// 		}
// 		bestmove = info->bmove;
// 	}
	
// 	int game_over = game->over(maxNode); //// 4 => not over

// 	if (game_over == 1)
// 		return pos;
// 	else if (game_over == -1)
// 		return neg;
// 	else if (game_over == 2 || game_over == 3)
// 		return flatwin(game);
// 	else if (depth == 0)
// 		return eval(game);
// 	else
// 	{
// 		bool localW2F;
// 		if (maxNode)
// 		{
// 			g = neg;
// 			int a = alpha;

// 			////// first flatstones
// 			vector<string> moves; generate_moves(moves,game,game->player_id);
// 			// check(moves,"4d3<22");
// 			sort(moves.begin(),moves.end(),moveComp);
// 			int moves_size = moves.size();
// 			int temp;

// 			if (bestmove == "")
// 				bestmove = moves[0];
// 			game->update_board(bestmove,game->player_id);
// 			localW2F = game->wallToFlat;
// 			// cerr<<bestmove<<endl;
// 			temp = abtt(game,a,beta,depth-1,false);
// 			game->wallToFlat = localW2F;
// 			game->undo_move(bestmove,game->player_id);
// 			g = std::max(g,temp);
// 			a = std::max(a,g);
// 			if (a >= beta)
// 				return g;
			
// 			for (int i=0; i<moves_size; i++)
// 			{
// 				if (moves[i]==bestmove) continue;
// 				game->update_board(moves[i],game->player_id);
// 				localW2F = game->wallToFlat;
// 				temp = abtt(game,a,beta,depth-1,false);
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->player_id);
// 				if (temp > g)
// 				{
// 					g = temp;
// 					bestmove = moves[i];
// 				}
// 				a = std::max(a,g);
// 				if (a >= beta)
// 					break;
// 			}
// 		}
// 		else
// 		{
// 			g = pos;
// 			int b = beta;
// 			vector<string> moves; generate_moves(moves,game,game->other_player);
// 			sort(moves.begin(),moves.end(),moveCompopp);
// 			int moves_size = moves.size();
// 			int temp;
// 			if (bestmove == "")
// 				bestmove = moves[0];

// 			game->update_board(bestmove,game->other_player);
// 			localW2F = game->wallToFlat;
// 			// cerr<<bestmove<<endl;
// 			temp = abtt(game,alpha,b,depth-1,true);
// 			game->wallToFlat = localW2F;
// 			game->undo_move(bestmove,game->other_player);
// 			g = std::min(g,temp);	
// 			b = std::min(b,g);
// 			if (b <= alpha)
// 				return g;
			
			
// 			for (int i=0; i<moves_size; i++)
// 			{
// 				if (moves[i]==bestmove) continue;
// 				game->update_board(moves[i],game->other_player);
// 				localW2F = game->wallToFlat;

// 				temp = abtt(game,alpha,b,depth-1,true);
				
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->other_player);

// 				if (temp < g)
// 				{
// 					g = temp;
// 					bestmove = moves[i];
// 				}
// 				b = std::min(b,g);
// 				if (b <= alpha)
// 					break;
				
// 			}
// 		}
// 	}

// 		return g;

// }

int abtt (gamestate* game, int alpha, int beta, int depth, bool maxNode)
{
	nodes++;
	int g;
	int v;
	auto entry = ttable.find(game->getHash());
	EvalInfo* info;
	string bestmove = "";

	if (entry != ttable.end())
	{
		info = &(entry->second);
		// if (info->depth >= depth)
		// {
		// 	if (info->lbound >= beta)
		// 		return info->lbound;
		// 	else if (info->ubound <= alpha || info->ubound == info->lbound)
		// 		return info->ubound;
		// 	alpha = std::max(alpha,info->lbound);
		// 	beta = std::min(beta,info->ubound);
		// 	// if (alpha >= beta)
		// 	// 	return info->value;
		// }
		// else
		// {
		// 	info->lbound = neg;
		// 	info->ubound = pos;
		// }
		bestmove = info->bmove;
	}

	int game_over = game->over(maxNode); //// 4 => not over

	if (game_over == 1)
		g = pos;
	else if (game_over == -1)
		g = neg;
	else if (game_over == 2 || game_over == 3)
		g = flatwin(game);
	else if (depth == 0)
		g = eval(game);
	else
	{
		bool localW2F;
		if (maxNode)
		{
			g = neg;
			int a = alpha;

			////// first flatstones
			vector<string> moves; generate_moves(moves,game,game->player_id);
			int moves_size = moves.size();
			// check(moves,"4d3<22");
			// sort(moves.begin(),moves.end(),moveComp);
				vector<pair<string,int> > sorted_moves;
				for (int i=0; i<moves_size; i++)
				{
					v = history[moves[i]];
					sorted_moves.push_back(make_pair(moves[i],v));
				}
				sort(sorted_moves.begin(),sorted_moves.end(),myComparison);

			
			int temp;

			if (bestmove == "")
				bestmove = sorted_moves[0].first;
			game->update_board(bestmove,game->player_id);
			localW2F = game->wallToFlat;
			// cerr<<bestmove<<endl;
			temp = abtt(game,a,beta,depth-1,false);
			game->wallToFlat = localW2F;
			game->undo_move(bestmove,game->player_id);
			g = std::max(g,temp);
			a = std::max(a,g);
			if (g >= beta)
				goto hell;
			
			for (int i=0; i<moves_size; i++)
			{
				if (sorted_moves[i].first==bestmove) continue;
				game->update_board(sorted_moves[i].first,game->player_id);
				localW2F = game->wallToFlat;
				temp = abtt(game,a,beta,depth-1,false);
				game->wallToFlat = localW2F;
				game->undo_move(sorted_moves[i].first,game->player_id);
				if (temp > g)
				{
					g = temp;
					bestmove = sorted_moves[i].first;
				}
				a = std::max(a,g);
				if (g >= beta)
					break;
			}
		}
		else
		{
			g = pos;
			int b = beta;
			vector<string> moves; generate_moves(moves,game,game->other_player);
			int moves_size = moves.size();

				vector<pair<string,int> > sorted_moves;
				for (int i=0; i<moves_size; i++)
				{
					v = historyopp[moves[i]];
					sorted_moves.push_back(make_pair(moves[i],v));
				}
				sort(sorted_moves.begin(),sorted_moves.end(),myComparison);


			// sort(moves.begin(),moves.end(),moveCompopp);
			// int moves_size = moves.size();
			int temp;
			if (bestmove == "")
				bestmove = sorted_moves[0].first;

			game->update_board(bestmove,game->other_player);
			localW2F = game->wallToFlat;
			// cerr<<bestmove<<endl;
			temp = abtt(game,alpha,b,depth-1,true);
			game->wallToFlat = localW2F;
			game->undo_move(bestmove,game->other_player);
			g = std::min(g,temp);	
			b = std::min(b,g);
			if (g <= alpha)
				goto hell;
			
			
			for (int i=0; i<moves_size; i++)
			{
				if (sorted_moves[i].first==bestmove) continue;
				game->update_board(sorted_moves[i].first,game->other_player);
				localW2F = game->wallToFlat;

				temp = abtt(game,alpha,b,depth-1,true);
				
				game->wallToFlat = localW2F;
				game->undo_move(sorted_moves[i].first,game->other_player);

				if (temp < g)
				{
					g = temp;
					bestmove = sorted_moves[i].first;
				}
				b = std::min(b,g);
				if (g <= alpha)
					break;
				
			}
		}

		hell:
		// history[bestmove] += pow(2,depth);
		// (maxNode ? history[bestmove] : historyopp[bestmove] )+= depth*depth;
		(maxNode ? history[bestmove] : historyopp[bestmove] )+= pow(2,depth);
		auto e = &(ttable[game->getHash()]);
		e->depth = depth;
		e->bmove = bestmove;

		if (g <= alpha)
			e->ubound = g;
		else if (g > alpha && g < beta)
		{
			e->lbound = g;
			e->ubound = g;
		}
		else
			e->lbound = g;
		
		// cerr<<"type: "<<flag<<" g: "<<g/25<<" depth: "<<depth<<" bm: "<<bestmove<<endl;
	}
	return g;
}

// int abpruning (gamestate* game, int alpha, int beta, int depth, bool maxNode)getH// {
// 	nodes++;
// 	int value;
// 	auto entry = ttable.find(game->hash());
// 	evalInfo* info;
// 	string bestmove = "";

// 	if (entry != ttable.end())
// 	{
// 		info = &(entry->second);
// 		if (info->depth >= depth)
// 		{
// 			if (info->type == exact)
// 				return info->value;
// 			else if (info->type == lowerbound)
// 				alpha = std::max(alpha,info->value);
// 			else if (info->type == upperbound)
// 				beta = std::min(beta,info->value);
// 			if (alpha >= beta)
// 				return info->value;
// 		}
// 		bestmove = info->bmove;
// 	}

// 	int game_over = game->over(maxNode); //// 4 => not over

// 	if (game_over == 1)
// 		return pos;
// 	else if (game_over == -1)
// 		return neg;
// 	else if (game_over == 2 || game_over == 3)
// 		return flatwin(game);
// 	else if (depth == 0)
// 		return eval(game);
// 	else
// 	{
// 		bool localW2F;
// 		if (maxNode)
// 		{
// 			value = neg;
// 			int a = alpha;
// 			vector<string> moves;
// 			int moves_size, temp;

// 			////// first flatstones
// 			place_new(moves,game,game->player_id,true);
// 			sort(moves.begin(),moves.end(),myplacecomp);
// 			moves_size = moves.size();

// 			if (bestmove == "")
// 				bestmove = moves[0];
// 			game->update_board(bestmove,game->player_id);
// 			localW2F = game->wallToFlat;
// 			temp = abpruning(game,a,beta,depth-1,false);
// 			game->wallToFlat = localW2F;
// 			game->undo_move(bestmove,game->player_id);
// 			value = std::max(value,temp);
// 			if (value >= beta)
// 				goto hell;
// 			a = std::max(a,value);

// 			for (int i=0; i<moves_size; i++)
// 			{
// 				if (moves[i]==bestmove) continue;
// 				game->update_board(moves[i],game->player_id);
// 				localW2F = game->wallToFlat;

// 				temp = abpruning(game,a,beta,depth-1,false);
				
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->player_id);

// 				if (temp > value)
// 				{
// 					value = temp;
// 					bestmove = moves[i];
// 				}
// 				if (value >= beta)
// 					break;
// 				a = std::max(a,value);
// 			}

// 			////// stack moves
// 			generate_moves(moves,game,game->player_id);
// 			sort(moves.begin(),moves.end(),mystackcomp);
// 			moves_size = moves.size();

// 			for (int i=0; i<moves_size; i++)
// 			{
// 				game->update_board(moves[i],game->player_id);
// 				localW2F = game->wallToFlat;

// 				temp = abpruning(game,a,beta,depth-1,false);
				
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->player_id);

// 				if (temp > value)
// 				{
// 					value = temp;
// 					bestmove = moves[i];
// 				}
// 				if (value >= beta)
// 					break;
// 				a = std::max(a,value);
// 			}


// 			///// standing stones
// 			place_new(moves,game,game->player_id,false);
// 			sort(moves.begin(),moves.end(),mystandingcomp);
// 			moves_size = moves.size();

// 			for (int i=0; i<moves_size; i++)
// 			{
// 				game->update_board(moves[i],game->player_id);
// 				localW2F = game->wallToFlat;

// 				temp = abpruning(game,a,beta,depth-1,false);
				
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->player_id);

// 				if (temp > value)
// 				{
// 					value = temp;
// 					bestmove = moves[i];
// 				}
// 				if (value >= beta)
// 					break;
// 				a = std::max(a,value);
// 			}
// 		}
// 		else
// 		{
// 			value = pos;
// 			int b = beta;
// 			vector<string> moves; 
// 			int moves_size, temp;

// 			///// first flat
// 			place_new(moves,game,game->other_player,true);
// 			sort(moves.begin(),moves.end(),oppplacecomp);
// 			moves_size = moves.size();
// 			if (bestmove == "")
// 				bestmove = moves[0];

// 			game->update_board(bestmove,game->other_player);
// 			localW2F = game->wallToFlat;
// 			temp = abpruning(game,alpha,b,depth-1,true);
// 			game->wallToFlat = localW2F;
// 			game->undo_move(bestmove,game->other_player);
// 			value = std::min(value,temp);	
// 			if (value <= alpha)
// 				goto hell;
// 			b = std::min(b,value);
			
// 			for (int i=0; i<moves_size; i++)
// 			{
// 				if (moves[i]==bestmove) continue;
// 				game->update_board(moves[i],game->other_player);
// 				localW2F = game->wallToFlat;

// 				temp = abpruning(game,alpha,b,depth-1,true);
				
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->other_player);

// 				if (temp < value)
// 				{
// 					value = temp;
// 					bestmove = moves[i];
// 				}
				
// 				if (value <= alpha)
// 					break;
// 				b = std::min(b,value);
// 			}


// 			////// stack moves
// 			generate_moves(moves,game,game->other_player);
// 			sort(moves.begin(),moves.end(),oppstackcomp);
// 			moves_size = moves.size();
			
// 			for (int i=0; i<moves_size; i++)
// 			{
// 				game->update_board(moves[i],game->other_player);
// 				localW2F = game->wallToFlat;

// 				temp = abpruning(game,alpha,b,depth-1,true);
				
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->other_player);

// 				if (temp < value)
// 				{
// 					value = temp;
// 					bestmove = moves[i];
// 				}
				
// 				if (value <= alpha)
// 					break;
// 				b = std::min(b,value);
// 			}


// 			////// standing
// 			place_new(moves,game,game->other_player,false);
// 			sort(moves.begin(),moves.end(),oppstandingcomp);
// 			moves_size = moves.size();
			
// 			for (int i=0; i<moves_size; i++)
// 			{
// 				game->update_board(moves[i],game->other_player);
// 				localW2F = game->wallToFlat;

// 				temp = abpruning(game,alpha,b,depth-1,true);
				
// 				game->wallToFlat = localW2F;
// 				game->undo_move(moves[i],game->other_player);

// 				if (temp < value)
// 				{
// 					value = temp;
// 					bestmove = moves[i];
// 				}
				
// 				if (value <= alpha)
// 					break;
// 				b = std::min(b,value);
// 			}

// 		}

// 		hell:
// 		if (maxNode)
// 		{
// 			if (bestmove[0] == 'F' || bestmove[0] == 'C')
// 				myflatcap[bestmove] += depth*depth;
// 			else if(bestmove[0] == 'S')
// 				mystanding[bestmove] += depth*depth;
// 			else
// 				mystack[bestmove] += depth*depth;
// 		}
// 		else
// 		{
// 			if (bestmove[0] == 'F' || bestmove[0] == 'C')
// 				oppflatcap[bestmove] += depth*depth;
// 			else if(bestmove[0] == 'S')
// 				oppstanding[bestmove] += depth*depth;
// 			else
// 				oppstack[bestmove] += depth*depth;
// 		}
		
// 		entry_type flag = exact;
// 		if (value <= alpha)
// 			flag = upperbound;
// 		else if (value >= beta)
// 			flag = lowerbound;
// 		else
// 			flag = exact;
// 		auto e = &(ttable[game->hash()]);
// 		e->type = flag;
// 		e->value = value;
// 		e->depth = depth;
// 		e->bmove = bestmove;
// 		// cerr<<"type: "<<flag<<" value: "<<value/25<<" depth: "<<depth<<" bm: "<<bestmove<<endl;
// 	}
// 	return value;
// }
