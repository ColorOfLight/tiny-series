import { useCallback, useMemo, useState, useRef } from "react";

import TitleGroup from "../modules/TitleGroup";
import SelectRenderType from "../modules/SelectRenderType";
import FieldSetRenderOptions from "../modules/FormRenderOptions";

import useFormRenderOptions from "../modules/FormRenderOptions.hook";
import useEmscriptenModule from "../hooks/useEmscriptenModule";

import TinyRendererModule from "../wasm/TinyRenderer";
import TinyRendererWasm from "../wasm/TinyRenderer.wasm?url";

import { writeFileToFS, getUrlFromFS } from "../utils/emscripten";

function App() {
  const [outImageLink, setOutImageLink] = useState<string>();
  const [zBufferLink, setZBufferLink] = useState<string>();
  const [shadowMapLink, setShadowMapLink] = useState<string>();
  const [aoMapLink, setAoMapLink] = useState<string>();
  const [renderType, setRenderType] = useState<string>("result");

  const resultDivRef = useRef<HTMLDivElement>(null);

  const formHookResult = useFormRenderOptions();

  const { isLoaded, emModule } = useEmscriptenModule(
    TinyRendererModule,
    TinyRendererWasm
  );

  const handleSubmitOptions = useCallback(
    async (event: React.FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      if (!isLoaded || !emModule) {
        throw new Error("Emscripten module not loaded");
      }

      if (!resultDivRef.current) {
        throw new Error("Div tag for result is not found");
      }

      const { modelAsset, cameraX, cameraY, cameraZ, lightX, lightY, lightZ } =
        formHookResult;

      await Promise.all([
        writeFileToFS(emModule, modelAsset.obj, "/model.obj"),
        writeFileToFS(emModule, modelAsset.diffuse, "/diffuse.png"),
        writeFileToFS(emModule, modelAsset.normal, "/normal.png"),
      ]);

      const cameraPosition = [
        Number(cameraX),
        Number(cameraY),
        Number(cameraZ),
      ];
      const lightDirection = [Number(lightX), Number(lightY), Number(lightZ)];

      const width = resultDivRef.current.clientWidth;
      const height = resultDivRef.current.clientHeight;

      emModule.render(lightDirection, cameraPosition, width, height);

      setOutImageLink(getUrlFromFS(emModule, "/output.png"));
      setZBufferLink(getUrlFromFS(emModule, "/z_buffer.png"));
      setShadowMapLink(getUrlFromFS(emModule, "/shadow_map.png"));
      setAoMapLink(getUrlFromFS(emModule, "/ao.png"));
    },
    [isLoaded, emModule, formHookResult]
  );

  const handleRenderTypeChange = useCallback(
    (event: React.ChangeEvent<HTMLSelectElement>) => {
      setRenderType(event.target.value);
    },
    []
  );

  const imageLink = useMemo(() => {
    return {
      result: outImageLink,
      zBuffer: zBufferLink,
      shadowBuffer: shadowMapLink,
      ao: aoMapLink,
    }[renderType];
  }, [outImageLink, zBufferLink, shadowMapLink, aoMapLink, renderType]);

  return (
    <main className="p-8 flex flex-col items-center">
      <div className="max-w-2xl w-full shrink-0">
        <TitleGroup />
        <div className="mt-6 flex gap-8 flex-col">
          <div className="flex flex-col">
            <SelectRenderType onChange={handleRenderTypeChange} />
            <div
              className="w-full bg-gray-300 aspect-square"
              ref={resultDivRef}
            >
              {imageLink && <img src={imageLink} alt="result image" />}
            </div>
          </div>
          <FieldSetRenderOptions
            hookResult={formHookResult}
            onSubmit={handleSubmitOptions}
          />
        </div>
      </div>
    </main>
  );
}

export default App;
