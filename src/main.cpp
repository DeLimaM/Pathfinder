#include "Constants.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include "visualization/Window.hpp"
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

void createGridGraph(Graph &graph, int rows, int cols, float spacing = 100.0f) {
  float startX = 0.0f;
  float startY = 0.0f;

  std::vector<size_t> vertices;
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      float x = startX + col * spacing;
      float y = startY + row * spacing;
      vertices.push_back(graph.addVertex({x, y}));
    }
  }

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      size_t current = row * cols + col;

      if (col < cols - 1) {
        graph.addEdge(vertices[current], vertices[current + 1]);
      }
      if (row < rows - 1) {
        graph.addEdge(vertices[current], vertices[current + cols]);
      }
    }
  }

  graph.getVertex(vertices[0])->setColor(START_NODE_COLOR);
  graph.getVertex(vertices[vertices.size() - 1])->setColor(END_NODE_COLOR);
}

int main() {
  Dijkstra algorithm;
  Graph graph;
  Window window = Window(graph);

  const int rows = 25;
  const int cols = 25;
  createGridGraph(graph, rows, cols);

  size_t start = 0;
  size_t end = (rows * cols) - 1;

  algorithm.setVisualizationCallback(
      [&graph, start, end](const std::vector<size_t> &currentPath) {
        for (const auto &vertex : graph.getVertices()) {
          size_t index = std::find(graph.getVertices().begin(),
                                   graph.getVertices().end(), vertex) -
                         graph.getVertices().begin();
          if (index != start && index != end) {
            vertex->setColor(NODE_COLOR);
          }
        }

        for (size_t i = 0; i < currentPath.size(); i++) {
          if (currentPath[i] != start && currentPath[i] != end) {
            auto vertex = graph.getVertex(currentPath[i]);
            vertex->setColor(PATH_COLOR);
          }
        }

        graph.getVertex(start)->setColor(START_NODE_COLOR);
        graph.getVertex(end)->setColor(END_NODE_COLOR);

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
      });

  std::thread pathfindingThread([&]() {
    std::vector<size_t> path = algorithm.findPath(graph, start, end);
  });

  window.run();

  pathfindingThread.join();

  return 0;
}