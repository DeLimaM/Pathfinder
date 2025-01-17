#pragma once
#include "../graph/Graph.hpp"
#include "raylib.h"

class Window {
public:
  Window(int width = 800, int height = 600);
  ~Window();
  void run();
  void draw(const Graph &graph);

private:
  int width;
  int height;
  void handleEvents();
};