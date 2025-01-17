#include "Constants.hpp"
#include "graph/Graph.hpp"
#include "visualization/Window.hpp"

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
  Window window(800, 600);
  Graph graph;

  createGridGraph(graph, 15, 20);

  while (!window.shouldClose()) {
    window.clear();
    window.draw(graph);
    window.display();
  }

  return 0;
}