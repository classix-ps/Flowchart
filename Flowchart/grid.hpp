#pragma once

#include "node.hpp"
#include "arrow.hpp"

class Grid {
public:
  Grid();
  Grid(const sf::Vector2u& size);

  void draw(sf::RenderWindow& window) const;

  void addNode();
  void addArrow();

  void updateArrows();

  bool onNode(const sf::Vector2f& pos);
  bool startArrow(const sf::Vector2f& pos);
  void select(const sf::Vector2f& pos);
  bool grab(const sf::Vector2f& pos);
  void deselect(bool force=false);
  void move(const sf::Vector2f& delta);
  bool highlightSingle(const sf::Vector2f& pos);
  void highlightArrow(const sf::Vector2f& pos);
  void highlightSelect(const sf::FloatRect& box);
  void dehighlight();
  void selectHighlighted(const sf::Vector2f& pos);
  void setSelectionsMovement();
  void deleteSelected();

  void setNodeOutlinePosition(const sf::Vector2f& pos);
  bool showNodeOutline = false;

  void setArrowOutlinePosition(const sf::Vector2f& pos);
  bool showArrowOutline = false;

private:
  std::vector<sf::VertexArray> lines;
  std::vector<Node> nodes;
  std::vector<Arrow> arrows;

  Node nodeOutline;
  Arrow arrowOutline;

  std::map<size_t, sf::Vector2f> selections;
  std::vector<size_t> highlights;

  bool validCell(const sf::Vector2i& cell) const;
};
