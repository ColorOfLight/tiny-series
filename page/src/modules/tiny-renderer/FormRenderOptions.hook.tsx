import React, { FormEvent, useMemo, useState, useCallback } from "react";

import useInput from "@/hooks/useInput";
import useSelect from "@/hooks/useSelect";

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
  errorMessage: string | null;
  isDirty: boolean;
  handleSubmit: (
    onSubmit?: React.EventHandler<FormEvent>
  ) => React.EventHandler<FormEvent>;
}

const useFormRenderOptions = (): UseFormRenderOptionsResult => {
  const [isDirty, setIsDirty] = useState(false);

  const { value: model, onChange: onModelChange } = useSelect("shark");

  const { value: cameraX, onChange: onCameraXChange } = useInput("1");
  const { value: cameraY, onChange: onCameraYChange } = useInput("1");
  const { value: cameraZ, onChange: onCameraZChange } = useInput("1");

  const { value: lightX, onChange: onLightXChange } = useInput("0");
  const { value: lightY, onChange: onLightYChange } = useInput("0");
  const { value: lightZ, onChange: onLightZChange } = useInput("2");

  const errorMessage = useMemo(() => {
    if (cameraX === "") {
      return "Camera X cannot be empty";
    }

    if (cameraY === "") {
      return "Camera Y cannot be empty";
    }

    if (cameraZ === "") {
      return "Camera Z cannot be empty";
    }

    if (lightX === "") {
      return "Light X cannot be empty";
    }

    if (lightY === "") {
      return "Light Y cannot be empty";
    }

    if (lightZ === "") {
      return "Light Z cannot be empty";
    }

    if (cameraX === "0" && cameraY === "0" && cameraZ === "0") {
      return "Camera position cannot be 0, 0, 0";
    }

    if (lightX === "0" && lightY === "0" && lightZ === "0") {
      return "Light position cannot be 0, 0, 0";
    }

    return null;
  }, [cameraX, cameraY, cameraZ, lightX, lightY, lightZ]);

  const handleSubmit = useCallback(
    (onSubmit?: React.EventHandler<FormEvent>) => {
      return (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault();

        setIsDirty(true);

        if (errorMessage) {
          return;
        }

        setIsDirty(false);

        onSubmit?.(e);
      };
    },
    [errorMessage]
  );

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
    errorMessage,
    isDirty,
    handleSubmit,
  };
};

export default useFormRenderOptions;
