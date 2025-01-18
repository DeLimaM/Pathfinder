#pragma once
#include "Constants.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

class Edge;

class Vertex {
public:
  Vertex(Vector2 pos, Color color = NODE_COLOR);
  ~Vertex() = default;

  void addEdge(std::shared_ptr<Edge> edge);
  void removeEdge(std::shared_ptr<Edge> edge);

  Vector2 getPosition() const { return position; }
  void setPosition(Vector2 pos) { position = pos; }
  Color getColor() const { return color; }
  void setColor(Color c) { color = c; }
  const std::vector<std::shared_ptr<Edge>> &getEdges() const { return edges; }

private:
  Vector2 position;
  Color color;
  std::vector<std::shared_ptr<Edge>> edges;
};