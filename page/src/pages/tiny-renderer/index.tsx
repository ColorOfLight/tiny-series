import { useCallback, useMemo, useState, useRef } from "react";

import SelectRenderType from "@/modules/SelectRenderType";
import FieldSetRenderOptions from "@/modules/tiny-renderer/FormRenderOptions";
import useFormRenderOptions from "@/modules/tiny-renderer/FormRenderOptions.hook";
import RenderWorker from "@/workers/tiny-renderer?worker";
import { WorkerResponse } from "@/workers/tiny-renderer";
import { generateUrlFromBuffer } from "@/utils/image";
import ContentLayout from "@/layouts/ContentLayout";
import ResultImage from "@/modules/ResultImage";

const TinyRendererPage = () => {
  const [outImageLink, setOutImageLink] = useState<string>();
  const [zBufferLink, setZBufferLink] = useState<string>();
  const [shadowMapLink, setShadowMapLink] = useState<string>();
  const [aoMapLink, setAoMapLink] = useState<string>();
  const [renderType, setRenderType] = useState<string>("result");
  const [isRendering, setIsRendering] = useState<boolean>(false);

  const resultDivRef = useRef<HTMLDivElement>(null);

  const formHookResult = useFormRenderOptions();

  const handleSubmitOptions = useCallback(async () => {
    try {
      if (isRendering) {
        throw new Error("Rendering is in progress");
      }

      if (!resultDivRef.current) {
        throw new Error("Div tag for result is not found");
      }

      setIsRendering(true);

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
      worker.onmessage = (e: MessageEvent<WorkerResponse>) => {
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
          setIsRendering(false);
        }
      };

      worker.postMessage({
        type: "init",
      });
    } catch (e) {
      setIsRendering(false);
      throw e;
    }
  }, [isRendering, formHookResult]);

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
    <ContentLayout
      title="TinyRenderer"
      description="CPU-only model renderer written in C++"
    >
      <div className="flex gap-8 flex-col">
        <div className="flex flex-col">
          <SelectRenderType
            onChange={handleRenderTypeChange}
            disabled={imageLink == null}
          />
          <ResultImage
            isRendering={isRendering}
            imageLink={imageLink}
            ref={resultDivRef}
          />
        </div>
        <FieldSetRenderOptions
          hookResult={formHookResult}
          onSubmit={handleSubmitOptions}
          isRendering={isRendering}
        />
      </div>
    </ContentLayout>
  );
};

export default TinyRendererPage;
