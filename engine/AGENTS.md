# Framework Development Instructions

## Scope

This directory contains instructions for modifying the **framework core** — files in `engine/`, `tools/`, `engine_tests/`, and build scripts. These instructions do NOT apply to app development.

## Core Files

### `engine/engine.h` — Public API

Defines the framework's public interface:
- **Types:** `byte`, `col3`, `col4`, `pal256`, `FIXED`
- **Constants:** `CANVAS_SZ`, `BPP`, `GAME_FPS`, `FRAME_MS`
- **Exported functions (app must implement):** `I()`, `K()`, `L()`, `P()`, `R()`, `F()`
- **Rendering utilities:** `drawPixel4()`, `drawRaster()`, `drawRoundedRect()`, `drawRect()`
- **RNG:** `simpleRng()`, `simpleRngRange()`, `simpleRngSeed()`

Rules:
- Do NOT change exported function signatures — apps depend on them
- New utilities can be added, but document them
- All constants use `#define` with units in comments

### `engine/engine.c` — Implementation

- Rendering utilities implementation
- LCG RNG
- Exported `F()` returns `GAME_FPS`, `W()` returns `CANVAS_SZ`

### `engine/boot.js` — Runtime (development)

- Initializes canvas, memory, animation loop
- Sets up input handlers (mouse/keyboard → `K()`/`L()`)
- Calls `P()` via `setInterval`, `R()` via `requestAnimationFrame`

### `engine/boot_tpl.js` — Runtime (production)

- Minimal WASM loader: instantiates WASM, extracts `js` string, evals it
- Extremely minimal — no canvas setup, no input handling
- The `js` export from WASM contains the full runtime code

### `engine/html_tpl.html` — HTML template

- Single line: `<script src="main.z.js"></script>`
- Everything is inlined into the final HTML by `inline.js`

## Build Pipeline

```
app.c + engine.c + assets.c
       │
       ├── clang (C23, -Oz) → app.wasm
       │                       │
       │                   small-wasm-trimmer
       │                       │
       │                   gzip + base64
       │                       │
       ├── stringify.js → main0.js
       ├── uglifyjs → main.u.js
       ├── regpack → main.z.js
       │                       │
       └── inline.js → app_Z.html
```

Key scripts:
- `compile.sh` — C → WASM + compression
- `build.sh` — JS minification + HTML inlining
- `compile_cli.sh` — native build for testing (no WASM)
- `run_tests.sh` — unit tests (native clang)

Build config: `build.env` — set `APP_SRC`, `APP_BUILD`, `APP_FPS`, `CLANG`

## TDD Workflow (RECOMMENDED)

**Write tests BEFORE implementation.** This is the preferred development methodology:

1. **Write tests first** — implement test functions that describe expected behavior
2. **Run tests** — they will fail (expected, this is "red")
3. **Implement** — write minimal code to make tests pass ("green")
4. **Refactor** — clean up code, keep tests passing

### Key principles

- **Tests are specifications** — they define what the code should do
- **Each test does one thing** — single responsibility, easy to debug
- **Failures are expected** — a failing test means you need to implement something
- **Run after every change** — `./run_tests.sh`
- **Cover edge cases** — boundaries, null states, extreme values

### Test naming convention

- **ALL tests** (main function): `test_framework.c` — one file per app
- **Unit tests** (component tests): `test_*.c` — one file per feature group

## Constraints

- **No external libraries** — everything must compile with clang alone
- **WASM32 target** — no pointers larger than 32-bit
- **Zero heap usage** — canvas buffer is stack-allocated (`byte canvas[150*150*4]`)
- **No floating point** — use fixed-point arithmetic (`FIXED`, `FIXED_SCALE=128`)
- **No C standard library** — no `malloc`, `printf`, etc. in WASM builds
- **CANVAS_SZ is compile-time constant** — don't assume specific size
- **FPS is compile-time constant** — don't assume specific frame rate
- **Do not** modify documentation files without direct request

## Debugging Tools

For debugging and investigation, use **coreutils** utilities (`cat`, `grep`, `awk`, `sed`, `head`, `tail`, `wc`, `sort`, `uniq`, `diff`, `xxd`, `od`, `hexdump`, `base64`, `gzip`, `bc`, `seq`, `yes`, `printf`, `env`, `test`, `expr`, `tr`, `cut`, `paste`, `join`, `comm`, `split`, `tee`, `timeout`, `stdbuf`, `shuf`, `od`, `xxd`).

**Do NOT use Python.** If you need a script, write it in:
- **C** — compile with `clang`, run the binary
- **Node.js** — without any external modules (only built-in `fs`, `path`, `crypto`, etc.)
