#pragma once

#include <stdbool.h>
#include <stdint.h>

// ============================================================
// Canvas Engine — общие определения для canvas-игр
// ============================================================

typedef unsigned char byte;

// --- Цвета ---

typedef struct {
  union {
    byte v[3];
    struct {
      byte r, g, b;
    };
  };
} col3;

typedef struct {
  union {
    byte v[4];
    struct {
      byte r, g, b, a;
    };
  };
} col4;

// --- Размер экрана, px ---
#ifndef CANVAS_SZ
#define CANVAS_SZ (150)
#endif
// байт на пиксель
#define BPP (4)

// --- Шрифт ---
#ifndef FONT_CHAR_W
#define FONT_CHAR_W (5)
#endif
#ifndef FONT_CHAR_H
#define FONT_CHAR_H (5)
#endif

// --- Fixed-point арифметика ---
typedef int64_t FIXED;
#ifndef FIXED_SCALE
#define FIXED_SCALE (128)
#endif
#define fixedToInt(f) ((int32_t)(f)/FIXED_SCALE)
#define intToFixed(i) ( (FIXED)(i)*FIXED_SCALE)

// --- Палитра ---
typedef col4 pal256[256];

// --- FPS игры (30, 45 или 60) ---
#ifndef GAME_FPS
#define GAME_FPS  30
#endif

// длительность кадра, мс
#define FRAME_MS    (1000 / GAME_FPS)

// --- Макросы-хелперы ---
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// ============================================================
// Экспортируемые функции (обязательные для реализации в app.c)
// ============================================================

// I() — инициализация игры (вызывается один раз)
extern void I(void);

// K() — нажатие клавиши / touch down
extern void K(void);

// L() — отпускание клавиши / touch up
extern void L(void);

// P() — один кадр физики (вызывать по setInterval)
extern void P(void);

// R() — рендеринг кадра (requestAnimationFrame)
extern void R(byte *canvas);

// F() — возвращает FPS из компиляции
extern unsigned int F(void);

// W() — возвращает ширину canvas который нужно создать
extern unsigned int W(void);

// ============================================================
// Утилиты рендеринга (engine.c)
// ============================================================

// Рисовать пиксель (проверка границ)
void drawPixel4(byte *canvas, int toX, int toY, const col4 *col);

// Рисовать одноцветный растр (bitmap) 1 байт = 1 пиксель
void drawRaster(byte *canvas, int x, int y, const byte *rast, int w, int h,
                const col4 *col);

// Рисовать прямоугольник
#define drawRect(canvas, x, y, w, h,  col4l) drawRoundedRect((canvas), (x), (y), (w), (h), false, (col4l))

// Рисовать скруглённый прямоугольник (1px скругление)
void drawRoundedRect(byte *canvas, int x, int y, int w, int h, bool rounded,
                     const col4 *col);

/*
// Рисовать символ (использует шрифт из app_assets)
void putChar(byte *canvas, int x, int y, char c, const col4 *pal);

// Рисовать строку
void putStr(byte *canvas, int x, int y, char const *str, byte span,
            const col4 *pal);

// Рисовать число (поддержка отрицательных)
void putNumber(byte *canvas, int x, int y, int n, const col4 *pal);
*/

// ============================================================
// Генератор случайных чисел (engine.c)
// ============================================================

// LCG RNG (0-32767)
uint16_t simpleRng(void);

// LCG RNG в диапазоне [0, max)
uint16_t simpleRngRange(uint16_t max);

// Инициализация RNG с seed
void simpleRngSeed(uint32_t seed);
