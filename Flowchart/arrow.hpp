#pragma once

#define _USE_MATH_DEFINES

#include "node.hpp"
#include <array>

class Arrow {
public:
  Arrow();

  void draw(sf::RenderWindow& window) const;

  bool onArrow(const sf::Vector2f& pos);

  void highlight();
  void lock();

  void setOrigin(const sf::Vector2f& pos);
  void setDestination(const sf::Vector2f& pos);
  void setOriginNode(size_t index);
  void setDestinationNode(size_t index);

  void setHead(const sf::FloatRect& boundingRect);

  void setDirectLineColor(sf::Color color);

  sf::Vector2f getDestination() const;
  size_t getOriginNode() const;
  size_t getDestinationNode() const;

  static std::array<float, 8> angleTemplate;

private:
  // segments are an idea for standardizing arrow paths in grid
  //std::vector<sf::VertexArray> segments;
  sf::VertexArray directLine;
  sf::CircleShape head;

  sf::Vector2f origin;
  sf::Vector2f destination;

  size_t originNode = 0;
  size_t destinationNode = 0;
};