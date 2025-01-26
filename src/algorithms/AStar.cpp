#include "algorithms/AStar.hpp"
#include <cmath>
#include <limits>
#include <queue>

float heuristic(const Vector2 &a, const Vector2 &b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<size_t> AStar::findPath(const Graph &graph, size_t start,
                                    size_t end) {
  const size_t V = graph.getVertices().size();
  std::vector<float> gScore(V, std::numeric_limits<float>::max());
  std::vector<float> fScore(V, std::numeric_limits<float>::max());
  std::vector<size_t> previous(V, V);
  std::vector<bool> visited(V, false);

  using Node = std::pair<float, size_t>;
  std::priority_queue<Node, std::vector<Node>, std::greater<>> openSet;

  gScore[start] = 0;
  fScore[start] = heuristic(graph.getVertex(start)->getPosition(),
                            graph.getVertex(end)->getPosition());
  openSet.push({fScore[start], start});

  while (!openSet.empty()) {
    size_t current = openSet.top().second;
    openSet.pop();

    if (visited[current])
      continue;
    visited[current] = true;

    std::vector<size_t> currentPath;
    if (current != start) {
      for (size_t at = current; at != V; at = previous[at]) {
        currentPath.insert(currentPath.begin(), at);
      }
      if (visualCallback) {
        visualCallback(currentPath);
      }
    }

    if (current == end)
      break;

    auto currentVertex = graph.getVertex(current);
    for (const auto &edge : currentVertex->getEdges()) {
      auto neighbor = edge->getOtherVertex(currentVertex);
      size_t neighborIndex = std::find(graph.getVertices().begin(),
                                       graph.getVertices().end(), neighbor) -
                             graph.getVertices().begin();

      if (!visited[neighborIndex]) {
        float tentative_gScore = gScore[current] + 1;

        if (tentative_gScore < gScore[neighborIndex]) {
          previous[neighborIndex] = current;
          gScore[neighborIndex] = tentative_gScore;
          fScore[neighborIndex] =
              gScore[neighborIndex] +
              heuristic(neighbor->getPosition(),
                        graph.getVertex(end)->getPosition());
          openSet.push({fScore[neighborIndex], neighborIndex});
        }
      }
    }
  }

  std::vector<size_t> path;
  if (gScore[end] != std::numeric_limits<float>::max()) {
    for (size_t at = end; at != V; at = previous[at]) {
      path.insert(path.begin(), at);
    }
  }

  return path;
}