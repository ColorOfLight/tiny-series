import { memo, forwardRef, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";
import Sidebar from "@/components/Sidebar";

export interface NavigationPaneProps extends ComponentPropsWithoutRef<"div"> {
  onOutsideClick?: React.EventHandler<React.MouseEvent>;
}

const NavigationPane = memo(
  forwardRef<HTMLDivElement, NavigationPaneProps>(
    ({ className, onOutsideClick, ...restProps }, ref) => {
      return (
        <div
          className={clsx("fixed inset-0 bg-cover flex", className)}
          {...restProps}
          ref={ref}
        >
          <Sidebar className="w-56 grow-0" />
          <div className="grow" onClick={onOutsideClick} />
        </div>
      );
    }
  )
);

export default NavigationPane;
