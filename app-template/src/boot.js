// Math.ceil(1 + BS / 16 / 1024) = 7
// U = Uint8Array;
// console.log(inst.exports)
let _D = document,
    _C = _D.write(`<canvas id=c></canvas>`),
    _E = inst.exports,
    _T, S = _E.W(), BS = S * S * 4,    
    computingFlag = 0,
    image = (_T = (_C = _D.getElementById(`c`, _E.memory.grow(7 /*Math.ceil(1 + BS / 16 / 1024)*/)))
        .getContext(`2d`))
        .getImageData(0, 0, S, _C.width = _C.height = S),
    pixPtr = _E.__heap_base.value, // __heap_base .. __heap_end
    pixBuf = new U(mem = _E.memory.buffer).subarray(pixPtr, pixPtr + BS),
    // FPS вкомпилирован в wasm
    // FRAME_DELAY = 1000/FPS
    _Z = 1000 / _E.F(),
    // рендер
    processFrame = (_) => {
        if (computingFlag) { requestAnimationFrame(processFrame); return; }
        _E.R(pixPtr)
        image.data.set(pixBuf)
        _T.putImageData(image, 0, 0)
        requestAnimationFrame(processFrame);
    },    
    WH90 = `min(90vw,90vh)`;
_C.style = `width:${WH90};height:${WH90};image-rendering:pixelated`

_E.I()
// обработка физики
setInterval((_) => { computingFlag = 1; _E.P(); computingFlag = 0 }, _Z);
// ввод
_D.onmousedown = _D.onkeydown = (_) => _E.K()
_D.onmouseup = _D.onkeyup = (_) => _E.L()
processFrame(0)
