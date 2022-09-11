#include "../include/Snake.hpp"

Snake::Snake(WINDOW *win, Direction dir, coord pos):
  m_win(win),
  m_dir (dir),
  m_start_pos(pos),
  m_body_size(2),
  m_snake_body(' '),  // Change these to customize
  m_snake_head(' ') { // the snake's head and body

  if (has_colors()) start_color();
  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  m_body_pos.push_back(pos);
}

auto Snake::draw() -> void {
  for (size_t c = 0; c < m_body_pos.size(); c++) {
    const coord &current = m_body_pos[c];

    if (c + 1 == m_body_pos.size()) /* Snake's head */ { 
      mvwaddch(m_win, current.y, current.x, m_snake_head | COLOR_PAIR(1));
    } else if (m_body_size > 1) mvwaddch(m_win, current.y, current.x, m_snake_body | A_STANDOUT);
  }
}

auto Snake::change_dir(Direction dir) -> void {
  if (m_dir == dir || m_dir % 2 == dir % 2) return;
  m_dir = dir;
}

auto Snake::grow() -> void {
  m_body_size++;
}

auto Snake::step() -> void {
  switch (m_dir) {
    case Direction::Up:
      m_start_pos.y -= 1;
      break;
    case Direction::Down:
      m_start_pos.y += 1;
      break;
    case Direction::Left:
      m_start_pos.x -= 1;
      break;
    case Direction::Right:
      m_start_pos.x += 1;
      break;
  } if (m_body_pos.size() >= 4 && check_body_coll(m_start_pos)) throw GameOver();
  m_body_pos.push_back(m_start_pos);

  if (m_body_pos.size() > m_body_size) {
    mvwaddch(m_win, m_body_pos.begin()->y, m_body_pos.begin()->x, ' ');
    m_body_pos.erase(m_body_pos.begin());
  }
}

auto Snake::get_pos() -> coord {
  return m_body_pos.back();
}

auto Snake::check_body_coll(coord pos) -> bool {
  for (const coord &c : m_body_pos)
    if (c.x == pos.x && c.y == pos.y)
      return true;
  return false;
}

auto Snake::get_body() -> std::vector<coord> {
  return std::move(std::vector<coord>(m_body_pos.begin(), m_body_pos.end() - 1));
}
