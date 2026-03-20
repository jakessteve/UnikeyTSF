import React, { useState, useEffect, useCallback } from 'react';
import { SettingsLayout } from './components/SettingsLayout';
import { BasicTab } from './components/tabs/BasicTab';
import { UniKeyConfig, InputMethod, Charset, ToneType, ToggleKey } from './types/unikey';

const DEFAULT_CONFIG: UniKeyConfig = {
  version: 2,
  inputEnabled: true,
  inputMethod: InputMethod.TELEX,
  charset: Charset.UNICODE,
  toneType: ToneType.MODERN,
  spellCheck: true,
  macroEnabled: false,
  freeToneMarking: true,
  toggleKey: ToggleKey.CTRL_SHIFT,
  macroFilePath: '',
};

const App: React.FC = () => {
  const [activeTab, setActiveTab] = useState('basic');
  const [config, setConfig] = useState<UniKeyConfig>(DEFAULT_CONFIG);

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

  const handleConfigChange = (updates: Partial<UniKeyConfig>) => {
    setConfig(prev => {
      const next = { ...prev, ...updates };
      postConfig(next);
      return next;
    });
  };

  // Listen for config from C++ host
  useEffect(() => {
    const handler = (event: any) => {
      try {
        const message = typeof event.data === 'string' ? JSON.parse(event.data) : event.data;
        if (message.type === 'SET_CONFIG') {
          setConfig(message.payload);
        }
      } catch (e) {
        console.error('Failed to parse message from host:', e);
      }
    };

    window.chrome?.webview?.addEventListener('message', handler);
    
    // Notify host that we are ready
    if (window.chrome?.webview) {
      window.chrome.webview.postMessage(JSON.stringify({ type: 'UI_READY' }));
    }

    return () => {
      window.chrome?.webview?.removeEventListener('message', handler);
    };
  }, []);

  return (
    <SettingsLayout activeTab={activeTab} onTabChange={setActiveTab}>
      {activeTab === 'basic' && (
        <BasicTab config={config} onChange={handleConfigChange} />
      )}
      {activeTab === 'advanced' && (
        <div style={{ padding: '20px' }}>Advanced settings coming soon...</div>
      )}
      {activeTab === 'about' && (
        <div style={{ padding: '20px' }}>UniKey TSF Reborn v0.1.0</div>
      )}
    </SettingsLayout>
  );
};

export default App;
