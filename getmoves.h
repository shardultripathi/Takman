#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

void place_new(vector<string> &, gamestate*, int, bool);
void move_stack(int, int, int, int, string, int,vector<string> &, gamestate*);
void generate_moves(vector<string> &, gamestate* , int); 