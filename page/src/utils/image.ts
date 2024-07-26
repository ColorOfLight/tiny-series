export const generateUrlFromBuffer = (buffer: ArrayBufferLike) => {
  const fileContent = new Uint8Array(buffer);
  const blob = new Blob([fileContent], { type: "image/png" });
  return URL.createObjectURL(blob);
};
