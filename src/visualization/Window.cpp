#include "visualization/Window.hpp"
#include "graph/Graph.hpp"
#include <cstddef>

Window::Window(int width, int height) : width(width), height(height) {
  InitWindow(width, height, WINDOW_TITLE);
  SetTargetFPS(TARGET_FPS);
}

Window::~Window() { CloseWindow(); }

void Window::run() {
  while (!WindowShouldClose()) {
    handleEvents();

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    EndDrawing();
  }
}

void Window::handleEvents() {
  int newWidth = GetScreenWidth();
  int newHeight = GetScreenHeight();

  if (newWidth != width || newHeight != height) {
    width = newWidth;
    height = newHeight;
  }
}

void Window::draw(const Graph &graph) {
  if (graph.getVertices().empty())
    return;

  Vector2 min = graph.getVertex(0).position;
  Vector2 max = min;
  for (const auto &vertex : graph.getVertices()) {
    min.x = std::min(min.x, vertex.position.x);
    min.y = std::min(min.y, vertex.position.y);
    max.x = std::max(max.x, vertex.position.x);
    max.y = std::max(max.y, vertex.position.y);
  }

  float scaleX = (width - 2 * PADDING) / (max.x - min.x);
  float scaleY = (height - 2 * PADDING) / (max.y - min.y);
  float scale = std::min(scaleX, scaleY);

  float offsetX = (width - (max.x - min.x) * scale) / 2.0f - min.x * scale;
  float offsetY = (height - (max.y - min.y) * scale) / 2.0f - min.y * scale;

  for (size_t i = 0; i < graph.getVertices().size(); i++) {
    const auto &vertex = graph.getVertex(i);
    Vector2 pos1 = {vertex.position.x * scale + offsetX,
                    vertex.position.y * scale + offsetY};

    for (size_t neighborId : vertex.neighbors) {
      if (neighborId > i) {
        const auto &neighbor = graph.getVertex(neighborId);
        Vector2 pos2 = {neighbor.position.x * scale + offsetX,
                        neighbor.position.y * scale + offsetY};
        DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, EDGE_COLOR);
      }
    }
  }

  for (const auto &vertex : graph.getVertices()) {
    Vector2 pos = {vertex.position.x * scale + offsetX,
                   vertex.position.y * scale + offsetY};
    DrawCircle(pos.x, pos.y, 5.0f, vertex.color);
  }
}
