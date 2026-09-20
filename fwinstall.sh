#!/usr/bin/env sh

echo "== Готовим фреймворк к разработке =="

FRAMEWORK_DIR="$(cd "$(dirname "$0")" && pwd)"
. "$FRAMEWORK_DIR/build.env" || exit 1
test -f "$FRAMEWORK_DIR/build.local.env" && . "$FRAMEWORK_DIR/build.local.env"

echo "Проверяем наличие необходимых утилит"

if ! command -v "$CLANG" >/dev/null; then
    echo "нужна команда $CLANG, установите clang"
    exit 1
fi

if ! command -v "$WLDD" >/dev/null; then
    echo "нужна команда $WLDD, установите lld"
    exit 1
fi

if ! command -v wasm-opt >/dev/null; then
    echo "нужна команда wasm-opt, установите binaryen"
    exit 1
fi

if ! command -v gzip >/dev/null; then
    echo "нужна команда gzip, установите gzip"
    exit 1
fi

if ! command -v base64 >/dev/null; then
    echo "нужна команда base64, установите base64"
    exit 1
fi

if ! command -v node >/dev/null; then
    echo "нужна команда node, установите node.js"
    exit 1
fi

if ! command -v npm >/dev/null; then
    echo "нужна команда npm, установите npm"
    exit 1
fi

echo ""
echo "== Скачиваем сторонние зависимости =="

mkdir -p "$FRAMEWORK_DIR"/tools/3d-party || exit 1

DEP_3DP_TRIMMER_C="$FRAMEWORK_DIR"/tools/3d-party/small-wasm-trimmer.c
DEP_3DP_TRIMMER_BIN="$FRAMEWORK_DIR"/tools/3d-party/small-wasm-trimmer.bin
DEP_3DP_TRIMMER_URL=https://raw.githubusercontent.com/NathanARoss/small-wasm-trimmer/refs/heads/master/small-wasm-trimmer.c

if test ! -f "$DEP_3DP_TRIMMER_C"; then
    echo "Скачиваем отсутствующий small-wasm-trimmer."
    curl "$DEP_3DP_TRIMMER_URL" -o "$DEP_3DP_TRIMMER_C" || exit 1
fi

if test ! -f "$DEP_3DP_TRIMMER_BIN"; then
    echo "Собираем отсутствующий small-wasm-trimmer."
    "$CLANG" -o "$DEP_3DP_TRIMMER_BIN" "$DEP_3DP_TRIMMER_C" || exit 1
fi

echo ""
echo "== Ставим модули NPM =="

npm ci || exit 1

echo "OK - фреймворк готов к разработке"
