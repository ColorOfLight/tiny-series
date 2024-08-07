import TitleGroup from "../modules/TitleGroup";

export interface ContentLayoutProps {
  title: string;
  children?: React.ReactNode;
}

const ContentLayout = ({ title, children }: ContentLayoutProps) => {
  return (
    <div>
      <TitleGroup
        title={title}
        description="CPU-only model renderer written in C++"
      />
      <div className="mt-6">{children}</div>
    </div>
  );
};

export default ContentLayout;
