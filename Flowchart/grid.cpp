#include "grid.hpp"

Grid::Grid() : lineTemplate{ sf::Lines, 2 } {
  nodeCellWidth = int(Node::fieldTmplt.getSize().x / gridSpaces);
  nodeCellHeight = int(Node::fieldTmplt.getSize().y / gridSpaces);
}

Grid::Grid(const sf::Vector2u& size) : lineTemplate{ sf::Lines, 2 } {
  lineTemplate[0].color = sf::Color(150, 150, 150);
  lineTemplate[1].color = lineTemplate[0].color;

  nodeCellWidth = int(Node::fieldTmplt.getSize().x / gridSpaces);
  nodeCellHeight = int(Node::fieldTmplt.getSize().y / gridSpaces);
}

sf::Vector2i Grid::posToCell(const sf::Vector2f& pos) const {
  return sf::Vector2i(int(pos.x / gridSpaces), int(pos.y / gridSpaces));
}

sf::Vector2f Grid::cellToPos(const sf::Vector2i& pos) const {
  return sf::Vector2f(gridSpaces * pos.x + float(gridSpaces) / 2, gridSpaces * pos.y + float(gridSpaces) / 2);
}

bool Grid::validCell(const sf::Vector2i& cell) const {
  for (size_t i = 0; i < nodes.size(); i++) {
    bool selectedNode = false;
    for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& selection : selections) {
      if (selection.first == i) {
        selectedNode = true;
      }
    }
    if (!selectedNode) {
      sf::Vector2i delta = cell - posToCell(nodes[i].getCenter());
      if (abs(delta.x) < nodeCellWidth + 2 && abs(delta.y) < nodeCellHeight + 2) {
        return false;
      }
    }
  }

  return true;
}

void Grid::draw(sf::RenderWindow& window, int outlines) const {
  sf::VertexArray line(lineTemplate);
  const sf::View currentView = window.getView();
  float left = currentView.getCenter().x - currentView.getSize().x / 2;
  float right = currentView.getCenter().x + currentView.getSize().x / 2;
  float top = currentView.getCenter().y - currentView.getSize().y / 2;
  float bottom = currentView.getCenter().y + currentView.getSize().y / 2;

  line[0].position.y = top;
  line[1].position.y = bottom;
  for (int i = gridSpaces * int(left / gridSpaces); i <= gridSpaces * int(right / gridSpaces); i += gridSpaces) {
    line[0].position.x = float(i);
    line[1].position.x = float(i);
    window.draw(line);
  }
  line[0].position.x = left;
  line[1].position.x = right;
  for (int i = gridSpaces * int(top / gridSpaces); i <= gridSpaces * int(bottom / gridSpaces); i += gridSpaces) {
    line[0].position.y = float(i);
    line[1].position.y = float(i);
    window.draw(line);
  }

  for (const Arrow& arrow : arrows) {
    arrow.draw(window);
  }

  if (outlines == 2) {
    arrowOutline.draw(window);
  }

  for (const Node& node : nodes) {
    node.draw(window);
  }

  if (outlines == 1) {
    nodeOutline.draw(window);
  }
}

bool Grid::addNode() {
  if (validCell(posToCell(nodeOutline.getCenter()))) {
    nodes.push_back(nodeOutline);
    nodes.back().setState(NodeState::Locked);
    return true;
  }
  else {
    return false;
  }
}

bool Grid::addArrow() {
  sf::Vector2f destination = arrowOutline.getDestination();
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i].contains(destination) && arrowOutline.getOriginNode() != i && !nodes[arrowOutline.getOriginNode()].pointsTo(i)) {
      // Disallows double-ended arrows
      if (nodes[i].pointsTo(arrowOutline.getOriginNode())) {
        return false;
      }

      arrowOutline.setDestinationNode(i);
      nodes[arrowOutline.getOriginNode()].point(i);

      arrowOutline.setDestination(nodes[i].getCenter());
      arrowOutline.setHead(nodes[i].getBounds());
      arrows.push_back(arrowOutline);
      arrows.back().setDirectLineColor(sf::Color::Black);

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
      selections[i] = std::make_pair(sf::Vector2f(), sf::Vector2f());
      return;
    }
  }
}

int Grid::grab(const sf::Vector2f& pos) {
  bool onNode = false;
  bool preSelected = false;
  size_t i = 0;
  for (i; i < nodes.size(); i++) {
    if (nodes[i].contains(pos)) {
      if (nodes[i].onTextbox(pos)) {
        editing = i;
        nodes[i].setTextboxState(gui::State::StateFocused);
        return 2;
      }

      onNode = true;
      preSelected = nodes[i].getState() == NodeState::Selected;
      break;
    }
  }

  if (preSelected) {
    for (std::map<size_t, std::pair<sf::Vector2f, sf::Vector2f>>::iterator iter = selections.begin(); iter != selections.end(); iter++) {
      iter->second = std::make_pair(pos - nodes[iter->first].getCenter(), nodes[iter->first].getCenter());
      nodes[iter->first].setState(NodeState::Moving);
    }
  }
  else if (i < nodes.size()) {
    deselect(true);
    nodes[i].setState(NodeState::Moving);
    selections[i] = std::make_pair(pos - nodes[i].getCenter(), nodes[i].getCenter());
  }

  return onNode;
}

void Grid::deselect(bool force) {
  if (selections.size() < 2 || force) {
    for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& selection : selections) {
      nodes[selection.first].setState(NodeState::Locked);
    }
    selections.clear();
  }
}

bool Grid::move(const sf::Vector2f& pos) {
  bool validPositions = true;
  for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& select : selections) {
    if (!validCell(posToCell(pos - select.second.first))) {
      validPositions = false;
      break;
    }
  }

  for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& select : selections) {
    nodes[select.first].setPosition(cellToPos(posToCell(pos - select.second.first)));
  }

  for (Arrow& arrow : arrows) {
    std::map<size_t, std::pair<sf::Vector2f, sf::Vector2f>>::iterator originIt = selections.find(arrow.getOriginNode());
    std::map<size_t, std::pair<sf::Vector2f, sf::Vector2f>>::iterator destinationIt = selections.find(arrow.getDestinationNode());
    if (originIt != selections.end() && destinationIt != selections.end()) {
      arrow.setOrigin(nodes[originIt->first].getCenter());
      arrow.setDestination(nodes[destinationIt->first].getCenter());
      arrow.setHead(nodes[destinationIt->first].getBounds());
    }
    else if (originIt != selections.end()) {
      arrow.setOrigin(nodes[originIt->first].getCenter());
      arrow.setDestination(nodes[arrow.getDestinationNode()].getCenter());
      arrow.setHead(nodes[arrow.getDestinationNode()].getBounds());
    }
    else if (destinationIt != selections.end()) {
      arrow.setDestination(nodes[destinationIt->first].getCenter());
      arrow.setHead(nodes[destinationIt->first].getBounds());
    }
  }

  return validPositions;
}

bool Grid::confirmMove() {
  bool validPositions = true;
  for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& select : selections) {
    if (!validCell(posToCell(nodes[select.first].getCenter()))) {
      validPositions = false;
      break;
    }
  }

  if (!validPositions) {
    for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& select : selections) {
      nodes[select.first].setPosition(cellToPos(posToCell(select.second.second)));
    }

    for (Arrow& arrow : arrows) {
      std::map<size_t, std::pair<sf::Vector2f, sf::Vector2f>>::iterator originIt = selections.find(arrow.getOriginNode());
      std::map<size_t, std::pair<sf::Vector2f, sf::Vector2f>>::iterator destinationIt = selections.find(arrow.getDestinationNode());
      if (originIt != selections.end() && destinationIt != selections.end()) {
        arrow.setOrigin(nodes[originIt->first].getCenter());
        arrow.setDestination(nodes[destinationIt->first].getCenter());
        arrow.setHead(nodes[destinationIt->first].getBounds());
      }
      else if (originIt != selections.end()) {
        arrow.setOrigin(nodes[originIt->first].getCenter());
        arrow.setDestination(nodes[arrow.getDestinationNode()].getCenter());
        arrow.setHead(nodes[arrow.getDestinationNode()].getBounds());
      }
      else if (destinationIt != selections.end()) {
        arrow.setDestination(nodes[destinationIt->first].getCenter());
        arrow.setHead(nodes[destinationIt->first].getBounds());
      }
    }
  }

  return validPositions;
}

bool Grid::highlightSingle(const sf::Vector2f& pos) {
  bool overNode = false;
  for (size_t i = 0; i < nodes.size(); i++) {
    if (nodes[i].contains(pos)) {
      overNode = true;
    }
    bool selectedNode = false;
    for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& selection : selections) {
      if (selection.first == i) {
        selectedNode = true;
      }
    }
    if (!selectedNode) {
      if (nodes[i].contains(pos)) {
        nodes[i].setState(NodeState::Highlighted);
        highlights.insert(i);
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
      highlights.insert(i);
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
    selections[highlight] = std::make_pair(sf::Vector2f(), sf::Vector2f());
  }
  highlights.clear();
}

void Grid::setSelectionsMovement() {
  for (const std::pair<size_t, std::pair<sf::Vector2f, sf::Vector2f>>& selection : selections) {
    nodes[selection.first].setState(NodeState::Selected);
  }
}

bool Grid::deleteSelected() {
  for (std::map<size_t, std::pair<sf::Vector2f, sf::Vector2f>>::reverse_iterator iter = selections.rbegin(); iter != selections.rend(); iter++) {
    nodes.erase(nodes.begin() + iter->first);

    for (Node& node : nodes) {
      node.removePointing(iter->first);
      node.lowerPointing(iter->first);
    }

    for (int i = arrows.size() - 1; i > -1; i--) {
      if (arrows[i].getOriginNode() == iter->first || arrows[i].getDestinationNode() == iter->first) {
        arrows.erase(arrows.begin() + i);
      }
      else {
        arrows[i].setOriginNode(arrows[i].getOriginNode() - (arrows[i].getOriginNode() > iter->first));
        arrows[i].setDestinationNode(arrows[i].getDestinationNode() - (arrows[i].getDestinationNode() > iter->first));
      }
    }
  }

  if (!selections.empty()) {
    selections.clear();
    return true;
  }
  return false;
}

bool Grid::selectArrow(const sf::Vector2f& pos) {
  for (size_t i = 0; i < arrows.size(); i++) {
    if (arrows[i].onArrow(pos)) {
      arrows[i].highlight();
      selectedArrows.insert(i);
      return true;
    }
  }

  return false;
}

void Grid::deselectArrows() {
  for (const size_t& i : selectedArrows) {
    arrows[i].lock();
  }

  selectedArrows.clear();
}

bool Grid::deleteSelectedArrows() {
  for (std::set<size_t>::reverse_iterator iter = selectedArrows.rbegin(); iter != selectedArrows.rend(); iter++) {
    nodes[arrows[*iter].getOriginNode()].removePointing(arrows[*iter].getDestinationNode());
    arrows.erase(arrows.begin() + *iter);
  }

  if (!selectedArrows.empty()) {
    selectedArrows.clear();
    return true;
  }
  return false;
}

void Grid::addText(sf::Uint32 unicode) {
  nodes[editing].addText(unicode);
}

void Grid::confirmText() {
  nodes[editing].setTextboxState(gui::State::StateDefault);
}

void Grid::applyToText(const sf::Event::KeyEvent& key) {
  nodes[editing].applyToText(key);
}

bool Grid::onEdit(const sf::Vector2f& pos) const {
  return nodes[editing].onTextbox(pos);
}

void Grid::setTextCursor(const sf::Vector2f& pos) {
  nodes[editing].setTextCursor(pos);
}

bool Grid::setNodeOutlinePosition(const sf::Vector2f& pos) {
  sf::Vector2i centerCell = posToCell(pos);

  nodeOutline.setPosition(cellToPos(centerCell));

  if (validCell(centerCell)) {
    return true;
  }
  else {
    return false;
  }
}

void Grid::setArrowOutlinePosition(const sf::Vector2f& pos) {
  arrowOutline.setDestination(pos);
}

void Grid::save() const {
  std::ofstream gridFile("grid.csv");
  for (const Node& node : nodes) {
    gridFile << node.getText().toAnsiString() << ';';
    for (const size_t& pointed : node.getPointed()) {
      gridFile << nodes[pointed].getText().toAnsiString() << ';';
    }
    gridFile << std::endl;
  }

  gridFile.close();
}

void Grid::saveToJson() const {

}

void Grid::load() {

}

void Grid::loadFromJson() {

}