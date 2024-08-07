import { Outlet } from "react-router-dom";
import Navigation from "../modules/Navigation";
import Divider from "../components/Divider";

const RootPage = () => {
  return (
    <div>
      <main className="p-8 flex flex-col items-center">
        <div className="max-w-2xl w-full shrink-0">
          <Navigation title="Tiny renderer" />

          <Divider className="mt-4" />

          <Outlet />
        </div>
      </main>
    </div>
  );
};

export default RootPage;
