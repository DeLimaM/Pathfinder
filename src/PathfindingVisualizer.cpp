#include "PathfindingVisualizer.hpp"
#include "Constants.hpp"
#include "algorithms/PathfindingAlgorithm.hpp"
#include "graph/Graph.hpp"
#include "graph/GraphGeneration.hpp"
#include "visualization/Window.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
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

  while (*isPaused && !shouldExit) {
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
  }

  static std::vector<size_t> previousPath;

  for (size_t node : previousPath) {
    if (std::find(currentPath.begin(), currentPath.end(), node) ==
            currentPath.end() &&
        node != start && node != end) {
      graph.getVertex(node)->setColor(NODE_COLOR);
      auto vertex = graph.getVertex(node);
      for (const auto &edge : vertex->getEdges()) {
        edge->setColor(EDGE_COLOR);
      }
    }
  }

  updatePathColors(currentPath);
  updateEndpointColors();

  previousPath = currentPath;

  std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
}

void PathfindingVisualizer::generateGraph(size_t vertices, Graph &graph) {
  std::atomic<bool> generationComplete(false);
  std::atomic<float> progress(0.0f);
  std::atomic<const char *> status("Initializing graph generation...");

  std::chrono::duration<double> elapsed_time;
  auto start_time = std::chrono::high_resolution_clock::now();

  std::thread genThread([&generationComplete, &graph, vertices, &elapsed_time,
                         start_time, &progress, &status]() {
    status.store("Setting up grid layout...");
    progress.store(0.1f);
    createRandomGraph(graph, vertices, &progress, &status);
    auto end_time = std::chrono::high_resolution_clock::now();
    elapsed_time = end_time - start_time;
    generationComplete.store(true);
  });

  while (!generationComplete && !shouldExit) {
    window.handleEvents();
    if (window.shouldClose()) {
      shouldExit = true;
      break;
    }
    window.drawLoadingScreen(status.load(), progress.load());
  }

  if (genThread.joinable()) {
    genThread.join();
  }
  window.invalidateStaticCache();

  std::cout << "  [DEBUG] Graph generation took " << elapsed_time.count() * 1000
            << " milliseconds for " << vertices << " vertices" << std::endl;
}

void PathfindingVisualizer::run(PathfindingAlgorithm &algorithm) {
  auto startTime = std::chrono::high_resolution_clock::now();
  int visitedNodes = 0;

  std::thread algorithmThread([&]() {
    algorithm.setVisualizationCallback(
        [this, &visitedNodes, &startTime,
         &algorithm](const std::vector<size_t> &path) {
          visitedNodes++;
          auto currentTime = std::chrono::high_resolution_clock::now();
          float elapsed =
              std::chrono::duration<float>(currentTime - startTime).count();

          window.setStats(algorithm.getName(), visitedNodes, elapsed);
          this->visualizePath(path);
        });
    algorithm.findPath(graph, start, end);
  });

  window.run();

  if (algorithmThread.joinable()) {
    algorithmThread.join();
  }
}

void PathfindingVisualizer::setPauseState(std::atomic<bool> *pauseState) {
  isPaused = pauseState;
  window.setPauseState(pauseState);
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