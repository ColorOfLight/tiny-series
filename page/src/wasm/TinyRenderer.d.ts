import { WasmModule } from "./_types";

interface ModuleFunctions {
  render: (
    lightPosition: number[],
    cameraPosition: number[],
    width: number,
    height: number
  ) => void;
}

declare const Module: WasmModule<ModuleFunctions>;

export default Module;
