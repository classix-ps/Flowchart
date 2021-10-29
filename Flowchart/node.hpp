#include <iostream>

#include <SFML/Graphics.hpp>
#include "roundedRectangle.hpp"
#include "Gui/TextBox.hpp"

class Node {
public:
  Node();
  Node(const sf::Vector2f& pos);
  void draw(sf::RenderWindow& window) const;

  sf::Vector2f getPosition() const;

  void setPosition(const sf::Vector2f& pos);
  void setEdit();
  void setLock();

  // Field
  bool contains(const sf::Vector2f& pos) const;
  bool protrudes(const sf::RenderWindow& window) const;
  bool overlaps(const std::vector<Node>& nodes) const;

  // Text
  void appendText(const sf::String& str);

  static sf::RoundedRectangleShape fieldTmplt;
  static sf::Font font;
  static sf::Text textTmplt;
private:
  sf::RoundedRectangleShape field;
  sf::Text text;
};
