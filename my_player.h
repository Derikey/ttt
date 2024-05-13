#pragma once
#include <ostream>
#include "player.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "silly_player.hpp"

/*
 This is example player, that plays absolutely randomly.
*/
class RandomPlayer : public Player {
    std::string _name;
public:
    RandomPlayer(const std::string& name): _name(name) {}
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override { /*does nothing*/ }
    void notify(const GameView&, const Event&) override { /*does nothing*/ }
};


// Weights:
#define w_one 20
#define w_two 50
#define w_three 100
#define w_four 200
#define w_added 10

#define w_pow_added 2
#define w_min_def 76
#define w_win 180
//


class MyPlayer : public Player {
    std::string _name;
public:
    MyPlayer(const std::string& name) : _name(name) {}
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override { /*does nothing*/ }
    void notify(const GameView&, const Event&) override { /*does nothing*/ }
};


class ManualPlayer : public Player {
    std::string _name;
public:
    ManualPlayer(const std::string& name) : _name(name) {}
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override { /*does nothing*/ }
    void notify(const GameView&, const Event&) override { /*does nothing*/ }

};

class MyPlayer_v2 : public Player {
    std::string _name;
public:
    MyPlayer_v2(const std::string& name) : _name(name) {}
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override { /*does nothing*/ }
    void notify(const GameView&, const Event&) override { /*does nothing*/ }
};

enum direction {HORIZONTAL, VERTICAL, DIAGONAL_DOWN, DIAGONAL_UP,
                INV_HORIZONTAL, INV_VERTICAL, INV_DIAGONAL_DOWN, INV_DIAGONAL_UP
                };

bool move(Point& point, direction direction, Boundary b);
int Evaluate(int length);
int addedValue(const GameView& game, const Point& pt, direction direct, Mark myMark, int dist_to_enemy, int length);

/**
 * Simple observer, that logs every event in given output stream.
 */
class BasicObserver : public Observer {
    std::ostream& _out;

    std::ostream& _print_mark(Mark mark);
public:
    BasicObserver(std::ostream& out_stream): _out(out_stream) {}
    void notify(const GameView&, const Event& event) override;
};
