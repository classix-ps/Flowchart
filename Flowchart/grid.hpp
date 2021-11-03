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

  bool startArrow(const sf::Vector2f& pos);
  void select(const sf::Vector2f& pos);
  int grab(const sf::Vector2f& pos);
  void deselect(bool force=false);
  void move(const sf::Vector2f& delta);
  bool highlightSingle(const sf::Vector2f& pos);
  void highlightArrow(const sf::Vector2f& pos);
  void highlightSelect(const sf::FloatRect& box);
  void dehighlight();
  void selectHighlighted(const sf::Vector2f& pos);
  void setSelectionsMovement();
  void deleteSelected();

  void addText(sf::Uint32 unicode);
  void confirmText();
  bool onEdit(const sf::Vector2f& pos) const;
  void setTextCursor(const sf::Vector2f& pos);

  void setNodeOutlinePosition(const sf::Vector2f& pos);
  bool showNodeOutline = false;

  void setArrowOutlinePosition(const sf::Vector2f& pos);
  bool showArrowOutline = false;

  void saveToJson() const;

private:
  int gridSpaces = 20;
  int nodeCellWidth;
  int nodeCellHeight;

  sf::VertexArray lineTemplate;

  std::vector<Node> nodes;
  std::vector<Arrow> arrows;

  Node nodeOutline;
  Arrow arrowOutline;

  std::map<size_t, sf::Vector2f> selections;
  std::vector<size_t> highlights;
  size_t editing = 0;

  sf::Vector2i posToCell(const sf::Vector2f& pos) const;
  bool validCell(const sf::Vector2i& cell) const;
};
