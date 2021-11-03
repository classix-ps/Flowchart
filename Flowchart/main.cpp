#define _USE_MATH_DEFINES

#include "windowManager.hpp"

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

/*
#include "Gui/Menu.hpp"
#include "Gui/Theme.hpp"
#include "Gui/Gui.hpp"
 
//#define MENU

sf::Color hex2color(const std::string& hexcolor)
{
  sf::Color color = sf::Color::Black;
  if (hexcolor.size() == 7) // #ffffff
  {
    color.r = strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16);
    color.g = strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16);
    color.b = strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16);
  }
  else if (hexcolor.size() == 4) // #fff
  {
    color.r = strtoul(hexcolor.substr(1, 1).c_str(), NULL, 16) * 17;
    color.g = strtoul(hexcolor.substr(2, 1).c_str(), NULL, 16) * 17;
    color.b = strtoul(hexcolor.substr(3, 1).c_str(), NULL, 16) * 17;
  }
  return color;
}

struct Theme
{
  sf::Color backgroundColor;
  std::string texturePath;
};

int main()
{
  Theme defaultTheme = {
      hex2color("#dddbde"),
      "demo/texture-default.png"
  };

  Theme win98Theme = {
      hex2color("#d4d0c8"),
      "demo/texture-win98.png"
  };

  // Create the main window
  sf::RenderWindow app(sf::VideoMode(640, 480), "SFML Widgets", sf::Style::Close);

  gui::Menu menu(app);

  gui::Theme::loadFont("demo/tahoma.ttf");
  gui::Theme::loadTexture(defaultTheme.texturePath);
  gui::Theme::textSize = 11;
  gui::Theme::PADDING = 2.f;
  //gui::Theme::windowBgColor = defaultTheme.backgroundColor;

  sf::View view = app.getDefaultView();
  view.zoom(0.5);
  app.setView(view);
  // Textbox
  gui::TextBox* textbox = new gui::TextBox();
  textbox->setCallback([&]() {

    });
#ifdef MENU
  menu.add(textbox);
#endif
  textbox->setPosition(240, 240);

  // Start the application loop
  while (app.isOpen())
  {
    // Process events
    sf::Event event;
    while (app.pollEvent(event))
    {
      // Send events to menu
#ifdef MENU
      menu.onEvent(event);
#else
      textbox->setCursor(textbox->getCursor());
      if (event.type == sf::Event::TextEntered) {
        textbox->onTextEntered(event.text.unicode);
      }
      else if (event.type == sf::Event::MouseMoved) {
        textbox->onMouseMoved(event.mouseButton.x, event.mouseButton.y);
      }
      else if (event.type == sf::Event::MouseButtonPressed) {
        textbox->onMousePressed(event.mouseButton.x, event.mouseButton.y);
        textbox->onStateChanged(gui::State::StateFocused);
      }
#endif
      if (event.type == sf::Event::Closed)
        app.close();
    }

    // Clear screen
    app.clear(sf::Color::White);
    //app.clear(gui::Theme::windowBgColor);
#ifdef MENU
    app.draw(menu);
#else
    app.draw(*textbox);
#endif
    app.display();
    //std::cout << textbox->getText().toAnsiString() << std::endl;
  }

  return EXIT_SUCCESS;
}
*/