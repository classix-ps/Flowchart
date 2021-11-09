#include "gui.hpp"

Gui::Gui() : slider{ 400.f, gui::Slider::Type::Vertical } {

}

Gui::Gui(unsigned int width, unsigned int height, float zoom, float minZoom, float maxZoom) : slider{ float(height) / 3, gui::Slider::Type::Vertical, 20 } {
  // Slider
  slider.setStep(1);
  slider.setPosition(width * 0.98f, (height - slider.getSize().y) / 2);
  slider.setValue(int(100 * (zoom - minZoom) / (maxZoom - minZoom)));

  // Slider background
  sliderBackground.setSize(sf::Vector2f(slider.getSize().x / 2, slider.getSize().y));
  sliderBackground.setCornersRadius(5.f);
  sliderBackground.setCornerPointCount(5);
  sliderBackground.setFillColor(sf::Color(80, 80, 80));
  sliderBackground.setOutlineColor(sf::Color(50, 50, 50, 150));
  sliderBackground.setOutlineThickness(1.f);
  sliderBackground.setPosition(slider.getPosition().x + slider.getSize().x / 4 - 0.5f, slider.getSize().y);

  std::string path = "../Resources/textures/";

  // Toolbar
  for (std::shared_ptr<sf::Texture>& texture : toolbarTextures) {
    texture = std::make_shared<sf::Texture>();
  }
  toolbarTextures[0]->loadFromFile(path + "slots.png");
  toolbarTextures[1]->loadFromFile(path + "addTx.png");
  toolbarTextures[2]->loadFromFile(path + "confirmTx.png");
  toolbarTextures[3]->loadFromFile(path + "deleteTx.png");
  toolbarTextures[4]->loadFromFile(path + "drawTx.png");
  toolbarTextures[5]->loadFromFile(path + "menuTx.png");
  toolbarTextures[6]->loadFromFile(path + "quitTx.png");
  toolbarTextures[7]->loadFromFile(path + "randomTx.png");
  toolbarTextures[8]->loadFromFile(path + "saveTx.png");
  toolbarTextures[9]->loadFromFile(path + "viewTx.png");

  sf::Vector2f toolbarPos(width * 0.75f, height * 0.022f);

  slots.setTexture(*toolbarTextures[0]);
  slots.setPosition(toolbarPos);

  gui::SpriteButton add(*toolbarTextures[1]);
  add.setPosition(toolbarPos.x + 53.f, toolbarPos.y + 5.f);
  dynamic.insert(std::make_pair(ButtonUse::Add, add));

  gui::SpriteButton confirm(*toolbarTextures[2]);
  confirm.setPosition(toolbarPos.x + 154.f, toolbarPos.y + 5.f);
  clickable.insert(std::make_pair(ButtonUse::Confirm, confirm));

  gui::SpriteButton del(*toolbarTextures[3]);
  del.setPosition(toolbarPos.x + 204.f, toolbarPos.y + 5.f);
  clickable.insert(std::make_pair(ButtonUse::Del, del));

  gui::SpriteButton draw(*toolbarTextures[4]);
  draw.setPosition(toolbarPos.x + 104.f, toolbarPos.y + 5.f);
  dynamic.insert(std::make_pair(ButtonUse::Text, draw));

  gui::SpriteButton menu(*toolbarTextures[5]);
  menu.setPosition(toolbarPos.x + 406.f, toolbarPos.y + 5.f);
  clickable.insert(std::make_pair(ButtonUse::Menu, menu));

  gui::SpriteButton quit(*toolbarTextures[6]);
  quit.setPosition(toolbarPos.x + 355.f, toolbarPos.y + 5.f);
  clickable.insert(std::make_pair(ButtonUse::Quit, quit));

  gui::SpriteButton random(*toolbarTextures[7]);
  random.setPosition(toolbarPos.x + 255.f, toolbarPos.y + 5.f);
  clickable.insert(std::make_pair(ButtonUse::Random, random));

  gui::SpriteButton save(*toolbarTextures[8]);
  save.setPosition(toolbarPos.x + 305.f, toolbarPos.y + 5.f);
  clickable.insert(std::make_pair(ButtonUse::Save, save));

  gui::SpriteButton view(*toolbarTextures[9]);
  view.setPosition(toolbarPos.x + 4.f, toolbarPos.y + 5.f);
  dynamic.insert(std::make_pair(ButtonUse::View, view));

  dynamic.at(dynamicState).onStateChanged(gui::StatePressed);
}

void Gui::draw(sf::RenderWindow& window) const {
  window.draw(sliderBackground);
  window.draw(slider);

  window.draw(slots);
  for (const std::pair<ButtonUse, gui::SpriteButton>& button : dynamic) {
    window.draw(button.second);
  }
  for (const std::pair<ButtonUse, gui::SpriteButton>& button : clickable) {
    window.draw(button.second);
  }
}

bool Gui::onGui(int x, int y) const {
  for (std::map<ButtonUse, gui::SpriteButton>::const_iterator iter = dynamic.begin(); iter != dynamic.end(); iter++) {
    if (iter->first != dynamicState && iter->second.getState() == gui::StatePressed) {
      return true;
    }
  }
  for (std::map<ButtonUse, gui::SpriteButton>::const_iterator iter = clickable.begin(); iter != clickable.end(); iter++) {
    if (iter->second.getState() == gui::StatePressed) {
      return true;
    }
  }
  return slots.getGlobalBounds().contains(float(x), float(y)) || slider.containsPoint(sf::Vector2f(float(x), float(y)) - slider.getPosition());
}

bool Gui::onSlider(int x, int y) {
  if (slider.containsPoint(sf::Vector2f(float(x), float(y)) - slider.getPosition())) {
    slider.onMousePressed(x - slider.getPosition().x, y - slider.getPosition().y);
    return true;
  }

  return false;
}

void Gui::moveSlider(int x, int y) {
  slider.onMouseMoved(x - slider.getPosition().x, y - slider.getPosition().y);
}

void Gui::releaseSlider(int x, int y) {
  slider.onMouseReleased(x - slider.getPosition().x, y - slider.getPosition().y);
}

void Gui::setSliderValue(int value) {
  slider.setValue(value);
}

int Gui::getSliderValue() const {
  return slider.getValue();
}

ButtonUse Gui::onButton(int x, int y, bool click) {
  if (click) {
    for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = dynamic.begin(); iter != dynamic.end(); iter++) {
      if (iter->second.containsPoint(sf::Vector2f(float(x), float(y)) - iter->second.getPosition())) {
        for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = dynamic.begin(); iter != dynamic.end(); iter++) {
          iter->second.onStateChanged(gui::State::StateDefault);
        }

        dynamicState = iter->first;
        iter->second.onStateChanged(gui::State::StatePressed);
        return iter->first;
      }
    }
    for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = clickable.begin(); iter != clickable.end(); iter++) {
      if (iter->second.containsPoint(sf::Vector2f(float(x), float(y)) - iter->second.getPosition())) {
        iter->second.onStateChanged(gui::State::StatePressed);
        return iter->first;
      }
    }
  }
  else {
    for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = clickable.begin(); iter != clickable.end(); iter++) {
      if (iter->second.getState() == gui::StatePressed) {
        return ButtonUse::None;
      }
    }

    for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = dynamic.begin(); iter != dynamic.end(); iter++) {
      if (iter->second.getState() != gui::StatePressed) {
        if (iter->second.containsPoint(sf::Vector2f(float(x), float(y)) - iter->second.getPosition())) {
          iter->second.onStateChanged(gui::State::StateHovered);
          return ButtonUse::None;
        }
        else {
          iter->second.onStateChanged(gui::State::StateDefault);
        }
      }
    }
    for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = clickable.begin(); iter != clickable.end(); iter++) {
      if (iter->second.getState() != gui::StatePressed) {
        if (iter->second.containsPoint(sf::Vector2f(float(x), float(y)) - iter->second.getPosition())) {
          iter->second.onStateChanged(gui::State::StateHovered);
          return ButtonUse::None;
        }
        else {
          iter->second.onStateChanged(gui::State::StateDefault);
        }
      }
    }
  }

  return ButtonUse::None;
}

void Gui::releaseButton(int x, int y) {
  for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = clickable.begin(); iter != clickable.end(); iter++) {
    if (iter->second.containsPoint(sf::Vector2f(float(x), float(y)) - iter->second.getPosition())) {
      iter->second.onStateChanged(gui::State::StateHovered);
    }
    else {
      iter->second.onStateChanged(gui::State::StateDefault);
    }
  }
}

void Gui::resetButtons() {
  for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = clickable.begin(); iter != clickable.end(); iter++) {
    iter->second.onStateChanged(gui::StateDefault);
  }
}

void Gui::setDynamic(ButtonUse button) {
  for (std::map<ButtonUse, gui::SpriteButton>::iterator iter = dynamic.begin(); iter != dynamic.end(); iter++) {
    iter->second.onStateChanged(gui::State::StateDefault);
  }

  dynamicState = button;
  dynamic.at(dynamicState).onStateChanged(gui::StatePressed);
}