import { forwardRef, memo, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";

import MenuIcon from "../components/MenuIcon";

export interface NavigationProps extends ComponentPropsWithoutRef<"div"> {
  title: string;
}

const Navigation = memo(
  forwardRef<HTMLDivElement, NavigationProps>(
    ({ className, title, ...restProps }, ref) => {
      return (
        <div
          className={clsx("flex items-center ml-m2", className)}
          {...restProps}
          ref={ref}
        >
          <button className="p-2 border-0">
            <MenuIcon className="fill-gray-500" />
          </button>
          <nav className="text-gray-400 ml-1">
            <ul className="flex gap-1">
              <li>Tiny Series</li>
              <li className="text-gray-700">{title}</li>
            </ul>
          </nav>
        </div>
      );
    }
  )
);

export default Navigation;
