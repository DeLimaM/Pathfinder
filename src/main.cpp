#include "graph/Graph.hpp"
#include "visualization/Window.hpp"

int main() {
  Window window(800, 600);

  Graph graph;

  while (!window.shouldClose()) {
    window.clear();
    window.draw(graph);
    window.display();
  }

  return 0;
}