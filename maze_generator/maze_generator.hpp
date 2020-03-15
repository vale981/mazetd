#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H 1
#include <random>
#include <vector>
#include <entt/entt.hpp>
#include <random>
#include <stack>
#include "../factory.hpp"
#include <iostream>

void dump_maze(std::vector<bool> &maze, int width, int height) {
  for (int y {0}; y < height; y++) {
    for (int x {0}; x < width; x++) {
      std::cout << (maze.at(x + y*width) ? " " : "█");
    }

    std::cout << '\n';
  }
}

template<typename... MazeComponentTypes>
std::vector<entt::entity>
generate_maze(entt::registry &reg,
              int width, int height, unsigned long seed) {

  // TODO: check width, height
  using intpair = std::pair<int, int>;
  std::uniform_int_distribution<int> uniform_dist{0, 4}; // Note: think of a better way
  std::default_random_engine engine{seed};

  std::vector<bool> maze(width*height, false);
  std::stack<int> stack;

  maze.at(0) = true;
  stack.push(0);

  auto wrap_x =
    [=](int x) {
      return x < 0 ? (width + x) : (x % width);
    };

  auto wrap_y =
    [=](int y) {
      return y < 0 ? (height + y) : (y % height);
    };

  auto wrap_cell =
    [=](std::pair<int, int> in) {
      return std::make_pair(wrap_x(in.first), wrap_y(in.second));
    };

  auto get_cell =
    [=](std::pair<int, int> in) {
        return in.first + width*in.second;
    };

  auto get_coords =
    [=] (int current) {
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

    std::array<intpair, 4> neightbours
      {{ {x, y - 2}, {x, y + 2}, {x - 2, y}, {x + 2, y} }};
    std::transform(neightbours.begin(), neightbours.end(),
                   neightbours.begin(), wrap_cell);

    std::array<intpair, 4> walls
      {{ {x, y - 1}, {x, y + 1}, {x - 1, y}, {x + 1, y} }};
    std::transform(walls.begin(), walls.end(),
                   walls.begin(), wrap_cell);

    std::vector<char> unvisited;
    unvisited.reserve(4);

    for (char i {0}; i < 4; i++) {
      if(!maze.at(get_cell(neightbours.at(i)))) {
        unvisited.push_back(i);
      }
    }

    if (unvisited.empty())
      continue;

    stack.push(current);
    int chosen = unvisited.at(uniform_dist(engine) % unvisited.size());

    auto neighbour = get_cell(neightbours[chosen]);
    maze.at(get_cell(walls[chosen])) = true;
    maze.at(neighbour) = true;
    stack.push(neighbour);


    spd::debug("x={} y={} chosen=[ x={} y={} ] stack[{}]={}",
               x, y, neightbours.at(chosen).first, neightbours.at(chosen).second,
               stack.size() - 1, stack.top());
    dump_maze(maze, width, height);
  }

  std::vector<entt::entity> maze_entities;
  maze_entities.reserve(width*height);

  for (int y {0}; y < height; y++) {
    for (int x {0}; x < width; x++) {
      auto e = create_bg_tile(reg, maze[x + y*width] ? 0 : 1, {x, y});
      maze_entities.push_back(e);
    }
  }

  return maze_entities;
}


std::vector<entt::entity>
generate_maze(entt::registry &reg, int width, int height) {
  std::random_device rd;
  return generate_maze(reg, width, height, rd());
}

// class MazeGenerator {
// public:
//   MazeGenerator(const uint width, const uint height)
//     : m_width{width}, m_height{height} {};

//   void generate(int);

// private:
//   bool **m_maze;

//   const uint m_width;
//   const uint m_height;

//   const std::random_device m_rand;
// };

#endif
