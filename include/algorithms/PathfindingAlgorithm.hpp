#pragma once
#include "graph/Graph.hpp"
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

class PathfindingAlgorithm {
public:
  using VisualizationCallback =
      std::function<void(const std::vector<size_t> &)>;

  explicit PathfindingAlgorithm(const std::string &algorithmName)
      : name(algorithmName) {}
  virtual ~PathfindingAlgorithm() = default;
  virtual std::vector<size_t> findPath(const Graph &graph, size_t start,
                                       size_t end) = 0;
  void setVisualizationCallback(VisualizationCallback callback) {
    visualCallback = callback;
  }
  const std::string &getName() const { return name; }

protected:
  VisualizationCallback visualCallback;
  std::string name;
};