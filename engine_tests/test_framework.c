// ============================================================
// Шаблон юнит-тестов фреймворка
// ============================================================

#include "engine.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Глобальные переменные для отчёта
static int testsRun = 0;
static int testsPassed = 0;
static int testsFailed = 0;

// Макросы для assertion
#define TEST_ASSERT(cond, msg)                                                 \
  do {                                                                         \
    testsRun++;                                                                \
    if (cond) {                                                                \
      testsPassed++;                                                           \
    } else {                                                                   \
      testsFailed++;                                                           \
      printf("FAIL: %s (%s) at %s:%d\n", msg, #cond, __FILE__, __LINE__);      \
    }                                                                          \
  } while (0)

#define TEST(name) void name(void)
#define RUN_TEST(name)                                                         \
  do {                                                                         \
    printf("TEST: %s\n", #name);                                               \
    name();                                                                    \
  } while (0)

// Состояние экрана
static byte canvas[BPP * CANVAS_SZ * CANVAS_SZ];

// Вспомогательные фкнкции

byte *clearCanvas() {
  memset(canvas, 0, BPP * CANVAS_SZ * CANVAS_SZ);
  return canvas;
}

col4 canvasAtXY(int x, int y) {
  col4 col;
  int index = y * BPP * CANVAS_SZ + x * BPP;
  col.r = canvas[index + 0];
  col.g = canvas[index + 1];
  col.b = canvas[index + 2];
  col.a = canvas[index + 3];
  return col;
}

bool isSameColor4(col4 a, col4 b) {
  return memcmp(a.v, b.v, 4 * sizeof(byte)) == 0;
}

// ============================================================
// Тесты рендеринга
// ============================================================

TEST(test_draw_pixel4) {
  col4 red = {255, 10, 10, 255};
  I();

  drawPixel4(clearCanvas(), 10, 20, &red);

  TEST_ASSERT(isSameColor4(red, canvasAtXY(10, 20)),
              "пиксель должен установиться в буфере");
  TEST_ASSERT(!isSameColor4(red, canvasAtXY(10 + 1, 20)),
              "соседний пиксель не должен установиться в буфере");
}

// ============================================================
// Запуск тестов
// ============================================================

int main(void) {
  printf("=== %s framework unit tests ===\n\n", "game");

  RUN_TEST(test_draw_pixel4);

  printf("\n=== Results ===\n");
  printf("Run: %d\n", testsRun);
  printf("Asserts passed: %d\n", testsPassed);
  printf("Asserts failed: %d\n", testsFailed);

  return testsFailed > 0 ? 1 : 0;
}
