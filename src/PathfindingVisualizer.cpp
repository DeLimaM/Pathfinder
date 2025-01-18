#include "PathfindingVisualizer.hpp"
#include "Constants.hpp"
#include "algorithms/PathfindingAlgorithm.hpp"
#include "graph/Graph.hpp"
#include "visualization/Window.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <thread>
#include <vector>

PathfindingVisualizer::PathfindingVisualizer(Graph &graph,
                                             std::atomic<bool> &shouldExit,
                                             size_t start, size_t end)
    : graph(graph), start(start), end(end), shouldExit(shouldExit),
      window(graph, shouldExit) {}

void PathfindingVisualizer::visualizePath(
    const std::vector<size_t> &currentPath) {
  if (shouldExit)
    return;

  resetColors();
  updatePathColors(currentPath);
  updateEndpointColors();

  std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
}

void PathfindingVisualizer::run(PathfindingAlgorithm &algorithm) {
  std::thread algorithmThread([&]() {
    algorithm.setVisualizationCallback(
        [this](const std::vector<size_t> &path) { this->visualizePath(path); });
    algorithm.findPath(graph, start, end);
  });

  window.run();

  if (algorithmThread.joinable()) {
    algorithmThread.join();
  }
}

void PathfindingVisualizer::resetColors() {
  for (const auto &vertex : graph.getVertices()) {
    size_t index = std::find(graph.getVertices().begin(),
                             graph.getVertices().end(), vertex) -
                   graph.getVertices().begin();
    if (index != start && index != end) {
      vertex->setColor(NODE_COLOR);
      for (const auto &edge : vertex->getEdges()) {
        edge->setColor(EDGE_COLOR);
      }
    }
  }
}

void PathfindingVisualizer::updatePathColors(const std::vector<size_t> &path) {
  for (size_t i = 0; i < path.size() - 1; i++) {
    colorPathNode(path[i]);
    colorPathEdge(path[i], path[i + 1]);
  }
  if (!path.empty()) {
    colorPathNode(path.back());
  }
}

void PathfindingVisualizer::colorPathNode(size_t nodeIndex) {
  if (nodeIndex != start && nodeIndex != end) {
    graph.getVertex(nodeIndex)->setColor(PATH_COLOR);
  }
}

void PathfindingVisualizer::colorPathEdge(size_t from, size_t to) {
  auto vertex1 = graph.getVertex(from);
  auto vertex2 = graph.getVertex(to);

  for (const auto &edge : vertex1->getEdges()) {
    if (edge->getOtherVertex(vertex1) == vertex2) {
      edge->setColor(PATH_COLOR);
      break;
    }
  }
}

void PathfindingVisualizer::updateEndpointColors() {
  graph.getVertex(start)->setColor(START_NODE_COLOR);
  graph.getVertex(end)->setColor(END_NODE_COLOR);
}