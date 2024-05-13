#include "my_player.h"
#include <cstdlib>

static field_index_t rand_int(field_index_t min, field_index_t max) {
    return min + rand() % (max - min + 1);
}

Point RandomPlayer::play(const GameView& game) {
    Boundary b = game.get_settings().field_size;
    Point result;
    do {
        result = {
            .x = rand_int(b.min.x, b.max.x),
            .y = rand_int(b.min.y, b.max.y),
        };
    } while(game.get_state().field->get_value(result) != Mark::None);
    return result;
}

bool move(Point& point, direction direction, Boundary b) {
    switch (direction)
    {
    case HORIZONTAL:
        if (point.x == b.max.x && point.y == b.max.y) {
            point.x++;
            return true;
        }
        point.x++;
        if (point.x > b.max.x) {
            point.y++;
            point.x = b.min.x;
            return true;
        }
        break;
    case VERTICAL:
        if (point.x == b.max.x && point.y == b.max.y) {
            point.y++;
            return true;
        }
        point.y++;
        if (point.y > b.max.y) {
            point.x++;
            point.y = b.min.y;
            return true;
        }
        break;
    case DIAGONAL_DOWN:
        if (point.x == b.max.x && point.y == b.min.y) {
            point.x++;
            return true;
        }
        if (point.x == b.max.x && point.y == b.max.y) {
            point.x = b.min.x + 1;
            point.y = b.min.y;
            return true;

        }
        point.x++; point.y++;
        if (point.x > b.max.x) {
            point.x = b.min.x + (point.y - b.min.y) + 2;
            point.y = b.min.y;
            return true;
        }
        else if (point.y > b.max.y) {
            point.y = b.max.y - (point.x - b.min.x);
            point.x = b.min.x;
            return true;
        }
        break;
    case DIAGONAL_UP:
        if (point.x == b.max.x && point.y == b.max.y) {
            point.x++;
            point.y++;
            return true;
        }
        if (point.x == b.max.x && point.y == b.min.y) {
            point.x = b.min.x + 1;
            point.y = b.max.y;
            return true;
        }
        point.x++; point.y--;
        if (point.x > b.max.x) {
            point.x = b.min.x + (point.y - b.min.y) + 2;
            point.y = b.max.y;
            return true;
        }
        else if (point.y < b.min.y) {
            point.y = point.x;
            point.x = b.min.x;
            return true;
        }
        break;
    // Inverted //
    case INV_HORIZONTAL:
        if (point.x == b.min.x && point.y == b.max.y) {
            point.x--;
            return true;
        }
        point.x--;
        if (point.x < b.min.x) {
            point.y++;
            point.x = b.max.x;
            return true;
        }
        break;
    case INV_VERTICAL:
        if (point.x == b.max.x && point.y == b.min.y) {
            point.y--;
            return true;
        }
        point.y--;
        if (point.y < b.min.y) {
            point.x++;
            point.y = b.max.y;
            return true;
        }
        break;
    case INV_DIAGONAL_DOWN:
        if (point.x == b.max.x && point.y == b.min.y) {
            point.x--;
            point.y--;
            return true;
        }
        if (point.x == b.min.x && point.y == b.min.y) {
            point.x = b.max.x;
            point.y = b.max.y - 1;
            return true;

        }
        point.x--; point.y--;
        if (point.x < b.min.x) {
            point.x = b.max.x - (point.y - b.min.y);
            point.y = b.max.y;
            return true;
        }
        else if (point.y < b.min.y) {
            point.y = b.max.y - (point.x - b.min.x) - 2;
            point.x = b.max.x;
            return true;
        }
        break;
    case INV_DIAGONAL_UP:
        if (point.x == b.max.x && point.y == b.max.y) {
            point.x++;
            point.y++;
            return true;
        }
        if (point.x == b.min.x && point.y == b.max.y) {
            point.x = b.max.x;
            point.y = b.min.y + 1;
            return true;
        }
        point.x--; point.y++;
        if (point.x < b.min.x) {
            point.x = point.y;
            point.y = b.min.y;
            return true;
        }
        else if (point.y > b.max.y) {
            point.y = (point.x - b.min.x) + 2 + b.min.y;
            point.x = b.max.x;
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

int Evaluate(int length) {
    switch (length) {
    case 1: return w_one;
    case 2: return w_two;
    case 3: return w_three;
    case 4: return w_four;
    default: break;
    }
}

int addedValue(const GameView& game, const Point& pt, direction direct, Mark myMark, int dist_to_enemy, int length) {
    Boundary b = game.get_settings().field_size;
    int added_value = 0;
    Point temp(pt);
    int count = 0;

    if (length != 4) added_value -= Evaluate(length) / (pow(w_pow_added, dist_to_enemy - length));
    else added_value -= w_added / (pow(w_pow_added, dist_to_enemy - length));

    bool end_field = move(temp, direct, b);
    for (int k = 0; k < 3; k++) {
        if (!b.is_within(temp) || end_field) {
            added_value -= w_added / (pow(w_pow_added, k));
            break;
        }
        Mark tmp_mark = game.get_state().field->get_value(temp);
        if (tmp_mark != Mark::None && tmp_mark != myMark) {
            if (length != 4) added_value -= Evaluate(length) / (pow(w_pow_added, k));
            else added_value -= w_added / (pow(w_pow_added, k));
            break;
        }
        else if (tmp_mark == myMark) {
            added_value += w_added / ((k + 1) * 2);
            count++;
            if (count - k == 1 && count + length == 4) added_value += 200;
        }
        end_field = move(temp, direct, b);
    }
    return added_value;
}


Point MyPlayer::play(const GameView& game) {
    Boundary b = game.get_settings().field_size;
    Mark myMark = game.get_state().current_move;

    Point center((b.max.x+b.min.x)/2, (b.max.y + b.min.y) / 2);
    if (game.get_state().number_of_moves == 0) return center;
    else if (game.get_state().number_of_moves == 1) {
        if (game.get_state().field->get_value(center) == Mark::None) return center;
        else {
            int x = 0, y = 0;
            do {
                x = rand_int((b.max.x + b.min.x) / 2 - 1, (b.max.x + b.min.x) / 2 + 1);
                y = rand_int((b.max.y + b.min.y) / 2 - 1, (b.max.y + b.min.y) / 2 + 1);
            } while (Point(x,y) == center);
            return Point(x, y);
        }
    }

    int size_y = b.get_width();
    int size_x = b.get_height();
    int size = size_y * size_x;

    int* eval_array = new int[size_x * size_y](0);
    int* eval_array_enemy = new int[size_x * size_y](0);


    int length;
    int enemy_length;
    int dist_to_my;
    int dist_to_enemy;
    for (int i = HORIZONTAL; i < INV_DIAGONAL_UP + 1; i++) {
        direction direct = static_cast<direction>(i);

        Point pt(b.min.x, b.min.y);
        if (direct == DIAGONAL_DOWN || direct == INV_VERTICAL || direct == INV_DIAGONAL_DOWN) pt.y = b.max.y;
        else if (direct == INV_HORIZONTAL) pt.x = b.max.x;

        bool new_line = true;
        while (b.is_within(pt)) {
            if (new_line) {
                length = enemy_length = dist_to_my = dist_to_enemy = 0;
            }
            Mark current = game.get_state().field->get_value(pt);

            if (current == myMark) { enemy_length = 0; dist_to_my = 0; dist_to_enemy++; length++; }
            else if (current != Mark::None) { length = 0; dist_to_enemy = 0; dist_to_my++; enemy_length++; }
            else {
                if (length == 4) return pt;
                if (length) {
                    eval_array[(pt.y - b.min.y) * size_y + (pt.x - b.min.x)] +=
                        Evaluate(length) + addedValue(game, pt, direct, myMark, dist_to_enemy, length);
                }
                else if (enemy_length) {
                    Mark enemy_mark = Mark::Zero;
                    if (myMark == Mark::Zero) enemy_mark = Mark::Cross;

                    int eval_enemy = Evaluate(enemy_length) +
                        addedValue(game, pt, direct, enemy_mark, dist_to_my, enemy_length);

                    eval_array_enemy[(pt.y - b.min.y) * size_y + (pt.x - b.min.x)] += eval_enemy;
                    eval_array[(pt.y - b.min.y) * size_y + (pt.x - b.min.x)] += eval_enemy / 2;
                }
                length = enemy_length = 0;
                dist_to_my++; dist_to_enemy++;
            }
            new_line = move(pt, direct, b);
        }
    }

    int dist = 0;

    int* max_me = std::max_element(eval_array, eval_array + size);
    int* max_enemy = std::max_element(eval_array_enemy, eval_array_enemy + size);
    if (*max_me < w_win && *max_enemy >= w_min_def) dist = max_enemy - eval_array_enemy;
    else {
        if (*max_me == 0) {
            *max_me = -1000;
            for (int* ptr = eval_array; ptr != eval_array + size + 1; ptr++) {
                if (*ptr != 0 && *ptr > *max_me) max_me = ptr;
            }
        }
        if (*max_me == 0) {
            max_me = std::max_element(eval_array_enemy, eval_array_enemy + size);
        }
        dist = max_me - eval_array;
    }
    int x = dist % size_y + b.min.x;
    int y = (dist / size_y) + b.min.y;


    delete[] eval_array;
    delete[] eval_array_enemy;
    return Point(x, y);
}



Point ManualPlayer::play(const GameView& game) {
    Boundary b = game.get_settings().field_size;
    Point result(0, 0);

    int x, y;
    do {
        fseek(stdin, 0, SEEK_END);
        printf("Input x, y: ");
        scanf_s("%i %i", &result.x, &result.y);
    } while (!b.is_within(result) || (game.get_state().field->get_value(result) != Mark::None));
    return result;
}

//           //
// Observers //
//           //
void BasicObserver::notify(const GameView& game, const Event& event) {
    if (event.get_type() == MoveEvent::TYPE) {
        auto &data = get_data<MoveEvent>(event);
        _out << "Move:\tx = " <<  data.point.x 
            << ",\ty = " << data.point.y << ":\t";
        _print_mark(data.mark) << '\n';
        std::cout << "Field:\n   ";
        Boundary b = game.get_settings().field_size;
        for (int i = b.min.x; i < b.max.x + 1; i++) printf("%3i", i);
        std::cout << std::endl;
        for (int y = b.min.y; y < b.max.y + 1; y++) {
            printf("%3i", y);
            for (int x = b.min.x; x < b.max.x + 1; x++) {
                Mark mark = game.get_state().field->get_value(Point(x, y));
                std::cout << "|_" << (mark == Mark::None ? '_' : (mark == Mark::Zero ? 'O' : 'X'));
            } std::cout << std::endl;
        }
        return;
    }
    if (event.get_type() == PlayerJoinedEvent::TYPE) {
        auto &data = get_data<PlayerJoinedEvent>(event);
        _out << "Player '" << data.name << "' joined as ";
        _print_mark(data.mark) << '\n';
        return;
    }
    if (event.get_type() == GameStartedEvent::TYPE) {
        _out << "Game started\n";
        return;
    }
    if (event.get_type() == WinEvent::TYPE) {
        auto &data = get_data<WinEvent>(event);
        _out << "Player playing ";
        _print_mark(data.winner_mark) << " has won\n";
        return;
    }
    if (event.get_type() == DrawEvent::TYPE) {
        auto &data = get_data<DrawEvent>(event);
        _out << "Draw happened, reason: '" << data.reason << "'\n";
        return;
    }
    if (event.get_type() == DisqualificationEvent::TYPE) {
        auto &data = get_data<DisqualificationEvent>(event);
        _out << "Player playing ";
        _print_mark(data.player_mark) << " was disqualified, reason: '" 
            << data.reason << "'\n";
        return;
    }
}

std::ostream& BasicObserver::_print_mark(Mark m) {
    if (m == Mark::Cross) return _out << "X";
    if (m == Mark::Zero) return _out << "O";
    return _out << "?";
}
