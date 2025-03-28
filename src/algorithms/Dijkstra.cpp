#include "algorithms/Dijkstra.hpp"
#include <limits>
#include <queue>

std::vector<size_t> Dijkstra::findPath(const Graph &graph, size_t start,
                                       size_t end) {
  const size_t V = graph.getVertices().size();
  std::vector<size_t> distances(V, std::numeric_limits<size_t>::max());
  std::vector<size_t> previous(V, V);
  std::vector<bool> visited(V, false);

  std::priority_queue<std::pair<size_t, size_t>,
                      std::vector<std::pair<size_t, size_t>>, std::greater<>>
      pq;

  distances[start] = 0;
  pq.push({0, start});

  while (!pq.empty()) {
    size_t current = pq.top().second;
    pq.pop();

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
        size_t newDist = distances[current] + 1;
        if (newDist < distances[neighborIndex]) {
          distances[neighborIndex] = newDist;
          previous[neighborIndex] = current;
          pq.push({newDist, neighborIndex});
        }
      }
    }
  }

  std::vector<size_t> path;
  if (distances[end] != std::numeric_limits<size_t>::max()) {
    for (size_t at = end; at != V; at = previous[at]) {
      path.insert(path.begin(), at);
    }
  }

  return path;
}