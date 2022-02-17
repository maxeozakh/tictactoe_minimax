#include "main.hpp"

int turn = human, prev_turn = computer;
field_type field = {{'*', '*', '*'}, {'*', '*', '*'}, {'*', '*', '*'}};
state_type DEFAULT_STATE = {
    {-10000, {0, 0}},
    {10000, {0, 0}},
};
std::string color_codes[] = {"97", "92", "91", "93", "96"};
enum colors
{
    grey,
    green,
    red,
    yellow,
    cyan
};

void color_print(char &value, colors color)
{
    std::cout << "\033[" << color_codes[color] << "m" << value << "\033[0m";
}

int main()
{
    game_loop();
    return 1;
}

void render_field()
{
    std::cout << '\n';
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (j == 1)
            {
                std::cout << "|";
            }
            if (field[i][j] == ICONS[human])
            {
                color_print(field[i][j], yellow);
            }
            else if (field[i][j] == ICONS[computer])
            {
                color_print(field[i][j], green);
            }
            else
            {
                std::cout << field[i][j];
            }
            if (j == 1)
            {
                std::cout << "|";
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

bool is_draw()
{
    if (is_win_for(0) || is_win_for(1))
    {
        return false;
    }

    bool is_empty_space = false;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (field[i][j] == '*')
            {
                // TODO: break it after success for unneccesary repeats
                is_empty_space = true;
            }
        }
    }

    return !is_empty_space;
}

bool is_win_for(int turn)
{
    const char player_symbol = ICONS[turn];
    for (int i = 0; i < 8; i++)
    {
        int counter = 0;
        for (int j = 0; j < 3; j++)
        {
            int x = WIN_POSITIONS[i][j].first,
                y = WIN_POSITIONS[i][j].second;

            if (player_symbol != field[x][y])
            {
                break;
            };
            counter++;

            if (counter == 3)
            {
                return true;
            }
        }
    }
    return false;
};

move_with_utility_type minimax_value;
int count = 0;
move_with_utility_type minimax(int turn, state_type state)
{
    count++;
    const char ICON = ICONS[turn];

    if (is_draw())
    {
        move_with_utility_type move = {0, {NULL, NULL}};
        return move;
    }

    if (is_win_for(!turn))
    {
        move_with_utility_type move = {get_utility(!turn), {NULL, NULL}};
        return move;
    }

    available_moves_type available_moves = get_available_moves();

    for (pair_ints_type move : available_moves)
    {
        const int x = move.first, y = move.second;
        field[x][y] = ICON;

        state_type local_state = DEFAULT_STATE;
        minimax_value = minimax(!turn, local_state);

        if (turn)
        {
            if (minimax_value.first < state.best_o.first)
            {
                state.best_o.first = minimax_value.first;
                state.best_o.second.first = move.first;
                state.best_o.second.second = move.second;
            }
        }
        else
        {
            if (minimax_value.first > state.best_x.first)
            {
                state.best_x.first = minimax_value.first;
                state.best_x.second.first = move.first;
                state.best_x.second.second = move.second;
            }
        }
        reset_move(move);
    }

    return turn ? state.best_o : state.best_x;
};

void reset_move(pair_ints_type move)
{
    const int x = move.first,
              y = move.second;

    field[x][y] = '*';
};

int get_utility_factorial(int turn)
{
    return turn ? -1 : 1;
}

int get_utility(int turn)
{
    int factorial = get_utility_factorial(turn);
    int available_moves_count = get_available_moves().size();

    return factorial * (available_moves_count + 1);
};

available_moves_type get_available_moves()
{
    available_moves_type alailable_moves;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (field[i][j] == '*')
            {
                alailable_moves.push_back({i, j});
            }
        }
    }

    return alailable_moves;
}

void human_move()
{
    std::string user_input;
    pair_ints_type move;
    std::cout << "   MAKE YOUR TURN: ";
    std::cin >> user_input;
    move.first = std::stoi(user_input.substr(0, 1));
    move.second = std::stoi(user_input.substr(2, 3));

    while (!is_move_valid(move))
    {
        std::cout << "make a valid turn....: ";
        std::cin >> user_input;
        move.first = std::stoi(user_input.substr(0, 1));
        move.second = std::stoi(user_input.substr(1, 1));
    }

    field[move.first][move.second] = ICONS[human];
};

void computer_move()
{
    std::cout << "👾 THE COMPUTER STARTS TO CONSIDER POSSIBLE MOVES" << '\n';

    state_type state = DEFAULT_STATE;
    move_with_utility_type op_move_3000 = minimax(computer, state);
    const int x = op_move_3000.second.first, y = op_move_3000.second.second;
    std::cout << "   " << count << " HAVE BEEN CALCULATED" << '\n';
    count = 0;
    std::cout << "   THE COMPUTER MAKES A MOVE " << x << "-" << y << '\n';
    field[x][y] = ICONS[computer];
};

void game_loop()
{
    pair_ints_type current_move;

    while (!is_win_for(prev_turn) && !is_draw())
    {
        prev_turn = turn;
        render_field();
        turn == 0 ? human_move() : computer_move();
        turn = !turn;
    }
    render_field();

    if (is_win_for(prev_turn))
    {
        std::cout << "😻 THE COMPUKTER WON!" << '\n';
    }
    else
    {
        std::cout << "💤 TIE" << '\n';
    }
};

bool is_move_valid(pair_ints_type &move)
{
    const int x = move.first, y = move.second;
    if ((x < 0 || y < 0 || x > 2 || y > 2) || field[x][y] != '*')
    {
        return false;
    }

    return true;
};
