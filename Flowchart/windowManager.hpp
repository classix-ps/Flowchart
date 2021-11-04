#pragma once

#include "grid.hpp"

enum class State { View, Add, Connect, Move, MoveNode, Select, SelectNode, Text };

class WindowManager {
public:
  WindowManager();
  WindowManager(unsigned int width, unsigned int height);

  void run();

private:
  sf::RenderWindow window;
  sf::View view;

  Grid grid;

  sf::Cursor cursorDefault;
  sf::Cursor cursorPointer;
  sf::Cursor cursorGrab;
  sf::Cursor cursorCrosshair;

  sf::RectangleShape selectionBox;

  State state;

  sf::Vector2f oldPos;

  sf::Vector2f nodeOffset;

  bool selecting = false;

  float zoom = 1.5f;

  void hover(const sf::Vector2f& pos);
};