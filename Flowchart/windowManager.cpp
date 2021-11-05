#include "windowManager.hpp"

WindowManager::WindowManager() : state{ State::View }, slider{ 400.f, gui::Slider::Type::Vertical } {

}

WindowManager::WindowManager(unsigned int width, unsigned int height) : window{ sf::VideoMode(width, height), "Flowchart Creator" }, grid{ sf::Vector2u(width, height) }, state{ State::View }, slider{ float(height) / 3, gui::Slider::Type::Vertical } {
  std::string path = "../Resources/";
  
  // Icon
  sf::Image icon;
  icon.loadFromFile(path + "icon.png");
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

  // Cursors
  sf::Image cursorDefaultImg;
  cursorDefaultImg.loadFromFile(path + "cursors/default.png");
  cursorDefault.loadFromPixels(cursorDefaultImg.getPixelsPtr(), cursorDefaultImg.getSize(), sf::Vector2u(8, 9));

  sf::Image cursorPointerImg;
  cursorPointerImg.loadFromFile(path + "cursors/pointer.png");
  cursorPointer.loadFromPixels(cursorPointerImg.getPixelsPtr(), cursorPointerImg.getSize(), sf::Vector2u(33, 8));

  sf::Image cursorGrabImg;
  cursorGrabImg.loadFromFile(path + "cursors/dnd-move.png");
  cursorGrab.loadFromPixels(cursorGrabImg.getPixelsPtr(), cursorGrabImg.getSize(), sf::Vector2u(32, 32));

  sf::Image cursorCrosshairImg;
  cursorCrosshairImg.loadFromFile(path + "cursors/crosshair.png");
  cursorCrosshair.loadFromPixels(cursorCrosshairImg.getPixelsPtr(), cursorCrosshairImg.getSize(), sf::Vector2u(32, 32));

  sf::Image cursorPointerBlockImg;
  cursorPointerBlockImg.loadFromFile(path + "cursors/pointer-not-allowed.png");
  cursorPointerBlock.loadFromPixels(cursorPointerBlockImg.getPixelsPtr(), cursorPointerBlockImg.getSize(), sf::Vector2u(21, 8));

  sf::Image cursorGrabBlockImg;
  cursorGrabBlockImg.loadFromFile(path + "cursors/dnd-move-not-allowed.png");
  cursorGrabBlock.loadFromPixels(cursorGrabBlockImg.getPixelsPtr(), cursorGrabBlockImg.getSize(), sf::Vector2u(28, 25));

  window.setMouseCursor(cursorDefault);

  // Selection box
  selectionBox.setFillColor(sf::Color(0, 128, 255, 120));
  selectionBox.setOutlineColor(sf::Color(50, 50, 50, 150));
  selectionBox.setOutlineThickness(.2f);

  // Gui View
  guiView = window.getDefaultView();

  // Slider
  slider.setStep(2);
  slider.setPosition(width * 0.95f, (height - slider.getSize().y) / 2);
  slider.setValue(int(100 * (zoom - minZoom) / (maxZoom - minZoom)));
  slider.setCallback([&]() {
    zoom = minZoom + slider.getValue() * (maxZoom - minZoom) / 100;
    view.setSize(window.getDefaultView().getSize());
    view.zoom(zoom);
    window.setView(view);
    });

  // Slider background
  sliderBackground.setSize(slider.getSize());
  sliderBackground.setCornersRadius(10.f);
  sliderBackground.setCornerPointCount(50);
  sliderBackground.setFillColor(sf::Color(80, 80, 80));
  sliderBackground.setOutlineColor(sf::Color(50, 50, 50, 150));
  sliderBackground.setOutlineThickness(1.f);
  sliderBackground.setPosition(slider.getPosition());

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
          zoom = std::min(maxZoom, zoom + 0.05f);
        }
        else if (event.mouseWheelScroll.delta > 0) {
          zoom = std::max(minZoom, zoom - 0.05f);
        }
        slider.setValue(int(100 * (zoom - minZoom) / (maxZoom - minZoom)));
        
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
          if (slider.containsPoint(sf::Vector2f(float(event.mouseButton.x), float(event.mouseButton.y)) - slider.getPosition())) {
            slider.onMousePressed(event.mouseButton.x - slider.getPosition().x, event.mouseButton.y - slider.getPosition().y);
            state = State::Zoom;
          }
          else {
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
        }
        // Enter add state
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
          grid.deselect(true);
          if (grid.setNodeOutlinePosition(globalPos)) {
            window.setMouseCursor(cursorPointer);
          }
          else {
            window.setMouseCursor(cursorPointerBlock);
          }
          state = State::Add;
        }
        // Enter connect state
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
          grid.deselect(true);
          if (grid.startArrow(globalPos)) {
            window.setMouseCursor(cursorCrosshair);
            state = State::Connect;
          }
          else {
            // TODO: Delete arrow
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
        // Return to origin
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
          view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
        }

        break;
      };
      case State::Add:
      {
        // Enter view state
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
          hover(globalPos);
          state = State::View;
        }
        // Create node
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
          grid.addNode();
        }
        // Show node outline
        else {
          if (grid.setNodeOutlinePosition(globalPos)) {
            window.setMouseCursor(cursorPointer);
          }
          else {
            window.setMouseCursor(cursorPointerBlock);
          }
        }

        break;
      };
      case State::Connect:
      {
        // Create arrow, enter view state
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
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
          window.setView(view);
          oldPos = mousePos;
        }

        break;
      };
      case State::MoveNode:
      {
        // Enter view state
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
          grid.confirmMove();

          grid.deselect();
          grid.setSelectionsMovement();
          window.setMouseCursor(cursorPointer);
          hover(globalPos);
          state = State::View;
        }
        // Move node(s)
        else {
          if (grid.move(globalPos)) {
            window.setMouseCursor(cursorGrab);
          }
          else {
            window.setMouseCursor(cursorGrabBlock);
          }
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
          // TODO
        }
        // Undo
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
          // TODO
        }
        else {
          hover(globalPos);
        }

        break;
      };
      case State::Text:
      {
        // Add text
        if (event.type == sf::Event::TextEntered) {
          grid.addText(event.text.unicode);
        }
        // Confirm
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
          grid.confirmText();
          hover(globalPos);
          state = State::View;
        }
        // Confirm/set cursor
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
      case State::Zoom:
      {
        if (event.type == sf::Event::MouseMoved) {
          slider.onMouseMoved(event.mouseMove.x - slider.getPosition().x, event.mouseMove.y - slider.getPosition().y);
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
          slider.onMouseReleased(event.mouseButton.x - slider.getPosition().x, event.mouseButton.y - slider.getPosition().y);
          state = State::View;
        }

        break;
      };
      }

      window.clear(sf::Color::White);
      switch (state) {
      case State::Add:
        grid.draw(window, 1);
        break;
      case State::Connect:
        grid.draw(window, 2);
        break;
      default:
        grid.draw(window);
        break;
      }
      if (selecting) {
        window.draw(selectionBox);
      }
      window.setView(guiView);
      window.draw(sliderBackground);
      window.draw(slider);
      window.setView(view);
      window.display();
    }
  }
}