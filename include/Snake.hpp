#pragma once
#ifndef _SNAKE_HPP_
#define _SNAKE_HPP_

#include <vector>
#include <ncurses.h>

class GameOver : public std::exception {
  public:
    GameOver() = default;
    GameOver(const GameOver&) = default;
    GameOver &operator=(const GameOver&) = default;
    GameOver(GameOver&&) = default;
    GameOver &operator=(GameOver&&) = default;

    virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
      return "Game Over\n";
    }
};

class Snake {
  public:
    enum Direction {
      Up = 1,
      Down = 3,
      Left = 2,
      Right = 4
    };

    struct coord {
      int x, y;
    };

  private:
    WINDOW *m_win;

    char m_snake_head;
    char m_snake_body;

    int m_body_size;

    Direction m_dir;

    coord m_start_pos;
    std::vector<coord> m_body_pos;

    auto check_body_coll(coord pos) -> bool;

  public:
    Snake(WINDOW *win, Direction dir, coord pos);

    auto draw() -> void;
    auto change_dir(Direction dir) -> void;
    auto grow() -> void;
    auto step() -> void;
    auto get_body() -> std::vector<coord>;

    auto get_pos() -> coord;
};

#endif // ! _SNAKE_HPP_
