import { WasmModule } from "./_types";

interface ModuleFunctions {
  render: (
    lightDirection: number[],
    cameraPosition: number[],
    width: number,
    height: number
  ) => void;
}

declare const Module: WasmModule<ModuleFunctions>;

export default Module;
