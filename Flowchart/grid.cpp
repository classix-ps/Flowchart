#include "grid.hpp"

Grid::Grid() {

}

Grid::Grid(const sf::Vector2u& size) {
  sf::VertexArray line(sf::Lines, 2);
  line[0].color = sf::Color(150, 150, 150);
  line[1].color = line[0].color;
  for (size_t i = 0; i <= size.x; i += 4) {
    line[0].position = sf::Vector2f(float(i), 0.f);
    line[1].position = sf::Vector2f(float(i), float(size.y));
    lines.push_back(line);
  }
  for (size_t i = 0; i <= size.y; i += 4) {
    line[0].position = sf::Vector2f(0.f, float(i));
    line[1].position = sf::Vector2f(float(size.x), float(i));
    lines.push_back(line);
  }

  std::vector<bool> column(size.y / 4, false);
  cells = std::vector<std::vector<bool>>(size.x / 4, column);
}

void Grid::draw(sf::RenderWindow& window) const {
  for (const sf::VertexArray& line : lines) {
    window.draw(line);
  }

  for (const Node& node : nodes) {
    node.draw(window);
  }

  for (const Arrow& arrow : arrows) {
    arrow.draw(window);
  }
}

void Grid::addNode(sf::Vector2u center) {
  nodes.push_back(Node(sf::Vector2f(center)));


}