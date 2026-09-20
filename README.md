# Framework 3kc1

Фреймворк для создания минималистичных canvas-игр. Основной код приложения Си, компилирукмый в WebAssembly, код обёртки - JavaScript.
Оптимизирован под экстремальное уменьшение размера дистрибутива (менее 4k) и распрастранение в виде data url-ов.

Название **Framework 3kc1** читается "три-ка си-один", ("three-ka si-one" по-английски).

Фреймврк создан из доработанного инструментария проектов [distance-21](https://github.com/ein-gast/distance-21) и [wamicro-38](https://github.com/ein-gast/wamicro-38)

## Возможности "среды"

Фреймворк предоставляет минималистичную среду. 

Дисплей всегда квадрвнтый и всегда 8-bit RGBA (как у HTML5 canvas). Для ввода доступно два события: кнопку нажали, кнопку отпустили. "Кнопкой" считается любая клавиша клавитауры, мыши или нажатие на тачскрин.

## Быстрый старт

### 1. Зависимости

```bash
./fwinstall.sh
```

### 2. Создание игры

```bash
# Скопируйте шаблон
cp -r app-template app-mygame

# Укажите путь в build.env
# APP_SRC="./app-mygame"
# APP_BUILD="./app-mygame/build"
```

### 3. Разработка

```bash
./run_tests.sh          # Unit-тесты (нативный clang, не WASM)
./compile_cli.sh        # CLI-версия для интерактивного тестирования, инструмент для ИИ-агентов
./build.sh              # Полная сборка → HTML + data URI
```

### 4. Результат

После `./build.sh` в `APP_BUILD`:

| Файл | Описание |
|------|----------|
| `app_Z.html` | Финальный HTML (всё встроено) |
| `app_Z.url.md` | Telegram markdown (base64 data URI) |
| `app.wasm` | Финальный WASM-файл игры |

Откройте в браузере `app_Z.html` или перешлите `app_Z.url.md` в мессенджере.

## API приложения

Реализуйте экспортируемые функции в `app.c`:

```c
void I(void)          // Инициализация (вызывается один раз)
void K(void)          // Событие "Нажатие кнопки" / touch down
void L(void)          // Событие "Отпускание кнопки" / touch up
void P(void)          // Событие "Рассчёт кадра" (по setInterval из JS)
void R(byte *canvas)  // Рендер кадра (по requestAnimationFrame from JS)
```

Буфер `canvas` — `byte*` в формате RGBA (4 байта на пиксель). Индекс: `(x + y * CANVAS_SZ) * 4`.

## Тестирование

### Unit-тесты

```c
// app-mygame/tests/my_test.c
TEST(test_name) {
    I();
    state.score = 100;
    P();
    TEST_ASSERT(state.score >= 0, "score неотрицательный");
}
```

```bash
./run_tests.sh
```

### CLI-тестирование

```bash
./compile_cli.sh
echo -e "I\nP\nK\nR\nS\nQ" | ./app-mygame/build/game_cli.bin
```

Команды: `I` (init), `P` (physics), `K` (key down), `L` (key up), `R` (render), `S` (state snapshot), `Q` (quit).

## Структура проекта

```
framework/
├── engine/            # Ядро (engine.h, engine.c, boot.js, html_tpl.html)
├── tools/             # Build-утилиты (stringify, inline)
├── app-template/      # Пример игры → скопируйте и модифицируйте
│   ├── src/           # app.c, app.h, assets.c/h
│   └── tests/         # Unit-тесты
├── build.env          # Конфигурация (APP_SRC, FPS, CLANG)
├── compile.sh         # C → WASM
├── build.sh           # Полная сборка
├── compile_cli.sh     # CLI для тестирования
└── run_tests.sh       # Unit-тесты
````
