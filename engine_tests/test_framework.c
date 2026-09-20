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
// Тесты ГСЧ (LCG)
// ============================================================

TEST(test_rng_seeded_sequence) {
  // Один и тот же seed → одинаковая последовательность
  simpleRngSeed(42);
  uint32_t a0 = simpleRng();
  uint32_t a1 = simpleRng();
  uint32_t a2 = simpleRng();

  simpleRngSeed(42);
  uint32_t b0 = simpleRng();
  uint32_t b1 = simpleRng();
  uint32_t b2 = simpleRng();

  TEST_ASSERT(a0 == b0, "seed(42) → первое значение совпадает");
  TEST_ASSERT(a1 == b1, "seed(42) → второе значение совпадает");
  TEST_ASSERT(a2 == b2, "seed(42) → третье значение совпадает");
}

TEST(test_rng_range_0_32767) {
  // simpleRng возвращает [0, 32767] (15 бит)
  for (int i = 0; i < 1000; i++) {
    uint32_t v = simpleRng();
    TEST_ASSERT(v <= 32767, "значение в диапазоне [0, 32767]");
  }
}

TEST(test_rng_different_seeds_different_values) {
  simpleRngSeed(1);
  uint32_t a = simpleRng();

  simpleRngSeed(2);
  uint32_t b = simpleRng();

  TEST_ASSERT(a != b, "разные seed дают разные значения");
}

TEST(test_rng_range_function) {
  simpleRngSeed(123);
  for (int i = 0; i < 500; i++) {
    uint32_t v = simpleRngRange(10);
    TEST_ASSERT(v < 10, "simpleRngRange(10) < 10");
    TEST_ASSERT(v >= 0, "simpleRngRange(10) >= 0");
  }
}

TEST(test_rng_range_max_1) {
  // max=1 → всегда 0
  simpleRngSeed(999);
  for (int i = 0; i < 100; i++) {
    uint32_t v = simpleRngRange(1);
    TEST_ASSERT(v == 0, "simpleRngRange(1) == 0");
  }
}

TEST(test_rng_range_max_2) {
  // max=2 → только 0 или 1
  simpleRngSeed(777);
  for (int i = 0; i < 200; i++) {
    uint32_t v = simpleRngRange(2);
    TEST_ASSERT(v == 0 || v == 1, "simpleRngRange(2) ∈ {0, 1}");
  }
}

TEST(test_rng_seed_reset_midstream) {
  simpleRngSeed(100);
  uint32_t x = simpleRng(); // 1-й
  uint32_t y = simpleRng(); // 2-й

  simpleRngSeed(100);
  uint32_t x2 = simpleRng(); // должен совпасть с x

  TEST_ASSERT(x == x2, "seed сбросил → первое значение совпало");
  TEST_ASSERT(y != x2, "второе значение из первой серии ≠ из второй");
}

TEST(test_rng_various_seeds) {
  // Проверка, что разные seed дают разные результаты
  uint32_t prev = 0;
  for (uint32_t s = 1; s <= 20; s++) {
    simpleRngSeed(s);
    uint32_t v = simpleRng();
    TEST_ASSERT(v != prev, "разные seed дают разные значения");
    prev = v;
  }
}

// ============================================================
// Запуск тестов
// ============================================================

int main(void) {
  printf("=== %s framework unit tests ===\n\n", "game");

  RUN_TEST(test_draw_pixel4);

  // RNG
  RUN_TEST(test_rng_seeded_sequence);
  RUN_TEST(test_rng_range_0_32767);
  RUN_TEST(test_rng_different_seeds_different_values);
  RUN_TEST(test_rng_range_function);
  RUN_TEST(test_rng_range_max_1);
  RUN_TEST(test_rng_range_max_2);
  RUN_TEST(test_rng_seed_reset_midstream);
  RUN_TEST(test_rng_various_seeds);

  printf("\n=== Results ===\n");
  printf("Run: %d\n", testsRun);
  printf("Asserts passed: %d\n", testsPassed);
  printf("Asserts failed: %d\n", testsFailed);

  return testsFailed > 0 ? 1 : 0;
}
