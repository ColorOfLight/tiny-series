import { useState, useCallback } from "react";

const useNavigationPane = () => {
  const [isPaneOpen, setIsPaneOpen] = useState(false);

  const openPane = useCallback(() => setIsPaneOpen(true), []);
  const closePane = useCallback(() => setIsPaneOpen(false), []);

  return {
    isPaneOpen,
    openPane,
    closePane,
  };
};

export default useNavigationPane;
