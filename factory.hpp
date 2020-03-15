#ifndef FACTORY_H
#define FACTORY_H 1
#include <entt/entt.hpp>
#include "definitions.h"
#include "components/components.hpp"
#include <SFML/System/Vector2.hpp>

entt::entity create_bg_tile(entt::registry &reg, char index, const sf::Vector2i &pos) {
  auto e = reg.create();

  reg.assign<Tile>(e, pos, index);
  reg.assign<Background>(e);

  return e;
}

#endif
