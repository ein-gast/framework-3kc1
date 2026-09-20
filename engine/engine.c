#include "engine.h"

// ============================================================
// Стандартные экспортируемые WASM-символы
// ============================================================

// --- exported F() = FPS ---
unsigned int F(void) { return GAME_FPS; }
// --- exported W() = CANVAS_SZ ---
unsigned int W(void) { return CANVAS_SZ; }

// ============================================================
// Утилиты рендеринга
// ============================================================

void drawPixel4(byte *canvas, int toX, int toY, const col4 *col) {
  if (col->a != 0 && toX >= 0 && toY >= 0 && toX < CANVAS_SZ && toY < CANVAS_SZ) {
    int ofs = toX + toY * CANVAS_SZ;
    ofs *= BPP;
    canvas[ofs + 0] = col->r;
    canvas[ofs + 1] = col->g;
    canvas[ofs + 2] = col->b;
    canvas[ofs + 3] = 255;
  }
}

void drawRaster(byte *canvas, int x, int y, const byte *rast, int w, int h,
                const col4 *col) {
  int py, px;
  for (py = 0; py < h; py++) {
    for (px = 0; px < w; px++) {
      if (rast[py * w + px]) {
        drawPixel4(canvas, x + px, y + py, col);
      }
    }
  }
}

void drawRoundedRect(byte *canvas, int x, int y, int w, int h, bool rounded,
                     const col4 *col) {
  int px, py;
  for (py = 0; py < h; py++) {
    for (px = 0; px < w; px++) {
      if (rounded) {
        if (px == 0 && py == 0)
          continue;
        if (px == w - 1 && py == 0)
          continue;
        if (px == 0 && py == h - 1)
          continue;
        if (px == w - 1 && py == h - 1)
          continue;
      }
      drawPixel4(canvas, x + px, y + py, col);
    }
  }
}

/*
void putChar(byte *canvas, int x, int y, char c, const col4 *pal) {
  // Placeholder — реализация зависит от шрифтов игры
  // Каждая игра определяет свои шрифты в app_assets.c
  // Этот хелпер переопределяется в app.c при необходимости
  (void)canvas;
  (void)x;
  (void)y;
  (void)c;
  (void)pal;
}

void putStr(byte *canvas, int x, int y, char const *str, byte span,
            const col4 *pal) {
  for (uint32_t i = 0; i < 100 && str[i] != 0; i++) {
    putChar(canvas, x, y, str[i], pal);
    x += FONT_CHAR_W + span;
  }
}

void putNumber(byte *canvas, int x, int y, int n, const col4 *pal) {
  if (n < 0) {
    putChar(canvas, x, y, '-', pal);
    n = -n;
  }
  x += FONT_CHAR_W;

  int digits[6] = {0, 0, 0, 0, 0, 0};
  int count = 0;
  if (n == 0)
    count = 1;
  while (n > 0) {
    digits[count++] = n % 10;
    n /= 10;
  }

  for (int i = count - 1; i >= 0; i--) {
    putChar(canvas, x, y, digits[i] + '0', pal);
    x += FONT_CHAR_W;
  }
}
*/

// ============================================================
// Генератор случайных чисел (LCG)
// ============================================================

static uint32_t _simpleRngState = 810101;

void simpleRngSeed(uint32_t seed) { _simpleRngState = seed; }

uint16_t simpleRng(void) {
  _simpleRngState = _simpleRngState * 1103515245u + 12345u;
  return (uint16_t)((_simpleRngState >> 16) & 0x7fff);
}

uint16_t simpleRngRange(uint16_t max) { return simpleRng() % max; }
