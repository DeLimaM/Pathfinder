#include "PathfindingVisualizer.hpp"
#include "algorithms/AStar.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include <atomic>

int main() {
  Graph graph;
  std::atomic<bool> shouldExit = false;
  std::atomic<bool> isPaused = true;

  const size_t vertices = DEFAULT_VERTICES * 8;
  size_t start = 0;
  size_t end = vertices - 1;

  Dijkstra algorithm;
  PathfindingVisualizer visualizer(graph, shouldExit, start, end);
  visualizer.setPauseState(&isPaused);

  visualizer.generateGraph(vertices);
  visualizer.run(algorithm);

  return 0;
}