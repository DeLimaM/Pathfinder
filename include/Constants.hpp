#pragma once
#include "raylib.h"

constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;
constexpr int TARGET_FPS = 60;
constexpr const char *WINDOW_TITLE = "Pathfinder";
const float WINDOW_PADDING = 50.0f;

constexpr Color BACKGROUND_COLOR = BLACK;
constexpr Color NODE_COLOR = GRAY;
constexpr Color EDGE_COLOR = DARKGRAY;
constexpr Color START_NODE_COLOR = GREEN;
constexpr Color END_NODE_COLOR = RED;
constexpr Color PATH_COLOR = YELLOW;

constexpr float BASE_NODE_RADIUS = 5.0f;
constexpr float START_END_NODE_RADIUS = 10.0f;
constexpr int SLEEP_TIME = 100;