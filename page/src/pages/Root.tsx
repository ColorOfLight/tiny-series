import { Outlet } from "react-router-dom";
import Navigation from "@/modules/Navigation";
import NavigationPane from "@/modules/NavigationPane";
import useNavigationPane from "@/modules/NavigationPane.hook";

const RootPage = () => {
  const { isPaneOpen, openPane, closePane } = useNavigationPane();

  return (
    <div>
      <main className="flex flex-col items-center">
        <Navigation title="Tiny renderer" onMenuClick={openPane} />
        <div className="max-w-2xl w-full shrink-0 p-8">
          <Outlet />
        </div>
      </main>
      {isPaneOpen && <NavigationPane onOutsideClick={closePane} />}
    </div>
  );
};

export default RootPage;
