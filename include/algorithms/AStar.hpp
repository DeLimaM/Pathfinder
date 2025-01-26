#pragma once
#include "PathfindingAlgorithm.hpp"
#include "graph/Graph.hpp"
#include <vector>

class AStar : public PathfindingAlgorithm {
public:
  using PathfindingAlgorithm::PathfindingAlgorithm;

  std::vector<size_t> findPath(const Graph &graph, size_t start,
                               size_t end) override;
};