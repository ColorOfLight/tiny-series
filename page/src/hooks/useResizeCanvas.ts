import { useRef, useEffect } from "react";

const useResizeCanvas = (resizeCallback?: () => void) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const canvas = canvasRef.current;

    const resizeCanvas = () => {
      if (canvas != null) {
        canvas.height = canvas.width;
        resizeCallback?.();
      }
    };

    window.addEventListener("resize", resizeCanvas);
    resizeCanvas(); // Initial resize

    return () => {
      window.removeEventListener("resize", resizeCanvas);
    };
  }, [resizeCallback]);

  return { canvasRef };
};

export default useResizeCanvas;
