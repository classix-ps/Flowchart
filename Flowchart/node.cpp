#include "node.hpp"

sf::RoundedRectangleShape Node::fieldTmplt;

Node::Node() {
  field = sf::RoundedRectangleShape(fieldTmplt);

  sf::Text text("", gui::Theme::getFont());
  text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  textbox.setCallback([&]() {
    text.setString(textbox.getText());
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  });
}

Node::Node(const sf::Vector2f& pos) {
  field = sf::RoundedRectangleShape(fieldTmplt);

  sf::Text text("", gui::Theme::getFont());
  text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  textbox.setCallback([&]() {
    text.setString(textbox.getText());
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  });

  field.setPosition(sf::Vector2f(pos));
  text.setPosition(sf::Vector2f(pos));
}

void Node::draw(sf::RenderWindow& window) const {
  window.draw(field);
  window.draw(textbox);
}

sf::Vector2f Node::getPosition() const {
  return field.getPosition();
}

void Node::setPosition(const sf::Vector2f& pos) {
  field.setPosition(sf::Vector2f(pos));
  textbox.setPosition(sf::Vector2f(pos));
}

void Node::setState(NodeState state) {
  switch (state) {

  }
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