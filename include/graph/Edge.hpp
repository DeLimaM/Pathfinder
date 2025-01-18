#pragma once
#include "Vertex.hpp"
#include <memory>

class Edge {
public:
  Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2);
  ~Edge() = default;

  std::shared_ptr<Vertex>
  getOtherVertex(const std::shared_ptr<Vertex> &vertex) const;
  std::shared_ptr<Vertex> getVertex1() const { return vertex1.lock(); }
  std::shared_ptr<Vertex> getVertex2() const { return vertex2.lock(); }

private:
  std::weak_ptr<Vertex> vertex1;
  std::weak_ptr<Vertex> vertex2;
};