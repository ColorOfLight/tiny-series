import TitleGroup from "../modules/TitleGroup";

export interface ContentLayoutProps {
  title: string;
  children?: React.ReactNode;
}

const ContentLayout = ({ title, children }: ContentLayoutProps) => {
  return (
    <main className="p-8 flex flex-col items-center">
      <div className="max-w-2xl w-full shrink-0">
        <TitleGroup
          title={title}
          description="CPU-only model renderer written in C++"
        />
        <div className="mt-6">{children}</div>
      </div>
    </main>
  );
};

export default ContentLayout;
