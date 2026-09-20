#pragma once

// --- Состояние игры ---
typedef struct {
  int timeMs;   // мс с начала игры
  bool holding; // флаг состояния "кнопки"
} gameState;
