#include "graph/Graph.hpp"
#include <algorithm>

size_t Graph::addVertex(Vector2 position) {
  vertices.push_back({position, {}, NODE_COLOR});
  return vertices.size() - 1;
}

void Graph::addEdge(size_t from, size_t to) {
  if (from >= vertices.size() || to >= vertices.size())
    return;
  if (!hasEdge(from, to)) {
    vertices[from].neighbors.push_back(to);
    vertices[to].neighbors.push_back(from);
  }
}

void Graph::removeVertex(size_t id) {
  if (id >= vertices.size())
    return;

  for (auto &vertex : vertices) {
    auto &neighbors = vertex.neighbors;
    neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), id),
                    neighbors.end());

    for (auto &neighbor : neighbors) {
      if (neighbor > id) {
        neighbor--;
      }
    }
  }

  vertices.erase(vertices.begin() + id);
}

void Graph::removeEdge(size_t from, size_t to) {
  if (from >= vertices.size() || to >= vertices.size())
    return;

  auto &fromNeighbors = vertices[from].neighbors;
  auto &toNeighbors = vertices[to].neighbors;

  fromNeighbors.erase(
      std::remove(fromNeighbors.begin(), fromNeighbors.end(), to),
      fromNeighbors.end());

  toNeighbors.erase(std::remove(toNeighbors.begin(), toNeighbors.end(), from),
                    toNeighbors.end());
}

bool Graph::hasEdge(size_t from, size_t to) const {
  if (from >= vertices.size() || to >= vertices.size())
    return false;

  const auto &neighbors = vertices[from].neighbors;
  return std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
}

void Graph::setVertexColor(size_t id, Color color) {
  if (id < vertices.size()) {
    vertices[id].color = color;
  }
}

void Graph::setVertexPosition(size_t id, Vector2 position) {
  if (id < vertices.size()) {
    vertices[id].position = position;
  }
}