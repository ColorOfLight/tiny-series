import { memo, forwardRef, ComponentPropsWithoutRef } from "react";
import clsx from "clsx";

export interface FooterProps extends ComponentPropsWithoutRef<"footer"> {}

const Footer = memo(
  forwardRef<HTMLElement, FooterProps>(({ className, ...restProps }, ref) => {
    return (
      <footer
        className={clsx("p-4 text-gray-600 text-sm", className)}
        {...restProps}
        ref={ref}
      >
        MIT 2024 © ColorOfLight
      </footer>
    );
  })
);

export default Footer;
