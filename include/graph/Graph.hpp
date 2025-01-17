#pragma once
#include "Constants.hpp"
#include "raylib.h"
#include <cstddef>
#include <unordered_map>
#include <vector>

class Graph {
public:
  struct Vertex {
    Vector2 position;
    std::vector<size_t> neighbors;
    Color color = NODE_COLOR;
  };

  Graph() = default;
  ~Graph() = default;

  size_t addVertex(Vector2 position);
  void addEdge(size_t from, size_t to);
  void removeVertex(size_t id);
  void removeEdge(size_t from, size_t to);

  const std::vector<Vertex> &getVertices() const { return vertices; }
  const Vertex &getVertex(size_t id) const { return vertices[id]; }
  bool hasEdge(size_t from, size_t to) const;

  void setVertexColor(size_t id, Color color);
  void setVertexPosition(size_t id, Vector2 position);

private:
  std::vector<Vertex> vertices;
};