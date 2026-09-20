# App Development Instructions

## Scope

This directory contains instructions for developing **games** using the framework. You modify files in `app-template/src/` and `app-template/tests/` only. **Do NOT modify files outside this directory** — those are framework internals.

## Project Structure

```
app-template/
├── src/
│   ├── app.c          # Game logic (MODIFY THIS)
│   ├── app.h          # Game state typedef (MODIFY THIS)
│   ├── assets.c       # Colors, bitmaps (MODIFY THIS)
│   ├── assets.h       # Asset declarations (MODIFY THIS)
│   ├── boot.js        # JS runtime (DO NOT MODIFY)
│   └── app_cli.c      # CLI wrapper (DO NOT MODIFY)
└── tests/
    └── test_app.c     # Unit tests (MODIFY THIS)
```

## Game API

Implement these 6 exported functions in `app.c`:

```c
void I(void)          // Initialization — called once at startup
void K(void)          // Key pressed / touch down
void L(void)          // Key released / touch up
void P(void)          // Physics frame — called ~30/45/60 times/sec
void R(byte *canvas)  // Render frame — called via requestAnimationFrame
```

Rules:
- `I()` must initialize all game state
- `P()` updates game state (physics, AI, timers)
- `R()` draws to canvas — always clear screen first, then draw objects
- All functions are `void` — no parameters except `R()`
- State lives in global `gameState` struct

## Game State

Define your state in `app.h`:

```c
typedef struct {
    int score;
    int playerX;
    bool isRunning;
    // ... your fields
} gameState;
```

Then in `app.c`:

```c
gameState state;  // single global instance
```

Rules:
- State is a single global `gameState` struct
- Use `int` for positions, counters, scores
- Use `bool` for flags
- Use `FIXED` for sub-pixel fractional values (fixed-point arithmetic) if needed

## Assets

Define colors and bitmaps in `assets.c` / `assets.h`:

```c
// assets.h
extern const col4 colBg;
extern const col4 colMain;
extern const byte rastBar[RAST_BAR_H][RAST_BAR_W];

// assets.c
const col4 colBg = {15, 15, 25, 255};  // dark blue background
const col4 colMain = {255, 255, 255, 255};  // white
const byte rastBar[8][20] = {  // 8px tall, 20px wide
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    // ...
};
```

Rules:
- `col4` = `{r, g, b, a}` each 0–255
- Bitmaps are 2D arrays: `byte[height][width]`
- `0` = transparent, non-zero = use `col4` color
- Keep bitmaps small (≤ 64×64 px)

## Rendering

Canvas size is square `CANVAS_SZ * CANVAS_SZ` defined at compile-time.

Available utilities (from `engine.h`):

```c
drawPixel4(canvas, x, y, &color);
drawRaster(canvas, x, y, rast, w, h, &color);
drawRoundedRect(canvas, x, y, w, h, true, &color);
drawRect(canvas, x, y, w, h, &color);
```

Rules:
- Always clear screen first in `R()` (fill with background color)
- Draw order = z-order (later draws appear on top)
- All coordinates are in pixels (0–149)
- Check bounds before drawing directly to canvas
- Prefer engine utilities to draw, they check bounds

## Fixed-Point Arithmetic

For fractional values, use fixed-point (no floating point):

```c
FIXED value = intToFixed(5);   // 5 * 128 = 640
int i = fixedToInt(value);     // 640 / 128 = 5

// Operations:
FIXED sum = fx1 + fx2;
FIXED diff = fx1 - fx2;
```
DO NOT multyply FIXED-type values, only FIXED to int:
```c
FIXED value = intToFixed(5);
value = value * 3;
```

Keep in mind that `FIXED_SCALE` constant can be redefined.

## TDD Workflow (RECOMMENDED)

**Write tests BEFORE implementation.** This is the preferred development methodology:

1. **Plan** — create a test plan in `.gigacode_vsc/plans/` listing all scenarios
2. **Write tests first** — implement test functions that describe expected behavior
3. **Run tests** — they will fail (expected, this is "red")
4. **Implement** — write minimal code to make tests pass ("green")
5. **Refactor** — clean up code, keep tests passing

### Test naming convention

- **ALL tests** (main function): `test_app.c` — one file per app
- **Unit tests** (component tests): `test_*.c` — one file per feature group
- **E2E tests** (complex game-specific scenarios): `e2e_*.c` — one file per scenario, for example:
  - `e2e_movement.c` — movement
  - `e2e_prize_pickup.c` — pickup a prize
  - `e2e_gamover.c` — gamover conditions

### Example TDD cycle

 1. Write the test (describes expected behavior)
 2. Run: FAILS (red)
 3. Implement feature
 4. Run: PASSES (green)

### Key principles

- **Tests are specifications** — they define what the code should do
- **Each test does one thing** — single responsibility, easy to debug
- **Failures are expected** — a failing test means you need to implement something
- **Run after every change** — `./run_tests.sh`
- **Cover edge cases** — boundaries, null states, extreme values
- **E2E tests catch integration bugs** — unit tests alone miss state-machine interactions

## Testing

### Unit Tests

Write tests in `app-template/tests/test_*.c`:

```c
// test_app.c

TEST(test_name) {
    I();                    // Initialize
    state.score = 100;      // Arrange
    P();                    // Act
    TEST_ASSERT(state.score >= 0, "score non-negative");  // Assert
}
```

Run: `./run_tests.sh`

### CLI Testing

Build and test interactively:

```bash
./compile_cli.sh
echo -e "I\nP\nP\nK\nS\nQ" | ./app-template/build/game_cli.bin
```

Commands: `I` (init), `P` (physics), `K` (key down), `L` (key up), `R` (render), `S` (state snapshot), `Q` (quit)

## Build & Deploy

```bash
# Full build → HTML + data URI
./build.sh

# Output in APP_BUILD/:
#   app_Z.html        — standalone HTML
#   app_Z.url.md      — Telegram markdown with data URI
```

Configuration in `build.env`:
- `APP_SRC` — path to app source directory
- `APP_BUILD` — path to build output directory
- `APP_FPS` — 30, 45, or 60
- `APP_CANVAS` — screen size (default 150)

## Constraints

- **No external libraries** — only framework-provided functions
- **No `malloc`/`free`** — all memory is stack-allocated or static
- **No floating point** — use fixed-point arithmetic
- **No C standard library** in WASM builds (no `stdio.h`, etc.)
- **CANVAS_SZ is compile-time constant** — don't assume specific size
- **FPS is compile-time constant** — don't assume specific frame rate
- **C23 only** — use `#embed` for resource embedding (clang 20+)

## Debugging Tools

For debugging and investigation, use **coreutils** utilities (`cat`, `grep`, `awk`, `sed`, `head`, `tail`, `wc`, `sort`, `uniq`, `diff`, `xxd`, `od`, `hexdump`, `base64`, `gzip`, `bc`, `seq`, `yes`, `printf`, `env`, `test`, `expr`, `tr`, `cut`, `paste`, `join`, `comm`, `split`, `tee`, `timeout`, `stdbuf`, `shuf`, `od`, `xxd`).

**Do NOT use Python.** If you need a script, write it in:
- **C** — compile with `clang`, run the binary
- **Node.js** — without any external modules (only built-in `fs`, `path`, `crypto`, etc.)
