import TitleGroup from "@/modules/TitleGroup";

export interface ContentLayoutProps {
  title: string;
  description: string;
  children?: React.ReactNode;
}

const ContentLayout = ({
  title,
  description,
  children,
}: ContentLayoutProps) => {
  return (
    <div>
      <TitleGroup title={title} description={description} />
      <div className="mt-6">{children}</div>
    </div>
  );
};

export default ContentLayout;
