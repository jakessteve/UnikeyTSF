export enum InputMethod {
  TELEX = 0,
  VNI = 1,
  VIQR = 2,
}

export enum Charset {
  UNICODE = 0,
  TCVN3 = 1,
  VNI_WIN = 2,
}

export enum ToneType {
  MODERN = 0,  // oà
  CLASSIC = 1, // òa
}

export enum ToggleKey {
  CTRL_SHIFT = 0,
  ALT_Z = 1,
}

export interface UniKeyConfig {
  version: number;
  inputEnabled: boolean;
  inputMethod: InputMethod;
  charset: Charset;
  toneType: ToneType;
  spellCheck: boolean;
  macroEnabled: boolean;
  freeToneMarking: boolean;
  toggleKey: ToggleKey;
  macroFilePath: string;
}

declare global {
  interface Window {
    chrome?: {
      webview?: {
        postMessage: (message: any) => void;
        addEventListener: (type: string, listener: (event: any) => void) => void;
        removeEventListener: (type: string, listener: (event: any) => void) => void;
      };
    };
  }
}
