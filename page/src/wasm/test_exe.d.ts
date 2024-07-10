import { WasmModule } from "./_types";

interface ModuleFunctions {
  simulate_dice_trials: (numTrials: number) => EmMap<number, number>;
}

declare const Module: WasmModule<ModuleFunctions>;

export default Module;
