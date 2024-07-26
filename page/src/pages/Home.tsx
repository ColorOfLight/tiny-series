import { useCallback, useMemo, useState, useRef } from "react";

import TitleGroup from "../modules/TitleGroup";
import SelectRenderType from "../modules/SelectRenderType";
import FieldSetRenderOptions from "../modules/FormRenderOptions";

import useFormRenderOptions from "../modules/FormRenderOptions.hook";

import RenderWorker from "../workers/tiny-renderer?worker";
import { generateUrlFromBuffer } from "../utils/image";

function App() {
  const [outImageLink, setOutImageLink] = useState<string>();
  const [zBufferLink, setZBufferLink] = useState<string>();
  const [shadowMapLink, setShadowMapLink] = useState<string>();
  const [aoMapLink, setAoMapLink] = useState<string>();
  const [renderType, setRenderType] = useState<string>("result");

  const resultDivRef = useRef<HTMLDivElement>(null);

  const formHookResult = useFormRenderOptions();

  const handleSubmitOptions = useCallback(
    async (event: React.FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      if (!resultDivRef.current) {
        throw new Error("Div tag for result is not found");
      }

      const { model, cameraX, cameraY, cameraZ, lightX, lightY, lightZ } =
        formHookResult;

      if (model == null) {
        throw new Error("Model is not set");
      }

      if (lightX == null || lightY == null || lightZ == null) {
        throw new Error("Light position is not set");
      }

      if (cameraX == null || cameraY == null || cameraZ == null) {
        throw new Error("Camera position is not set");
      }

      const lightPosition = [
        Number.parseFloat(lightX),
        Number.parseFloat(lightY),
        Number.parseFloat(lightZ),
      ];

      const cameraPosition = [
        Number.parseFloat(cameraX),
        Number.parseFloat(cameraY),
        Number.parseFloat(cameraZ),
      ];

      const width = resultDivRef.current.clientWidth;
      const height = resultDivRef.current.clientHeight;

      const worker = new RenderWorker();
      worker.onmessage = (e) => {
        const data = e.data;

        if (data.type === "initComplete" && data.success) {
          worker.postMessage({
            type: "render",
            model,
            lightPosition,
            cameraPosition,
            width,
            height,
          });
        } else if (data.type === "renderComplete") {
          setOutImageLink(generateUrlFromBuffer(data.outputPngData));
          setZBufferLink(generateUrlFromBuffer(data.zBufferPngData));
          setShadowMapLink(generateUrlFromBuffer(data.shadowMapPngData));
          setAoMapLink(generateUrlFromBuffer(data.aoPngData));
        }
      };

      worker.postMessage({
        type: "init",
      });
    },
    [formHookResult]
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
