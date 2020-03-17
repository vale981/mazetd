#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H 1
#include <iostream>
#include <random>
#include <vector>

namespace maze_generator {
void dump_maze(const std::vector<bool> &maze, const int width, const int height) {
  for (int y{0}; y < height; y++) {
    for (int x{0}; x < width; x++) {
      std::cout << (maze.at(x + y * width) ? " " : "█");
    }

    std::cout << '\n';
  }
}

  std::vector<bool> generate_maze(const int width, const int height);
  std::vector<bool> generate_maze(const int width, const int height, const unsigned long seed);

  std::vector<bool> generate_maze(const int width, const int height) {
    std::random_device rd;
    return generate_maze(width, height, rd());
  }

} // namespace maze_generator

#endif
