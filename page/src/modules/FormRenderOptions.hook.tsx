import { useMemo } from "react";

import useInput from "../hooks/useInput";
import useSelect from "../hooks/useSelect";
import { canParseFloat } from "../utils/number";

export interface UseFormRenderOptionsResult {
  model?: string;
  onModelChange: (e: React.ChangeEvent<HTMLSelectElement>) => void;
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
  isDisabled: boolean;
}

const useFormRenderOptions = (): UseFormRenderOptionsResult => {
  const { value: model, onChange: onModelChange } = useSelect("shark");

  const { value: cameraX, onChange: onCameraXChange } = useInput("1");
  const { value: cameraY, onChange: onCameraYChange } = useInput("1");
  const { value: cameraZ, onChange: onCameraZChange } = useInput("1");

  const { value: lightX, onChange: onLightXChange } = useInput("0");
  const { value: lightY, onChange: onLightYChange } = useInput("0");
  const { value: lightZ, onChange: onLightZChange } = useInput("2");

  const isDisabled = useMemo(() => {
    return (
      canParseFloat(cameraX) ||
      canParseFloat(cameraY) ||
      canParseFloat(cameraZ) ||
      canParseFloat(lightX) ||
      canParseFloat(lightY) ||
      canParseFloat(lightZ)
    );
  }, [cameraX, cameraY, cameraZ, lightX, lightY, lightZ]);

  return {
    model,
    onModelChange,
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
    isDisabled,
  };
};

export default useFormRenderOptions;
