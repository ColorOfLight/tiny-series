import { forwardRef, ComponentPropsWithoutRef, memo } from "react";
import clsx from "clsx";

export interface TitleGroupProps extends ComponentPropsWithoutRef<"div"> {
  title: string;
  description: string;
}

const TitleGroup = memo(
  forwardRef<HTMLDivElement, TitleGroupProps>(
    ({ className, title, description, ...restProps }, ref) => {
      return (
        <div>
          <div
            className={clsx("flex items-top justify-between", className)}
            {...restProps}
            ref={ref}
          >
            <div>
              <h1 className="text-2xl text-gray-900 font-bold tracking-wide">
                {title}
              </h1>
              <p className="mt-4 text-gray-500">{description}</p>
            </div>
            <div className="shrink-0">
              <a
                href="https://github.com/ColorOfLight/tiny-series/"
                target="_blank"
              >
                <img src="/logos/github.svg" width="28" height="28" />
              </a>
            </div>
          </div>
        </div>
      );
    }
  )
);

export default TitleGroup;
