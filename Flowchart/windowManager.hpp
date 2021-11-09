#pragma once

#include "grid.hpp"
#include "gui.hpp"
#include <stack>

enum class State { View, Add, Connect, Move, MoveNode, Select, Text, Zoom };

class WindowManager {
public:
  WindowManager();
  WindowManager(unsigned int width, unsigned int height);

  void run();

private:
  sf::RenderWindow window;
  sf::View view;
  sf::View guiView;

  Grid grid;
  std::stack<Grid> history;

  sf::Cursor cursorDefault;
  sf::Cursor cursorPointer;
  sf::Cursor cursorGrab;
  sf::Cursor cursorCrosshair;
  sf::Cursor cursorPointerBlock;
  sf::Cursor cursorGrabBlock;

  sf::RectangleShape selectionBox;

  State state;

  sf::Vector2f oldPos;

  sf::Vector2f nodeOffset;

  bool selecting = false;

  float zoom = 1.5f;
  float maxZoom = 5.5f;
  float minZoom = 0.5f;

  Gui gui;

  bool previouslyAdding = false;

  void hover(const sf::Vector2f& pos);
  void hoverNodeOutline(const sf::Vector2f& pos);

  sf::Vector2i handleEvent(const sf::Event& event);
  void handleAction(ButtonUse action);
};