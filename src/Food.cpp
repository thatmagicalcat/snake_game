#include "../include/Food.hpp"

Food::Food(coord end_pos, coord start_pos) {
  srand(time(NULL));

  start_end_coord[1] = start_pos;
  start_end_coord[0] = end_pos;
}

auto Food::get_food() -> coord {
  return {
    .x = get_rand(
      start_end_coord[1].x,
      start_end_coord[0].x
    ),

    .y = get_rand(
      start_end_coord[1].y,
      start_end_coord[0].y
    )
  };
}

auto Food::get_rand(int start, int end) -> int {
  return rand() % (end - start + 1) + start;
}
