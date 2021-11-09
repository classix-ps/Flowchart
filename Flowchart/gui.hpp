#pragma once

#include "roundedRectangle.hpp"
#include "Gui/Gui.hpp"
#include <array>

enum class ButtonUse { Add, Confirm, Del, Text, Menu, Quit, Random, Save, View, None };

class Gui {
public:
  Gui();
  Gui(unsigned int width, unsigned int height, float zoom, float minZoom, float maxZoom);

  void draw(sf::RenderWindow& window) const;

  bool onGui(int x, int y) const;

  bool onSlider(int x, int y);
  void moveSlider(int x, int y);
  void releaseSlider(int x, int y);

  void setSliderValue(int value);
  int getSliderValue() const;

  ButtonUse onButton(int x, int y, bool click);
  void releaseButton(int x, int y);
  void resetButtons();
  void setDynamic(ButtonUse button);

private:
  sf::RoundedRectangleShape sliderBackground;
  gui::Slider slider;

  sf::Sprite slots;
  std::array<std::shared_ptr<sf::Texture>, 10> toolbarTextures;
  std::map<ButtonUse, gui::SpriteButton> dynamic;
  std::map<ButtonUse, gui::SpriteButton> clickable;
  ButtonUse dynamicState = ButtonUse::View;
};