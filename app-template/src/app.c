#include "app.h"
#include "assets.h"
#include "engine.h"
#include "fonts.h"
#include <stdint.h>

gameState state;

// --- exported I() = init() ---
void I() {
  state.holding = false;
  state.timeMs = 0L;
  setFont(fontD1Raster, FONT_D1_W, FONT_D1_H);
}

// --- exported K() = keyInput() ---
void K() {
  state.holding = true;
}

// --- exported L() = release() ---
void L() {
  state.holding = false;
}

// --- exported P() = process() ---
void P() {
  state.timeMs += FRAME_MS;
}

// --- exported R() = renderFrame() ---
void R(byte *canvas) {
  // очистка экрана
  drawRoundedRect(canvas, 0, 0, CANVAS_SZ, CANVAS_SZ, false, &colBg);

  // отображение текущей секунды слева сверху
  int seconds = state.timeMs / 1000;
  putNumber(canvas, FONT_D1_H, FONT_D1_H, seconds, &colMain);

  // отрисовка растра
  const col4 *activeColor;
  if(state.holding) {
    activeColor = &colSecond; // при нажатой кнопке подсвечивать
  } else {
    activeColor = &colMain;
  }

  drawRaster(
    canvas, 
    (CANVAS_SZ-RAST_BAR_W)/2, 
    (CANVAS_SZ-RAST_BAR_H)/2, 
    (byte*)rastBar, 
    RAST_BAR_W,
    RAST_BAR_H,
    activeColor);
}
