#include <SFML/Graphics.hpp>
#include "roundedRectangle.hpp"
#include "Gui/TextBox.hpp"
#include "Gui/Theme.hpp"

enum class NodeState { Placing, Selected, Locked };

class Node {
public:
  Node();
  Node(const sf::Vector2f& pos);

  void draw(sf::RenderWindow& window) const;

  sf::Vector2f getPosition() const;

  void setPosition(const sf::Vector2f& pos);
  void setState(NodeState state);

  bool contains(const sf::Vector2f& pos) const;
  bool protrudes(const sf::RenderWindow& window) const;
  bool overlaps(const std::vector<Node>& nodes) const;

  static sf::RoundedRectangleShape fieldTmplt;

private:
  sf::RoundedRectangleShape field;
  gui::TextBox textbox;
};
