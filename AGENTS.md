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

## TDD Workflow (RECOMMENDED)

**Write tests BEFORE implementation.** This is the preferred development methodology:

1. **Plan** — create a test plan listing all scenarios
2. **Write tests first** — implement test functions that describe expected behavior
3. **Run tests** — they will fail (expected, this is "red")
4. **Implement** — write minimal code to make tests pass ("green")
5. **Refactor** — clean up code, keep tests passing

### Test naming convention

- **ALL tests** (main function): `test_app.c` or `test_framework.c` — one file per app
- **Unit tests** (component tests): `test_*.c` — one file per feature group

### Key principles

- **Tests are specifications** — they define what the code should do
- **Each test does one thing** — single responsibility, easy to debug
- **Failures are expected** — a failing test means you need to implement something
- **Run after every change** — `./run_tests.sh`
- **Cover edge cases** — boundaries, null states, extreme values
- **E2E tests catch integration bugs** — unit tests alone miss state-machine interactions

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

## Debugging Tools

For debugging and investigation, use **coreutils** utilities (`cat`, `grep`, `awk`, `sed`, `head`, `tail`, `wc`, `sort`, `uniq`, `diff`, `xxd`, `od`, `hexdump`, `base64`, `gzip`, `bc`, `seq`, `yes`, `printf`, `env`, `test`, `expr`, `tr`, `cut`, `paste`, `join`, `comm`, `split`, `tee`, `timeout`, `stdbuf`, `shuf`, `od`, `xxd`).

**Do NOT use Python.** If you need a script, write it in:
- **C** — compile with `clang`, run the binary
- **Node.js** — without any external modules (only built-in `fs`, `path`, `crypto`, etc.)
