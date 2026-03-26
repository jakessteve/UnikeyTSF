import React, { useState, useEffect, useCallback } from 'react';
import { SettingsLayout } from './components/SettingsLayout';
import { BasicTab } from './components/tabs/BasicTab';
import { AdvancedTab } from './components/tabs/AdvancedTab';
import { AboutTab } from './components/tabs/AboutTab';
import { UniKeyConfig, InputMethod, Charset, ToneType, ToggleKey } from './types/unikey';

const DEFAULT_CONFIG: UniKeyConfig = {
  version: 3,
  inputEnabled: true,
  inputMethod: InputMethod.TELEX,
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

const App: React.FC = () => {
  const [activeTab, setActiveTab] = useState('basic');
  const [config, setConfig] = useState<UniKeyConfig>(DEFAULT_CONFIG);
  const [blacklist, setBlacklist] = useState<string[]>([]);
  const [autoStart, setAutoStart] = useState(false);

  // Send config updates to C++ host
  const postConfig = useCallback((newConfig: UniKeyConfig) => {
    if (window.chrome?.webview) {
      window.chrome.webview.postMessage(JSON.stringify({
        type: 'UPDATE_CONFIG',
        payload: newConfig
      }));
    } else {
      console.log('WebView2 not detected. Config update:', newConfig);
    }
  }, []);

  // Send blacklist updates to C++ host
  const postBlacklist = useCallback((list: string[]) => {
    if (window.chrome?.webview) {
      window.chrome.webview.postMessage(JSON.stringify({
        type: 'UPDATE_BLACKLIST',
        value: list
      }));
    } else {
      console.log('WebView2 not detected. Blacklist update:', list);
    }
  }, []);

  const handleConfigChange = (updates: Partial<UniKeyConfig>) => {
    setConfig(prev => {
      const next = { ...prev, ...updates };
      postConfig(next);
      return next;
    });
  };

  const handleBlacklistChange = (list: string[]) => {
    setBlacklist(list);
    postBlacklist(list);
  };

  // Listen for config from C++ host
  useEffect(() => {
    const handler = (event: any) => {
      try {
        const message = typeof event.data === 'string' ? JSON.parse(event.data) : event.data;
        if (message.type === 'SET_CONFIG') {
          setConfig(message.payload);
        } else if (message.type === 'BLACKLIST_DATA') {
          setBlacklist(message.value || []);
        } else if (message.type === 'AUTOSTART_STATE') {
          setAutoStart(message.enabled || false);
        }
      } catch (e) {
        console.error('Failed to parse message from host:', e);
      }
    };

    window.chrome?.webview?.addEventListener('message', handler);
    
    // Notify host that we are ready and request initial data
    if (window.chrome?.webview) {
      window.chrome.webview.postMessage(JSON.stringify({ type: 'UI_READY' }));
      window.chrome.webview.postMessage(JSON.stringify({ type: 'REQ_BLACKLIST' }));
    }

    return () => {
      window.chrome?.webview?.removeEventListener('message', handler);
    };
  }, []);

  return (
    <SettingsLayout
      activeTab={activeTab}
      onTabChange={setActiveTab}
      inputEnabled={config.inputEnabled}
    >
      {activeTab === 'basic' && (
        <BasicTab
          config={config}
          onChange={handleConfigChange}
          autoStart={autoStart}
          onAutoStartChange={(enabled: boolean) => {
            setAutoStart(enabled);
            if (window.chrome?.webview) {
              window.chrome.webview.postMessage(JSON.stringify({
                type: 'SET_AUTOSTART',
                enabled
              }));
            }
          }}
        />
      )}
      {activeTab === 'advanced' && (
        <AdvancedTab 
          config={config}
          onChange={handleConfigChange}
          blacklist={blacklist} 
          onBlacklistChange={handleBlacklistChange} 
        />
      )}
      {activeTab === 'about' && (
        <AboutTab />
      )}
    </SettingsLayout>
  );
};

export default App;
