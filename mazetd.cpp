#include "mazetd.hpp"
#include <SFML/Graphics.hpp>
#include "maze_generator/maze_generator.hpp"

Game::Game()
  : m_reg{}, m_game_state{Game::running},
    m_background_system{config.bg_tilemap_location, config.width,
                        config.height, sf::Vector2u{64, 64}, 2,
                        m_reg}
{
  m_reg.set<config_t>(config);

  // test changing
  auto maze = generate_maze(m_reg, config.width, config.height, 1);
  m_background_system.update();

  // create the window
  sf::RenderWindow window(sf::VideoMode(512, 512), "Tilemap");
  window.setFramerateLimit(config.FPS);
  while (window.isOpen())
    {
      // handle events
      sf::Event event;
      while (window.pollEvent(event))
        {
          if(event.type == sf::Event::Closed)
            window.close();
        }

      // draw the map
      window.clear();
      window.draw(m_background_system);
      window.display();
    }

}

void set_up_logging() {
  spd::set_level(config.log_level);
}

int main() {
  set_up_logging();
  Game game{};

  return 0;
}
