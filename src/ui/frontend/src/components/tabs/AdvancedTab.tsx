import React, { useState } from 'react';
import {
  Card,
  CardHeader,
  Input,
  Button,
  makeStyles,
  shorthands,
  tokens,
  Text,
  Tooltip,
  Switch,
} from '@fluentui/react-components';
import {
  ShieldCheckmarkRegular,
  AddRegular,
  DeleteRegular,
  AppGenericRegular,
  SearchRegular,
  SettingsRegular,
} from '@fluentui/react-icons';
import type { UniKeyConfig } from '../../types/unikey';

const useStyles = makeStyles({
  container: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('16px'),
    ...shorthands.padding('20px', '24px'),
    maxWidth: '600px',
  },
  sectionIcon: {
    color: tokens.colorBrandForeground1,
  },
  card: {
    ...shorthands.padding('0px'),
  },
  cardBody: {
    ...shorthands.padding('4px', '16px', '16px', '16px'),
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('12px'),
  },
  addRow: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('8px'),
  },
  listContainer: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('4px'),
    maxHeight: '280px',
    overflowY: 'auto',
    ...shorthands.padding('4px', '0'),
  },
  listItem: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    ...shorthands.padding('8px', '12px'),
    ...shorthands.borderRadius(tokens.borderRadiusMedium),
    backgroundColor: tokens.colorNeutralBackground3,
    ':hover': {
      backgroundColor: tokens.colorNeutralBackground3Hover,
    },
  },
  listItemLeft: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('10px'),
    minWidth: 0,
  },
  listItemIcon: {
    color: tokens.colorNeutralForeground3,
    flexShrink: 0,
  },
  listItemText: {
    overflowX: 'hidden',
    textOverflow: 'ellipsis',
    whiteSpace: 'nowrap',
  },
  emptyState: {
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    justifyContent: 'center',
    ...shorthands.padding('32px', '20px'),
    ...shorthands.gap('8px'),
    opacity: 0.6,
  },
  emptyIcon: {
    color: tokens.colorNeutralForeground3,
    marginBottom: '4px',
  },
  emptyText: {
    fontSize: tokens.fontSizeBase200,
    color: tokens.colorNeutralForeground3,
    textAlign: 'center',
  },
  countBadge: {
    fontSize: tokens.fontSizeBase200,
    color: tokens.colorNeutralForeground3,
  },
});

interface AdvancedTabProps {
  config: UniKeyConfig;
  onChange: (updates: Partial<UniKeyConfig>) => void;
  blacklist: string[];
  onBlacklistChange: (list: string[]) => void;
}

export const AdvancedTab: React.FC<AdvancedTabProps> = ({
  config,
  onChange,
  blacklist,
  onBlacklistChange,
}) => {
  const styles = useStyles();
  const [newEntry, setNewEntry] = useState('');

  const handleAdd = () => {
    const trimmed = newEntry.trim();
    if (!trimmed) return;
    // Check duplicate (case-insensitive)
    if (blacklist.some((b) => b.toLowerCase() === trimmed.toLowerCase())) {
      setNewEntry('');
      return;
    }
    onBlacklistChange([...blacklist, trimmed]);
    setNewEntry('');
  };

  const handleRemove = (index: number) => {
    onBlacklistChange(blacklist.filter((_, i) => i !== index));
  };

  const handleKeyDown = (e: React.KeyboardEvent) => {
    if (e.key === 'Enter') {
      e.preventDefault();
      handleAdd();
    }
  };

  return (
    <div className={styles.container}>
      <Card className={styles.card}>
        <CardHeader
          image={<SettingsRegular className={styles.sectionIcon} fontSize={20} />}
          header={<Text weight="semibold">Tùy chọn khác</Text>}
        />
        <div className={styles.cardBody} style={{ gap: '4px', paddingBottom: '20px' }}>
          <Switch 
            checked={config.restoreKeyEnabled} 
            onChange={(_, data) => onChange({ restoreKeyEnabled: data.checked })}
            label="Cho phép khôi phục phím với từ sai" 
          />
          <Switch 
            checked={config.useClipboardForUnicode} 
            onChange={(_, data) => onChange({ useClipboardForUnicode: data.checked })}
            label="Luôn sử dụng clipboard cho unicode" 
          />
          <Switch 
            checked={config.showDialogOnStartup} 
            onChange={(_, data) => onChange({ showDialogOnStartup: data.checked })}
            label="Bật hộp thoại này khi khởi động" 
          />
          <Switch 
            checked={config.perAppInputState} 
            onChange={(_, data) => onChange({ perAppInputState: data.checked })}
            label="Ghi nhớ trạng thái E/V theo từng ứng dụng" 
          />
        </div>
      </Card>

      <Card className={styles.card}>
        <CardHeader
          image={
            <ShieldCheckmarkRegular className={styles.sectionIcon} fontSize={20} />
          }
          header={<Text weight="semibold">Ứng dụng loại trừ</Text>}
          description={
            <Text className={styles.countBadge}>
              {blacklist.length > 0
                ? `${blacklist.length} ứng dụng`
                : 'Chưa có ứng dụng nào'}
            </Text>
          }
        />
        <div className={styles.cardBody}>
          <Text
            style={{
              fontSize: tokens.fontSizeBase200,
              color: tokens.colorNeutralForeground3,
            }}
          >
            Bộ gõ tiếng Việt sẽ tự động tắt khi bạn sử dụng các ứng dụng trong
            danh sách này.
          </Text>

          {/* Add new entry */}
          <div className={styles.addRow}>
            <Input
              style={{ flexGrow: 1 }}
              placeholder="Nhập tên process (vd: code.exe)..."
              value={newEntry}
              onChange={(_, data) => setNewEntry(data.value)}
              onKeyDown={handleKeyDown}
              contentBefore={<SearchRegular fontSize={16} />}
            />
            <Tooltip content="Thêm ứng dụng" relationship="label">
              <Button
                appearance="primary"
                icon={<AddRegular />}
                onClick={handleAdd}
                disabled={!newEntry.trim()}
              >
                Thêm
              </Button>
            </Tooltip>
          </div>

          {/* Blacklist Items */}
          {blacklist.length === 0 ? (
            <div className={styles.emptyState}>
              <AppGenericRegular className={styles.emptyIcon} fontSize={48} />
              <Text className={styles.emptyText}>
                Chưa có ứng dụng nào được loại trừ.
                <br />
                Thêm tên process (.exe) để bộ gõ tự động tắt.
              </Text>
            </div>
          ) : (
            <div className={styles.listContainer}>
              {blacklist.map((entry, index) => (
                <div key={`${entry}-${index}`} className={styles.listItem}>
                  <div className={styles.listItemLeft}>
                    <AppGenericRegular
                      className={styles.listItemIcon}
                      fontSize={18}
                    />
                    <Text className={styles.listItemText}>{entry}</Text>
                  </div>
                  <Tooltip content="Xóa" relationship="label">
                    <Button
                      appearance="subtle"
                      icon={<DeleteRegular />}
                      size="small"
                      onClick={() => handleRemove(index)}
                      aria-label={`Xóa ${entry}`}
                    />
                  </Tooltip>
                </div>
              ))}
            </div>
          )}
        </div>
      </Card>
    </div>
  );
};
