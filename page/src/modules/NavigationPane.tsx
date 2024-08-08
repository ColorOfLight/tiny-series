import { memo, forwardRef, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";
import Sidebar from "@/components/Sidebar";
import Footer from "@/components/Footer";

export interface NavigationPaneProps extends ComponentPropsWithoutRef<"div"> {
  onOutsideClick?: React.EventHandler<React.MouseEvent>;
  onLinksClick?: React.EventHandler<React.MouseEvent>;
  currentPath?: string;
}

const NavigationPane = memo(
  forwardRef<HTMLDivElement, NavigationPaneProps>(
    (
      { className, onOutsideClick, onLinksClick, currentPath, ...restProps },
      ref
    ) => {
      return (
        <div
          className={clsx("fixed inset-0 bg-cover flex", className)}
          {...restProps}
          ref={ref}
        >
          <div className="w-56 grow-0 bg-white flex flex-col justify-between">
            <Sidebar onLinksClick={onLinksClick} currentPath={currentPath} />
            <Footer />
          </div>
          <div className="grow" onClick={onOutsideClick} />
        </div>
      );
    }
  )
);

export default NavigationPane;
