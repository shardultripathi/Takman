#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;



int flatwin(gamestate* );
int eval(gamestate*);
int max_val(int, int);
int min_val(int, int);
bool myComparison(const pair<string,int> &,const pair<string,int> &);
bool myComparison2(const pair<string,int> &,const pair<string,int> &);
pair<int,string> value(gamestate* , int, int, int, int, bool);