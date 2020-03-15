#ifndef TILEMAP_H
#define TILEMAP_H 1

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <entt/entt.hpp>

#include <string>
#include <filesystem>

#include "../components/components.hpp"
#include "../definitions.h"

template <typename ComponentType>
class TileMapSystem : public sf::Drawable, public sf::Transformable {
public:
  TileMapSystem(const std::string &tileset, const unsigned int width,
                const unsigned int height, const sf::Vector2u tile_size,
                const unsigned int tile_count, entt::registry &reg) :
                m_width{width}, m_height{height}, m_tile_size{tile_size},
                m_tile_count{tile_count}, m_reg{reg},
                m_observer{m_reg, entt::collector.replace<Tile>().where<ComponentType>()}
  {
    spd::debug("Creating {}", *this);

    if (!m_tileset.loadFromFile(tileset)) {
      throw std::filesystem::filesystem_error
        (("Could not load the tileset: " + tileset).c_str(),
         std::error_code());
    }

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    m_reg.on_construct<ComponentType>()
      .template connect<&TileMapSystem<ComponentType>::on_tile_create>(*this);
  }

  ~TileMapSystem() {
    m_reg.on_construct<ComponentType>()
          .template disconnect<&TileMapSystem<ComponentType>::on_tile_create>(*this);

  }

  void update() {
    for (const auto &entity : m_observer) {
      const auto tile = m_reg.get<Tile>(entity);
      update(tile);
    }

    m_observer.clear();
  }

  void update(const Tile &tile) {
    auto pos = tile.pos;

    int tu = tile.index % (m_tileset.getSize().x / m_tile_size.x);
    int tv = tile.index / (m_tileset.getSize().x / m_tile_size.x);
    sf::Vertex* quad = &m_vertices[(pos.x + pos.y * m_width) * 4];

    // define its 4 corners
    quad[0].position = sf::Vector2f(pos.x * m_tile_size.x, pos.y * m_tile_size.y);
    quad[1].position = sf::Vector2f((pos.x + 1) * m_tile_size.x, pos.y * m_tile_size.y);
    quad[2].position = sf::Vector2f((pos.x + 1) * m_tile_size.x, (pos.y + 1) * m_tile_size.y);
    quad[3].position = sf::Vector2f(pos.x * m_tile_size.x, (pos.y + 1) * m_tile_size.y);

    // define its 4 texture coordinates
    quad[0].texCoords = sf::Vector2f(tu * m_tile_size.x, tv * m_tile_size.y);
    quad[1].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, tv * m_tile_size.y);
    quad[2].texCoords = sf::Vector2f((tu + 1) * m_tile_size.x, (tv + 1) * m_tile_size.y);
    quad[3].texCoords = sf::Vector2f(tu * m_tile_size.x, (tv + 1) * m_tile_size.y);
  }

  void on_tile_create(const entt::registry &reg, const entt::entity &e) {
      const auto tile = reg.get<Tile>(e);
      update(tile);
  }

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const TileMapSystem<ComponentType> &c) {
    return os << typeid(TileMapSystem<ComponentType>).name() << " Width=" << c.m_width
              << " Height=" << c.m_height ;
  }

private:
  sf::VertexArray m_vertices;
  sf::Texture m_tileset;

  const unsigned int m_width;
  const unsigned int m_height;
  const sf::Vector2u m_tile_size;
  const unsigned int m_tile_count;

  entt::registry &m_reg;
  entt::observer m_observer;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
  };
};



#endif // TILEMAP_H
