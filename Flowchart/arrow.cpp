#include "arrow.hpp"

std::array<float, 8> Arrow::angleTemplate;

Arrow::Arrow() : head{ 4.f * Node::fieldTmplt.getOutlineThickness(), 3 }, directLine{ sf::Quads, 4 } {
  for (size_t i = 0; i < 4; i++) {
    directLine[i].color = sf::Color(0, 102, 51, 255);
  }
  head.setFillColor(sf::Color(0, 102, 51, 255));
  head.setOrigin(head.getRadius(), 0.f);
  head.setScale(sf::Vector2f(0.8f, 1.f));
}

void Arrow::draw(sf::RenderWindow& window) const {
  sf::View view = window.getView();
  sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
  if (directLine.getBounds().intersects(viewRect) || head.getGlobalBounds().intersects(viewRect)) {
    window.draw(directLine);
    window.draw(head);
  }
}

bool sameSide(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f a, sf::Vector2f b) {
  sf::Vector2f ls = b - a;
  sf::Vector2f l1 = p1 - a;
  sf::Vector2f l2 = p2 - a;
  float cp1 = ls.x * l1.y - ls.y * l1.x;
  float cp2 = ls.x * l2.y - ls.y * l2.x;
  return cp1 * cp2 >= 0;
}

bool Arrow::onArrow(const sf::Vector2f& pos) {
  sf::Vector2f delta = destination - origin;

  if (directLine.getBounds().contains(pos)) {
    float theta = atan2(delta.y, delta.x);
    float thickness = abs(0.75f * Node::fieldTmplt.getOutlineThickness() / cos(theta));

    if (delta.x == 0.f) {
      if (pos.x > destination.y - thickness && pos.x < destination.x + thickness) {
        return true;
      }
    }

    float m = delta.y / delta.x;

    float lineCenter = origin.y + m * (pos.x - origin.x);
    if (pos.y > lineCenter - thickness && pos.y < lineCenter + thickness) {
      return true;
    }
  }
  if (head.getGlobalBounds().contains(pos)) {
    float length = sqrt(delta.x * delta.x + delta.y * delta.y);
    sf::Vector2f deltaNorm = delta / length;
    sf::Vector2f deltaNormPerp(-deltaNorm.y, deltaNorm.x);

    sf::Vector2f v1 = head.getPosition();
    sf::Vector2f baseCenter = v1 - head.getRadius() * 1.5f * deltaNorm;
    sf::Vector2f baseHalf = head.getRadius() * 0.5f * sqrt(3.f) * deltaNormPerp;
    sf::Vector2f v2 = baseCenter + baseHalf * 0.8f;
    sf::Vector2f v3 = baseCenter - baseHalf * 0.8f;

    if (sameSide(pos, v1, v2, v3) && sameSide(pos, v2, v1, v3) && sameSide(pos, v3, v1, v2)) {
      return true;
    }
  }
  return false;
}

void Arrow::highlight() {
  for (size_t i = 0; i < 4; i++) {
    directLine[i].color = sf::Color(0, 0, 153, 255);
  }
  head.setFillColor(sf::Color(0, 0, 153, 255));
}

void Arrow::lock() {
  for (size_t i = 0; i < 4; i++) {
    directLine[i].color = sf::Color::Black;
  }
  head.setFillColor(sf::Color::Black);
}

void Arrow::setOrigin(const sf::Vector2f& pos) {
  origin = pos;
}

void Arrow::setDestination(const sf::Vector2f& pos) {
  sf::Vector2f delta = pos - origin;

  head.setPosition(pos);
  head.setRotation((atan2(delta.y, delta.x) * 180.f / float(M_PI)) + 90.f);

  sf::FloatRect headBoundingRect = head.getGlobalBounds();
  sf::Vector2f headCenter(headBoundingRect.left + headBoundingRect.width / 2, headBoundingRect.top + headBoundingRect.height / 2);

  float length = sqrt(delta.x * delta.x + delta.y * delta.y);
  sf::Vector2f deltaNorm = delta / length;
  sf::Vector2f deltaNormPerp(-deltaNorm.y, deltaNorm.x);

  directLine[0].position = origin + 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;
  directLine[1].position = origin - 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;
  directLine[2].position = headCenter - 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;
  directLine[3].position = headCenter + 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;

  destination = pos;
}

void Arrow::setOriginNode(size_t index) {
  originNode = index;
}

void Arrow::setDestinationNode(size_t index) {
  destinationNode = index;
}

void Arrow::setHead(const sf::FloatRect& boundingRect) {
  sf::Vector2f center(boundingRect.left + boundingRect.width / 2, boundingRect.top + boundingRect.height / 2);

  sf::Vector2f delta = origin - getDestination();
  delta.y = -delta.y;
  float theta = atan2(delta.y, delta.x);
  float thetaDeg = theta * 180.f / float(M_PI);
  float thetaDegPositive = thetaDeg + 180.f;

  // Bottom left -> Counter-clockwise
  if (thetaDegPositive >= angleTemplate[1] && thetaDegPositive < angleTemplate[2]) {
    if (thetaDegPositive == 90.f) {
      head.setPosition(sf::Vector2f(center.x, boundingRect.top + boundingRect.height));
    }
    else {
      head.setPosition(sf::Vector2f(center.x - boundingRect.height / (2 * tan(theta)), boundingRect.top + boundingRect.height));
    }
  }
  else if (thetaDegPositive >= angleTemplate[3] && thetaDegPositive < angleTemplate[4]) {
    if (thetaDegPositive == 180.f) {
      head.setPosition(sf::Vector2f(boundingRect.left + boundingRect.width, center.y));
    }
    else {
      head.setPosition(sf::Vector2f(boundingRect.left + boundingRect.width, center.y - boundingRect.width / (2 * tan(((float(M_PI) / 2) - theta)))));
    }
  }
  else if (thetaDegPositive >= angleTemplate[5] && thetaDegPositive < angleTemplate[6]) {
    if (thetaDegPositive == 270.f) {
      head.setPosition(sf::Vector2f(center.x, boundingRect.top));
    }
    else {
      head.setPosition(sf::Vector2f(center.x + boundingRect.height / (2 * tan(theta)), boundingRect.top));
    }
  }
  else if (thetaDegPositive >= angleTemplate[7] || thetaDegPositive < angleTemplate[0]) {
    if (thetaDegPositive == 0.f) {
      head.setPosition(sf::Vector2f(boundingRect.left, center.y));
    }
    else {
      head.setPosition(sf::Vector2f(boundingRect.left, center.y + boundingRect.width / (2 * tan((float(M_PI) / 2) - theta))));
    }
  }
  else {
    // Mathematica: Reduce[{a + p == (b + q)*m, p^2 + q^2 == c}, {p, q}]
    float r = Node::fieldTmplt.getCornersRadius() + Node::fieldTmplt.getOutlineThickness(); // radius of rounded corners
    float m = abs(delta.y / delta.x);
    float a = (boundingRect.height / 2 - r);
    float b = (boundingRect.width / 2 - r);
    float c = r * r;
    float p = (-a + b * m + sqrt((c - a * a) * m * m + 2 * a * b * m * m * m + (c - b * b) * m * m * m * m)) / (1 + m * m);
    float q = (a - b * m + p) / m;

    if (thetaDegPositive >= angleTemplate[0] && thetaDegPositive < angleTemplate[1]) {
      head.setPosition(sf::Vector2f(boundingRect.left + (r - q), boundingRect.top + boundingRect.height - (r - p)));
    }
    else if (thetaDegPositive >= angleTemplate[2] && thetaDegPositive < angleTemplate[3]) {
      head.setPosition(sf::Vector2f(boundingRect.left + boundingRect.width - (r - q), boundingRect.top + boundingRect.height - (r - p)));
    }
    else if (thetaDegPositive >= angleTemplate[4] && thetaDegPositive < angleTemplate[5]) {
      head.setPosition(sf::Vector2f(boundingRect.left + boundingRect.width - (r - q), boundingRect.top + (r - p)));
    }
    else if (thetaDegPositive >= angleTemplate[6] && thetaDegPositive < angleTemplate[7]) {
      head.setPosition(sf::Vector2f(boundingRect.left + (r - q), boundingRect.top + (r - p)));
    }
  }
  
  sf::FloatRect headBoundingRect = head.getGlobalBounds();
  sf::Vector2f headCenter(headBoundingRect.left + headBoundingRect.width / 2, headBoundingRect.top + headBoundingRect.height / 2);
  delta.y = -delta.y;
  float length = sqrt(delta.x * delta.x + delta.y * delta.y);
  sf::Vector2f deltaNorm = delta / length;
  sf::Vector2f deltaNormPerp(-deltaNorm.y, deltaNorm.x);

  directLine[0].position = origin + 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;
  directLine[1].position = origin - 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;
  directLine[2].position = headCenter - 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;
  directLine[3].position = headCenter + 0.75f * Node::fieldTmplt.getOutlineThickness() * deltaNormPerp;
}

void Arrow::setDirectLineColor(sf::Color color) {
  for (size_t i = 0; i < 4; i++) {
    directLine[i].color = color;
  }
  head.setFillColor(color);
}

sf::Vector2f Arrow::getDestination() const {
  return destination;
}

size_t Arrow::getOriginNode() const {
  return originNode;
}

size_t Arrow::getDestinationNode() const {
  return destinationNode;
}