#pragma once
#include "../graph/Graph.hpp"
#include "./raylib.h"
#include "Constants.hpp"
#include <atomic>

class Window {
public:
  Window(const Graph &graph, std::atomic<bool> &shouldExit,
         int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT,
         const char *title = WINDOW_TITLE);
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
  void setPauseState(std::atomic<bool> *pauseState) { isPaused = pauseState; }
  void invalidateStaticCache() { needsStaticRedraw = true; }
  void drawLoadingScreen(const char *status, float progress);

private:
  Vector2 calculateBounds(const Graph &graph, Vector2 &min, Vector2 &max);
  float calculateScale(const Vector2 &min, const Vector2 &max);
  void drawEdges(const Graph &graph, float scale, float offsetX, float offsetY,
                 Color filterColor);
  void drawVertices(const Graph &graph, float scale, float offsetX,
                    float offsetY);
  Vector2 transformPosition(const Vector2 &pos, float scale, float offsetX,
                            float offsetY);
  void drawStaticElements(float scale, float offsetX, float offsetY);
  void drawPathVertices(const Graph &graph, float scale, float offsetX,
                        float offsetY);
  void drawProgressBar(const char *status, float progress);

private:
  int width;
  int height;
  const Graph &graph;
  std::atomic<bool> &shouldExit;
  std::atomic<bool> *isPaused;
  RenderTexture2D staticBackground;
  bool needsStaticRedraw;
};