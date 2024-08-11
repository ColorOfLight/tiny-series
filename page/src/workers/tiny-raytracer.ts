/// <reference lib="webworker" />

import { ExtendedEmModule } from "@/wasm/_types";
import TinyRaytracerModule, { ModuleFunctions } from "@/wasm/TinyRaytracer";
import TinyRaytracerWasm from "@/wasm/TinyRaytracer.wasm?url";

import { writeFileToFS, initModule } from "@/utils/emscripten";

interface InitMessage {
  type: "init";
}

interface RenderMessage {
  type: "render";
  background: string;
  width: number;
  height: number;
}

type WorkerMessage = InitMessage | RenderMessage;

interface InitCompleteMessage {
  type: "initComplete";
  success: boolean;
  error?: Error;
}

interface RenderCompleteMessage {
  type: "renderComplete";
  outputPngData: ArrayBuffer;
}

export type WorkerResponse = InitCompleteMessage | RenderCompleteMessage;

const backgroundAssets: Record<string, string> = {
  rosendal: "/backgrounds/rosendal_plains/rosendal_plains_2k.jpg",
  church: "/backgrounds/church/church_2k.jpg",
  estate: "/backgrounds/estate_square/estate_square_2k.jpg",
};

let wasmModule: ExtendedEmModule & ModuleFunctions;

self.onmessage = async (e: MessageEvent<WorkerMessage>) => {
  const data = e.data;

  if (data.type === "init") {
    try {
      wasmModule = await initModule(TinyRaytracerModule, TinyRaytracerWasm);
      self.postMessage({ type: "initComplete", success: true });
    } catch (error) {
      self.postMessage({ type: "initComplete", success: false, error: error });
    }
  } else if (data.type === "render") {
    if (wasmModule) {
      const width = data.width;
      const height = data.height;

      const backgroundAsset = backgroundAssets[data.background];

      await Promise.all([
        writeFileToFS(wasmModule, backgroundAsset, "background.jpg"),
      ]);

      // Call the render function
      wasmModule.render(width, height);

      // Read the file from the virtual filesystem
      const outputPngBin = wasmModule.FS.readFile("output.png", {
        encoding: "binary",
      });

      // Send the file content back to the main thread
      self.postMessage(
        {
          type: "renderComplete",
          outputPngData: outputPngBin.buffer,
        },
        [outputPngBin.buffer]
      );
    }
  }
};
