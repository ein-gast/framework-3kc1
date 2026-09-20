// файл для встраивания ресурсов в итоговый бинарник

// --- exported js = eval(boot.js) ---
const char js[] = {
#ifdef APP_MIN_JS
#embed "boot.min.js"
#else
#embed "boot.js"
#endif
    , '\0' // null terminator
};
