import { useState } from "react";

const useSelect = (initialValue?: string) => {
  const [value, setValue] = useState(initialValue);
  const onChange = (e: React.ChangeEvent<HTMLSelectElement>) => {
    setValue(e.target.value);
  };
  return { value, onChange };
};

export default useSelect;
