#ifndef MAZETD_H
#define MAZETD_H 1

#include "definitions.h"
#include "factory.hpp"
#include "tilemap/tilemap.hpp"
//#include "maze_generator/maze_generator.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <entt/entt.hpp>
#include <string>

const struct config_t {
  const unsigned int height{15};
  const unsigned int width{15};
  const spd::level::level_enum log_level{spd::level::debug};
  const std::string bg_tilemap_location{"./assets/bg_tilemap.tga"};
  const unsigned int FPS{1};
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
