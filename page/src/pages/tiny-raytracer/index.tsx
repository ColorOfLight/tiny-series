import { useState, useRef, useCallback } from "react";
import ContentLayout from "@/layouts/ContentLayout";
import ResultImage from "@/modules/ResultImage";
import FormRenderOptions from "@/modules/tiny-raytracer/FormRenderOptions";
import useFormRenderOptions from "@/modules/tiny-raytracer/FormRenderOptions.hook";
import { generateUrlFromBuffer } from "@/utils/image";
import RenderWorker from "@/workers/tiny-raytracer?worker";
import { WorkerResponse } from "@/workers/tiny-raytracer";

const TinyRaytracerPage = () => {
  const [outImageLink, setOutImageLink] = useState<string>();
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

      const width = resultDivRef.current.clientWidth;
      const height = resultDivRef.current.clientHeight;

      const { background } = formHookResult;

      const worker = new RenderWorker();
      worker.onmessage = (e: MessageEvent<WorkerResponse>) => {
        const data = e.data;

        if (data.type === "initComplete" && data.success) {
          worker.postMessage({
            type: "render",
            background,
            width,
            height,
          });
        } else if (data.type === "renderComplete") {
          setOutImageLink(generateUrlFromBuffer(data.outputPngData));
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
  }, [formHookResult, isRendering]);

  return (
    <ContentLayout
      title="Tiny Raytracer"
      description="CPU-only ray tracer written in C++"
    >
      <div className="flex flex-col gap-8 mt-4">
        <ResultImage
          imageLink={outImageLink}
          isRendering={isRendering}
          ref={resultDivRef}
        />
        <FormRenderOptions
          isRendering={isRendering}
          hookResult={formHookResult}
          onSubmit={handleSubmitOptions}
        />
      </div>
    </ContentLayout>
  );
};

export default TinyRaytracerPage;
