import React, { FormEvent, useState, useCallback } from "react";

import useSelect from "@/hooks/useSelect";

export interface UseFormRenderOptionsResult {
  background?: string;
  onBackgroundChange: (e: React.ChangeEvent<HTMLSelectElement>) => void;
  isDirty: boolean;
  errorMessage: string | null;
  handleSubmit: (
    onSubmit?: React.EventHandler<FormEvent>
  ) => React.EventHandler<FormEvent>;
}

const useFormRenderOptions = (): UseFormRenderOptionsResult => {
  const [isDirty] = useState(false);

  const { value: background, onChange: onBackgroundChange } =
    useSelect("Rosendal");

  const handleSubmit = useCallback(
    (onSubmit?: React.EventHandler<FormEvent>) => {
      return (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault();
        onSubmit?.(e);
      };
    },
    []
  );

  const errorMessage = null;

  return {
    background,
    onBackgroundChange,
    errorMessage,
    isDirty,
    handleSubmit,
  };
};

export default useFormRenderOptions;
