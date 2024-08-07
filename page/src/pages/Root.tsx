import { Outlet } from "react-router-dom";
import Navigation from "@/modules/Navigation";

const RootPage = () => {
  return (
    <div>
      <main className="flex flex-col items-center">
        <Navigation title="Tiny renderer" />
        <div className="max-w-2xl w-full shrink-0 p-8">
          <Outlet />
        </div>
      </main>
    </div>
  );
};

export default RootPage;
