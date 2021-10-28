#include "node.hpp"

#include <iostream>

enum class State {Default, Text, MovingField};

int main() {
  // Window
  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Flowchart Creator");

  // Nodes
  std::vector<Node> nodes;

  // Node template
  Node::fieldTmplt.setSize(sf::Vector2f(150.f, 100.f));
  Node::fieldTmplt.setCornersRadius(10.f);
  Node::fieldTmplt.setCornerPointCount(10);
  Node::fieldTmplt.setFillColor(sf::Color::White);
  Node::fieldTmplt.setOutlineColor(sf::Color::Black);
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
  sf::String input;

  // Process loop
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      switch (state) {
      case State::Default:
      {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          sf::Vector2i mousePos = sf::Mouse::getPosition(window);

          std::vector<Node>::iterator iter;
          for (iter = nodes.begin(); iter != nodes.end(); iter++) {
            if (iter->contains(sf::Mouse::getPosition(window))) {
              break;
            }
          }

          if (iter != nodes.end()) {
            selected = &(*iter);
            state = State::MovingField;
          }
          else {
            Node newNode(sf::Mouse::getPosition(window));

            if (newNode.protrudes(window)) {
              break;
            }

            if (newNode.overlaps(nodes)) {
              break;
            }

            nodes.push_back(newNode);
            selected = &nodes.back();

            state = State::Text;
          }
        }

        break;
      };
      case State::Text:
      {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
          || (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !selected->contains(sf::Mouse::getPosition(window)))) {
          selected->setFillColor(sf::Color(220, 220, 220));
          input.clear();
          state = State::Default;
        }
        else if (event.type == sf::Event::TextEntered) {
          if (event.text.unicode > 31 && event.text.unicode < 127) {
            sf::String newInput = input + event.text.unicode;
            selected->setString(newInput);
            bool fits = true;
            while (currentText.getGlobalBounds().width > currentField.getGlobalBounds().width * 0.9 && fits) {
              unsigned int newCharSize = currentText.getCharacterSize() - 1;
              if (newCharSize < 12) {
                currentText.setString(input);
                fits = false;
              }
              else {
                currentText.setCharacterSize(newCharSize);
              }
            }
            currentText.setOrigin(currentText.getGlobalBounds().width / 2.f, currentText.getGlobalBounds().height / 2.f);
            currentText.setPosition(currentField.getPosition());

            if (fits) {
              input = newInput;
            }
          }
          else if (event.text.unicode == 8) {
            // TODO: backspace with size increase
          }
        }

        break;
      };
      case State::MovingField:
      {
        // TODO: Move field until mouse release
        if (selectedField == NULL) {
          std::cout << "Error with selected field" << std::endl;
          return 0;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
          state = State::Default;
        }
        else {
          selectedField->setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
        }

        break;
      };
      }
    }

    window.clear(sf::Color::White);
    for (const sf::RoundedRectangleShape& field : fields) {
      window.draw(field);
    }
    for (const sf::Text& text : texts) {
      window.draw(text);
    }
    window.display();
  }

  return 0;
}