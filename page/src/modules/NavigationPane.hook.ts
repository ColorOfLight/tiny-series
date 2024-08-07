import { useState, useCallback, useEffect } from "react";

// TODO: Replace with the real value
const PANE_WIDTH_LIMIT = 640;

const useNavigationPane = () => {
  const [isPaneOpen, setIsPaneOpen] = useState(false);

  const openPane = useCallback(() => setIsPaneOpen(true), []);
  const closePane = useCallback(() => setIsPaneOpen(false), []);

  useEffect(() => {
    const handler = () => {
      if (window.innerWidth > PANE_WIDTH_LIMIT) {
        closePane();
      }
    };

    window.addEventListener("resize", handler);

    return () => {
      window.removeEventListener("resize", handler);
    };
  }, [closePane]);

  useEffect(() => {
    if (isPaneOpen) {
      document.body.style.overflow = "hidden";
    } else {
      document.body.style.overflow = "";
    }
  }, [isPaneOpen]);

  return {
    isPaneOpen,
    openPane,
    closePane,
  };
};

export default useNavigationPane;
