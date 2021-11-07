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
  std::set<size_t> getPointed() const;

  void addText(sf::Uint32 unicode);
  void setTextboxState(gui::State textboxState);
  void setTextCursor(const sf::Vector2f& pos);
  void applyToText(const sf::Event::KeyEvent& key);
  sf::String getText() const;

  void point(size_t i);
  bool pointsTo(size_t i) const;
  void removePointing(size_t i);
  void lowerPointing(size_t i);

  static sf::RoundedRectangleShape fieldTmplt;

private:
  NodeState state;

  sf::RoundedRectangleShape field;
  gui::TextBox textbox;

  std::set<size_t> pointing;
};
