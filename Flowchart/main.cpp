#include <SFML/Graphics.hpp>
#include "roundedRectangle.hpp"

#include <iostream>

enum class State {Default, Text, MovingField};

int main() {
  // Window
  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Flowchart Creator");

  // Text boxes
  std::vector<sf::RoundedRectangleShape> fields;
  sf::RoundedRectangleShape rect(sf::Vector2f(150.f, 100.f), 10.f, 10);
  rect.setFillColor(sf::Color::White);
  rect.setOutlineColor(sf::Color::Black);
  rect.setOutlineThickness(5.f);
  rect.setOrigin(sf::Vector2f(rect.getSize().x / 2, rect.getSize().y / 2));

  // Text
  sf::String input;
  std::vector<sf::Text> texts;
  sf::Font font;
  if (!font.loadFromFile("arial.ttf")) {
    return 0;
  }
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(24);
  text.setFillColor(sf::Color::Black);

  State state = State::Default;

  // Process loop
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      else if (state == State::Default && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        std::vector<sf::RoundedRectangleShape>::iterator iter;
        for (iter = fields.begin(); iter != fields.end(); iter++) {
          if (iter->getGlobalBounds().contains(sf::Vector2f(mousePos))) {
            break;
          }
        }

        if (iter != fields.end()) {
          state = State::MovingField;
        }
        else {
          sf::RoundedRectangleShape newRect(rect);
          newRect.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));

          // Check for intersection with window border
          sf::FloatRect bounds = newRect.getGlobalBounds();
          if (bounds.left < 10.f || bounds.left + bounds.width + 10.f > window.getSize().x || bounds.top < 10.f || bounds.top + bounds.height + 10.f > window.getSize().y) {
            break;
          }

          // Check for intersection with other fields
          bool intersects = false;
          for (const sf::RoundedRectangleShape& field : fields) {
            if (field.getGlobalBounds().intersects(newRect.getGlobalBounds())) {
              intersects = true;
              break;
            }
          }
          if (intersects) {
            continue;
          }

          // Valid position
          fields.push_back(newRect);

          state = State::Text;
          texts.push_back(text);
        }
      }
      else if (state == State::MovingField) {
        // TODO: Move field until mouse release
      }
      else if (state == State::Text
        && ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
          || (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !fields.back().getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))))) {
        fields.back().setFillColor(sf::Color(220, 220, 220));

        input.clear();
        state = State::Default;
      }
      else if (state == State::Text && event.type == sf::Event::TextEntered) {
        if (event.text.unicode > 31 && event.text.unicode < 127) {
          sf::Text& currentText = texts.back();
          sf::RoundedRectangleShape& currentField = fields.back();

          sf::String newInput = input + event.text.unicode;
          currentText.setString(newInput);
          bool fits = true;
          while (currentText.getGlobalBounds().width > currentField.getGlobalBounds().width * 0.9 && fits) {
            unsigned int newCharSize = currentText.getCharacterSize() - 1;
            if (newCharSize < 10) {
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
          // TODO: backspace with size increase if necessary
        }
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