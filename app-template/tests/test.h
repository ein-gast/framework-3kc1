#pragma once

#include<stdio.h>

extern int testsRun;
extern int testsPassed;
extern int testsFailed;

#define TEST(name) void name(void)
#define RUN_TEST(name)                                                         \
do {                                                                         \
  printf("TEST: %s\n", #name);                                               \
  name();                                                                    \
} while (0)

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

// TEST_ASSERT с форматированием сообщения
#define TEST_ASSERT_FMT(cond, fmt, ...) do { \
    testsRun++; \
    if (cond) { testsPassed++; } \
    else { testsFailed++; printf("FAIL: " fmt " (%s) at %s:%d\n", __VA_ARGS__, #cond, __FILE__, __LINE__); } \
} while(0)
