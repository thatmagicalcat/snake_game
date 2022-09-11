#include <iostream>
#include <ncurses.h>
#include <thread>

#include "../include/Snake.hpp"
#include "../include/Food.hpp"

using coord = Food::coord;

auto coll(Snake::coord snake_pos, Food::coord food_pos) -> bool;
auto coll_wall(Snake::coord snake_pos) -> bool;
auto game() -> void;

//// Main ////
auto main() -> int {
  game();
}

auto coll(Snake::coord snake_pos, Food::coord food_pos) -> bool {
  return (snake_pos.x == food_pos.x && snake_pos.y == food_pos.y);
}

auto coll_wall(Snake::coord snake_pos) -> bool {
  return (snake_pos.x >= 59 || snake_pos.y >= 29 || snake_pos.x < 1 || snake_pos.y < 1);
}

auto game() -> void {
  initscr();

  noecho();
  raw();
  curs_set(false);

  if (has_colors()) start_color();
  init_pair(5, COLOR_GREEN, COLOR_BLACK);

  int height, width;
  getmaxyx(stdscr, height, width);

  attron(A_STANDOUT);
  mvprintw(height / 2 - 16, width / 2 - 49, " Use WASD keys to move, P to pause, Q to quit ");
  attroff(A_STANDOUT);

  WINDOW *win = newwin(
    30, // Win height
    60, // Win width
    (height / 2) - 15, // Win y position
    (width / 2) - 50  // Win x position
  ); 

  coord end;
  getmaxyx(win, end.y, end.x);

  Snake snake(win, Snake::Direction::Right, {5, 5});
  Food food({
    .x = end.x - 2, .y = end.y - 2
  }, { 1, 1 });

  snake.draw();

  WINDOW *score_win = newwin(10, 20, (height / 2) - 15, (width / 2) + 10);

  box(win, 0, 0);
  box(score_win, 0, 0);
  
  refresh();

  wrefresh(win);
  wrefresh(score_win);

  halfdelay(5); // only wait 500ms for user input

  coord food_pos = food.get_food(), pre_pos;
  pre_pos = food_pos;

  mvwprintw(score_win, 1, 1, "Score: %i", 0);
  mvwaddch(win, food_pos.y, food_pos.x, '*' | A_BOLD | COLOR_PAIR(5));

  wrefresh(win);
  wrefresh(score_win);

  char ch;

  bool pause;
  int game_score = 0;

  while (1) {
    // mvwprintw(score_win, 3, 1, "X:%i,Y:%i", food_pos.x, food_pos.y);
    if (pause)
      mvwprintw(score_win, 2, 1, "Paused");
    wrefresh(score_win);

    while (pause) { 
      ch = wgetch(win);
      if (::tolower(ch) == 'p') { 
        pause = !pause; 
        mvwprintw(score_win, 2, 1, "      ");
        continue;
      } else if (::tolower(ch) == 'q') goto game_over;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    } ch = wgetch(win);

    switch (::tolower(ch)) {
      case 'w':
        snake.change_dir(Snake::Direction::Up);
        break;
      case 's':
        snake.change_dir(Snake::Direction::Down);
        break;
      case 'a':
        snake.change_dir(Snake::Direction::Left);
        break;
      case 'd':
        snake.change_dir(Snake::Direction::Right);
        break;
      case 'q':
        goto game_over;
        break;
      case 'p':
        pause = !pause;
        continue;
    }

    try { snake.step(); } catch (const GameOver &e) { break; }

    mvwaddch(win, food_pos.y, food_pos.x, '*' | A_BOLD | COLOR_PAIR(5));
    if (coll(snake.get_pos(), food_pos)) {
      snake.grow();

      bool has_collided = true;
      while (1) {
        pre_pos = food.get_food(); 

        if (pre_pos.x != food_pos.x && pre_pos.y != food_pos.y)
          for (const Snake::coord &p : snake.get_body())
            if (p.x == pre_pos.x && p.y == pre_pos.y) {
              has_collided = false;
              break;
            }
        if (has_collided) break;
      } food_pos = pre_pos;

      mvwaddch(win, food_pos.y, food_pos.x, '*' | A_BOLD | COLOR_PAIR(5));

      mvwprintw(score_win, 1, 1, "Score: %i", ++game_score);
      wrefresh(score_win);
    } else if (coll_wall(snake.get_pos())) break;

    snake.draw();
  }

game_over:
  clear();
  printw("Game over\nScore: %i\nPress any key to exit", game_score);

  while (getch() == -1) {} // wait

  curs_set(true);
  endwin();
}
