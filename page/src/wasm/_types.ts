interface ModuleOptions {
  instantiateWasm(
    imports: WebAssembly.Imports,
    successCallback: (instance: WebAssembly.Instance) => void
  ): WebAssembly.Imports;
}

interface extendedFS extends Emscripten.FileSystemType {
  writeFile: typeof FS.writeFile;
  readFile: typeof FS.readFile;
  mkdir: typeof FS.mkdir;
}

export interface ExtendedEmModule extends EmscriptenModule {
  FS: extendedFS;
}

export type WasmModule<T> = (
  options: ModuleOptions
) => Promise<ExtendedEmModule & T>;

export interface EmVector<Value> {
  get: (key: number) => Value;
  size: () => number;
}

export interface EmMap<Key extends number | string, Value> {
  get: (key: Key) => Value;
  keys: () => EmVector<Key>;
}
