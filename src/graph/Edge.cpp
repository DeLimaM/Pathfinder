#include "graph/Edge.hpp"

Edge::Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, Color color)
    : vertex1(v1), vertex2(v2), color(color) {}

std::shared_ptr<Vertex>
Edge::getOtherVertex(const std::shared_ptr<Vertex> &vertex) const {
  if (auto v1 = vertex1.lock()) {
    if (v1 == vertex) {
      return vertex2.lock();
    }
  }
  return vertex1.lock();
}