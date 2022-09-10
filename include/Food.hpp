#pragma once
#ifndef _FOOD_HPP_
#define _FOOD_HPP_

#include <random>

class Food {
  public:
    struct coord {
      int x, y;
    };

  private:
    coord start_end_coord[2];
    
    auto get_rand(int start, int end) -> int;

  public:
    Food(coord end_pos, coord start_pos = { 0, 0 });
    
    auto get_food() -> coord;
};

#endif // !_FOOD_HPP_
