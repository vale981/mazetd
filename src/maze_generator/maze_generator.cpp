#include "maze_generator/maze_generator.hpp"
#include <cassert>
#include <array>
#include <stack>

using namespace maze_generator;

std::vector<bool> generate_maze(const int width, const int height, const unsigned long seed) {
  assert(width > 0 && height > 0 && "Width and height have to be positive.");

  using intpair = std::pair<int, int>;
  std::uniform_int_distribution<int> uniform_dist{
      0, 4}; // Note: think of a better way
  std::default_random_engine engine{seed};

  std::vector<bool> maze(width * height, false);
  std::stack<int> stack;

  maze.at(0) = true;
  stack.push(0);

  auto get_cell = [=](std::pair<int, int> in) {
    return in.first + width * in.second;
  };

  auto get_coords = [=](int current) {
    auto x = (current % width);
    return std::make_pair(x, (current - x) / width);
  };

  while (!stack.empty()) {
    auto current = stack.top();
    auto [x, y] = get_coords(current);
    stack.pop();

/*
  1
  2 x 3 -- cell layout
  4

  We use toroidal boundary conditions.
*/

    std::array<intpair, 4> neighbours{
        {{x, y - 2}, {x, y + 2}, {x - 2, y}, {x + 2, y}}};

    std::array<intpair, 4> walls // FIXME: i know... kinda to much
        {{{x, y - 1}, {x, y + 1}, {x - 1, y}, {x + 1, y}}};

    std::vector<char> unvisited;
    unvisited.reserve(4);

    for (char i{0}; i < 4; i++) {
      auto neighbour{neighbours.at(i)};
      if (neighbour.first >= 0 && neighbour.second >= 0 &&
          neighbour.first < height && neighbour.second < width &&
          !maze.at(get_cell(neighbour))) {
        unvisited.push_back(i);
      }
    }

    if (unvisited.empty())
      continue;

    stack.push(current);
    int chosen = unvisited.at(uniform_dist(engine) % unvisited.size());

    auto neighbour = get_cell(neighbours[chosen]);
    maze.at(get_cell(walls[chosen])) = true;
    maze.at(neighbour) = true;
    stack.push(neighbour);

    // spd::debug("x={} y={} chosen=[ x={} y={} ] stack[{}]={}",
    //            x, y, neighbours.at(chosen).first,
    //            neighbours.at(chosen).second, stack.size() - 1,
    //            stack.top());
    // dump_maze(maze, width, height);
  }

  return maze;
}
