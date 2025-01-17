#include "visualization/Window.hpp"

Window::Window(int width, int height) : width(width), height(height) {
  InitWindow(width, height, "Pathfinder");
  SetTargetFPS(60);
}

Window::~Window() { CloseWindow(); }

void Window::run() {
  while (!WindowShouldClose()) {
    handleEvents();

    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
  }
}

void Window::handleEvents() {
  // TODO: Handle events
}

void Window::draw(const Graph &graph) {
  // TODO: Draw the graph
}