#pragma once
#include "Constants.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"
#include <memory>
#include <vector>

class Graph {
public:
  Graph() = default;
  ~Graph() = default;

  size_t addVertex(Vector2 position);
  void addEdge(size_t from, size_t to);
  void removeVertex(size_t id);
  void removeEdge(size_t from, size_t to);

  const std::vector<std::shared_ptr<Vertex>> &getVertices() const {
    return vertices;
  }
  std::shared_ptr<Vertex> getVertex(size_t id) const { return vertices[id]; }
  bool hasEdge(size_t from, size_t to) const;

  void setVertexColor(size_t id, Color color);
  void setVertexPosition(size_t id, Vector2 position);

private:
  std::vector<std::shared_ptr<Vertex>> vertices;
  std::vector<std::shared_ptr<Edge>> edges;
};