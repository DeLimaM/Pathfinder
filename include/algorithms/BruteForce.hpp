#pragma once
#include "PathfindingAlgorithm.hpp"

class BruteForce : public PathfindingAlgorithm {
public:
  BruteForce() = default;
  ~BruteForce() override = default;

  std::vector<size_t> findPath(const Graph &graph, size_t start,
                               size_t end) override;

private:
  void exploreNode(const Graph &graph, size_t current, size_t end,
                   std::vector<bool> &visited, std::vector<size_t> &currentPath,
                   std::vector<size_t> &shortestPath);

  void exploreNeighbors(const Graph &graph, size_t current, size_t end,
                        std::vector<bool> &visited,
                        std::vector<size_t> &currentPath,
                        std::vector<size_t> &shortestPath);

  void updateShortestPath(const std::vector<size_t> &currentPath,
                          std::vector<size_t> &shortestPath);
};