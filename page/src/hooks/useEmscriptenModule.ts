import { useState, useEffect } from "react";

import { ExtendedEmModule, WasmModule } from "../wasm/_types";

const useEmscriptenModule = <M extends WasmModule<MF>, MF>(
  wasmModule: M & WasmModule<MF>,
  wasmBinaryPath: string
) => {
  const [isLoaded, setIsLoaded] = useState(false);
  const [emModule, setEmModule] = useState<ExtendedEmModule & MF>();

  useEffect(() => {
    const initModule = async () => {
      const wasmBin = await fetch(wasmBinaryPath);

      const module = await wasmModule({
        instantiateWasm: (
          imports: WebAssembly.Imports,
          successCallback: (instance: WebAssembly.Instance) => void
        ) => {
          WebAssembly.instantiateStreaming(wasmBin, imports)
            .then((instance) => {
              successCallback(instance.instance);
              setIsLoaded(true);
            })
            .catch((error) => {
              throw error;
            });
          return {};
        },
      });

      setEmModule(module);
    };

    initModule();
  }, [wasmModule, wasmBinaryPath]);

  return {
    isLoaded,
    emModule,
  };
};

export default useEmscriptenModule;
