import { ExtendedEmModule } from "../wasm/_types";

export const writeFileToFS = async (
  emModule: ExtendedEmModule,
  filePath: string,
  writePath: string
) => {
  const file = await fetch(filePath);
  const fileBuffer = await file.arrayBuffer();
  const fileArray = new Uint8Array(fileBuffer);
  emModule.FS.writeFile(writePath, fileArray);
};

export const getUrlFromFS = (
  emModule: ExtendedEmModule,
  path: string
): string => {
  const data = emModule.FS.readFile(path);
  const blob = new Blob([data], { type: "image/png" });
  return URL.createObjectURL(blob);
};
