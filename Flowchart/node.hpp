#pragma once

#include <set>
#include "roundedRectangle.hpp"
#include "Gui/Gui.hpp"

enum class NodeState { Placing, Highlighted, Selected, Moving, Locked };

class Node {
public:
  Node();
  Node(const sf::Vector2f& pos);

  void draw(sf::RenderWindow& window) const;

  bool contains(const sf::Vector2f& pos) const;
  bool onTextbox(const sf::Vector2f& pos) const;

  void setPosition(const sf::Vector2f& pos);
  void setState(NodeState nodeState);

  sf::Vector2f getCenter() const;
  sf::FloatRect getBounds() const;
  NodeState getState() const;

  void addText(sf::Uint32 unicode);
  void setTextboxState(gui::State textboxState);
  void setTextCursor(const sf::Vector2f& pos);
  void applyToText(const sf::Event::KeyEvent& key);

  void point(size_t i);
  bool pointsTo(size_t i) const;

  static sf::RoundedRectangleShape fieldTmplt;

private:
  NodeState state;

  sf::RoundedRectangleShape field;
  gui::TextBox textbox;

  std::set<size_t> pointing;
};
