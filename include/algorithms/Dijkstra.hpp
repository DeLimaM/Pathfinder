#pragma once
#include "PathfindingAlgorithm.hpp"
#include "graph/Graph.hpp"
#include <vector>

class Dijkstra : public PathfindingAlgorithm {
public:
  using PathfindingAlgorithm::PathfindingAlgorithm;

  Dijkstra() : PathfindingAlgorithm("Dijkstra") {}
  std::vector<size_t> findPath(const Graph &graph, size_t start,
                               size_t end) override;
};