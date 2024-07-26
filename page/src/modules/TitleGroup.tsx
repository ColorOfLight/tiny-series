import { forwardRef, ComponentPropsWithoutRef, memo } from "react";
import clsx from "clsx";

export interface TitleGroupProps extends ComponentPropsWithoutRef<"div"> {}

const TitleGroup = memo(
  forwardRef<HTMLDivElement, TitleGroupProps>(
    ({ className, ...restProps }, ref) => {
      return (
        <div
          className={clsx("flex items-top justify-between", className)}
          {...restProps}
          ref={ref}
        >
          <div>
            <h1 className="text-2xl text-gray-900 font-bold tracking-wide">
              TinyRenderer
            </h1>
            <p className="mt-4 text-gray-500">
              CPU-only model renderer written in C++
            </p>
          </div>
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
