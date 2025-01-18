#include "algorithms/BruteForce.hpp"

void BruteForce::exploreNode(const Graph &graph, size_t current, size_t end,
                             std::vector<bool> &visited,
                             std::vector<size_t> &currentPath,
                             std::vector<size_t> &shortestPath) {
  visited[current] = true;
  currentPath.push_back(current);

  if (visualCallback)
    visualCallback(currentPath);

  if (current == end) {
    updateShortestPath(currentPath, shortestPath);
  } else {
    exploreNeighbors(graph, current, end, visited, currentPath, shortestPath);
  }

  visited[current] = false;
  currentPath.pop_back();
}

void BruteForce::exploreNeighbors(const Graph &graph, size_t current,
                                  size_t end, std::vector<bool> &visited,
                                  std::vector<size_t> &currentPath,
                                  std::vector<size_t> &shortestPath) {
  auto currentVertex = graph.getVertex(current);
  for (const auto &edge : currentVertex->getEdges()) {
    auto neighborVertex = edge->getOtherVertex(currentVertex);
    auto vertices = graph.getVertices();
    auto it = std::find(vertices.begin(), vertices.end(), neighborVertex);
    if (it != vertices.end()) {
      size_t neighborIndex = std::distance(vertices.begin(), it);
      if (!visited[neighborIndex]) {
        exploreNode(graph, neighborIndex, end, visited, currentPath,
                    shortestPath);
      }
    }
  }
}

void BruteForce::updateShortestPath(const std::vector<size_t> &currentPath,
                                    std::vector<size_t> &shortestPath) {
  if (shortestPath.empty() || currentPath.size() < shortestPath.size()) {
    shortestPath = currentPath;
  }
}

std::vector<size_t> BruteForce::findPath(const Graph &graph, size_t start,
                                         size_t end) {
  if (start == end) {
    return {start};
  }

  std::vector<bool> visited(graph.getVertices().size(), false);
  std::vector<size_t> currentPath;
  std::vector<size_t> shortestPath;

  exploreNode(graph, start, end, visited, currentPath, shortestPath);

  return shortestPath;
}