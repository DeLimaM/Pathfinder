#pragma once
#include "../graph/Graph.hpp"
#include "./raylib.h"
#include "Constants.hpp"

class Window {
public:
  Window(int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT);
  ~Window();
  void run();
  void draw(const Graph &graph);

private:
  int width;
  int height;
  void handleEvents();
};