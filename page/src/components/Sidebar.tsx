import { memo, forwardRef, ComponentPropsWithoutRef } from "react";

export interface SidebarProps extends ComponentPropsWithoutRef<"div"> {}

const Sidebar = memo(
  forwardRef<HTMLDivElement, SidebarProps>(({ ...restProps }, ref) => {
    return <div {...restProps} ref={ref}></div>;
  })
);

export default Sidebar;
