// ============================================================
// CLI-обёртка для тестирования игры
// ============================================================

#ifdef CLI_BUILD
#include "app.h"
#include "engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CLI обёртка: ввод из stdin, вывод в stdout ---
// Экспорты из app.c
extern gameState state;
extern void I(void);
extern void K(void);
extern void L(void);
extern void P(void);
extern void R(byte *canvas);

static byte canvas[BPP * CANVAS_SZ * CANVAS_SZ];

static void print_help(void) {
  printf("Позволяет тестировать логику игры через stdin/stdout\n");
  printf("Команды:\n");
  printf("  I         — инициализация\n");
  printf("  P         — один кадр физики\n");
  printf("  K         — нажатие кнопки\n");
  printf("  L         — отпускание кнопки\n");
  printf("  R         — рендер\n");
  printf("  S         — полный snapshot состояния игры\n");
  printf("  ?         — справка по командам\n");
  printf("  Q         — выход\n");
}

static void print_ok(void) { printf("OK timeMs=%d\n", state.timeMs); }

static void print_state(void) {
  // Замените на вашу структуру gameState
  printf("FPS=%d CANVAS_SZ=%d\n", GAME_FPS, CANVAS_SZ);
  printf("timeMs=%d\n", state.timeMs);
  printf("holding=%d\n", state.holding);
}

static void print_canvas(void) {
  printf("CANVAS_SZ=%d RGBA rows:\n", CANVAS_SZ);
  int idx;
  for (int j = 0; j < CANVAS_SZ; j++) {
    for (int i = 0; i < CANVAS_SZ; i++) {
      idx = j * CANVAS_SZ * BPP + i * BPP;
      printf("%02x%02x%02x%02x ", canvas[idx + 0], canvas[idx + 1], canvas[idx + 2], canvas[idx + 3]);
    }
    printf("\n");
  }
}

static void process_command(const char *line) {
  char cmd[16] = {0};
  int i;
  for (i = 0; i < 15 && line[i]; i++) {
    if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r')
      break;
    cmd[i] = line[i];
  }

  if (cmd[0] == 'I') {
    I();
    print_ok();
  } else if (cmd[0] == 'P') {
    P();
    print_ok();
  } else if (cmd[0] == 'K') {
    K();
    print_ok();
  } else if (cmd[0] == 'L') {
    L();
    print_ok();
  } else if (cmd[0] == 'R') {
    R(canvas);
    print_canvas();
    printf("END\n");
  } else if (cmd[0] == 'S') {
    print_state();
    printf("END\n");
  } else if (cmd[0] == '?') {
    print_help();
    printf("END\n");
  } else if (cmd[0] == 'Q') {
    printf("BYE\n");
    exit(0);
  } else {
    printf("ERR unknown command: %s\n", cmd);
  }
  fflush(stdout);
}

int main(void) {
  char line[256];
  while (fgets(line, sizeof(line), stdin)) {
    size_t len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }
    if (len == 0)
      continue;
    process_command(line);
  }
  return 0;
}

#endif
