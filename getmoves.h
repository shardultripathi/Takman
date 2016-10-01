#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

void place_new(std::vector<std::string> &, gamestate*);
void move_stack(int, int, int, int, std::string, int,std::vector<std::string> &, gamestate*);
void generate_moves(std::vector<std::string> &, gamestate* ); 