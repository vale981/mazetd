#ifndef FACTORY_H
#define FACTORY_H 1
#include "./maze_generator/maze_generator.hpp"
#include "components/components.hpp"
#include "definitions.h"
#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>

namespace factory {
entt::entity bg_tile(entt::registry &reg, char index, const sf::Vector2i &pos) {
  auto e = reg.create();

  reg.assign<Tile>(e, pos, index);
  reg.assign<Background>(e);

  return e;
}

std::vector<entt::entity> background_maze(entt::registry &reg, int width,
                                          int height) {
  auto maze = maze_generator::generate_maze(width, height);
  std::vector<entt::entity> maze_entities;
  maze_entities.reserve(width * height);

  for (int y{0}; y < height; y++) {
    for (int x{0}; x < width; x++) {
      auto e = bg_tile(reg, maze[x + y * width] ? 0 : 1, {x, y});
      maze_entities.push_back(e);
    }
  }

  return maze_entities;
}
} // namespace factory

#endif
