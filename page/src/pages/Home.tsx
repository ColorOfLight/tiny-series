import { useState, useCallback } from "react";
import reactLogo from "../assets/react.svg";
import viteLogo from "/vite.svg";
import "./App.css";

import useEmscriptenModule from "../hooks/useEmscriptenModule";

import Module from "../wasm/test_exe";
import wasmBinary from "../wasm/test_exe_wasm.wasm?url";

function App() {
  const [dice, setDice] = useState(0);
  const { isLoaded, moduleFunctions } = useEmscriptenModule(Module, wasmBinary);

  const rollDice = useCallback(() => {
    if (isLoaded && moduleFunctions) {
      const results = moduleFunctions.simulate_dice_trials(1);
      const keys = results.keys();

      setDice(keys.get(0));
    } else {
      throw new Error("Wasm Module not loaded");
    }
  }, [isLoaded, moduleFunctions]);

  return (
    <>
      <div>
        <a href="https://vitejs.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <h1>Vite + React</h1>
      <div className="card">
        <button onClick={rollDice}>Roll dice! : {dice}</button>
        <p>
          Edit <code>src/App.tsx</code> and save to test HMR
        </p>
      </div>
      <p className="read-the-docs">
        Click on the Vite and React logos to learn more
      </p>
    </>
  );
}

export default App;
