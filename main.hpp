#pragma once
#include <utility>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>

enum PLAYERS
{
    human,
    computer
};

typedef std::pair<int, int> pair_ints_type;
typedef pair_ints_type win_positions_type[8][3];
typedef char field_type[3][3];
typedef std::vector<pair_ints_type> available_moves_type;
typedef std::pair<int, pair_ints_type> move_with_utility_type;

const char ICONS[] = {'x', 'o'};
win_positions_type WIN_POSITIONS = {
    // horizontal
    {{0, 0}, {0, 1}, {0, 2}},
    {{1, 0}, {1, 1}, {1, 2}},
    {{2, 0}, {2, 1}, {2, 2}},
    // vertical
    {{0, 0}, {1, 0}, {2, 0}},
    {{0, 1}, {1, 1}, {2, 1}},
    {{0, 2}, {1, 2}, {2, 2}},
    // idk how its called, 45 deg
    {{0, 0}, {1, 1}, {2, 2}},
    {{0, 2}, {1, 1}, {2, 0}},
};

struct state_type
{
    move_with_utility_type best_x;
    move_with_utility_type best_o;
};
void reset_move(pair_ints_type move);
available_moves_type get_available_moves();
void game_loop();
void render_field();
move_with_utility_type mimimax(int turn, state_type state);
void fill_max_min();
void human_move();
void computer_move();

bool is_move_valid(pair_ints_type &move);
bool is_draw();
bool is_win_for(int turn);

int get_utility(int turn);
int get_available_moves_count();