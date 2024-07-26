import { forwardRef, memo, ComponentPropsWithoutRef } from "react";

export interface SpinnerProps extends ComponentPropsWithoutRef<"svg"> {}

const Spinner = memo(
  forwardRef<SVGSVGElement, SpinnerProps>((props, ref) => {
    return (
      <svg
        width="100"
        height="100"
        viewBox="0 0 100 100"
        xmlns="http://www.w3.org/2000/svg"
        fill="currentColor"
        {...props}
        ref={ref}
      >
        <defs>
          <linearGradient
            id="spinner-gradient"
            x1="0%"
            y1="0%"
            x2="100%"
            y2="100%"
          >
            <stop offset="0%" style={{ stopColor: "#555", stopOpacity: 1 }} />
            <stop offset="100%" style={{ stopColor: "#777", stopOpacity: 1 }} />
          </linearGradient>
        </defs>
        <circle
          cx="50"
          cy="50"
          r="35"
          stroke="#777"
          strokeWidth="10"
          fill="none"
        />
        <circle
          cx="50"
          cy="50"
          r="35"
          stroke="url(#spinner-gradient)"
          strokeWidth="10"
          fill="none"
          strokeDasharray="164"
          strokeDashoffset="82"
        >
          <animateTransform
            attributeName="transform"
            type="rotate"
            repeatCount="indefinite"
            dur="1s"
            from="0 50 50"
            to="360 50 50"
          />
        </circle>
      </svg>
    );
  })
);

export default Spinner;
