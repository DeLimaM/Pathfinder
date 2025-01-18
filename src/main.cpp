#include "Constants.hpp"
#include "algorithms/BruteForce.hpp"
#include "graph/Graph.hpp"
#include "visualization/Window.hpp"
#include <chrono>
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

  graph.setVertexColor(vertices[0], START_NODE_COLOR);
  graph.setVertexColor(vertices[vertices.size() - 1], END_NODE_COLOR);
}

int main() {
  Window window(800, 600, WINDOW_TITLE);
  Graph graph;
  createGridGraph(graph, 5, 5);

  size_t start = 0;
  size_t end = graph.getVertices().size() - 1;
  BruteForce bruteForce;

  std::vector<Color> originalColors;
  for (const auto &vertex : graph.getVertices()) {
    originalColors.push_back(vertex.color);
  }

  bruteForce.setVisualizationCallback(
      [&](const std::vector<size_t> &currentPath) {
        for (size_t i = 0; i < graph.getVertices().size(); i++) {
          graph.setVertexColor(i, originalColors[i]);
        }

        for (size_t nodeIndex : currentPath) {
          graph.setVertexColor(nodeIndex, PATH_COLOR);
        }

        graph.setVertexColor(start, START_NODE_COLOR);
        graph.setVertexColor(end, END_NODE_COLOR);

        window.clear();
        window.draw(graph);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      });

  while (!window.shouldClose()) {
    window.clear();
    window.draw(graph);
    window.display();

    if (IsKeyPressed(KEY_SPACE)) {
      auto path = bruteForce.findPath(graph, start, end);

      for (size_t nodeIndex : path) {
        graph.setVertexColor(nodeIndex, PATH_COLOR);
      }
      graph.setVertexColor(start, START_NODE_COLOR);
      graph.setVertexColor(end, END_NODE_COLOR);
    }
  }

  return 0;
}