#include "PathfindingVisualizer.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include <atomic>
#include <cstddef>
#include <random>
#include <set>
#include <stdexcept>

void createRandomGraph(Graph &graph, size_t vertices, size_t edges) {
  size_t maxEdges = (vertices * (vertices - 1)) / 2;

  if (edges > maxEdges) {
    throw std::invalid_argument("Too many edges for given vertices");
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> posDist(
      WINDOW_PADDING, DEFAULT_WINDOW_WIDTH - WINDOW_PADDING);
  std::uniform_int_distribution<size_t> dis(0, vertices - 1);

  std::set<std::pair<size_t, size_t>> addedEdges;

  for (size_t i = 0; i < vertices; i++) {
    Vector2 position = {posDist(gen), posDist(gen)};
    graph.addVertex(position);
  }

  while (addedEdges.size() < edges) {
    size_t from = dis(gen);
    size_t to = dis(gen);

    if (from != to) {
      auto edge = std::minmax(from, to);
      if (addedEdges.insert(edge).second) {
        graph.addEdge(from, to);
      }
    }
  }
}

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

  const size_t vertices = 20;
  const size_t edges = 40;
  createRandomGraph(graph, vertices, edges);

  size_t start = 0;
  size_t end = vertices - 1;

  Dijkstra algorithm;
  PathfindingVisualizer visualizer(graph, shouldExit, start, end);
  visualizer.run(algorithm);

  return 0;
}