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

  std::vector<Vector2> positions;
  positions.reserve(vertices);
  graph.getVertices().reserve(vertices);
  graph.getEdges().reserve(gridWidth * gridHeight);

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
  for (size_t i = 0; i < positions.size(); i++) {
    size_t vertexId = graph.addVertex(positions[i]);
    if (i == 0) {
      graph.getVertex(vertexId)->setColor(START_NODE_COLOR);
    } else if (i == vertices - 1) {
      graph.getVertex(vertexId)->setColor(END_NODE_COLOR);
    }
  }

  // add edges
  for (size_t i = 0; i < vertices; i++) {
    size_t x = i % gridWidth;
    size_t y = i / gridWidth;

    if (x < gridWidth - 1 && i + 1 < vertices) {
      graph.addEdge(i, i + 1);
    }
    if (y < gridHeight - 1 && i + gridWidth < vertices) {
      graph.addEdge(i, i + gridWidth);
    }
  }
}