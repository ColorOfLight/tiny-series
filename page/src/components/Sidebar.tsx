import { memo, forwardRef, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";
import HomeIcon from "@/icons/HomeIcon";
import { Link } from "react-router-dom";

export interface SidebarProps extends ComponentPropsWithoutRef<"div"> {
  onLinksClick?: React.EventHandler<React.MouseEvent>;
  currentPath?: string;
}

const Sidebar = memo(
  forwardRef<HTMLDivElement, SidebarProps>(
    ({ className, onLinksClick, currentPath, ...restProps }, ref) => {
      const selected = "text-gray-900";
      const unselected = "text-gray-400";

      return (
        <div
          className={clsx(className, "w-full bg-white px-4 py-6")}
          {...restProps}
          ref={ref}
        >
          <div className="flex mb-4">
            <Link to="/" onClick={onLinksClick}>
              <div className="p-1 grow-0">
                <HomeIcon className="fill-gray-500" />
              </div>
            </Link>
          </div>
          <ul>
            <Link to="/tiny-renderer" onClick={onLinksClick}>
              <li
                className={clsx(
                  "py-0.5",
                  currentPath === "/tiny-renderer" ? selected : unselected
                )}
              >
                Tiny Renderer
              </li>
            </Link>
            <Link to="tiny-raytracer" onClick={onLinksClick}>
              <li
                className={clsx(
                  "py-0.5",
                  currentPath === "/tiny-raytracer" ? selected : unselected
                )}
              >
                Tiny Raytracer
              </li>
            </Link>
          </ul>
        </div>
      );
    }
  )
);

export default Sidebar;
