import useInput from "../hooks/useInput";

export interface UseFormRenderOptionsResult {
  cameraX?: string;
  onCameraXChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
  cameraY?: string;
  onCameraYChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
  cameraZ?: string;
  onCameraZChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
  lightX?: string;
  onLightXChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
  lightY?: string;
  onLightYChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
  lightZ?: string;
  onLightZChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
}

const useFormRenderOptions = (): UseFormRenderOptionsResult => {
  const { value: cameraX, onChange: onCameraXChange } = useInput("1");
  const { value: cameraY, onChange: onCameraYChange } = useInput("1");
  const { value: cameraZ, onChange: onCameraZChange } = useInput("1");

  const { value: lightX, onChange: onLightXChange } = useInput("0");
  const { value: lightY, onChange: onLightYChange } = useInput("0");
  const { value: lightZ, onChange: onLightZChange } = useInput("-1");

  return {
    cameraX,
    onCameraXChange,
    cameraY,
    onCameraYChange,
    cameraZ,
    onCameraZChange,
    lightX,
    onLightXChange,
    lightY,
    onLightYChange,
    lightZ,
    onLightZChange,
  };
};

export default useFormRenderOptions;
