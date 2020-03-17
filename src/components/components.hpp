#ifndef COMPONENTS_H
#define COMPONENTS_H 1
#include "spdlog/fmt/ostr.h"
#include <SFML/System/Vector2.hpp>
#include <string>

struct Background {};

/**
   Tile component for discrete blocks.
 **/
struct Tile {
  sf::Vector2i pos;
  char index;

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const Tile &c) {
    return os << "Tile {pos={ x=" << c.pos.x << "; y=" << c.pos.y
              << " }; index=" << static_cast<int>(c.index) << "}";
  }
};

// class Sprite {
//   Sprite(std::string texturepath);

// };

#endif // COMPONENTS_H
