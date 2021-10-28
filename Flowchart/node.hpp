#include <iostream>

#include <SFML/Graphics.hpp>
#include "roundedRectangle.hpp"

class Node {
public:
  Node();
  Node(const sf::Vector2i& pos);
  void draw();

  bool contains(const sf::Vector2i& pos);
  bool protrudes(const sf::RenderWindow& window);
  bool overlaps(const std::vector<Node>& nodes);

  void setFillColor(const sf::Color& color);
  void setString(sf::String str);

  static sf::RoundedRectangleShape fieldTmplt;
  static sf::Font font;
  static sf::Text textTmplt;
private:
  sf::RoundedRectangleShape field;
  sf::Text text;
  sf::Vector2i pos;
};
