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
}

bool Grid::validCell(const sf::Vector2i& cell) const {
  for (size_t i = 0; i < nodes.size(); i++) {
    bool selectedNode = false;
    for (const std::pair<size_t, sf::Vector2f>& selection : selections) {
      if (selection.first == i) {
        selectedNode = true;
      }
    }
    if (!selectedNode) {
      sf::Vector2i delta = cell - posToCell(nodes[i].getCenter());
      if (abs(delta.x) < 10 && abs(delta.y) < 6) {
        return false;
      }
    }
  }

  return true;
}

void Grid::draw(sf::RenderWindow& window) const {
  for (const sf::VertexArray& line : lines) {
    window.draw(line);
  }

  for (const Arrow& arrow : arrows) {
    arrow.draw(window);
  }

  if (showArrowOutline) {
    arrowOutline.draw(window);
  }

  for (const Node& node : nodes) {
    node.draw(window);
  }

  if (showNodeOutline) {
    nodeOutline.draw(window);
  }
}

void Grid::addNode() {
  if (validCell(posToCell(nodeOutline.getCenter()))) {
    nodeOutline.setState(NodeState::Locked);
    nodes.push_back(nodeOutline);
    nodeOutline.setState(NodeState::Placing);
  }
}

void Grid::addArrow() {
  sf::Vector2f destination = arrowOutline.getDestination();
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i].contains(destination) && arrowOutline.getOriginNode() != i && !nodes[arrowOutline.getOriginNode()].pointsTo(i)) {
      arrowOutline.setDestinationNode(i);
      nodes[arrowOutline.getOriginNode()].point(i);

      arrowOutline.setDestination(nodes[i].getCenter());
      arrowOutline.setHead(nodes[i].getBounds());
      arrowOutline.setDirectLineColor(sf::Color::Black);
      arrows.push_back(arrowOutline);
      arrowOutline.setDirectLineColor(sf::Color(0, 102, 51, 255));

      break;
    }
  }
}

void Grid::updateArrows() {

}

bool Grid::onNode(const sf::Vector2f& pos) {
  for (const Node& node : nodes) {
    if (node.contains(pos)) {
      return true;
    }
  }

  return false;
}

bool Grid::startArrow(const sf::Vector2f& pos) {
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i].contains(pos)) {
      nodes[i].setState(NodeState::Highlighted);
      arrowOutline.setOrigin(nodes[i].getCenter());
      arrowOutline.setDestination(pos);
      arrowOutline.setOriginNode(i);
      return true;
    }
  }

  return false;
}

void Grid::select(const sf::Vector2f& pos) {
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i].contains(pos)) {
      nodes[i].setState(NodeState::Selected);
      selections[i] = sf::Vector2f();
      return;
    }
  }
}

bool Grid::grab(const sf::Vector2f& pos) {
  bool onNode = false;
  bool preSelected = false;
  size_t i = 0;
  for (i; i < nodes.size(); i++) {
    if (nodes[i].contains(pos)) {
      onNode = true;
      preSelected = nodes[i].getState() == NodeState::Selected;
      break;
    }
  }

  if (preSelected) {
    for (std::map<size_t, sf::Vector2f>::iterator iter = selections.begin(); iter != selections.end(); iter++) {
      iter->second = pos - nodes[iter->first].getCenter();
      nodes[iter->first].setState(NodeState::Moving);
    }
  }
  else if (i < nodes.size()) {
    deselect(true);
    nodes[i].setState(NodeState::Moving);
    selections[i] = pos - nodes[i].getCenter();
  }

  return onNode;
}

void Grid::deselect(bool force) {
  if (selections.size() < 2 || force) {
    for (const std::pair<size_t, sf::Vector2f>& selection : selections) {
      nodes[selection.first].setState(NodeState::Locked);
    }
    selections.clear();
  }
}

void Grid::move(const sf::Vector2f& pos) {
  for (const std::pair<size_t, sf::Vector2f>& select : selections) {
    sf::Vector2f newPos = pos - select.second;
    if (!validCell(posToCell(newPos))) {
      return;
    }
  }

  for (const std::pair<size_t, sf::Vector2f>& select : selections) {
    sf::Vector2f newPos = pos - select.second;
    nodes[select.first].setPosition(sf::Vector2f(4 * int(newPos.x / 4) + 2.f, 4 * int(newPos.y / 4) + 2.f));
  }
}

bool Grid::highlightSingle(const sf::Vector2f& pos) {
  bool overNode = false;
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i].contains(pos)) {
      overNode = true;
    }
    bool selectedNode = false;
    for (const std::pair<size_t, sf::Vector2f>& selection : selections) {
      if (selection.first == i) {
        selectedNode = true;
      }
    }
    if (!selectedNode) {
      if (nodes[i].contains(pos)) {
        nodes[i].setState(NodeState::Highlighted);
      }
      else {
        nodes[i].setState(NodeState::Locked);
      }
    }
  }

  return overNode;
}

void Grid::highlightArrow(const sf::Vector2f& pos) {
  for (size_t i = 0; i < nodes.size(); i++) {
    if (i != arrowOutline.getOriginNode()) {
      if (nodes[i].contains(pos)) {
        nodes[i].setState(NodeState::Highlighted);
      }
      else {
        nodes[i].setState(NodeState::Locked);
      }
    }
  }
}

void Grid::highlightSelect(const sf::FloatRect& box) {
  highlights.clear();
  for (size_t i = 0; i < nodes.size(); i++) {
    sf::FloatRect boundingRect = nodes[i].getBounds();
    if (box.contains(sf::Vector2f(boundingRect.left + boundingRect.width, boundingRect.top))
      && box.contains(sf::Vector2f(boundingRect.left, boundingRect.top + boundingRect.height))) {
      highlights.push_back(i);
      nodes[i].setState(NodeState::Highlighted);
    }
    else {
      nodes[i].setState(NodeState::Locked);
    }
  }
}

void Grid::dehighlight() {
  for (const size_t& higlight : highlights) {
    nodes[higlight].setState(NodeState::Locked);
  }
  highlights.clear();
}

void Grid::selectHighlighted(const sf::Vector2f& pos) {
  selections.clear();
  for (const size_t& highlight : highlights) {
    nodes[highlight].setState(NodeState::Selected);
    selections[highlight] = sf::Vector2f();
  }
  highlights.clear();
}

void Grid::setSelectionsMovement() {
  for (const std::pair<size_t, sf::Vector2f>& selection : selections) {
    nodes[selection.first].setState(NodeState::Selected);
  }
}

void Grid::deleteSelected() {

}

void Grid::setNodeOutlinePosition(const sf::Vector2f& pos) {
  if (validCell(posToCell(pos))) {
    nodeOutline.setPosition(sf::Vector2f(4 * int(pos.x / 4) + 2.f, 4 * int(pos.y / 4) + 2.f));
    showNodeOutline = true;
  }
  else {
    showNodeOutline = false;
  }
}

void Grid::setArrowOutlinePosition(const sf::Vector2f& pos) {
  for (const Node& node : nodes) {
    if (node.contains(pos)) {
      arrowOutline.setDestination(pos);
      return;
    }
  }

  arrowOutline.setDestination(pos);
}