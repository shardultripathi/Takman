#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

int flatwin(gamestate* );
int eval(gamestate* , int);
int max_val(int, int);
int min_val(int, int);
pair<int,string> value(gamestate* , int, int, int, int, bool);