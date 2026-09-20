#!/usr/bin/env sh

# Определяем директорию фреймворка для корректных путей
FRAMEWORK_DIR="$(cd "$(dirname "$0")" && pwd)"
. "$FRAMEWORK_DIR/build.env" || exit 1

FRAMEWORK_TMP="$FRAMEWORK_DIR"/tmp

# Определяем пути к исходникам
if ! test -d "$APP_SRC/src"; then
    echo "ERROR: не найдена директория приложения APP_SRC=$APP_SRC/src"
    exit 1
fi

mkdir -p "$FRAMEWORK_TMP"
if ! test -d "$FRAMEWORK_TMP"; then
    echo "ERROR: не найден временный каталог FRAMEWORK_TMP=$FRAMEWORK_TMP"
    exit 1
fi

TESTS_SRC="$APP_SRC"/tests
FW_TESTS_SRC="$FRAMEWORK_DIR"/engine_tests

TEST_BIN="$FRAMEWORK_TMP"/app_tests.bin
FW_TEST_BIN="$FRAMEWORK_TMP"/fw_tests.bin

echo "=== BUILDING app unit tests ==="

# Компиляция unit-тестов приложения в нативную программу (не WASM)
"$CLANG" \
    --language=c --std=c23 -O2 \
    $CFLAGS \
    -D TEST_MODE \
    -I "$FRAMEWORK_DIR/engine" \
    -I "$APP_SRC" \
    -o "$TEST_BIN" \
    "$APP_SRC"/src/*.c \
    "$FRAMEWORK_DIR"/engine/*.c \
    "$TESTS_SRC"/*.c ||
    exit 1

# Запуск unit-тестов приложения
"$TEST_BIN"

if test "$FW_TEST_FRAMEWORK" = "yes"; then
    echo ""
    echo "=== BUILDING framework unit tests ==="
    # Компиляция unit-тестов фреймворка в нативную программу (не WASM)
    "$CLANG" \
        --language=c --std=c23 -O2 \
        $CFLAGS \
        -D TEST_MODE \
        -I "$FRAMEWORK_DIR/engine" \
        -I "$APP_SRC" \
        -o "$FW_TEST_BIN" \
        "$APP_SRC"/src/*.c \
        "$FRAMEWORK_DIR"/engine/*.c \
        "$FW_TESTS_SRC"/*.c ||
        exit 1

    # Запуск unit-тестов фреймворка
    "$FW_TEST_BIN"
fi

echo ""
echo "=== Done ==="
