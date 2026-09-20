#!/usr/bin/env sh

# Сборка CLI-версии игры для тестирования
# Компилирует в нативную программу с интерфейсом stdin/stdout

# Определяем директорию фреймворка для корректных путей
FRAMEWORK_DIR="$(cd "$(dirname "$0")" && pwd)"
. "$FRAMEWORK_DIR/build.env" || exit 1
test -f "$FRAMEWORK_DIR/build.local.env" && . "$FRAMEWORK_DIR/build.local.env"

# Определяем пути к исходникам
if ! test -d "$APP_SRC/src"; then
    echo "ERROR: не найдена директория приложения APP_SRC=$APP_SRC/src"
    exit 1
fi

mkdir -p "$APP_BUILD"
if ! test -d "$APP_BUILD"; then
    echo "ERROR: не найден целевой каталог сборки APP_BUILD=$APP_BUILD"
    exit 1
fi

OUTPUT="$APP_BUILD/game_cli.bin"

echo "=== CLI build ==="
echo "Output: $OUTPUT"

# Компилируем в нативный код (не WASM!)
"$CLANG" \
    --language=c --std=c23 \
    $CFLAGS $DFLAGS -DGAME_FPS=$APP_FPS -DCANVAS_SZ=$APP_CANVAS \
    -D CLI_BUILD \
    -o "$OUTPUT" \
    "$APP_SRC"/src/*.c "$FRAMEWORK_DIR"/engine/*.c \
    -I "$FRAMEWORK_DIR/engine" || exit 1

echo "=== CLI compiled ==="
echo "?" | "$OUTPUT" || exit 1

echo "OK"
