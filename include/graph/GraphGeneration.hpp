#pragma once
#include "../graph/Graph.hpp"
#include <cstddef>

void createRandomGraph(Graph &graph, size_t vertices,
                       std::atomic<float> *progress,
                       std::atomic<const char *> *status);