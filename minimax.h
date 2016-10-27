#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

enum entry_type {exact,upperbound,lowerbound};
int flatwin(gamestate* );
int eval(gamestate*);
int max_val(int, int);
int min_val(int, int);
bool myComparison(const pair<string,int> &,const pair<string,int> &);
bool myComparison2(const pair<string,int> &,const pair<string,int> &);
void storeEntry(uint64_t , int , entry_type , int , string );
// void storeEntry(uint64_t , int , int , int  , string );
// void storeUpper(uint64_t , int , int , int  , string );
// void storeLower(uint64_t , int , int , int , string );
int mtdf (gamestate* , int , int );
pair<int,string> ids(gamestate* );
int negamax(gamestate* , int , int , int , bool);
// int abtt (gamestate* , int , int , int , bool );
// int test (gamestate* , int , int , int , bool );
pair<int,string> value(gamestate* , int, int, int, int, bool);