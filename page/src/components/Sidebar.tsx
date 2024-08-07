import { memo, forwardRef, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";

export interface SidebarProps extends ComponentPropsWithoutRef<"div"> {}

const Sidebar = memo(
  forwardRef<HTMLDivElement, SidebarProps>(
    ({ className, ...restProps }, ref) => {
      return (
        <div
          className={clsx(className, "w-full bg-gray-100")}
          {...restProps}
          ref={ref}
        >
          Sidebar
        </div>
      );
    }
  )
);

export default Sidebar;
