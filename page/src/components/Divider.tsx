import { memo, forwardRef, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";

export interface DividerProps extends ComponentPropsWithoutRef<"div"> {}

const Divider = memo(
  forwardRef<HTMLDivElement, DividerProps>(
    ({ className, ...restProps }, ref) => {
      return (
        <div
          className={clsx("w-full h-px my-8 bg-gray-200", className)}
          {...restProps}
          ref={ref}
        />
      );
    }
  )
);

export default Divider;
