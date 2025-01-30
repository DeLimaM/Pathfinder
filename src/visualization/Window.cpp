#include "visualization/Window.hpp"
#include "graph/Graph.hpp"
#include <cstddef>

Window::Window(const Graph &graph, std::atomic<bool> &shouldExit, int width,
               int height, const char *title)
    : width(width), height(height), graph(graph), shouldExit(shouldExit) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
  InitWindow(width, height, title);
  SetTargetFPS(TARGET_FPS);

  staticBackground = LoadRenderTexture(width, height);
  needsStaticRedraw = true;
}

Window::~Window() {
  UnloadRenderTexture(staticBackground);
  CloseWindow();
}

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

    UnloadRenderTexture(staticBackground);
    staticBackground = LoadRenderTexture(width, height);
    needsStaticRedraw = true;
  }

  if (IsKeyPressed(KEY_SPACE)) {
    *isPaused = !(*isPaused);
  }
}

void Window::draw(const Graph &graph) {
  if (graph.getVertices().empty())
    return;

  Vector2 min, max;
  calculateBounds(graph, min, max);
  float scale = calculateScale(min, max);
  float offsetX = (width - (max.x - min.x) * scale) / 2.0f - min.x * scale;
  float offsetY = (height - (max.y - min.y) * scale) / 2.0f - min.y * scale;

  drawStaticElements(scale, offsetX, offsetY);
  Rectangle sourceRec = {0.0f, 0.0f, (float)width, (float)-height};
  Vector2 position = {0.0f, 0.0f};
  DrawTextureRec(staticBackground.texture, sourceRec, position, WHITE);

  BeginBlendMode(BLEND_ALPHA);
  drawEdges(graph, scale, offsetX, offsetY, PATH_COLOR);
  drawPathVertices(graph, scale, offsetX, offsetY);
  EndBlendMode();

  DrawFPS(10, 10);
}

Vector2 Window::calculateBounds(const Graph &graph, Vector2 &min,
                                Vector2 &max) {
  min = graph.getVertex(0)->getPosition();
  max = min;
  for (const auto &vertex : graph.getVertices()) {
    const Vector2 &pos = vertex->getPosition();
    min.x = std::min(min.x, pos.x);
    min.y = std::min(min.y, pos.y);
    max.x = std::max(max.x, pos.x);
    max.y = std::max(max.y, pos.y);
  }
  return max;
}

float Window::calculateScale(const Vector2 &min, const Vector2 &max) {
  float scaleX = (width - 2 * WINDOW_PADDING) / (max.x - min.x);
  float scaleY = (height - 2 * WINDOW_PADDING) / (max.y - min.y);
  return std::min(scaleX, scaleY);
}

Vector2 Window::transformPosition(const Vector2 &pos, float scale,
                                  float offsetX, float offsetY) {
  return Vector2{pos.x * scale + offsetX, pos.y * scale + offsetY};
}

void Window::drawEdges(const Graph &graph, float scale, float offsetX,
                       float offsetY, Color filterColor) {
  for (const auto &vertex : graph.getVertices()) {
    Vector2 pos1 =
        transformPosition(vertex->getPosition(), scale, offsetX, offsetY);

    for (const auto &edge : vertex->getEdges()) {
      if (edge->getColor().r == filterColor.r &&
          edge->getColor().g == filterColor.g &&
          edge->getColor().b == filterColor.b) {
        auto neighbor = edge->getOtherVertex(vertex);
        if (neighbor) {
          Vector2 pos2 = transformPosition(neighbor->getPosition(), scale,
                                           offsetX, offsetY);
          DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, edge->getColor());
        }
      }
    }
  }
}

void Window::drawVertices(const Graph &graph, float scale, float offsetX,
                          float offsetY) {
  for (const auto &vertex : graph.getVertices()) {
    Vector2 pos =
        transformPosition(vertex->getPosition(), scale, offsetX, offsetY);
    float radius =
        ColorToInt(vertex->getColor()) == ColorToInt(START_NODE_COLOR) ||
                ColorToInt(vertex->getColor()) == ColorToInt(END_NODE_COLOR)
            ? START_END_NODE_RADIUS
            : BASE_NODE_RADIUS;
    DrawCircle(pos.x, pos.y, radius, vertex->getColor());
  }
}

void Window::drawStaticElements(float scale, float offsetX, float offsetY) {
  if (!needsStaticRedraw)
    return;

  BeginTextureMode(staticBackground);
  ClearBackground(BACKGROUND_COLOR);

  drawEdges(graph, scale, offsetX, offsetY, EDGE_COLOR);

  for (const auto &vertex : graph.getVertices()) {
    Vector2 pos =
        transformPosition(vertex->getPosition(), scale, offsetX, offsetY);
    Color color = vertex->getColor();

    if (ColorToInt(color) != ColorToInt(PATH_COLOR)) {
      float radius = (ColorToInt(color) == ColorToInt(START_NODE_COLOR) ||
                      ColorToInt(color) == ColorToInt(END_NODE_COLOR))
                         ? START_END_NODE_RADIUS
                         : BASE_NODE_RADIUS;

      DrawCircle(pos.x, pos.y, radius, color);
    }
  }
  EndTextureMode();

  needsStaticRedraw = false;
}

void Window::drawPathVertices(const Graph &graph, float scale, float offsetX,
                              float offsetY) {
  for (const auto &vertex : graph.getVertices()) {
    if (ColorToInt(vertex->getColor()) == ColorToInt(PATH_COLOR)) {
      Vector2 pos =
          transformPosition(vertex->getPosition(), scale, offsetX, offsetY);
      DrawCircle(pos.x, pos.y, BASE_NODE_RADIUS, PATH_COLOR);
    }
  }
}