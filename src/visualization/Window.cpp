#include "visualization/Window.hpp"
#include "graph/Graph.hpp"
#include <cstddef>

Window::Window(const Graph &graph, std::atomic<bool> &shouldExit, int width,
               int height, const char *title)
    : width(width), height(height), graph(graph), shouldExit(shouldExit) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(width, height, title);
  SetTargetFPS(TARGET_FPS);
}

Window::~Window() { CloseWindow(); }

void Window::run() {
  while (!WindowShouldClose()) {
    handleEvents();

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    draw(graph);
    EndDrawing();
  }
  shouldExit = true;
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

  Vector2 min = graph.getVertex(0)->getPosition();
  Vector2 max = min;
  for (const auto &vertex : graph.getVertices()) {
    min.x = std::min(min.x, vertex->getPosition().x);
    min.y = std::min(min.y, vertex->getPosition().y);
    max.x = std::max(max.x, vertex->getPosition().x);
    max.y = std::max(max.y, vertex->getPosition().y);
  }

  float scaleX = (width - 2 * WINDOW_PADDING) / (max.x - min.x);
  float scaleY = (height - 2 * WINDOW_PADDING) / (max.y - min.y);
  float scale = std::min(scaleX, scaleY);

  float offsetX = (width - (max.x - min.x) * scale) / 2.0f - min.x * scale;
  float offsetY = (height - (max.y - min.y) * scale) / 2.0f - min.y * scale;

  for (const auto &vertex : graph.getVertices()) {
    Vector2 pos1 = {vertex->getPosition().x * scale + offsetX,
                    vertex->getPosition().y * scale + offsetY};

    for (const auto &edge : vertex->getEdges()) {
      auto neighbor = edge->getOtherVertex(vertex);
      if (neighbor) {
        Vector2 pos2 = {neighbor->getPosition().x * scale + offsetX,
                        neighbor->getPosition().y * scale + offsetY};
        DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, edge->getColor());
      }
    }
  }

  for (const auto &vertex : graph.getVertices()) {
    Vector2 pos = {vertex->getPosition().x * scale + offsetX,
                   vertex->getPosition().y * scale + offsetY};
    float radius =
        ColorToInt(vertex->getColor()) == ColorToInt(START_NODE_COLOR) ||
                ColorToInt(vertex->getColor()) == ColorToInt(END_NODE_COLOR)
            ? START_END_NODE_RADIUS
            : BASE_NODE_RADIUS;
    DrawCircle(pos.x, pos.y, radius, vertex->getColor());
  }
}