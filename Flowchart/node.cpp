#include "node.hpp"

Node::Node() {

}

Node::Node(const sf::Vector2i& pos) {
  field.setPosition(sf::Vector2f(pos));
  text.setPosition(sf::Vector2f(pos)); // TODO
}

void Node::draw() {

}

bool Node::contains(const sf::Vector2i& pos) {
  return field.getGlobalBounds().contains(sf::Vector2f(pos));
}

bool Node::protrudes(const sf::RenderWindow& window) {
  sf::FloatRect bounds = field.getGlobalBounds();
  return (bounds.left > 10.f && bounds.left + bounds.width + 10.f < window.getSize().x
    && bounds.top > 10.f && bounds.top + bounds.height + 10.f < window.getSize().y);
}

bool Node::overlaps(const std::vector<Node>& nodes) {
  bool intersects = false;
  for (const Node& node : nodes) {
    if (field.getGlobalBounds().intersects(node.field.getGlobalBounds())) {
      intersects = true;
      break;
    }
  }
  return intersects;
}

void Node::setFillColor(const sf::Color& color) {
  field.setFillColor(color);
}

void Node::setString(sf::String str) {
  text.setString(str);
}