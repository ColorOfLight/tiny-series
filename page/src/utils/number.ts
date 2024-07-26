export const canParseFloat = (value: unknown): boolean =>
  typeof value == "string" && Number.isNaN(Number.parseFloat(value));
