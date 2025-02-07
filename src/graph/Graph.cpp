#include "graph/Graph.hpp"
#include <algorithm>

size_t Graph::addVertex(Vector2 position) {
  auto vertex = std::make_shared<Vertex>(position);
  vertices.push_back(vertex);
  return vertices.size() - 1;
}

void Graph::addEdge(size_t from, size_t to) {
  if (from >= vertices.size() || to >= vertices.size())
    return;
  if (!hasEdge(from, to)) {
    auto edge = std::make_shared<Edge>(vertices[from], vertices[to]);
    edges.push_back(edge);
    vertices[from]->addEdge(edge);
    vertices[to]->addEdge(edge);
  }
}

void Graph::removeVertex(size_t id) {
  if (id >= vertices.size())
    return;

  auto vertex = vertices[id];

  edges.erase(std::remove_if(edges.begin(), edges.end(),
                             [vertex](const std::shared_ptr<Edge> &edge) {
                               return edge->getVertex1() == vertex ||
                                      edge->getVertex2() == vertex;
                             }),
              edges.end());

  vertices.erase(vertices.begin() + id);

  for (auto &v : vertices) {
    for (auto &e : v->getEdges()) {
      if (auto other = e->getOtherVertex(v)) {
        if (std::find(vertices.begin(), vertices.end(), other) ==
            vertices.end()) {
          v->removeEdge(e);
        }
      }
    }
  }
}

void Graph::removeEdge(size_t from, size_t to) {
  if (from >= vertices.size() || to >= vertices.size())
    return;

  edges.erase(
      std::remove_if(edges.begin(), edges.end(),
                     [this, from, to](const std::shared_ptr<Edge> &edge) {
                       auto v1 = edge->getVertex1();
                       auto v2 = edge->getVertex2();
                       return (v1 == vertices[from] && v2 == vertices[to]) ||
                              (v1 == vertices[to] && v2 == vertices[from]);
                     }),
      edges.end());

  vertices[from]->removeEdge(edges.back());
  vertices[to]->removeEdge(edges.back());
}

bool Graph::hasEdge(size_t from, size_t to) const {
  if (from >= vertices.size() || to >= vertices.size())
    return false;

  return std::any_of(edges.begin(), edges.end(),
                     [this, from, to](const std::shared_ptr<Edge> &edge) {
                       auto v1 = edge->getVertex1();
                       auto v2 = edge->getVertex2();
                       return (v1 == vertices[from] && v2 == vertices[to]) ||
                              (v1 == vertices[to] && v2 == vertices[from]);
                     });
}

void Graph::addVerticesBulk(const std::vector<Vector2> &positions) {
  vertices.reserve(vertices.size() + positions.size());
  for (const auto &pos : positions) {
    vertices.push_back(std::make_shared<Vertex>(pos));
  }

  if (!vertices.empty()) {
    vertices.front()->setColor(START_NODE_COLOR);
    vertices.back()->setColor(END_NODE_COLOR);
  }
}

void Graph::addEdgesBulk(
    const std::vector<std::pair<size_t, size_t>> &edgePairs) {
  edges.reserve(edges.size() + edgePairs.size());
  for (const auto &[from, to] : edgePairs) {
    auto edge = std::make_shared<Edge>(vertices[from], vertices[to]);
    edges.push_back(edge);
    vertices[from]->addEdge(edge);
    vertices[to]->addEdge(edge);
  }
}