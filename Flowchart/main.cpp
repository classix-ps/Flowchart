#define _USE_MATH_DEFINES
//#define NODEMO
#include "windowManager.hpp"
#ifdef NODEMO
int main() {
  std::string path = "../Resources/";

  // Node template
  Node::fieldTmplt.setSize(sf::Vector2f(360.f, 200.f));
  Node::fieldTmplt.setCornersRadius(20.f);
  Node::fieldTmplt.setCornerPointCount(100);
  Node::fieldTmplt.setOutlineThickness(10.f);
  Node::fieldTmplt.setOrigin(Node::fieldTmplt.getSize() / 2.f);
  Node::fieldTmplt.setFillColor(sf::Color(230, 230, 230, 150));
  Node::fieldTmplt.setOutlineColor(sf::Color(0, 0, 0, 150));

  // Arrow angles
  float a1 = atan2((Node::fieldTmplt.getSize().y / 2) - Node::fieldTmplt.getCornersRadius(), Node::fieldTmplt.getSize().x / 2 + Node::fieldTmplt.getOutlineThickness()) * 180.f / float(M_PI);
  float a2 = atan2(Node::fieldTmplt.getSize().y / 2 + Node::fieldTmplt.getOutlineThickness(), (Node::fieldTmplt.getSize().x / 2) - Node::fieldTmplt.getCornersRadius()) * 180.f / float(M_PI);
  Arrow::angleTemplate[0] = a1;
  Arrow::angleTemplate[1] = a2;
  Arrow::angleTemplate[2] = 180.f - a2;
  Arrow::angleTemplate[3] = 180.f - a1;
  Arrow::angleTemplate[4] = 180.f + a1;
  Arrow::angleTemplate[5] = 180.f + a2;
  Arrow::angleTemplate[6] = 360.f - a2;
  Arrow::angleTemplate[7] = 360.f - a1;

  // Gui theme settings
  gui::Theme::loadFont(path + "fonts/tahoma.ttf");
  gui::Theme::textSize = 50;
  gui::Theme::loadTexture(path + "theme/texture-default.png");
  gui::Theme::PADDING = 2.f;
  gui::Theme::borderSize = 2;

  WindowManager window(1920, 1080);

  window.run();

  return EXIT_SUCCESS;
}
#else
#include "Gui/Menu.hpp"
#include "Gui/Theme.hpp"
#include "Gui/Gui.hpp"
 
int main()
{
  // Create the main window
  sf::RenderWindow app(sf::VideoMode(1920, 1080), "SFML Widgets", sf::Style::Close);

  gui::Theme::loadFont("demo/tahoma.ttf");
  gui::Theme::loadTexture("demo/texture-default.png");
  gui::Theme::textSize = 11;
  gui::Theme::PADDING = 1.f;
  //gui::Theme::windowBgColor = defaultTheme.backgroundColor;

  sf::View view = app.getDefaultView();
  //view.zoom(1.1);
  app.setView(view);
  // Textbox
  gui::TextBox* textbox = new gui::TextBox();
  textbox->setCallback([&]() {

    });
  textbox->setPosition(1080, 480);

  gui::Slider* slider = new gui::Slider(200.f, gui::Slider::Type::Vertical);
  slider->setPosition(1800, 400);
  slider->setStep(2);

  sf::Texture buttonImg;
  buttonImg.loadFromFile("C:/Users/psusk/source/repos/C++/Flowchart/Resources/textures/addTx.png");
  //buttonImg.loadFromFile("demo/themed-button.png");
  gui::SpriteButton* button = new gui::SpriteButton(buttonImg);
  button->setPosition(sf::Vector2f(100.f, 100.f));

  // Start the application loop
  while (app.isOpen())
  {
    // Process events
    sf::Event event;
    while (app.pollEvent(event))
    {
      sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition());

      if (event.type == sf::Event::TextEntered) {
        textbox->onTextEntered(event.text.unicode);
      }
      else if (event.type == sf::Event::MouseButtonPressed) {
        if (textbox->containsPoint(sf::Vector2f(event.mouseButton.x, event.mouseButton.y) - textbox->getPosition())) {
          textbox->onMousePressed(event.mouseButton.x - textbox->getPosition().x, event.mouseButton.y - textbox->getPosition().y);
          textbox->onStateChanged(gui::State::StateFocused);
        }
        else if (slider->containsPoint(sf::Vector2f(event.mouseButton.x, event.mouseButton.y) - slider->getPosition())) {
          slider->onMousePressed(event.mouseButton.x - slider->getPosition().x, event.mouseButton.y - slider->getPosition().y);
          //slider->onStateChanged(gui::State::StatePressed);
        }
        else if (button->containsPoint(sf::Vector2f(event.mouseButton.x, event.mouseButton.y) - button->getPosition())) {
          button->onMousePressed(event.mouseButton.x - button->getPosition().x, event.mouseButton.y - button->getPosition().y);
          button->onStateChanged(gui::State::StatePressed);
        }
        else {
          textbox->onStateChanged(gui::State::StateDefault);
          button->onStateChanged(gui::State::StateDefault);
        }
      }
      else if (event.type == sf::Event::MouseMoved) {
        slider->onMouseMoved(event.mouseMove.x - slider->getPosition().x, event.mouseMove.y - slider->getPosition().y);
        button->onMouseMoved(event.mouseMove.x - button->getPosition().x, event.mouseMove.y - button->getPosition().y);
        if (button->containsPoint(sf::Vector2f(event.mouseMove.x, event.mouseMove.y) - button->getPosition())) {
          button->onStateChanged(gui::State::StateHovered);
        }
      }

      if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        app.close();
    }

    // Clear screen
    app.clear(sf::Color::White);
    app.draw(*textbox);
    app.draw(*slider);
    app.draw(*button);
    app.display();
    //std::cout << textbox->getText().toAnsiString() << std::endl;
    //std::cout << sf::Mouse::getPosition().y << std::endl;
  }

  return EXIT_SUCCESS;
}
#endif