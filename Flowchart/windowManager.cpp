#include "windowManager.hpp"

WindowManager::WindowManager() : state{ State::View } {

}

WindowManager::WindowManager(unsigned int width, unsigned int height) : window(sf::VideoMode(width, height), "Flowchart Creator"), grid(sf::Vector2u(width, height)), state{ State::View } {
  // Icon
  sf::Image icon;
  icon.loadFromFile("../Resources/icon.png");
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

  // Cursors
  sf::Image cursorDefaultImg;
  cursorDefaultImg.loadFromFile("../Resources/cursors/default.png");
  cursorDefault.loadFromPixels(cursorDefaultImg.getPixelsPtr(), cursorDefaultImg.getSize(), sf::Vector2u(8, 9));

  sf::Image cursorPointerImg;
  cursorPointerImg.loadFromFile("../Resources/cursors/pointer.png");
  cursorPointer.loadFromPixels(cursorPointerImg.getPixelsPtr(), cursorPointerImg.getSize(), sf::Vector2u(33, 8));

  sf::Image cursorGrabImg;
  cursorGrabImg.loadFromFile("../Resources/cursors/dnd-move.png");
  cursorGrab.loadFromPixels(cursorGrabImg.getPixelsPtr(), cursorGrabImg.getSize(), sf::Vector2u(32, 32));

  sf::Image cursorCrosshairImg;
  cursorCrosshairImg.loadFromFile("../Resources/cursors/crosshair.png");
  cursorCrosshair.loadFromPixels(cursorCrosshairImg.getPixelsPtr(), cursorCrosshairImg.getSize(), sf::Vector2u(32, 32));

  window.setMouseCursor(cursorDefault);

  // Selection box
  selectionBox.setFillColor(sf::Color(0, 128, 255, 120));
  selectionBox.setOutlineColor(sf::Color(50, 50, 50, 150));
  selectionBox.setOutlineThickness(.2f);

  // View
  view = window.getDefaultView();
  view.zoom(zoom);
  window.setView(view);
}

void WindowManager::hover(const sf::Vector2f& pos) {
  if (grid.highlightSingle(pos) || state == State::Add) {
    window.setMouseCursor(cursorPointer);
  }
  else {
    window.setMouseCursor(cursorDefault);
  }
}

void WindowManager::run() {
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        window.close();
      }

      sf::Vector2i mousePosPx = sf::Mouse::getPosition(window);
      sf::Vector2f mousePos = sf::Vector2f(mousePosPx);
      sf::Vector2f globalPos = window.mapPixelToCoords(mousePosPx);
      
      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta < 0) {
          zoom = std::min(6.f, zoom + 0.05f);
        }
        else if (event.mouseWheelScroll.delta > 0) {
          zoom = std::max(0.5f, zoom - 0.05f);
        }
        
        view.setSize(window.getDefaultView().getSize());
        view.zoom(zoom);
        window.setView(view);
        globalPos = window.mapPixelToCoords(mousePosPx);
      }
      
      switch (state) {
      case State::View:
      {
        // Hover node
        hover(globalPos);

        // Move view or node
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          int onNode = grid.grab(globalPos);
          if (onNode == 0) {
            oldPos = mousePos;
            window.setMouseCursor(cursorGrab);
            state = State::Move;
          }
          else if (onNode == 1) {
            window.setMouseCursor(cursorGrab);
            state = State::MoveNode;
          }
          else {
            state = State::Text;
          }
        }
        // Enter add state
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
          grid.deselect(true);
          grid.setNodeOutlinePosition(globalPos);
          window.setMouseCursor(cursorPointer);
          state = State::Add;
        }
        // Enter connect state
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
          grid.deselect(true);
          if (grid.startArrow(globalPos)) {
            grid.showArrowOutline = true;
            window.setMouseCursor(cursorCrosshair);
            state = State::Connect;
          }
        }
        // Enter select state
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
          oldPos = globalPos;
          state = State::Select;
        }
        // Enter select node state
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LControl) {
          state = State::SelectNode;
        }
        // Delete selected nodes
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete) {
          grid.deleteSelected();
        }

        break;
      };
      case State::Add:
      {
        // Enter view state
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
          grid.showNodeOutline = false;
          state = State::View;
        }
        // Create node
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          grid.addNode();
        }
        // Show node outline
        else {
          grid.setNodeOutlinePosition(globalPos);
        }

        break;
      };
      case State::Connect:
      {
        // Create arrow, enter view state
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
          grid.showArrowOutline = false;
          grid.addArrow();
          hover(globalPos);
          state = State::View;
        }
        // Show arrow outline
        else {
          grid.setArrowOutlinePosition(globalPos);
          grid.highlightArrow(globalPos);
        }

        break;
      };
      case State::Move:
      {
        // Enter view state
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
          window.setMouseCursor(cursorDefault);
          state = State::View;
        }
        // Move view
        else {
          sf::Vector2f deltaPos = oldPos - mousePos;
          deltaPos.x *= zoom;
          deltaPos.y *= zoom;
          view.move(deltaPos);
          oldPos = mousePos;
        }

        break;
      };
      case State::MoveNode:
      {
        // Enter view state
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
          grid.deselect();
          grid.setSelectionsMovement();
          window.setMouseCursor(cursorPointer);
          hover(globalPos);
          state = State::View;
        }
        // Move node(s)
        else {
          grid.move(globalPos);
        }

        break;
      };
      case State::Select:
      {
        // Start selection box
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          selecting = true;
          selectionBox.setPosition(globalPos);
          selectionBox.setSize(sf::Vector2f(0.f, 0.f));
          window.setMouseCursor(cursorGrab);
          oldPos = globalPos;
          grid.deselect(true);
          grid.dehighlight();
        }
        // Cancel selection if not yet started
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift) {
          if (!selecting) {
            hover(globalPos);
            state = State::View;
          }
        }
        // Confirm selection
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
          selecting = false;
          grid.selectHighlighted(globalPos);
          hover(globalPos);
          state = State::View;
        }
        // Highlight selected nodes
        else {
          selectionBox.setSize(globalPos - oldPos);
          grid.highlightSelect(selectionBox.getGlobalBounds());
        }

        break;
      };
      case State::SelectNode:
      {
        // Enter view state
        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LControl) {
          state = State::View;
        }
        // Select node
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          grid.select(globalPos);
        }
        // Save
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
          // Save
        }
        // Undo
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {

        }
        else {
          hover(globalPos);
        }

        break;
      };
      case State::Text:
      {
        if (event.type == sf::Event::TextEntered) {
          grid.addText(event.text.unicode);
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
          grid.confirmText();
          hover(globalPos);
          state = State::View;
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
          if (grid.onEdit(globalPos)) {
            if (event.mouseButton.button == sf::Mouse::Left) {
              grid.setTextCursor(globalPos);
            }
          }
          else {
            grid.confirmText();
            hover(globalPos);
            state = State::View;
          }
        }

        break;
      };
      }
    }

    window.clear(sf::Color::White);
    grid.draw(window);
    if (selecting) {
      window.draw(selectionBox);
    }
    window.setView(view);
    window.display();
  }
}