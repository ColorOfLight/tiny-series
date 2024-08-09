import { forwardRef, memo, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";
import LoadingCover from "@/components/LoadingCover";

export interface ResultImageProps extends ComponentPropsWithoutRef<"div"> {
  isRendering?: boolean;
  imageLink?: string;
}

const ResultImage = memo(
  forwardRef<HTMLDivElement, ResultImageProps>(
    ({ className, isRendering, imageLink, ...restProps }, ref) => {
      return (
        <div
          className={clsx(
            "w-full bg-gray-300 aspect-square relative",
            className
          )}
          ref={ref}
          {...restProps}
        >
          {isRendering && (
            <LoadingCover>
              <p className="text-gray-300 text-sm sm:text-base">
                Rendering worker is running...
              </p>
            </LoadingCover>
          )}
          {imageLink && <img src={imageLink} alt="result image" />}
        </div>
      );
    }
  )
);

export default ResultImage;
