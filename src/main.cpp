#include "PathfindingVisualizer.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include <atomic>
#include <cstddef>

void createGridGraph(Graph &graph, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      float x =
          WINDOW_PADDING +
          (j * ((DEFAULT_WINDOW_WIDTH - 2 * WINDOW_PADDING) / (cols - 1)));
      float y =
          WINDOW_PADDING +
          (i * ((DEFAULT_WINDOW_HEIGHT - 2 * WINDOW_PADDING) / (rows - 1)));
      graph.addVertex(Vector2{x, y});
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      size_t current = i * cols + j;

      if (j < cols - 1) {
        graph.addEdge(current, current + 1);
      }

      if (i < rows - 1) {
        graph.addEdge(current, current + cols);
      }
    }
  }
}

int main() {
  Graph graph;
  std::atomic<bool> shouldExit = false;

  const int rows = 25;
  const int cols = 25;
  createGridGraph(graph, rows, cols);

  size_t start = 0;
  size_t end = (rows * cols) - 1;

  Dijkstra algorithm;
  PathfindingVisualizer visualizer(graph, shouldExit, start, end);
  visualizer.run(algorithm);

  return 0;
}