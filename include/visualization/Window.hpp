#pragma once
#include "../graph/Graph.hpp"
#include "./raylib.h"
#include "Constants.hpp"

class Window {
public:
  Window(const Graph &graph, int width = DEFAULT_WINDOW_WIDTH,
         int height = DEFAULT_WINDOW_HEIGHT, const char *title = WINDOW_TITLE);
  ~Window();
  void run();
  void draw(const Graph &graph);
  bool shouldClose() const { return WindowShouldClose(); }
  void clear() {
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
  }
  void display() { EndDrawing(); }
  void handleEvents();
  void setTargetFPS(int fps) { SetTargetFPS(fps); }

private:
  int width;
  int height;
  const Graph &graph;
};