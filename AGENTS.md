# Global Instructions

## Project Overview

**Framework 3kc1** — minimalist canvas game framework. C23 code compiles to WASM, bundled into a single HTML file (data URI). Optimized for extreme size reduction.

## Project Structure

```
├── engine/            # Framework core (engine.h, engine.c, boot.js, html_tpl.html)
├── tools/             # Build utilities (stringify.js, inline.js)
├── app-template/      # Example game → copy and modify for new apps
│   ├── src/           # app.c, app.h, assets.c/h, boot.js
│   └── tests/         # Unit tests
├── build.env          # Build config (APP_SRC, FPS, CLANG)
├── compile.sh         # C → WASM
├── build.sh           # Full build pipeline
├── compile_cli.sh     # CLI build for testing
└── run_tests.sh       # Unit tests
```

## Agent Instructions

Instructions are organized by context. Agents read instructions from the directory they are working in:

| Context | Instructions File | Purpose |
|---------|------------------|---------|
| `engine/`, `tools/`, build scripts | `engine/AGENTS.md` | Framework development |
| `app-template/`, `app-*` directories | `app-template/AGENTS.md` | App development |

## General Conventions

- **C:** C23, clang, no external libraries
- **Canvas:** RGBA, 4 bytes/pixel, 150×150 px by default (compile-time constant `CANVAS_SZ`)
- **FPS:** 30, 45, 60, etc. (compile-time constant `GAME_FPS`)
- **Timing:** milliseconds
- **Timestep:** fixed via `FRAME_MS`, computed from `GAME_FPS`
- **Naming:** camelCase for variables, PascalCase for types, ALL_CAPS for macros
- **Comments:** Russian or English, concise
- **Documentation:** Russian

## System Dependencies

- clang-20+ / wasm-ld-20+ (LLVM 20+)
- binaryen (wasm-opt)
- node + npm
- gzip, base64

Verify: `./fwinstall.sh`
