#include "PathfindingVisualizer.hpp"
#include "algorithms/AStar.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include <atomic>
#include <cstddef>
#include <random>
#include <set>
#include <stdexcept>

void createRandomGraph(Graph &graph, size_t vertices) {
  if (vertices < 2)
    return;

  std::random_device rd;
  std::mt19937 gen(rd());

  float centerY = DEFAULT_WINDOW_HEIGHT / 2.0f;
  float leftX = WINDOW_PADDING * 2;
  float rightX = DEFAULT_WINDOW_WIDTH - WINDOW_PADDING * 2;
  float midMinX = WINDOW_PADDING * 3;
  float midMaxX = DEFAULT_WINDOW_WIDTH - WINDOW_PADDING * 3;

  // start vertex
  graph.addVertex(Vector2{leftX, centerY});

  // middle vertices
  std::uniform_real_distribution<float> xDist(midMinX, midMaxX);
  std::uniform_real_distribution<float> yDist(
      WINDOW_PADDING, DEFAULT_WINDOW_HEIGHT - WINDOW_PADDING);

  for (size_t i = 1; i < vertices - 1; i++) {
    float x = xDist(gen);
    float y = yDist(gen);
    graph.addVertex(Vector2{x, y});
  }

  // end vertex
  graph.addVertex(Vector2{rightX, centerY});

  for (size_t i = 0; i < vertices - 1; i++) {
    graph.addEdge(i, i + 1);
  }

  // add random edges
  std::uniform_int_distribution<size_t> vertexDist(0, vertices - 1);
  std::uniform_int_distribution<size_t> extraEdgesDist(vertices / 4,
                                                       vertices / 2);
  size_t extraEdges = extraEdgesDist(gen);

  for (size_t i = 0; i < extraEdges; i++) {
    size_t from = vertexDist(gen);
    size_t to = vertexDist(gen);

    if (from != to && !graph.hasEdge(from, to)) {
      graph.addEdge(from, to);
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

  const size_t vertices = 128;
  createRandomGraph(graph, vertices);

  size_t start = 0;
  size_t end = vertices - 1;

  AStar algorithm;
  PathfindingVisualizer visualizer(graph, shouldExit, start, end);
  visualizer.run(algorithm);

  return 0;
}