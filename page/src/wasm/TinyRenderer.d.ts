import { WasmModule } from "./_types";

interface ModuleFunctions {
  render: () => void;
}

declare const Module: WasmModule<ModuleFunctions>;

export default Module;
