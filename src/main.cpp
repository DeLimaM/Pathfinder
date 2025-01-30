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

  size_t gridWidth = static_cast<size_t>(std::sqrt(vertices));
  size_t gridHeight = (vertices + gridWidth - 1) / gridWidth;

  float cellWidth =
      (DEFAULT_WINDOW_WIDTH - 2 * WINDOW_PADDING) / (gridWidth - 1);
  float cellHeight =
      (DEFAULT_WINDOW_HEIGHT - 2 * WINDOW_PADDING) / (gridHeight - 1);

  std::random_device rd;
  std::mt19937 gen(rd());

  for (size_t y = 0; y < gridHeight; y++) {
    for (size_t x = 0; x < gridWidth; x++) {
      if (y * gridWidth + x >= vertices)
        break;

      float posX = WINDOW_PADDING + x * cellWidth;
      float posY = WINDOW_PADDING + y * cellHeight;

      std::uniform_real_distribution<float> offset(-cellWidth / 4,
                                                   cellWidth / 4);
      posX += offset(gen);
      posY += offset(gen);

      size_t index = y * gridWidth + x;
      Vector2 position = {posX, posY};

      size_t vertexId = graph.addVertex(position);
      if (index == 0) {
        graph.getVertex(vertexId)->setColor(START_NODE_COLOR);
      } else if (index == vertices - 1) {
        graph.getVertex(vertexId)->setColor(END_NODE_COLOR);
      }
    }
  }

  for (size_t i = 0; i < vertices; i++) {
    size_t x = i % gridWidth;
    size_t y = i / gridWidth;

    if (x < gridWidth - 1 && i + 1 < vertices)
      graph.addEdge(i, i + 1);

    if (y < gridHeight - 1 && i + gridWidth < vertices)
      graph.addEdge(i, i + gridWidth);

    std::uniform_real_distribution<float> chance(0, 1);
    if (chance(gen) < 0.3) {
      if (x < gridWidth - 1 && y < gridHeight - 1 &&
          i + gridWidth + 1 < vertices)
        graph.addEdge(i, i + gridWidth + 1);
    }
  }
}

int main() {
  Graph graph;
  std::atomic<bool> shouldExit = false;
  std::atomic<bool> isPaused = true;

  const size_t vertices = DEFAULT_VERTICES;
  createRandomGraph(graph, vertices);

  size_t start = 0;
  size_t end = vertices - 1;

  Dijkstra algorithm;
  PathfindingVisualizer visualizer(graph, shouldExit, start, end);

  visualizer.setPauseState(&isPaused);

  visualizer.run(algorithm);

  return 0;
}