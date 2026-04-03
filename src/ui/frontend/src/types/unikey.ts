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

export const UNIKEY_CONFIG_VERSION = 2;

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
  restoreKeyEnabled: boolean;
  useClipboardForUnicode: boolean;
  showDialogOnStartup: boolean;
  perAppInputState: boolean;
  macroFilePath: string;
}

export const DEFAULT_UNIKEY_CONFIG: Readonly<UniKeyConfig> = {
  version: UNIKEY_CONFIG_VERSION,
  inputEnabled: true,
  inputMethod: InputMethod.VNI,
  charset: Charset.UNICODE,
  toneType: ToneType.MODERN,
  spellCheck: true,
  macroEnabled: false,
  freeToneMarking: true,
  toggleKey: ToggleKey.CTRL_SHIFT,
  restoreKeyEnabled: true,
  useClipboardForUnicode: false,
  showDialogOnStartup: true,
  perAppInputState: false,
  macroFilePath: '',
};

export interface WebViewMessageEvent {
  data: unknown;
}

export interface SetConfigMessage {
  type: 'SET_CONFIG';
  payload: UniKeyConfig;
}

export interface BlacklistDataMessage {
  type: 'BLACKLIST_DATA';
  value: string[];
}

export interface AutostartStateMessage {
  type: 'AUTOSTART_STATE';
  enabled: boolean;
}

export type HostToWebMessage = SetConfigMessage | BlacklistDataMessage | AutostartStateMessage;

export interface UpdateConfigMessage {
  type: 'UPDATE_CONFIG';
  payload: UniKeyConfig;
}

export interface UpdateBlacklistMessage {
  type: 'UPDATE_BLACKLIST';
  value: string[];
}

export interface SetAutostartMessage {
  type: 'SET_AUTOSTART';
  enabled: boolean;
}

export interface UiReadyMessage {
  type: 'UI_READY';
}

export interface RequestBlacklistMessage {
  type: 'REQ_BLACKLIST';
}

export type WebToHostMessage =
  | UpdateConfigMessage
  | UpdateBlacklistMessage
  | SetAutostartMessage
  | UiReadyMessage
  | RequestBlacklistMessage;

function isRecord(value: unknown): value is Record<string, unknown> {
  return typeof value === 'object' && value !== null;
}

function isBoolean(value: unknown): value is boolean {
  return typeof value === 'boolean';
}

function isFiniteNumber(value: unknown): value is number {
  return typeof value === 'number' && Number.isFinite(value);
}

function isString(value: unknown): value is string {
  return typeof value === 'string';
}

function isStringArray(value: unknown): value is string[] {
  return Array.isArray(value) && value.every(isString);
}

function isEnumValue(value: unknown, enumObject: object): value is number {
  if (!isFiniteNumber(value) || !Number.isInteger(value)) {
    return false;
  }

  return Object.values(enumObject).some((enumValue) => typeof enumValue === 'number' && enumValue === value);
}

export function isUniKeyConfig(value: unknown): value is UniKeyConfig {
  if (!isRecord(value)) {
    return false;
  }

  return value.version === UNIKEY_CONFIG_VERSION
    && isBoolean(value.inputEnabled)
    && isEnumValue(value.inputMethod, InputMethod)
    && isEnumValue(value.charset, Charset)
    && isEnumValue(value.toneType, ToneType)
    && isBoolean(value.spellCheck)
    && isBoolean(value.macroEnabled)
    && isBoolean(value.freeToneMarking)
    && isEnumValue(value.toggleKey, ToggleKey)
    && isBoolean(value.restoreKeyEnabled)
    && isBoolean(value.useClipboardForUnicode)
    && isBoolean(value.showDialogOnStartup)
    && isBoolean(value.perAppInputState)
    && isString(value.macroFilePath);
}

function normalizeHostMessagePayload(data: unknown): unknown {
  if (isString(data)) {
    try {
      return JSON.parse(data) as unknown;
    } catch {
      return null;
    }
  }

  return data;
}

export function parseHostToWebMessage(data: unknown): HostToWebMessage | null {
  const payload = normalizeHostMessagePayload(data);
  if (!isRecord(payload) || !isString(payload.type)) {
    return null;
  }

  switch (payload.type) {
    case 'SET_CONFIG':
      return isUniKeyConfig(payload.payload)
        ? { type: 'SET_CONFIG', payload: payload.payload }
        : null;
    case 'BLACKLIST_DATA':
      return isStringArray(payload.value)
        ? { type: 'BLACKLIST_DATA', value: payload.value }
        : null;
    case 'AUTOSTART_STATE':
      return isBoolean(payload.enabled)
        ? { type: 'AUTOSTART_STATE', enabled: payload.enabled }
        : null;
    default:
      return null;
  }
}

export function buildUpdateConfigMessage(config: UniKeyConfig): UpdateConfigMessage {
  return { type: 'UPDATE_CONFIG', payload: config };
}

export function buildUpdateBlacklistMessage(value: string[]): UpdateBlacklistMessage {
  return { type: 'UPDATE_BLACKLIST', value };
}

export function buildSetAutostartMessage(enabled: boolean): SetAutostartMessage {
  return { type: 'SET_AUTOSTART', enabled };
}

export function buildUiReadyMessage(): UiReadyMessage {
  return { type: 'UI_READY' };
}

export function buildRequestBlacklistMessage(): RequestBlacklistMessage {
  return { type: 'REQ_BLACKLIST' };
}

declare global {
  interface Window {
    chrome?: {
      webview?: {
        postMessage: (message: string) => void;
        addEventListener: (type: 'message', listener: (event: WebViewMessageEvent) => void) => void;
        removeEventListener: (type: 'message', listener: (event: WebViewMessageEvent) => void) => void;
      };
    };
  }
}
