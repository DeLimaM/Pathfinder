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
  bool shouldClose() const { return WindowShouldClose(); }
  void clear() {
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
  }
  void display() { EndDrawing(); }

private:
  int width;
  int height;
  void handleEvents();
};