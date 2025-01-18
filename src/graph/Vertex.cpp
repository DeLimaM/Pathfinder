#include "graph/Vertex.hpp"
#include "graph/Edge.hpp"
#include <algorithm>

Vertex::Vertex(Vector2 pos, Color c) : position(pos), color(c) {}

void Vertex::addEdge(std::shared_ptr<Edge> edge) { edges.push_back(edge); }

void Vertex::removeEdge(std::shared_ptr<Edge> edge) {
  edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
}