import { ComponentPropsWithoutRef, forwardRef, memo } from "react";
import clsx from "clsx";

import Spinner from "@/components/Spinner";

export interface LoadingCoverProps extends ComponentPropsWithoutRef<"div"> {}

const LoadingCover = memo(
  forwardRef<HTMLDivElement, LoadingCoverProps>(
    ({ className, children, ...restProps }, ref) => {
      return (
        <div
          className={clsx(
            "absolute inset-0 flex items-center justify-center bg-cover flex-col gap-4 px-8 text-center sm:gap-12",
            className
          )}
          {...restProps}
          ref={ref}
        >
          <Spinner className="scale-75 sm:scale-100" />
          {children}
        </div>
      );
    }
  )
);

export default LoadingCover;
