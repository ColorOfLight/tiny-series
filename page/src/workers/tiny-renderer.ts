/// <reference lib="webworker" />

import { ExtendedEmModule } from "../wasm/_types";
import TinyRendererModule, { ModuleFunctions } from "../wasm/TinyRenderer";
import TinyRendererWasm from "../wasm/TinyRenderer.wasm?url";

import { writeFileToFS, initModule } from "../utils/emscripten";

interface InitMessage {
  type: "init";
}

interface RenderMessage {
  type: "render";
  model: string;
  lightPosition: number[];
  cameraPosition: number[];
  width: number;
  height: number;
}

interface ModelAsset {
  obj: string;
  diffuse: string;
  normal: string;
}

type WorkerMessage = InitMessage | RenderMessage;

const modelAssets: Record<string, ModelAsset> = {
  shark: {
    obj: "/models/shark/shark.obj",
    diffuse: "/models/shark/shark_diffuse.png",
    normal: "/models/shark/shark_nm.png",
  },
};

let wasmModule: ExtendedEmModule & ModuleFunctions;

self.onmessage = async (e: MessageEvent<WorkerMessage>) => {
  const data = e.data;

  if (data.type === "init") {
    try {
      wasmModule = await initModule(TinyRendererModule, TinyRendererWasm);
      self.postMessage({ type: "initComplete", success: true });
    } catch (error) {
      self.postMessage({ type: "initComplete", success: false, error: error });
    }
  } else if (data.type === "render") {
    if (wasmModule) {
      const lightPosition = data.lightPosition;
      const cameraPosition = data.cameraPosition;
      const width = data.width;
      const height = data.height;

      const modelAsset = modelAssets[data.model];

      await Promise.all([
        writeFileToFS(wasmModule, modelAsset.obj, "/model.obj"),
        writeFileToFS(wasmModule, modelAsset.diffuse, "/diffuse.png"),
        writeFileToFS(wasmModule, modelAsset.normal, "/normal.png"),
      ]);

      // Call the render function
      wasmModule.render(lightPosition, cameraPosition, width, height);

      // Read the file from the virtual filesystem
      const outputPngBin = wasmModule.FS.readFile("/output.png", {
        encoding: "binary",
      });

      const zBufferPngBin = wasmModule.FS.readFile("/z_buffer.png", {
        encoding: "binary",
      });

      const shadowMapPngBin = wasmModule.FS.readFile("/shadow_map.png", {
        encoding: "binary",
      });

      const aoPngBin = wasmModule.FS.readFile("/ao.png", {
        encoding: "binary",
      });

      // Send the file content back to the main thread
      self.postMessage(
        {
          type: "renderComplete",
          outputPngData: outputPngBin.buffer,
          zBufferPngData: zBufferPngBin.buffer,
          shadowMapPngData: shadowMapPngBin.buffer,
          aoPngData: aoPngBin.buffer,
        },
        [
          outputPngBin.buffer,
          zBufferPngBin.buffer,
          shadowMapPngBin.buffer,
          aoPngBin.buffer,
        ]
      );
    }
  }
};
