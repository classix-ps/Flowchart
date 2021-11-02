#define _USE_MATH_DEFINES

#include "windowManager.hpp"
#include "SFGUI/Entry.hpp"

int main() {
  // Node template
  Node::fieldTmplt.setSize(sf::Vector2f(37.f, 21.f));
  Node::fieldTmplt.setCornersRadius(2.f);
  Node::fieldTmplt.setCornerPointCount(10);
  Node::fieldTmplt.setOutlineThickness(1.f);
  Node::fieldTmplt.setOrigin(sf::Vector2f(Node::fieldTmplt.getSize().x / 2, Node::fieldTmplt.getSize().y / 2));
  Node::fieldTmplt.setFillColor(sf::Color(230, 230, 230, 150));
  Node::fieldTmplt.setOutlineColor(sf::Color(0, 0, 0, 150));

  // Arrow angles
  float a1 = atan2((Node::fieldTmplt.getSize().y / 2) - Node::fieldTmplt.getCornersRadius(), Node::fieldTmplt.getSize().x / 2) * 180.f / float(M_PI);
  float a2 = atan2(Node::fieldTmplt.getSize().y / 2, (Node::fieldTmplt.getSize().x / 2) - Node::fieldTmplt.getCornersRadius()) * 180.f / float(M_PI);
  Arrow::angleTemplate[0] = a1;
  Arrow::angleTemplate[1] = a2;
  Arrow::angleTemplate[2] = 180.f - a2;
  Arrow::angleTemplate[3] = 180.f - a1;
  Arrow::angleTemplate[4] = 180.f + a1;
  Arrow::angleTemplate[5] = 180.f + a2;
  Arrow::angleTemplate[6] = 360.f - a2;
  Arrow::angleTemplate[7] = 360.f - a1;
  float a3 = atan2(Node::fieldTmplt.getSize().y / 2, Node::fieldTmplt.getSize().x / 2) / float(M_PI);
  Arrow::angleTemplate[8] = a3;
  Arrow::angleTemplate[9] = 180.f - a3;
  Arrow::angleTemplate[10] = 180.f + a3;
  Arrow::angleTemplate[11] = 360.f - a3;

  // Text template
  //gui::Theme::loadFont("../Resources/fonts/arial.ttf");
  //gui::Theme::textSize = 11;

  WindowManager window(1920, 1080);

  window.run();

  return 0;
}

/*
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cstdlib>

int main() {
  sf::RenderWindow render_window(sf::VideoMode(1024, 768, 32), "Guess My Number (SFGUI)", sf::Style::Titlebar | sf::Style::Close);

  // We have to do this because we don't use SFML to draw.
  render_window.resetGLStates();

  // Create an SFGUI. This is required before doing anything with SFGUI.
  sfg::SFGUI sfgui;

  auto current_number_entry = sfg::Entry::Create();
  auto tries_label = sfg::Label::Create();
  auto hint_label = sfg::Label::Create();
  auto guess_button = sfg::Button::Create("Guess");

  unsigned char current_number = 0;
  auto tries = 0u;

  // Custom properties.
  sfg::Context::Get().GetEngine().SetProperties(
    "Button#guess {"
    "	BackgroundColor: #006400FF;"
    "	BorderColor: #006400FF;"
    "}"
    "Button#guess:Prelight {"
    "	BackgroundColor: #008200FF;"
    "	BorderColor: #008200FF;"
    "}"
    "Button#guess > Label {"
    "	FontSize: 20;"
    "}"
  );

  auto update_ui = [&tries, &tries_label] {
    std::stringstream sstr;
    sstr << tries;
    tries_label->SetText(sstr.str());
  };

  auto reset_game = [update_ui, &tries, &current_number, &hint_label, &guess_button] {
    tries = 0;
    current_number = static_cast<unsigned char>(std::rand() % 100 + 1);

    hint_label->SetText("-");

    update_ui();
    guess_button->Show(true);
  };

  // Create widgets.
  auto window = sfg::Window::Create();
  window->SetTitle("Guess My Number (SFGUI)");

  auto new_game_button = sfg::Button::Create("New game");
  new_game_button->GetSignal(sfg::Widget::OnLeftClick).Connect([reset_game] { reset_game(); });

  guess_button->SetId("guess");
  guess_button->GetSignal(sfg::Widget::OnLeftClick).Connect([&current_number_entry, &hint_label, &tries, &update_ui, &current_number, &guess_button] {
    // Validate number.
    unsigned int buf_number(0);

    std::stringstream sstr(static_cast<std::string>(current_number_entry->GetText()));
    sstr >> buf_number;

    if (buf_number < 1 || buf_number > 100) {
      hint_label->SetText("Enter a number from 1 to 100.");
      return;
    }

    ++tries;
    update_ui();

    unsigned char number(static_cast<unsigned char>(buf_number));
    if (number < current_number) {
      hint_label->SetText("My number is higher.");
    }
    else if (number > current_number) {
      hint_label->SetText("My number is lower.");
    }
    else {
      hint_label->SetText("Correct!");
      guess_button->Show(false);
    }

    current_number_entry->SetText("");
    });

  // Layout.
  auto table = sfg::Table::Create();
  table->Attach(sfg::Label::Create("Your guess:"), sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table->Attach(current_number_entry, sf::Rect<sf::Uint32>(1, 0, 1, 1));
  table->Attach(sfg::Label::Create("Tries:"), sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table->Attach(tries_label, sf::Rect<sf::Uint32>(1, 1, 1, 1));
  table->Attach(sfg::Label::Create("Hint:"), sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL, sfg::Table::FILL);
  table->Attach(hint_label, sf::Rect<sf::Uint32>(1, 2, 1, 1));

  table->SetColumnSpacings(5.f);
  table->SetRowSpacings(5.f);

  auto buttons_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
  buttons_box->Pack(new_game_button);
  buttons_box->Pack(guess_button);

  auto content_vbox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
  content_vbox->Pack(sfg::Label::Create("Guess my number, it's from 1 to 100!"));
  content_vbox->Pack(table);
  content_vbox->Pack(buttons_box);

  window->Add(content_vbox);

  reset_game();

  window->SetPosition(
    sf::Vector2f(
      static_cast<float>(render_window.getSize().x / 2) - window->GetAllocation().width / 2.f,
      static_cast<float>(render_window.getSize().y / 2) - window->GetAllocation().height / 2.f
    )
  );

  // Make sure all properties are applied.
  window->Refresh();

  sf::Event event;

  while (render_window.isOpen()) {
    while (render_window.pollEvent(event)) {
      if (
        (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ||
        event.type == sf::Event::Closed
        ) {
        return 0;
      }

      window->HandleEvent(event);
    }

    window->Update(0.f);
    render_window.clear();
    sfgui.Display(render_window);
    render_window.display();
  }

  return 0;
}
*/
/*
#include "Gui/Menu.hpp"
#include "Gui/Theme.hpp"
#include "Gui/Gui.hpp"
#include <SFML/Graphics.hpp>


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
  menu.setPosition(10, 10);

  gui::Theme::loadFont("demo/tahoma.ttf");
  gui::Theme::loadTexture(defaultTheme.texturePath);
  gui::Theme::textSize = 11;
  gui::Theme::click.textColor = hex2color("#191B18");
  gui::Theme::click.textColorHover = hex2color("#191B18");
  gui::Theme::click.textColorFocus = hex2color("#000");
  gui::Theme::input.textColor = hex2color("#000");
  gui::Theme::input.textColorHover = hex2color("#000");
  gui::Theme::input.textColorFocus = hex2color("#000");
  gui::Theme::PADDING = 2.f;
  gui::Theme::windowBgColor = defaultTheme.backgroundColor;

  gui::HBoxLayout* hbox = menu.addHBoxLayout();
  gui::FormLayout* form = hbox->addFormLayout();

  sf::Text text("Hello world!", gui::Theme::getFont());
  text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
  text.setPosition(320, 360);

  // Textbox
  gui::TextBox* textbox = new gui::TextBox();
  textbox->setText("Hello world!");
  textbox->setCallback([&]() {
    text.setString(textbox->getText());
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    });
  form->addRow("Text", textbox);

  gui::TextBox* textbox2 = new gui::TextBox();
  textbox2->setText("Hello world!");
  textbox2->setMaxLength(5);
  form->addRow("Text with limit (5)", textbox2);

  gui::ProgressBar* pbar0 = new gui::ProgressBar();

  // Slider for rotation
  gui::Slider* sliderRotation = new gui::Slider();
  sliderRotation->setStep(1);
  sliderRotation->setCallback([&]() {
    text.setRotation(sliderRotation->getValue() * 360 / 100.f);
    pbar0->setValue(sliderRotation->getValue());
    });
  form->addRow("Rotation", sliderRotation);

  // Slider for scale
  gui::Slider* sliderScale = new gui::Slider();
  sliderScale->setCallback([&]() {
    float scale = 1 + sliderScale->getValue() * 2 / 100.f;
    text.setScale(scale, scale);
    });
  form->addRow("Scale", sliderScale);

  // OptionsBox for color
  gui::OptionsBox<sf::Color>* opt = new gui::OptionsBox<sf::Color>();
  opt->addItem("Red", sf::Color::Red);
  opt->addItem("Blue", sf::Color::Blue);
  opt->addItem("Green", sf::Color::Green);
  opt->addItem("Yellow", sf::Color::Yellow);
  opt->addItem("White", sf::Color::White);
  opt->setCallback([&]() {
    text.setFillColor(opt->getSelectedValue());
    });
  form->addRow("Color", opt);

  // Checbkox
  gui::CheckBox* checkboxBold = new gui::CheckBox();
  checkboxBold->setCallback([&]() {
    int style = text.getStyle();
    if (checkboxBold->isChecked())
      style |= sf::Text::Bold;
    else
      style &= ~sf::Text::Bold;
    text.setStyle(style);
    });
  form->addRow("Bold text", checkboxBold);

  gui::CheckBox* checkboxUnderlined = new gui::CheckBox();
  checkboxUnderlined->setCallback([&]() {
    int style = text.getStyle();
    if (checkboxUnderlined->isChecked())
      style |= sf::Text::Underlined;
    else
      style &= ~sf::Text::Underlined;
    text.setStyle(style);
    });
  form->addRow("Underlined text", checkboxUnderlined);

  // Progress bar
  form->addRow("Progress bar", pbar0);
  form->addRow("Default button", new gui::Button("button"));

  // Custom button
  sf::Texture imgbutton;
  imgbutton.loadFromFile("demo/themed-button.png");

  gui::SpriteButton* customButton = new gui::SpriteButton(imgbutton, "Play");
  customButton->setTextSize(20);
  form->addRow("Custom button", customButton);

  gui::VBoxLayout* vbox = hbox->addVBoxLayout();
  vbox->addLabel("This pannel is on the left");

  gui::OptionsBox<Theme>* themeBox = new gui::OptionsBox<Theme>();
  themeBox->addItem("Windows 98", win98Theme);
  themeBox->addItem("Default", defaultTheme);
  themeBox->setCallback([&]() {
    const Theme& theme = themeBox->getSelectedValue();
    gui::Theme::loadTexture(theme.texturePath);
    gui::Theme::windowBgColor = theme.backgroundColor;
    });
  vbox->add(themeBox);

  // Textbox
  gui::HBoxLayout* hbox2 = vbox->addHBoxLayout();
  gui::TextBox* textbox3 = new gui::TextBox(100);
  textbox3->setText("Button name");
  hbox2->add(textbox3);
  hbox2->addButton("Create button", [&]() {
    vbox->add(new gui::Button(textbox3->getText()));
    });

  // Small progress bar
  gui::HBoxLayout* hbox3 = vbox->addHBoxLayout();
  hbox3->addLabel("Small progress bar");
  gui::ProgressBar* pbar = new gui::ProgressBar(40);
  hbox3->add(pbar);

  gui::Slider* vslider = new gui::Slider(100, gui::Slider::Vertical);
  vslider->setCallback([&]() {
    pbar->setValue(vslider->getValue());
    });
  hbox->add(vslider);

  menu.addButton("Quit", [&]() {
    app.close();
    });

  sf::Texture texture;
  texture.loadFromFile("demo/sfml.png");

  sf::Sprite sprite(texture);
  sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
  sprite.setPosition(300, 360);

  // Start the application loop
  while (app.isOpen())
  {
    // Process events
    sf::Event event;
    while (app.pollEvent(event))
    {
      // Send events to menu
      menu.onEvent(event);
      if (event.type == sf::Event::Closed)
        app.close();
    }

    // Clear screen
    app.clear(gui::Theme::windowBgColor);
    app.draw(menu);
    app.draw(text);
    // Update the window
    app.display();
  }

  return EXIT_SUCCESS;
}
*/