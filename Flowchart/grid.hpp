#include "node.hpp"
#include "arrow.hpp"

class Grid {
public:
  Grid();
  Grid(const sf::Vector2u& size);

  void draw(sf::RenderWindow& window) const;

  void addNode(sf::Vector2u center);

private:
  std::vector<std::vector<bool>> cells;
  std::vector<sf::VertexArray> lines;
  std::vector<Node> nodes;
  std::vector<Arrow> arrows;

  Node* currentNode = NULL;
};
