import { forwardRef, memo, ElementRef } from "react";
import clsx from "clsx";
import { Select, SelectProps } from "@headlessui/react";

export interface SelectRenderTypeProps extends SelectProps {}

const SelectRenderType = memo(
  forwardRef<ElementRef<typeof Select>, SelectRenderTypeProps>(
    ({ className, ...restProps }, ref) => {
      return (
        <Select
          name="renderType"
          aria-label="Project status"
          className={clsx("mb-4 self-end", className)}
          {...restProps}
          ref={ref}
        >
          <option value="result">Result</option>
          <option value="zBuffer">Z buffer</option>
          <option value="shadowBuffer">Shadow buffer</option>
          <option value="ao">AO</option>
        </Select>
      );
    }
  )
);

export default SelectRenderType;
