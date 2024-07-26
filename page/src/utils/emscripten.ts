import { ExtendedEmModule, WasmModule } from "../wasm/_types";

export const initModule = async <M extends WasmModule<MF>, MF>(
  wasmModule: M & WasmModule<MF>,
  wasmBinaryPath: string
) => {
  const emModule = await wasmModule({
    instantiateWasm: (
      imports: WebAssembly.Imports,
      successCallback: (instance: WebAssembly.Instance) => void
    ) => {
      WebAssembly.instantiateStreaming(fetch(wasmBinaryPath), imports)
        .then((instance) => {
          successCallback(instance.instance);
        })
        .catch((error) => {
          throw error;
        });
      return {};
    },
  });

  return emModule;
};

export const writeFileToFS = async (
  emModule: ExtendedEmModule,
  filePath: string,
  writePath: string
) => {
  const file = await fetch(filePath);
  const fileBuffer = await file.arrayBuffer();
  const fileArray = new Uint8Array(fileBuffer);
  emModule.FS.writeFile(writePath, fileArray);
};
