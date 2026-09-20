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

## Testing Framework Changes

1. Copy `engine_tests/test_framework.c` to test new engine features
2. Run `./run_tests.sh` with `FW_TEST_FRAMEWORK=yes` in `build.env`
3. Tests compile as native x86-64 binary (not WASM)
4. Use `TEST_ASSERT(cond, "message")` for assertions

## Constraints

- **No external libraries** — everything must compile with clang alone
- **WASM32 target** — no pointers larger than 32-bit
- **Zero heap usage** — canvas buffer is stack-allocated (`byte canvas[150*150*4]`)
- **No floating point** — use fixed-point arithmetic (`FIXED`, `FIXED_SCALE=128`)
- **No C standard library** — no `malloc`, `printf`, etc. in WASM builds
- **CANVAS_SZ is compile-time constant** — don't assume specific size
- **FPS is compile-time constant** — don't assume specific frame rate
- **Do not** modify documentation files without direct request
