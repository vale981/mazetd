#ifndef MAZETD_H
#define MAZETD_H 1

#include "definitions.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <string>
#include <entt/entt.hpp>
#include "tilemap/tilemap.hpp"
#include "factory.hpp"

const struct config_t {
  const unsigned int height{14};
  const unsigned int width{14};
  const spd::level::level_enum log_level{spd::level::debug};
  const std::string bg_tilemap_location{"./assets/bg_tilemap.tga"};
  const unsigned int FPS{60};
} config;

class Game {
public:
  Game();

private:
  entt::registry m_reg;

  enum { stopped, running, paused } m_game_state;

  TileMapSystem<Background> m_background_system;
};


#endif // MAZETD_H
