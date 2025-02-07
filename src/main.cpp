#include "PathfindingVisualizer.hpp"
#include "algorithms/AStar.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include <atomic>
#include <cstring>
#include <iostream>
#include <string>

void printUsage() {
  std::cout << "Usage: pathfinder [-a|--algorithm] <algorithm_name> "
               "[-s|--size] <vertices>\n";
  std::cout << "Available algorithms: dijkstra, astar\n";
  std::cout << "Vertices: positive integer (default: " << DEFAULT_VERTICES
            << ")\n";
}

int main(int argc, char *argv[]) {
  if (argc != 3 && argc != 5) {
    printUsage();
    return 1;
  }

  if (strcmp(argv[1], "-a") != 0 && strcmp(argv[1], "--algorithm") != 0) {
    printUsage();
    return 1;
  }

  PathfindingAlgorithm *algorithm = nullptr;
  std::string algoName = argv[2];

  if (algoName == "dijkstra") {
    algorithm = new Dijkstra();
  } else if (algoName == "astar") {
    algorithm = new AStar();
  } else {
    std::cout << "Unknown algorithm: " << algoName << std::endl;
    printUsage();
    return 1;
  }

  size_t vertices = DEFAULT_VERTICES;
  if (argc == 5) {
    if (strcmp(argv[3], "-s") != 0 && strcmp(argv[3], "--size") != 0) {
      printUsage();
      delete algorithm;
      return 1;
    }
    try {
      vertices = std::stoul(argv[4]);
      if (vertices == 0) {
        throw std::out_of_range("Vertices must be positive");
      }
    } catch (const std::exception &e) {
      std::cout << "Invalid vertices count: " << argv[4] << std::endl;
      delete algorithm;
      return 1;
    }
  }

  Graph graph;
  std::atomic<bool> shouldExit;
  std::atomic_init(&shouldExit, false);
  std::atomic<bool> isPaused;
  std::atomic_init(&isPaused, true);

  size_t start = 0;
  size_t end = vertices - 1;

  PathfindingVisualizer visualizer(graph, shouldExit, start, end);
  visualizer.setPauseState(&isPaused);
  visualizer.generateGraph(vertices, graph);
  visualizer.run(*algorithm);

  delete algorithm;
  return 0;
}