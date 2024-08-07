import React from "react";
import ReactDOM from "react-dom/client";
import { createBrowserRouter, RouterProvider } from "react-router-dom";

import Root from "@/pages/Root";
import HomePage from "@/pages/Home";
import TinyRendererPage from "@/pages/tiny-renderer";
import TinyRaytracerPage from "@/pages/tiny-raytracer";
import "@/index.css";

const router = createBrowserRouter([
  {
    element: <Root />,
    children: [
      {
        element: <HomePage />,
        path: "/",
      },
      {
        element: <TinyRendererPage />,
        path: "/tiny-renderer",
      },
      {
        element: <TinyRaytracerPage />,
        path: "/tiny-raytracer",
      },
    ],
  },
]);

ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    <RouterProvider router={router} />
  </React.StrictMode>
);
