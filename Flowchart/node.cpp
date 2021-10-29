#include "node.hpp"

sf::RoundedRectangleShape Node::fieldTmplt;
sf::Font Node::font;
sf::Text Node::textTmplt;

Node::Node() {
  field = sf::RoundedRectangleShape(fieldTmplt);
  text = sf::Text(textTmplt);
}

Node::Node(const sf::Vector2f& pos) {
  field = sf::RoundedRectangleShape(fieldTmplt);
  text = sf::Text(textTmplt);

  field.setPosition(sf::Vector2f(pos));
  text.setPosition(sf::Vector2f(pos));
}

void Node::draw(sf::RenderWindow& window) const {
  window.draw(field);
  window.draw(text);
}

sf::Vector2f Node::getPosition() const {
  return field.getPosition();
}

void Node::setPosition(const sf::Vector2f& pos) {
  field.setPosition(sf::Vector2f(pos));
  text.setPosition(sf::Vector2f(pos));
}

void Node::setEdit() {
  field.setFillColor(sf::Color(230, 230, 230));
  field.setOutlineColor(sf::Color(204, 0, 0));
}

void Node::setLock() {
  field.setFillColor(sf::Color(230, 230, 230));
  field.setOutlineColor(sf::Color(0, 0, 0));
}

bool Node::contains(const sf::Vector2f& pos) const {
  return field.getGlobalBounds().contains(sf::Vector2f(pos));
}

bool Node::protrudes(const sf::RenderWindow& window) const {
  sf::FloatRect bounds = field.getGlobalBounds();
  return (bounds.left < 10.f || bounds.left + bounds.width + 10.f > window.getSize().x
    || bounds.top < 10.f || bounds.top + bounds.height + 10.f > window.getSize().y);
}

bool Node::overlaps(const std::vector<Node>& nodes) const {
  bool intersects = false;
  for (const Node& node : nodes) {
    if (field.getGlobalBounds().intersects(node.field.getGlobalBounds())) {
      intersects = true;
      break;
    }
  }
  return intersects;
}

void Node::appendText(const sf::String& str) {
  sf::String orig = text.getString();

  text.setString(orig + str);

  while (text.getGlobalBounds().width > field.getGlobalBounds().width * 0.9) {
    unsigned int newCharSize = text.getCharacterSize() - 1;
    if (newCharSize < 14) {
      text.setString(orig);
      break;
    }
    else {
      text.setCharacterSize(newCharSize);
    }
  }
  text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
}