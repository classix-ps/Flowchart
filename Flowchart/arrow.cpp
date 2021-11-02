#include "arrow.hpp"

std::array<float, 12> Arrow::angleTemplate;

Arrow::Arrow() : head{ 4.f, 3 }, directLine{ sf::Quads, 4 } {
  for (size_t i = 0; i < 4; i++) {
    directLine[i].color = sf::Color(0, 102, 51, 255);
  }
  head.setFillColor(sf::Color(0, 102, 51, 255));
  head.setOrigin(4.f, 0.5f);
  head.setScale(sf::Vector2f(0.8f, 1.f));
}

void Arrow::draw(sf::RenderWindow& window) const {
  for (const sf::VertexArray segment : segments) {
    window.draw(segment);
  }
  window.draw(directLine);
  window.draw(head);
}

void Arrow::setOrigin(const sf::Vector2f& pos) {
  origin = pos;
}

void Arrow::setDestination(const sf::Vector2f& pos) {
  sf::Vector2f delta = pos - origin;
  float length = sqrt(delta.x * delta.x + delta.y * delta.y);
  sf::Vector2f deltaNorm = delta / length;
  sf::Vector2f deltaNormPerp(-deltaNorm.y, deltaNorm.x);

  directLine[0].position = origin + 0.75f * deltaNormPerp;
  directLine[1].position = origin - 0.75f * deltaNormPerp;
  directLine[2].position = pos - 0.75f * deltaNormPerp;
  directLine[3].position = pos + 0.75f * deltaNormPerp;

  head.setPosition(pos);
  head.setRotation((atan2(delta.y, delta.x) * 180.f / float(M_PI)) + 90.f);
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
  if (thetaDegPositive >= angleTemplate[0] && thetaDegPositive < angleTemplate[1]) {
    if (thetaDegPositive < angleTemplate[8]) {

    }
    else {

    }
  }
  else if (thetaDegPositive >= angleTemplate[1] && thetaDegPositive < angleTemplate[2]) {
    if (thetaDegPositive == 90.f) {
      head.setPosition(sf::Vector2f(center.x, boundingRect.top + boundingRect.height));
    }
    else {
      head.setPosition(sf::Vector2f(center.x - boundingRect.height / (2 * tan(theta)), boundingRect.top + boundingRect.height));
    }
  }
  else if (thetaDegPositive >= angleTemplate[2] && thetaDegPositive < angleTemplate[3]) {
    if (thetaDegPositive < angleTemplate[9]) {

    }
    else {

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
  else if (thetaDegPositive >= angleTemplate[4] && thetaDegPositive < angleTemplate[5]) {
    if (thetaDegPositive < angleTemplate[10]) {

    }
    else {

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
  else if (thetaDegPositive >= angleTemplate[6] && thetaDegPositive < angleTemplate[7]) {
    if (thetaDegPositive < angleTemplate[11]) {

    }
    else {

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
  
  sf::FloatRect headBoundingRect = head.getGlobalBounds();
  sf::Vector2f headCenter(headBoundingRect.left + headBoundingRect.width / 2, headBoundingRect.top + headBoundingRect.height / 2);
  delta.y = -delta.y;
  float length = sqrt(delta.x * delta.x + delta.y * delta.y);
  sf::Vector2f deltaNorm = delta / length;
  sf::Vector2f deltaNormPerp(-deltaNorm.y, deltaNorm.x);

  directLine[0].position = origin + 0.75f * deltaNormPerp;
  directLine[1].position = origin - 0.75f * deltaNormPerp;
  directLine[2].position = headCenter - 0.75f * deltaNormPerp;
  directLine[3].position = headCenter + 0.75f * deltaNormPerp;
}

void Arrow::setDirectLineColor(sf::Color color) {
  for (size_t i = 0; i < 4; i++) {
    directLine[i].color = color;
  }
  head.setFillColor(color);
}

sf::Vector2f Arrow::getDestination() {
  return sf::Vector2f((directLine[2].position.x + directLine[3].position.x) / 2, (directLine[2].position.y + directLine[3].position.y) / 2);
}

size_t Arrow::getOriginNode() {
  return originNode;
}