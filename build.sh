#!/usr/bin/env sh

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


OP="GZipBase64"
if test ! -z "$1"; then
    OP="$1"
fi

echo "$OP"

"$FRAMEWORK_DIR/compile.sh" APP_MIN

case "$OP" in
"GZipBase64")
    OUT_HTML="$APP_BUILD"/app_Z.html
    OUT_MD2="$APP_BUILD"/app_Z.url.md
    node "$FRAMEWORK_DIR/tools/stringify.js" "$APP_BUILD"/app.wasm.gz.base64 >"$APP_BUILD"/main0.js || exit 1
    ;;
*)
    echo "??? $OP"
    exit 1
    ;;
esac

echo "=== APP build ==="
echo "Output: $OUT_HTML"
echo "Output: $OUT_MD2"

: >"$APP_BUILD"/main-$OP.js

# приписываем wasm к js:
head -n 1 "$APP_BUILD"/main0.js >"$APP_BUILD"/main-$OP.js || exit 1
tail -n+2 "$FRAMEWORK_DIR/engine/boot_tpl.js" >>"$APP_BUILD"/main-$OP.js || exit 1
tail -n 1 "$APP_BUILD"/main0.js >>"$APP_BUILD"/main-$OP.js || exit 1
rm "$APP_BUILD"/main0.js

# минимизируем js:
npx uglifyjs --toplevel --keep-fargs --rename "$APP_BUILD"/main-$OP.js >"$APP_BUILD"/main.u-$OP.js || exit 1
# uglifyjs зачем-то добавляет \r\n в конце
truncate -s -2 "$APP_BUILD"/main.u-$OP.js
npx regpack --reassignVars 0 "$APP_BUILD"/main.u-$OP.js | sed -e 's/^stats:.*$//g' >"$APP_BUILD"/main.z-$OP.js || exit 1

cp "$APP_BUILD"/main.z-$OP.js "$APP_BUILD"/main.z.js

# встраиваем js в html:
echo "MK: $OUT_HTML"
node "$FRAMEWORK_DIR/tools/inline.js" "$FRAMEWORK_DIR/engine/html_tpl.html" "$APP_BUILD" | tr -d '\r' | tr -d '\n' >"$OUT_HTML" || exit 1

# генерируем telegram markdown2
echo "MK: $OUT_MD2"
printf '```text\ndata:text/html,<script>' > "$OUT_MD2" || exit 1
echo '</script>' | cat "$APP_BUILD"/main.u-$OP.js - >> "$OUT_MD2" || exit 1
echo '```' >> "$OUT_MD2" || exit 1

# удаляем промежуточные вайлы
if test "$FW_BUILD_CLEANUP" = "yes" ; then
    rm -f "$APP_BUILD"/main.*.js
    rm -f "$APP_BUILD"/main-*.js
    rm -f "$APP_BUILD"/app.wasm.*
fi

echo OK
