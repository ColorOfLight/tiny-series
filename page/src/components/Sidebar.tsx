import { memo, forwardRef, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";
import { Link } from "react-router-dom";
import Divider from "@/components/Divider";

export interface SidebarProps extends ComponentPropsWithoutRef<"div"> {
  onLinksClick?: React.EventHandler<React.MouseEvent>;
  currentPath?: string;
}

const Sidebar = memo(
  forwardRef<HTMLDivElement, SidebarProps>(
    ({ className, onLinksClick, currentPath, ...restProps }, ref) => {
      const selectedItemClass = "text-gray-900";
      const unselectedItemClass = "text-gray-400 hover:text-gray-700";

      return (
        <div
          className={clsx(className, "w-full bg-white px-4 py-6 lg:px-6")}
          {...restProps}
          ref={ref}
        >
          <ul>
            <Link to="/" onClick={onLinksClick}>
              <li
                className={clsx(
                  "py-0.5",
                  currentPath === "/" ? selectedItemClass : unselectedItemClass
                )}
              >
                Home
              </li>
            </Link>
            <Divider className="!my-1 bg-white" />
            <Link to="/tiny-renderer" onClick={onLinksClick}>
              <li
                className={clsx(
                  "py-0.5",
                  currentPath === "/tiny-renderer"
                    ? selectedItemClass
                    : unselectedItemClass
                )}
              >
                Tiny Renderer
              </li>
            </Link>
            <Link to="tiny-raytracer" onClick={onLinksClick}>
              <li
                className={clsx(
                  "py-0.5",
                  currentPath === "/tiny-raytracer"
                    ? selectedItemClass
                    : unselectedItemClass
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
