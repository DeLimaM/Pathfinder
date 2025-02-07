#include "PathfindingVisualizer.hpp"
#include "algorithms/AStar.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include <atomic>

int main() {
  Graph graph;
  std::atomic<bool> shouldExit;
  std::atomic_init(&shouldExit, false);
  std::atomic<bool> isPaused;
  std::atomic_init(&isPaused, true);

  const size_t vertices = DEFAULT_VERTICES * 4;
  size_t start = 0;
  size_t end = vertices - 1;

  Dijkstra algorithm;
  PathfindingVisualizer visualizer(graph, shouldExit, start, end);
  visualizer.setPauseState(&isPaused);

  visualizer.generateGraph(vertices, graph);
  visualizer.run(algorithm);

  return 0;
}