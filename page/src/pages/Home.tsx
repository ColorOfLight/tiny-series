import { useCallback } from "react";

import TitleGroup from "../modules/TitleGroup";
import SelectRenderType from "../modules/SelectRenderType";
import FieldSetRenderOptions from "../modules/FormRenderOptions";

import useResizeCanvas from "../hooks/useResizeCanvas";
import useFormRenderOptions from "../modules/FormRenderOptions.hook";

function App() {
  const { canvasRef } = useResizeCanvas();

  const formHookResult = useFormRenderOptions();

  const handleSubmitOptions = useCallback(
    (event: React.FormEvent<HTMLFormElement>) => {
      event.preventDefault();

      console.log(event);
    },
    []
  );

  return (
    <main className="p-8 flex flex-col items-center">
      <div className="max-w-2xl w-full shrink-0">
        <TitleGroup />
        <div className="mt-6 flex gap-8 flex-col">
          <div className="flex flex-col">
            <SelectRenderType />
            <canvas className="w-full border bg-slate-200" ref={canvasRef} />
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
