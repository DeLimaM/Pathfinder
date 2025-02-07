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

  std::vector<std::shared_ptr<Vertex>> &getVertices() { return vertices; }
  std::vector<std::shared_ptr<Edge>> &getEdges() { return edges; }
  const std::vector<std::shared_ptr<Vertex>> &getVertices() const {
    return vertices;
  }
  const std::vector<std::shared_ptr<Edge>> &getEdges() const { return edges; }
  std::shared_ptr<Vertex> getVertex(size_t id) const { return vertices[id]; }
  bool hasEdge(size_t from, size_t to) const;

private:
  std::vector<std::shared_ptr<Vertex>> vertices;
  std::vector<std::shared_ptr<Edge>> edges;
};