#pragma once
#include "engine.h"

#define FONT_D1_W (4)
#define FONT_D1_H (5) 

// fontFunc - тип функции, возвращающей по букве растр определённого шрифта
typedef const byte *fontFunc(int letter);

// Шрифт D1 (только цифры)
extern fontFunc fontD1Raster;

// Установить текущий шрифт
void setFont(fontFunc font, int fontLetterWidth, int fontLetterHeight);

// Рисовать символ
void putChar(byte *canvas, int x, int y, int c, const col4 *pal);

// Рисовать строку
void putStr(byte *canvas, int x, int y, const char *str, const col4 *pal);

// Рисовать число (поддержка отрицательных)
void putNumber(byte *canvas, int x, int y, int n, const col4 *pal);
