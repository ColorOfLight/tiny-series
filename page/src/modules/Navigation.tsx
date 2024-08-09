import { forwardRef, memo, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";

import MenuIcon from "@/icons/MenuIcon";

export interface NavigationProps extends ComponentPropsWithoutRef<"div"> {
  title?: string;
  onMenuClick?: React.EventHandler<React.MouseEvent<HTMLButtonElement>>;
}

const Navigation = memo(
  forwardRef<HTMLDivElement, NavigationProps>(
    ({ className, title, onMenuClick, ...restProps }, ref) => {
      return (
        <div
          className={clsx(
            "flex items-center ml-m2 p-3 border-b w-full",
            className
          )}
          {...restProps}
          ref={ref}
        >
          <button className="p-2 border-0" onClick={onMenuClick}>
            <MenuIcon className="fill-gray-500" />
          </button>
          <nav className="text-gray-400 ml-1 text-sm">
            <ul className="flex gap-1">
              <li className={title == null ? "text-gray-700" : ""}>
                Tiny Series
              </li>
              {title ? <li className="text-gray-700">{title}</li> : null}
            </ul>
          </nav>
        </div>
      );
    }
  )
);

export default Navigation;
