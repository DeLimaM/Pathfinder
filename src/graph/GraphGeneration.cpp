#include "graph/GraphGeneration.hpp"
#include "graph/Graph.hpp"
#include <cstddef>
#include <random>
#include <set>
#include <stdexcept>

void createRandomGraph(Graph &graph, size_t vertices) {
  if (vertices < 2)
    return;

  size_t gridWidth = static_cast<size_t>(std::sqrt(vertices));
  size_t gridHeight = (vertices + gridWidth - 1) / gridWidth;

  // reserve space for vertices and edges
  std::vector<Vector2> positions;
  positions.reserve(vertices);
  graph.getVertices().reserve(vertices);
  size_t expectedEdges =
      (gridWidth - 1) * gridHeight + (gridHeight - 1) * gridWidth;
  graph.getEdges().reserve(expectedEdges);

  float cellWidth =
      (DEFAULT_WINDOW_WIDTH - 2 * WINDOW_PADDING) / (gridWidth - 1);
  float cellHeight =
      (DEFAULT_WINDOW_HEIGHT - 2 * WINDOW_PADDING) / (gridHeight - 1);

  std::random_device rd;
  std::mt19937 gen(1234);
  std::uniform_real_distribution<float> offset(-cellWidth / 4, cellWidth / 4);

  // pre-compute positions
  for (size_t y = 0; y < gridHeight; y++) {
    for (size_t x = 0; x < gridWidth; x++) {
      if (y * gridWidth + x >= vertices)
        break;

      float posX = WINDOW_PADDING + x * cellWidth + offset(gen);
      float posY = WINDOW_PADDING + y * cellHeight + offset(gen);
      positions.push_back({posX, posY});
    }
  }

  // add vertices
  graph.addVerticesBulk(positions);

  // add edges
  std::vector<std::pair<size_t, size_t>> edgePairs;
  edgePairs.reserve(expectedEdges);

  for (size_t i = 0; i < vertices; i++) {
    size_t x = i % gridWidth;
    size_t y = i / gridWidth;

    if (x < gridWidth - 1 && i + 1 < vertices) {
      edgePairs.emplace_back(i, i + 1);
    }
    if (y < gridHeight - 1 && i + gridWidth < vertices) {
      edgePairs.emplace_back(i, i + gridWidth);
    }
  }

  graph.addEdgesBulk(edgePairs);
}