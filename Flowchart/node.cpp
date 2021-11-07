#include "node.hpp"

sf::RoundedRectangleShape Node::fieldTmplt;

Node::Node() : textbox{ 250.f } {
  field = sf::RoundedRectangleShape(fieldTmplt);
}

Node::Node(const sf::Vector2f& pos) : textbox{ 250.f } {
  field = sf::RoundedRectangleShape(fieldTmplt);

  field.setPosition(pos);
  textbox.setPosition(pos - textbox.getSize() / 2.f);
}

void Node::draw(sf::RenderWindow& window) const {
  window.draw(field);
  window.draw(textbox);
}

bool Node::contains(const sf::Vector2f& pos) const {
  return field.getGlobalBounds().contains(pos);
}

bool Node::onTextbox(const sf::Vector2f& pos) const {
  return textbox.containsPoint(pos - textbox.getPosition());
}

void Node::setPosition(const sf::Vector2f& pos) {
  field.setPosition(pos);
  textbox.setPosition(pos - textbox.getSize() / 2.f);
}

void Node::setState(NodeState nodeState) {
  state = nodeState;

  switch (state) {
  case NodeState::Placing:
    field.setFillColor(sf::Color(230, 230, 230, 150));
    field.setOutlineColor(sf::Color(0, 0, 0, 150));
    break;
  case NodeState::Highlighted:
    field.setFillColor(sf::Color(230, 230, 230, 255));
    field.setOutlineColor(sf::Color(0, 102, 51, 255));
    break;
  case NodeState::Locked:
    field.setFillColor(sf::Color(230, 230, 230, 255));
    field.setOutlineColor(sf::Color(0, 0, 0, 255));
    break;
  case NodeState::Selected:
    field.setFillColor(sf::Color(230, 230, 230, 255));
    field.setOutlineColor(sf::Color(0, 0, 153, 255));
    break;
  case NodeState::Moving:
    field.setFillColor(sf::Color(230, 230, 230, 255));
    field.setOutlineColor(sf::Color(153, 0, 0, 255));
    break;
  }
}

sf::Vector2f Node::getCenter() const {
  return field.getPosition();
}

sf::FloatRect Node::getBounds() const {
  return field.getGlobalBounds();
}

NodeState Node::getState() const {
  return state;
}

std::set<size_t> Node::getPointed() const {
  return pointing;
}

void Node::addText(sf::Uint32 unicode) {
  textbox.onTextEntered(unicode);
}

void Node::setTextboxState(gui::State textboxState) {
  textbox.onStateChanged(textboxState);
}

void Node::setTextCursor(const sf::Vector2f& pos) {
  sf::Vector2f textPos = pos - textbox.getPosition();
  textbox.onMousePressed(textPos.x, textPos.y);
}

void Node::applyToText(const sf::Event::KeyEvent& key) {
  textbox.onKeyPressed(key);
}

sf::String Node::getText() const {
  return textbox.getText();
}

void Node::point(size_t i) {
  pointing.insert(i);
}

bool Node::pointsTo(size_t i) const {
  return std::find(pointing.begin(), pointing.end(), i) != pointing.end();
}

void Node::removePointing(size_t i) {
  pointing.erase(i);
}

void Node::lowerPointing(size_t i) {
  std::set<size_t> greaterValues;
  for (std::set<size_t>::iterator iter = pointing.upper_bound(i); iter != pointing.end(); iter++) {
    greaterValues.insert(*iter);
  }
  for (const size_t& val : greaterValues) {
    pointing.erase(val);
    pointing.insert(val - 1);
  }
}