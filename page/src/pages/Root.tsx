import { Outlet, useLocation } from "react-router-dom";
import Navigation from "@/modules/Navigation";
import NavigationPane from "@/modules/NavigationPane";
import useNavigationPane from "@/modules/NavigationPane.hook";
import Sidebar from "@/components/Sidebar";

const RootPage = () => {
  const { isPaneOpen, openPane, closePane } = useNavigationPane();
  const { pathname } = useLocation();

  return (
    <div>
      <main className="flex flex-col items-center justify-center lg:flex-row lg:items-start">
        <Navigation
          title="Tiny renderer"
          onMenuClick={openPane}
          className="lg:hidden"
        />
        <div className="hidden lg:block grow-0 w-80 pr-4">
          <div className="border rounded mt-8">
            <Sidebar currentPath={pathname} />
          </div>
        </div>
        <div className="max-w-2xl w-full shrink-0 p-8">
          <Outlet />
        </div>
      </main>
      {isPaneOpen && (
        <NavigationPane
          onOutsideClick={closePane}
          onLinksClick={closePane}
          currentPath={pathname}
        />
      )}
    </div>
  );
};

export default RootPage;
