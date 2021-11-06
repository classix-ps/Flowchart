#include "windowManager.hpp"

WindowManager::WindowManager() : state{ State::View } {

}

WindowManager::WindowManager(unsigned int width, unsigned int height) : window{ sf::VideoMode(width, height), "Flowchart Creator" }, gui{ width, height, zoom, minZoom, maxZoom }, grid{ sf::Vector2u(width, height) }, state{ State::View } {
  window.setFramerateLimit(60);

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

  // View
  view = window.getDefaultView();
  view.zoom(zoom);
}

void WindowManager::hover(const sf::Vector2f& pos) {
  if (grid.highlightSingle(pos) || state == State::Add) {
    window.setMouseCursor(cursorPointer);
  }
  else {
    window.setMouseCursor(cursorDefault);
  }
}

void WindowManager::hoverNodeOutline(const sf::Vector2f& pos) {
  if (grid.setNodeOutlinePosition(pos)) {
    window.setMouseCursor(cursorPointer);
  }
  else {
    window.setMouseCursor(cursorPointerBlock);
  }
}

void WindowManager::run() {
  while (window.isOpen()) {
    sf::Event event;

    if (state == State::Text) {
      while (window.pollEvent(event)) {
        handleEvent(event);
      }

      // Dynamic states need to be drawn every cycle
      window.clear(sf::Color::White);
      grid.draw(window);
      window.setView(guiView);
      gui.draw(window);
      window.setView(view);
      window.display();
    }
    else {
      if (window.waitEvent(event)) {
        sf::Vector2i pos = handleEvent(event);

        window.clear(sf::Color::White);
        window.setView(view);
        switch (state) {
        case State::Add:
          if (gui.onGui(pos.x, pos.y)) {
            window.setMouseCursor(cursorDefault);
            grid.draw(window);
          }
          else {
            grid.draw(window, 1);
          }
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
        gui.draw(window);
        window.setView(view);
        window.display();
      }
    }
  }
}

sf::Vector2i WindowManager::handleEvent(const sf::Event& event) {
  sf::Vector2i mousePosPx = sf::Mouse::getPosition(window);
  sf::Vector2f mousePos = sf::Vector2f(mousePosPx);
  sf::Vector2f globalPos = window.mapPixelToCoords(mousePosPx);

  if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
    window.close();
  }
  else if (event.type == sf::Event::MouseWheelScrolled) {
    if (event.mouseWheelScroll.delta < 0) {
      zoom = std::min(maxZoom, zoom + 0.05f);
    }
    else if (event.mouseWheelScroll.delta > 0) {
      zoom = std::max(minZoom, zoom - 0.05f);
    }
    gui.setSliderValue(int(100 * (zoom - minZoom) / (maxZoom - minZoom)));

    view.setSize(window.getDefaultView().getSize());
    view.zoom(zoom);
    window.setView(view);
    globalPos = window.mapPixelToCoords(mousePosPx);
  }

  switch (state) {
  case State::View:
  {
    // Move view or node
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
      // Gui
      if (gui.onGui(event.mouseButton.x, event.mouseButton.y)) {
        ButtonUse action = gui.onButton(event.mouseButton.x, event.mouseButton.y, true);
        if (action != ButtonUse::None) {

        }
        else if (gui.onSlider(event.mouseButton.x, event.mouseButton.y)) {
          state = State::Zoom;
        }
      }
      // Grid
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        grid.select(globalPos);
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
    // Release gui
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
      gui.releaseButton(event.mouseButton.x, event.mouseButton.y);
      if (!gui.onGui(event.mouseButton.x, event.mouseButton.y)) {
        hover(globalPos);
      }
    }
    // Enter add state
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
      grid.deselect(true);
      grid.deselectArrows();
      hoverNodeOutline(globalPos);
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
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
          grid.deselectArrows();
        }
        grid.selectArrow(globalPos);
      }
    }
    // Hovering
    else if (event.type == sf::Event::MouseMoved) {
      if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
        window.setMouseCursor(cursorDefault);
        grid.dehighlight();
        gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
      }
      else {
        gui.resetButtons();
        hover(globalPos);
      }
    }
    // Enter select state
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::LShift) {
      oldPos = globalPos;
      state = State::Select;
    }
    // Delete selected nodes
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete) {
      grid.deleteSelected();
      grid.deleteSelectedArrows();
    }
    // Return to origin
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
      view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    }
    // Save
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
      //TODO
    }
    // Undo
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
      //TODO
    }

    break;
  };
  case State::Add:
  {
    // Enter view state
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
      state = State::View;
      hover(globalPos);
    }
    // Create node or press gui
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
      if (gui.onGui(event.mouseButton.x, event.mouseButton.y)) {
        ButtonUse action = gui.onButton(event.mouseButton.x, event.mouseButton.y, true);
        if (action != ButtonUse::None) {

        }
        else if (gui.onSlider(event.mouseButton.x, event.mouseButton.y)) {
          previouslyAdding = true;
          state = State::Zoom;
        }
      }
      else {
        grid.addNode();
      }
    }
    // Release gui
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
      gui.releaseButton(event.mouseButton.x, event.mouseButton.y);
      hoverNodeOutline(globalPos);
    }
    // Show node outline if not hovering over gui
    else if (event.type == sf::Event::MouseMoved) {
      if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
        gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
      }
      else {
        gui.resetButtons();
        hoverNodeOutline(globalPos);
      }
    }
    // Place node outline for other actions (e.g. zooming)
    else {
      hoverNodeOutline(globalPos);
    }

    break;
  };
  case State::Connect:
  {
    // Create arrow, enter view state
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
      grid.addArrow();
      if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
        window.setMouseCursor(cursorDefault);
        grid.dehighlight();
        gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
      }
      else {
        hover(globalPos);
      }
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
      if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
        window.setMouseCursor(cursorDefault);
        grid.dehighlight();
        gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
      }
      else {
        window.setMouseCursor(cursorPointer);
        hover(globalPos);
      }
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
      grid.deselectArrows();
      grid.dehighlight();
    }
    // Cancel selection if not yet started
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::LShift) {
      if (!selecting) {
        if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
          window.setMouseCursor(cursorDefault);
          grid.dehighlight();
          gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
        }
        else {
          hover(globalPos);
        }
        state = State::View;
      }
    }
    // Confirm selection
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
      selecting = false;
      grid.selectHighlighted(globalPos);
      if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
        window.setMouseCursor(cursorDefault);
        grid.dehighlight();
        gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
      }
      else {
        hover(globalPos);
      }
      state = State::View;
    }
    // Highlight selected nodes
    else {
      selectionBox.setSize(globalPos - oldPos);
      grid.highlightSelect(selectionBox.getGlobalBounds());
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
    // Keys regarding text
    else if (event.type == sf::Event::KeyPressed) {
      grid.applyToText(event.key);
    }
    // Confirm/set cursor
    else if (event.type == sf::Event::MouseButtonPressed) {
      if (grid.onEdit(globalPos)) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          grid.setTextCursor(globalPos);
        }
      }
      else if (gui.onGui(event.mouseButton.x, event.mouseButton.y)) {
        ButtonUse action = gui.onButton(event.mouseButton.x, event.mouseButton.y, true);
        if (action != ButtonUse::None) {
          grid.confirmText();
          hover(globalPos);
          state = State::View;
        }
        else if (gui.onSlider(event.mouseButton.x, event.mouseButton.y)) {
          grid.confirmText();
          hover(globalPos);
          state = State::Zoom;
        }
      }
      else {
        grid.confirmText();
        hover(globalPos);
        state = State::View;
      }
    }
    // Check hovering gui
    else if (event.type == sf::Event::MouseMoved) {
      if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
        gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
        window.setMouseCursor(cursorDefault);
      }
      else {
        window.setMouseCursor(cursorPointer);
      }
    }

    break;
  };
  case State::Zoom:
  {
    // Enter view state
    if (event.type == sf::Event::MouseButtonReleased) {
      gui.releaseSlider(event.mouseButton.x, event.mouseButton.y);
      if (previouslyAdding) {
        state = State::Add;
        if (!gui.onGui(mousePosPx.x, mousePosPx.y)) {
          hoverNodeOutline(globalPos);
        }
        previouslyAdding = false;
      }
      else {
        if (gui.onGui(event.mouseMove.x, event.mouseMove.y)) {
          window.setMouseCursor(cursorDefault);
          grid.dehighlight();
          gui.onButton(event.mouseMove.x, event.mouseMove.y, false);
        }
        else {
          hover(globalPos);
        }
        state = State::View;
      }
    }
    // Move slider and zoom
    else if (event.type == sf::Event::MouseMoved) {
      gui.moveSlider(event.mouseMove.x, event.mouseMove.y);
      zoom = minZoom + gui.getSliderValue() * (maxZoom - minZoom) / 100;
      view.setSize(window.getDefaultView().getSize());
      view.zoom(zoom);
      window.setView(view);
    }

    break;
  };
  }

  return mousePosPx;
}