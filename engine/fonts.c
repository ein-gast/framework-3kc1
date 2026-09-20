#include "fonts.h"
#include <stddef.h>

// --- Текущий шрифт (устанавливается через setFont) ---
static const byte *(*currentFont)(int);
static int currentFontW;
static int currentFontH;

void setFont(fontFunc font, int fontLetterWidth, int fontLetterHeight) {
    currentFont = font;
    currentFontW = fontLetterWidth;
    currentFontH = fontLetterHeight;
}

// Рисовать символ (использует текущий шрифт)
void putChar(byte *canvas, int x, int y, int c, const col4 *col) {
    if (!currentFont) return;
    drawRaster(canvas, x, y, currentFont(c), currentFontW, currentFontH, col);
}

// Рисовать строку
void putStr(byte *canvas, int x, int y, const char *str, const col4 *col) {
    int cx = x;
    while (*str) {
        putChar(canvas, cx, y, (int)*(unsigned char *)str, col);
        cx += currentFontW;
        str++;
    }
}

// Рисовать число (поддержка отрицательных)
void putNumber(byte *canvas, int x, int y, int n, const col4 *col) {
    if (n < 0) {
        putChar(canvas, x, y, '-', col);
        x += currentFontW;
        n = -n;
    }
    // Собираем цифры в буфер (реверс)
    char buf[16];
    int idx = 0;
    do {
        buf[idx++] = (char)('0' + (n % 10));
        n /= 10;
    } while (n > 0);
    // Выводим в обратном порядке
    for (int i = idx - 1; i >= 0; i--) {
        putChar(canvas, x, y, buf[i], col);
        x += currentFontW;
    }
}


// --- fontD1 - шрифт с цифрами ---
// символ " "
static const byte fontD1_empty[FONT_D1_W * FONT_D1_H];

// символ "0"
const byte fontD1_0[FONT_D1_W * FONT_D1_H] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    1, 0, 0, 1,
    1, 0, 0, 1,
    0, 1, 1, 0,
};
// символ "1"
const byte fontD1_1[FONT_D1_W * FONT_D1_H] = {
    0, 0, 1, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 1, 0,
    0, 0, 1, 0,
};
// символ "2"
const byte fontD1_2[FONT_D1_W * FONT_D1_H] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    0, 0, 1, 0,
    0, 1, 0, 0,
    1, 1, 1, 1,
};
// символ "3"
const byte fontD1_3[FONT_D1_W * FONT_D1_H] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    0, 0, 1, 0,
    1, 0, 0, 1,
    0, 1, 1, 0,
};
// символ "4"
const byte fontD1_4[FONT_D1_W * FONT_D1_H] = {
    0, 0, 1, 1,
    0, 1, 0, 1,
    1, 0, 0, 1,
    1, 1, 1, 1,
    0, 0, 0, 1,
};
// символ "5"
const byte fontD1_5[FONT_D1_W * FONT_D1_H] = {
    1, 1, 1, 1,
    1, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 1,
    1, 1, 1, 1,
};
// символ "6"
const byte fontD1_6[FONT_D1_W * FONT_D1_H] = {
    0, 1, 1, 1,
    1, 0, 0, 0,
    1, 1, 1, 0,
    1, 0, 0, 1,
    0, 1, 1, 0,
};
// символ "7"
const byte fontD1_7[FONT_D1_W * FONT_D1_H] = {
    1, 1, 1, 1,
    0, 0, 0, 1,
    0, 0, 1, 1,
    0, 0, 1, 0,
    0, 0, 1, 0,
};
// символ "8"
const byte fontD1_8[FONT_D1_W * FONT_D1_H] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    0, 1, 1, 0,
    1, 0, 0, 1,
    0, 1, 1, 0,
};
// символ "9"
const byte fontD1_9[FONT_D1_W * FONT_D1_H] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    0, 1, 1, 1,
    0, 0, 0, 1,
    1, 1, 1, 0,
};
// символ "-"
const byte fontD1_minus[FONT_D1_W * FONT_D1_H] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};
// символ ":"
const byte fontD1_colon[FONT_D1_W * FONT_D1_H] = {
    0, 0, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,
};

// fontD1Raster() - получить растр шрифта D1
const byte *fontD1Raster(int letter) {
    switch(letter) {
        case '0':
            return  fontD1_0;
        case '1':
            return  fontD1_1;
        case '2':
            return  fontD1_2;
        case '3':
            return  fontD1_3;
        case '4':
            return  fontD1_4;
        case '5':
            return  fontD1_5;
        case '6':
            return  fontD1_6;
        case '7':
            return  fontD1_7;
        case '8':
            return  fontD1_8;
        case '9':
            return  fontD1_9;
        case '-':
            return  fontD1_minus;
        case ':':
            return  fontD1_colon;
    }
    return fontD1_empty;
}
