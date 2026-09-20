const fs = require('node:fs');

const fileName = process.argv[process.argv.length - 1];
const data = fs.readFileSync(fileName);

if (fileName.endsWith(".gz.base64")) {
    console.log(
        `U = Uint8Array;new Blob([U.from(atob('${data.toString()}'), c => c.charCodeAt(0))])` +
        `.stream().pipeThrough(new DecompressionStream("gzip"))` +
        `.getReader().read().then(({value:Z})=>{`,
        "\n})",
    )
    return 0;
}

if (fileName.endsWith(".gz")) {
    console.log(
        `U=Uint8Array;new Blob([new U([${new Uint8Array(data).toString()}])])` +
        `.stream().pipeThrough(new DecompressionStream("gzip"))` +
        `.getReader().read().then(({value:Z})=>{`,
        "\n})",
    )
    return 0;
}

if (fileName.endsWith(".base64")) {
    console.log(
        `U = Uint8Array; Z = U.from(atob('${data.toString()}'), c => c.charCodeAt(0))`,
        "\n/*-*/",
    )
    return 0;
}

if (fileName.endsWith(".wasm")) {

    console.log(
        `U = Uint8Array; Z = new U([${new Uint8Array(data).toString()}]);`,
        "\n/*-*/",
    )
    return 0;
}

console.error("Не указан входной файл")
return -1;
