import TitleGroup from "../components/TitleGroup";
import SelectRenderType from "../components/SelectRenderType";
import FieldSetRenderOptions from "../components/FieldSetRenderOptions";

import useResizeCanvas from "../hooks/useResizeCanvas";

function App() {
  const { canvasRef } = useResizeCanvas();

  return (
    <main className="p-8 flex flex-col items-center">
      <div className="max-w-2xl w-full shrink-0">
        <TitleGroup />
        <div className="mt-6 flex gap-8 flex-col">
          <div className="flex flex-col">
            <SelectRenderType />
            <canvas className="w-full border bg-slate-200" ref={canvasRef} />
          </div>
          <FieldSetRenderOptions />
        </div>
      </div>
    </main>
  );
}

export default App;
