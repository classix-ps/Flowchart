#include "node.hpp"

sf::RoundedRectangleShape Node::fieldTmplt;

Node::Node() /*: textbox{30.f}*/ {
  field = sf::RoundedRectangleShape(fieldTmplt);
  /*
  sf::Text text("", gui::Theme::getFont());
  text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  textbox.setCallback([&]() {
    text.setString(textbox.getText());
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  });*/
}

Node::Node(const sf::Vector2f& pos) /*: textbox{30.f}*/ {
  field = sf::RoundedRectangleShape(fieldTmplt);
  /*
  sf::Text text("", gui::Theme::getFont());
  text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  textbox.setCallback([&]() {
    text.setString(textbox.getText());
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  });*/

  field.setPosition(pos);
  //textbox.setPosition(pos - sf::Vector2f(textbox.getSize().x / 2, textbox.getSize().y / 2));
}

void Node::draw(sf::RenderWindow& window) const {
  window.draw(field);
  //window.draw(textbox);
}

bool Node::contains(const sf::Vector2f& pos) const {
  return field.getGlobalBounds().contains(pos);
}

void Node::setPosition(const sf::Vector2f& pos) {
  field.setPosition(pos);
  //textbox.setPosition(pos - sf::Vector2f(15.f, 8.f));
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

std::vector<sf::Vector2i> Node::getCells() const {
  std::vector<sf::Vector2i> cells;
  sf::Vector2i center = posToCell(this->getCenter());
  for (int xIter = -5; xIter <= 5; xIter++) {
    for (int yIter = -3; yIter <= 3; yIter++) {
      cells.push_back(sf::Vector2i(center.x + xIter, center.y + yIter));
    }
  }

  return cells;
}

sf::FloatRect Node::getBounds() const {
  return field.getGlobalBounds();
}

NodeState Node::getState() const {
  return state;
}

void Node::point(size_t i) {
  pointing.insert(i);
}

bool Node::pointsTo(size_t i) const {
  return std::find(pointing.begin(), pointing.end(), i) != pointing.end();
}