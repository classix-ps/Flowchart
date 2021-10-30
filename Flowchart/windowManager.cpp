#include "windowManager.hpp"

WindowManager::WindowManager() : state{ State::View } {

}

WindowManager::WindowManager(unsigned int width, unsigned int height) : window(sf::VideoMode(width, height), "Flowchart Creator"), grid(sf::Vector2u(width, height)), state{ State::View } {
  //window.setMouseCursorVisible(false);


  sf::Texture cursorDefaultTx;
  cursorDefaultTx.loadFromFile("../Resources/cursors/default.png");
  cursorTextures[Cursor::Default] = cursorDefaultTx;
  sf::Texture cursorPointTx;
  cursorPointTx.loadFromFile("../Resources/cursors/pointer.png");
  cursorTextures[Cursor::Point] = cursorPointTx;
  sf::Texture cursorGrabTx;
  cursorGrabTx.loadFromFile("../Resources/cursors/dnd-move.png");
  cursorTextures[Cursor::Grab] = cursorGrabTx;
  cursor.setTexture(cursorTextures[Cursor::Default]);

  // Node template
  Node::fieldTmplt.setSize(sf::Vector2f(window.getSize().x / 20.f, window.getSize().y / 20.f));
  Node::fieldTmplt.setCornersRadius(10.f);
  Node::fieldTmplt.setCornerPointCount(10);
  Node::fieldTmplt.setOutlineThickness(3.f);
  Node::fieldTmplt.setOrigin(sf::Vector2f(Node::fieldTmplt.getSize().x / 2, Node::fieldTmplt.getSize().y / 2));

  // Text template
  gui::Theme::loadFont("../Resources/fonts/arial.ttf");
  gui::Theme::textSize = 11;

  view = window.getDefaultView();
  view.zoom(zoom);

  cursor.setScale(zoom, zoom);
}

void WindowManager::zoomViewAt(sf::Vector2i pixel, float z) {
  const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
  view.zoom(z);
  window.setView(view);
  const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
  const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
  view.move(offsetCoords);
  //cursor.setPosition(afterCoord);
}

void WindowManager::run() {
  sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2.f, window.getSize().y / 2.f));

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        window.close();
      }

      //sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
      sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
      cursor.setPosition(mousePos);
      
      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
          zoomViewAt(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), 1 / 1.1f);
        }
        else if (event.mouseWheelScroll.delta < 0) {
          //zoomViewAt(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), 1.1f);
          view.zoom(1.1f);
        }
      }
      
      switch (state) {
      case State::View:
      {
        // Move view
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          clickPos = mousePos;
          cursor.setTexture(cursorTextures[Cursor::Grab]);
          state = State::Move;
        }

        break;
      };
      case State::Add:
      {
        // Create node
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          
        }

        break;
      };
      case State::Move:
      {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
          cursor.setTexture(cursorTextures[Cursor::Default]);
          state = State::View;
        }
        else {
          view.move((clickPos - mousePos) * 0.5f);
          clickPos = mousePos;
        }

        break;
      };
      }
    }

    window.clear(sf::Color::White);
    grid.draw(window);
    window.draw(cursor);
    window.setView(view);
    window.display();
  }
}