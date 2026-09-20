// ============================================================
// Тесты для шаблонной мини-игры
// ============================================================

#include <stdbool.h>
#include <string.h>
#include "engine.h"
#include "test.h"
#include "../src/app.h"

// Глобальные переменные для отчёта
int testsRun = 0;
int testsPassed = 0;
int testsFailed = 0;

// Состояние экрана
static byte canvas[BPP * CANVAS_SZ * CANVAS_SZ];

// Вспомогательные фкнкции

byte *clearCanvas() {
  memset(canvas, 0, BPP * CANVAS_SZ * CANVAS_SZ);
  return canvas;
}
// Состояние приложения
extern gameState state;

// --- Тесты инициализации ---

TEST(test_init_ok) {
    I();
    TEST_ASSERT(state.timeMs == 0, "timeMs = 0");
    TEST_ASSERT(state.holding == 0, "holding = 0");
}

// --- Тесты ввода ---

TEST(test_k_sets_holding) {
    I();
    
    K();
    TEST_ASSERT(state.holding == true, "holding = true после K()");
}

TEST(test_l_releases_holding) {
    I();
    state.holding = true;

    L();
    TEST_ASSERT(state.holding == false, "holding = false после L()");
}

// --- Тесты таймеров ---

TEST(test_p_forwards_time) {
    I();

    P();
    TEST_ASSERT(state.timeMs > 0, "таймер увеличивается после P()");
}
