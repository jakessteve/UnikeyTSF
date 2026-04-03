import React, { useState, useEffect, useCallback } from 'react';
import { SettingsLayout } from './components/SettingsLayout';
import { BasicTab } from './components/tabs/BasicTab';
import { AdvancedTab } from './components/tabs/AdvancedTab';
import { AboutTab } from './components/tabs/AboutTab';
import {
  buildRequestBlacklistMessage,
  buildSetAutostartMessage,
  buildUiReadyMessage,
  buildUpdateBlacklistMessage,
  buildUpdateConfigMessage,
  DEFAULT_UNIKEY_CONFIG,
  parseHostToWebMessage,
  UniKeyConfig,
  WebViewMessageEvent,
} from './types/unikey';

const App: React.FC = () => {
  const [activeTab, setActiveTab] = useState('basic');
  const [config, setConfig] = useState<UniKeyConfig>(DEFAULT_UNIKEY_CONFIG);
  const [blacklist, setBlacklist] = useState<string[]>([]);
  const [autoStart, setAutoStart] = useState(false);

  // Send config updates to C++ host
  const postConfig = useCallback((newConfig: UniKeyConfig) => {
    if (window.chrome?.webview) {
      window.chrome.webview.postMessage(JSON.stringify(buildUpdateConfigMessage(newConfig)));
    } else {
      console.log('WebView2 not detected. Config update:', newConfig);
    }
  }, []);

  // Send blacklist updates to C++ host
  const postBlacklist = useCallback((list: string[]) => {
    if (window.chrome?.webview) {
      window.chrome.webview.postMessage(JSON.stringify(buildUpdateBlacklistMessage(list)));
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
    const handler = (event: WebViewMessageEvent) => {
      const message = parseHostToWebMessage(event.data);
      if (!message) {
        console.error('Failed to parse message from host:', event.data);
        return;
      }

      switch (message.type) {
        case 'SET_CONFIG':
          setConfig(message.payload);
          break;
        case 'BLACKLIST_DATA':
          setBlacklist(message.value);
          break;
        case 'AUTOSTART_STATE':
          setAutoStart(message.enabled);
          break;
      }
    };

    window.chrome?.webview?.addEventListener('message', handler);
    
    // Notify host that we are ready and request initial data
    if (window.chrome?.webview) {
      window.chrome.webview.postMessage(JSON.stringify(buildUiReadyMessage()));
      window.chrome.webview.postMessage(JSON.stringify(buildRequestBlacklistMessage()));
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
                window.chrome.webview.postMessage(JSON.stringify(buildSetAutostartMessage(enabled)));
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
