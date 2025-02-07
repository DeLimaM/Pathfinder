#include "graph/GraphGeneration.hpp"
#include "graph/Graph.hpp"
#include <atomic>
#include <cstddef>
#include <random>
#include <set>
#include <stdexcept>
#include <thread>

struct GridDimensions {
  size_t width;
  size_t height;
  float cellWidth;
  float cellHeight;
};

struct GenerationProgress {
  std::atomic<float> *progress;
  std::atomic<const char *> *status;
};

GridDimensions calculateGridDimensions(size_t vertices) {
  size_t gridWidth = static_cast<size_t>(std::sqrt(vertices));
  size_t gridHeight = (vertices + gridWidth - 1) / gridWidth;
  float cellWidth =
      (DEFAULT_WINDOW_WIDTH - 2 * WINDOW_PADDING) / (gridWidth - 1);
  float cellHeight =
      (DEFAULT_WINDOW_HEIGHT - 2 * WINDOW_PADDING) / (gridHeight - 1);

  return {gridWidth, gridHeight, cellWidth, cellHeight};
}

std::vector<Vector2> generateVertexPositions(const GridDimensions &grid,
                                             size_t vertices) {
  std::vector<Vector2> positions;
  positions.reserve(vertices);

  std::mt19937 gen(1234);
  std::uniform_real_distribution<float> offset(-grid.cellWidth / 4,
                                               grid.cellWidth / 4);

  for (size_t y = 0; y < grid.height; y++) {
    for (size_t x = 0; x < grid.width; x++) {
      if (y * grid.width + x >= vertices)
        break;

      float posX = WINDOW_PADDING + x * grid.cellWidth + offset(gen);
      float posY = WINDOW_PADDING + y * grid.cellHeight + offset(gen);
      positions.push_back({posX, posY});
    }
  }
  return positions;
}

std::vector<std::pair<size_t, size_t>> generateEdges(const GridDimensions &grid,
                                                     size_t vertices) {
  size_t expectedEdges =
      (grid.width - 1) * grid.height + (grid.height - 1) * grid.width;
  std::vector<std::pair<size_t, size_t>> edgePairs;
  edgePairs.reserve(expectedEdges);

  for (size_t i = 0; i < vertices; i++) {
    size_t x = i % grid.width;
    size_t y = i / grid.width;

    if (x < grid.width - 1 && i + 1 < vertices) {
      edgePairs.emplace_back(i, i + 1);
    }
    if (y < grid.height - 1 && i + grid.width < vertices) {
      edgePairs.emplace_back(i, i + grid.width);
    }
  }
  return edgePairs;
}

void updateProgress(const GenerationProgress &progress, const char *message,
                    float value) {
  if (progress.status) {
    progress.status->store(message);
  }
  if (progress.progress) {
    progress.progress->store(value);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(LOADING_SLEEP_TIME));
}

void createRandomGraph(Graph &graph, size_t vertices,
                       std::atomic<float> *progress = nullptr,
                       std::atomic<const char *> *status = nullptr) {
  if (vertices < 2)
    return;

  GenerationProgress genProgress{progress, status};
  updateProgress(genProgress, "Calculating grid dimensions", 0.1f);

  auto grid = calculateGridDimensions(vertices);
  updateProgress(genProgress, "Pre-allocation", 0.2f);

  size_t expectedEdges =
      (grid.width - 1) * grid.height + (grid.height - 1) * grid.width;
  graph.getVertices().reserve(vertices);
  graph.getEdges().reserve(expectedEdges);

  updateProgress(genProgress, "Computing vertex positions", 0.5f);
  auto positions = generateVertexPositions(grid, vertices);

  updateProgress(genProgress, "Adding vertices", 0.7f);
  graph.addVerticesBulk(positions);

  updateProgress(genProgress, "Adding edges", 0.8f);
  auto edgePairs = generateEdges(grid, vertices);
  graph.addEdgesBulk(edgePairs);

  updateProgress(genProgress, "Graph generation complete", 1.0f);
}