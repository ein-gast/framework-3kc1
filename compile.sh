#!/usr/bin/env sh

# Сборка WASM-версии игры для тестирования
# Предварительный этап перед build.sh

# Определяем директорию фреймворкка для корректных путей
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

# Минифицированную версию boot.js при компиляции
if test "$1" = "APP_MIN"; then
    CFLAGS="$CFLAGS -D APP_MIN_JS"
    rm "$APP_SRC/src/boot.min.js" 2> /dev/null
    npx uglifyjs --toplevel --rename "$APP_SRC/src/boot.js" > "$APP_SRC/src/boot.min.js" || exit 1
    echo "$1"
fi

DEP_3DP_TRIMMER_BIN="$FRAMEWORK_DIR"/tools/3d-party/small-wasm-trimmer.bin

OUTPUT="$APP_BUILD"/app.wasm
OUTB64="$APP_BUILD"/app.wasm.base64
OUT_GZ="$APP_BUILD"/app.wasm.gz.base64

echo "=== WASM build ==="
echo "Output: $OUTPUT"
echo "Output: $OUTB64"
echo "Output: $OUT_GZ"

"$CLANG" --target="$WARCH" --language=c --std=c23 -Oz --no-standard-libraries \
    $CFLAGS $DFLAGS -DGAME_FPS=$APP_FPS -DCANVAS_SZ=$APP_CANVAS \
    -Wl,--export=I -Wl,--export=K -Wl,--export=L -Wl,--export=P -Wl,--export=R \
    -Wl,--export=F -Wl,--export=W -Wl,--export=__heap_base -Wl,--export=js\
    -Wl,--import-undefined \
    -Wl,--strip-all \
    -Wl,--no-entry \
    -I "$FRAMEWORK_DIR/engine" \
    -o "$OUTPUT" \
    "$APP_SRC"/src/*.c "$FRAMEWORK_DIR"/engine/*.c \
    || exit 1

chmod -x "$OUTPUT"

mv "$OUTPUT" "$OUTPUT"~ || exit 1
"$DEP_3DP_TRIMMER_BIN" --remove-sections producers < "$OUTPUT"~ > "$OUTPUT" || exit 1
rm "$OUTPUT"~

gzip -nm -kf "$OUTPUT" || exit 1 # x.wasm -> x.wasm.gz
base64 -w 0 "$OUTPUT" > "$OUTB64"  || exit 1
base64 -w 0 "$OUTPUT".gz > "$OUT_GZ"  || exit 1

echo COMPILED
