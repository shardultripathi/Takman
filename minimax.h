#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

void update_start_time(int,double);
enum entry_type {exact,upperbound,lowerbound};
int flatwin(gamestate* );
int eval(gamestate*);
bool myComparison(const pair<string,int> &,const pair<string,int> &);
bool myComparison2(const pair<string,int> &,const pair<string,int> &);
void storeEntry(uint64_t , int , entry_type , int , string );
void getpv(string& , gamestate* , int, int );
pair<int,string> ids(gamestate* );
// int negamax(gamestate* , int , int , int , bool);
// int negatest(gamestate* , int , int , int , bool);
int abtt (gamestate* , int , int , int , bool );