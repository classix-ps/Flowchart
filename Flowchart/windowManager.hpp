#include "grid.hpp"

enum class State { View, Add, Connect, Move, MoveNode };
enum class Cursor { Default, Point, Grab };

class WindowManager {
public:
  WindowManager();
  WindowManager(unsigned int width, unsigned int height);

  void run();

private:
  sf::RenderWindow window;
  sf::View view;

  Grid grid;

  sf::Sprite cursor;
  std::map<Cursor, sf::Texture> cursorTextures;

  State state;

  sf::Vector2f clickPos;

  sf::Vector2f nodeOffset;

  float zoom = 0.2f;
  //float zoom = 1.f;
  void zoomViewAt(sf::Vector2i pixel, float zoom);
};