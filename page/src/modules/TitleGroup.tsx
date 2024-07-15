import { forwardRef, ComponentPropsWithoutRef, memo } from "react";
import clsx from "clsx";

export interface TitleGroupProps extends ComponentPropsWithoutRef<"div"> {}

const TitleGroup = memo(
  forwardRef<HTMLDivElement, TitleGroupProps>(
    ({ className, ...restProps }, ref) => {
      return (
        <div
          className={clsx("flex items-center justify-between", className)}
          {...restProps}
          ref={ref}
        >
          <h1 className="text-2xl text-gray-900 font-bold tracking-wide">
            TinyRenderer
          </h1>
          <a
            href="https://github.com/ColorOfLight/tiny-series/"
            target="_blank"
          >
            <img src="/logos/github.svg" width="32" height="32" />
          </a>
        </div>
      );
    }
  )
);

export default TitleGroup;
