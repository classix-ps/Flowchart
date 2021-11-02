#include "conversions.hpp"

sf::Vector2i posToCell(const sf::Vector2f& pos) {
  return sf::Vector2i(int(pos.x / 4), int(pos.y / 4));
}