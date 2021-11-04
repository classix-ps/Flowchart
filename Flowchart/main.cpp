#define _USE_MATH_DEFINES
#define NODEMO
#include "windowManager.hpp"
#ifdef NODEMO
int main() {
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

  // Theme template
  gui::Theme::loadFont("../Resources/fonts/tahoma.ttf");
  gui::Theme::textSize = 50;
  gui::Theme::loadTexture("../Resources/theme/texture-default.png");
  gui::Theme::PADDING = 2.f;

  WindowManager window(1920, 1080);

  window.run();

  return EXIT_SUCCESS;
}
#else DEMO
#include "Gui/Menu.hpp"
#include "Gui/Theme.hpp"
#include "Gui/Gui.hpp"
 
int main()
{


  // Create the main window
  sf::RenderWindow app(sf::VideoMode(1920, 1080), "SFML Widgets", sf::Style::Close);

  gui::Theme::loadFont("demo/tahoma.ttf");
  gui::Theme::loadTexture("demo/texture-default.png");
  gui::Theme::textSize = 50;
  gui::Theme::PADDING = 3.f;
  //gui::Theme::windowBgColor = defaultTheme.backgroundColor;

  sf::View view = app.getDefaultView();
  view.zoom(1.5);
  app.setView(view);
  // Textbox
  gui::TextBox* textbox = new gui::TextBox();
  textbox->setCallback([&]() {

    });
#ifdef MENU
  menu.add(textbox);
#endif
  textbox->setPosition(1080, 480);

  // Start the application loop
  while (app.isOpen())
  {
    // Process events
    sf::Event event;
    while (app.pollEvent(event))
    {
      if (event.type == sf::Event::TextEntered) {
        textbox->onTextEntered(event.text.unicode);
      }
      else if (event.type == sf::Event::MouseMoved) {
        //textbox->onMouseMoved(event.mouseButton.x, event.mouseButton.y);
      }
      else if (event.type == sf::Event::MouseButtonPressed) {
        textbox->onMousePressed(event.mouseButton.x, event.mouseButton.y);
        textbox->onStateChanged(gui::State::StateFocused);
      }

      if (event.type == sf::Event::Closed)
        app.close();
    }

    // Clear screen
    app.clear(sf::Color::White);
    app.draw(*textbox);
    app.display();
    //std::cout << textbox->getText().toAnsiString() << std::endl;
  }

  return EXIT_SUCCESS;
}
#endif