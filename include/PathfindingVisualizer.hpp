#pragma once

#include "Constants.hpp"
#include "algorithms/PathfindingAlgorithm.hpp"
#include "graph/Graph.hpp"
#include "visualization/Window.hpp"
#include <atomic>
#include <chrono>
#include <cstddef>
#include <thread>
#include <vector>

class PathfindingVisualizer {
private:
  Graph &graph;
  size_t start;
  size_t end;
  std::atomic<bool> &shouldExit;
  std::atomic<bool> *isPaused;
  Window window;

public:
  PathfindingVisualizer(Graph &graph, std::atomic<bool> &shouldExit,
                        size_t start, size_t end);

  void visualizePath(const std::vector<size_t> &currentPath);
  void run(PathfindingAlgorithm &algorithm);
  void setPauseState(std::atomic<bool> *pauseState);
  void generateGraph(size_t vertices);

private:
  void resetColors();
  void updatePathColors(const std::vector<size_t> &path);
  void colorPathNode(size_t nodeIndex);
  void colorPathEdge(size_t from, size_t to);
  void updateEndpointColors();
};