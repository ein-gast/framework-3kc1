// ============================================================
// Тесты для шаблонной мини-игры
// ============================================================

#include <stdio.h>
#include <stdbool.h>
#include "engine.h"
#include "../src/app.h"

// Глобальные переменные для отчёта
static int testsRun = 0;
static int testsPassed = 0;
static int testsFailed = 0;

// Макросы для assertion
#define TEST_ASSERT(cond, msg) do { \
    testsRun++; \
    if (cond) { testsPassed++; } \
    else { testsFailed++; printf("FAIL: %s (%s) at %s:%d\n", msg, #cond, __FILE__, __LINE__); } \
} while(0)

#define TEST(name) void name(void)
#define RUN_TEST(name) do { \
    printf("TEST: %s\n", #name); \
    name(); \
} while(0)

// Состояние приложения
extern gameState state;
extern void I(void);
extern void K(void);
extern void L(void);
extern void P(void);

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

// ============================================================

int main(void) {
    printf("=== game unit tests ===\n\n");

    RUN_TEST(test_init_ok);
    RUN_TEST(test_k_sets_holding);
    RUN_TEST(test_l_releases_holding);
    RUN_TEST(test_p_forwards_time);

    printf("\n=== Results ===\n");
    printf("Run: %d\n", testsRun);
    printf("Asserts passed: %d\n", testsPassed);
    printf("Asserts failed: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}
