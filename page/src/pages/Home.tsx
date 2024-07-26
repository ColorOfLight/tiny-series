import { useCallback, useMemo, useState } from "react";

import TitleGroup from "../modules/TitleGroup";
import SelectRenderType from "../modules/SelectRenderType";
import FieldSetRenderOptions from "../modules/FormRenderOptions";

import useFormRenderOptions from "../modules/FormRenderOptions.hook";
import useEmscriptenModule from "../hooks/useEmscriptenModule";

import { ExtendedEmModule } from "../wasm/_types";
import TinyRendererModule from "../wasm/TinyRenderer";
import TinyRendererWasm from "../wasm/TinyRenderer.wasm?url";

const writeFileToFS = async (
  emModule: ExtendedEmModule,
  filePath: string,
  writePath: string
) => {
  const file = await fetch(filePath);
  const fileBuffer = await file.arrayBuffer();
  const fileArray = new Uint8Array(fileBuffer);
  emModule.FS.writeFile(writePath, fileArray);
};

const getUrlFromFS = (emModule: ExtendedEmModule, path: string): string => {
  const data = emModule.FS.readFile(path);
  const blob = new Blob([data], { type: "image/png" });
  return URL.createObjectURL(blob);
};

function App() {
  const [outImageLink, setOutImageLink] = useState<string>();
  const [zBufferLink, setZBufferLink] = useState<string>();
  const [shadowMapLink, setShadowMapLink] = useState<string>();
  const [aoMapLink, setAoMapLink] = useState<string>();
  const [renderType, setRenderType] = useState<string>("result");

  const formHookResult = useFormRenderOptions();

  const { isLoaded, emModule } = useEmscriptenModule(
    TinyRendererModule,
    TinyRendererWasm
  );

  const handleSubmitOptions = useCallback(
    async (event: React.FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      if (isLoaded && emModule) {
        await Promise.all([
          writeFileToFS(emModule, "/models/shark/shark.obj", "/model.obj"),
          writeFileToFS(
            emModule,
            "/models/shark/shark_diffuse.png",
            "/diffuse.png"
          ),
          writeFileToFS(emModule, "/models/shark/shark_nm.png", "/normal.png"),
        ]);

        const cameraPosition = [1, 1, 2];
        const lightDirection = [0, 0, -1];

        emModule.render(lightDirection, cameraPosition, 800, 800);

        setOutImageLink(getUrlFromFS(emModule, "/output.png"));
        setZBufferLink(getUrlFromFS(emModule, "/z_buffer.png"));
        setShadowMapLink(getUrlFromFS(emModule, "/shadow_map.png"));
        setAoMapLink(getUrlFromFS(emModule, "/ao.png"));
      }
    },
    [isLoaded, emModule]
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
            <div className="w-full bg-gray-300 aspect-square">
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
