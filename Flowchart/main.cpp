#include "node.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

enum class State {Default, Text, MovingField};

std::vector<Node>::iterator getClickedNode(std::vector<Node>& nodes, sf::Vector2f pos) {
  std::vector<Node>::iterator iter;
  for (iter = nodes.begin(); iter != nodes.end(); iter++) {
    if (iter->contains(pos)) {
      break;
    }
  }

  return iter;
}

int main() {
  sf::Vector2f targetWindowSize(sf::VideoMode::getDesktopMode().width * 0.8, sf::VideoMode::getDesktopMode().height * 0.8);

  sf::Vector2f fieldSize(150.f, 100.f);
  sf::Vector2f gridSize(fieldSize.x * 1.2, fieldSize.y * 1.4);
  sf::Vector2u gridCount(12, 9);

  // Window
  sf::Vector2u origWindowSize(unsigned int(gridSize.x * gridCount.x), unsigned int(gridSize.y * gridCount.y));
  sf::RenderWindow window(sf::VideoMode(unsigned int(gridSize.x * gridCount.x), unsigned int(gridSize.y * gridCount.y)), "Flowchart Creator");

  // Grid
  std::vector<sf::VertexArray> grid;
  sf::VertexArray line(sf::Lines, 2);
  line[0].color = sf::Color(150, 150, 150);
  line[1].color = line[0].color;
  for (size_t i = 1; i < gridSize.x; i++) {
    line[0].position = sf::Vector2f(gridSize.x * i, 0.f);
    line[1].position = sf::Vector2f(gridSize.x * i, float(window.getSize().y));
    grid.push_back(line);
  }
  for (size_t i = 1; i < gridSize.y; i++) {
    line[0].position = sf::Vector2f(0.f, gridSize.y * i);
    line[1].position = sf::Vector2f(window.getSize().x, float(gridSize.y * i));
    grid.push_back(line);
  }

  window.setSize(sf::Vector2u(targetWindowSize));

  // Nodes
  std::vector<Node> nodes;

  // Node template
  Node::fieldTmplt.setSize(sf::Vector2f(150.f, 100.f));
  Node::fieldTmplt.setCornersRadius(10.f);
  Node::fieldTmplt.setCornerPointCount(10);
  Node::fieldTmplt.setOutlineThickness(5.f);
  Node::fieldTmplt.setOrigin(sf::Vector2f(Node::fieldTmplt.getSize().x / 2, Node::fieldTmplt.getSize().y / 2));

  // Font
  Node::font.loadFromFile("arial.ttf");

  // Text template
  Node::textTmplt.setFont(Node::font);
  Node::textTmplt.setCharacterSize(24);
  Node::textTmplt.setFillColor(sf::Color::Black);

  State state = State::Default;
  Node* selected = NULL;
  sf::Vector2f offset;

  // Process loop
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

      switch (state) {
      case State::Default:
      {
        // Create/select node
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          std::vector<Node>::iterator clickedNode = getClickedNode(nodes, mousePos);
          if (clickedNode != nodes.end()) {
            if (selected == &(*clickedNode)) {
              selected->setEdit();
              state = State::Text;
            }
            else {
              selected = &(*clickedNode);
              offset = selected->getPosition() - mousePos;
              state = State::MovingField;
            }
          }
          else {
            Node newNode(mousePos);

            if (newNode.protrudes(window)) {
              break;
            }

            if (newNode.overlaps(nodes)) {
              break;
            }

            newNode.setEdit();
            nodes.push_back(newNode);
            selected = &nodes.back();
            state = State::Text;
          }
        }

        break;
      };
      case State::Text:
      {
        if (selected == NULL) {
          std::cout << "Error with selected field" << std::endl;
          return 0;
        }

        // Lock in node
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
          selected->setLock();
          selected = NULL;
          state = State::Default;
        }
        // Lock in node, move other node if clicked
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          if (selected->contains(mousePos)) {
            offset = selected->getPosition() - mousePos;
            state = State::MovingField;
          }

          selected->setLock();
          std::vector<Node>::iterator clickedNode = getClickedNode(nodes, mousePos);
          if (clickedNode != nodes.end()) {
            selected = &(*clickedNode);
            offset = selected->getPosition() - mousePos;
            state = State::MovingField;
          }
          else {
            selected = NULL;
            state = State::Default;
          }
        }
        // Add text to node
        else if (event.type == sf::Event::TextEntered) {
          if (event.text.unicode > 31 && event.text.unicode < 127) {
            selected->appendText(event.text.unicode);
          }
          else if (event.text.unicode == 8) {
            // TODO: backspace with size increase
          }
        }

        break;
      };
      case State::MovingField:
      {
        if (selected == NULL) {
          std::cout << "Error with selected field" << std::endl;
          return 0;
        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
          state = State::Default;
        }
        else {
          selected->setPosition(mousePos + offset);
        }

        break;
      };
      }
    }

    window.clear(sf::Color::White);
    for (const sf::VertexArray& line : grid) {
      window.draw(line);
    }
    for (const Node& node : nodes) {
      node.draw(window);
    }
    window.display();
  }

  return 0;
}