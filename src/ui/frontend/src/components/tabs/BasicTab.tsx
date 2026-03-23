import React from 'react';
import {
  Dropdown,
  Option,
  Label,
  RadioGroup,
  Radio,
  Checkbox,
  Input,
  Button,
  Card,
  CardHeader,
  Switch,
  makeStyles,
  shorthands,
  tokens,
  Tooltip,
  Text,
  Divider,
} from '@fluentui/react-components';
import {
  FolderRegular,
  KeyboardRegular,
  TextCaseTitleRegular,
  ToggleLeftRegular,
  OptionsFilled,
} from '@fluentui/react-icons';
import { UniKeyConfig, InputMethod, Charset, ToneType, ToggleKey } from '../../types/unikey';

const useStyles = makeStyles({
  container: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('16px'),
    ...shorthands.padding('20px', '24px'),
    maxWidth: '600px',
  },
  veToggleCard: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    ...shorthands.padding('16px', '20px'),
    ...shorthands.borderRadius(tokens.borderRadiusLarge),
  },
  veToggleLeft: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('12px'),
  },
  veIconWrapper: {
    width: '40px',
    height: '40px',
    ...shorthands.borderRadius('10px'),
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    flexShrink: 0,
  },
  veIconEnabled: {
    backgroundColor: 'rgba(16, 185, 129, 0.15)',
    color: '#10b981',
  },
  veIconDisabled: {
    backgroundColor: 'rgba(148, 163, 184, 0.15)',
    color: tokens.colorNeutralForeground3,
  },
  veTextBlock: {
    display: 'flex',
    flexDirection: 'column',
  },
  veTitle: {
    fontWeight: tokens.fontWeightSemibold,
    fontSize: tokens.fontSizeBase300,
  },
  veSubtitle: {
    fontSize: tokens.fontSizeBase200,
    color: tokens.colorNeutralForeground3,
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
  sectionIcon: {
    color: tokens.colorBrandForeground1,
  },
  row: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('12px'),
    flexWrap: 'wrap',
  },
  label: {
    minWidth: '80px',
    flexShrink: 0,
  },
  dropdown: {
    minWidth: '180px',
    flexGrow: 1,
  },
  radioSectionWrapper: {
    display: 'flex',
    ...shorthands.gap('12px'),
    flexWrap: 'wrap',
  },
  radioCard: {
    flexGrow: 1,
    flexBasis: '45%',
    minWidth: '200px',
    ...shorthands.padding('0px'),
  },
  radioCardBody: {
    ...shorthands.padding('4px', '16px', '12px', '16px'),
  },
  optionsGrid: {
    display: 'grid',
    gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))',
    ...shorthands.gap('8px'),
    alignItems: 'start',
  },
  helperText: {
    color: tokens.colorNeutralForeground3,
    fontSize: tokens.fontSizeBase200,
  },
  macroRow: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('8px'),
    marginTop: '4px',
    marginLeft: '28px',
  },
  macroInputWrapper: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('4px'),
  },
});

interface BasicTabProps {
  config: UniKeyConfig;
  onChange: (updates: Partial<UniKeyConfig>) => void;
  autoStart?: boolean;
  onAutoStartChange?: (enabled: boolean) => void;
}

export const BasicTab: React.FC<BasicTabProps> = ({ config, onChange, autoStart = false, onAutoStartChange }) => {
  const styles = useStyles();

  return (
    <div className={styles.container}>
      {/* ───── V/E Toggle ───── */}
      <Card className={styles.veToggleCard}>
        <div className={styles.veToggleLeft}>
          <div
            className={`${styles.veIconWrapper} ${
              config.inputEnabled ? styles.veIconEnabled : styles.veIconDisabled
            }`}
          >
            <KeyboardRegular fontSize={22} />
          </div>
          <div className={styles.veTextBlock}>
            <Text className={styles.veTitle}>
              {config.inputEnabled ? 'Tiếng Việt' : 'English'}
            </Text>
            <Text className={styles.veSubtitle}>
              {config.inputEnabled
                ? 'Đang gõ tiếng Việt'
                : 'Vietnamese input disabled'}
            </Text>
          </div>
        </div>
        <Switch
          checked={config.inputEnabled}
          onChange={(_, data) => onChange({ inputEnabled: data.checked })}
          label={config.inputEnabled ? 'V' : 'E'}
        />
      </Card>

      {/* ───── Input Settings Card ───── */}
      <Card className={styles.card}>
        <CardHeader
          image={<KeyboardRegular className={styles.sectionIcon} fontSize={20} />}
          header={<Text weight="semibold">Cài đặt gõ</Text>}
        />
        <div className={styles.cardBody}>
          <div className={styles.row}>
            <Label htmlFor="input-method" className={styles.label}>Kiểu gõ</Label>
            <Dropdown
              id="input-method"
              className={styles.dropdown}
              value={InputMethod[config.inputMethod]}
              selectedOptions={[config.inputMethod.toString()]}
              onOptionSelect={(_, data) =>
                onChange({ inputMethod: Number(data.selectedOptions[0]) })
              }
            >
              <Option value={InputMethod.TELEX.toString()}>Telex</Option>
              <Option value={InputMethod.VNI.toString()}>VNI</Option>
              <Option value={InputMethod.VIQR.toString()}>VIQR</Option>
            </Dropdown>
          </div>
          <div className={styles.row}>
            <Label htmlFor="charset" className={styles.label}>Bảng mã</Label>
            <Dropdown
              id="charset"
              className={styles.dropdown}
              value={Charset[config.charset]}
              selectedOptions={[config.charset.toString()]}
              onOptionSelect={(_, data) =>
                onChange({ charset: Number(data.selectedOptions[0]) })
              }
            >
              <Option value={Charset.UNICODE.toString()}>Unicode</Option>
              <Option value={Charset.TCVN3.toString()}>TCVN3 (ABC)</Option>
              <Option value={Charset.VNI_WIN.toString()}>VNI Windows</Option>
            </Dropdown>
          </div>
        </div>
      </Card>

      {/* ───── Toggle Key & Tone Type Cards ───── */}
      <div className={styles.radioSectionWrapper}>
        <Card className={styles.radioCard}>
          <CardHeader
            image={<ToggleLeftRegular className={styles.sectionIcon} fontSize={20} />}
            header={<Text weight="semibold">Phím chuyển</Text>}
          />
          <div className={styles.radioCardBody}>
            <RadioGroup
              value={config.toggleKey.toString()}
              onChange={(_, data) => onChange({ toggleKey: Number(data.value) })}
            >
              <Radio value={ToggleKey.CTRL_SHIFT.toString()} label="Ctrl+Shift" />
              <Radio value={ToggleKey.ALT_Z.toString()} label="Alt+Z" />
            </RadioGroup>
          </div>
        </Card>

        <Card className={styles.radioCard}>
          <CardHeader
            image={<TextCaseTitleRegular className={styles.sectionIcon} fontSize={20} />}
            header={<Text weight="semibold">Kiểu dấu</Text>}
          />
          <div className={styles.radioCardBody}>
            <RadioGroup
              value={config.toneType.toString()}
              onChange={(_, data) => onChange({ toneType: Number(data.value) })}
            >
              <Radio
                value={ToneType.MODERN.toString()}
                label={
                  <div style={{ display: 'flex', flexDirection: 'column' }}>
                    <Text>Mới (hoá)</Text>
                    <Text className={styles.helperText}>Bỏ dấu hòa thay vì hỏa</Text>
                  </div>
                }
              />
              <Radio
                value={ToneType.CLASSIC.toString()}
                label={
                  <div style={{ display: 'flex', flexDirection: 'column' }}>
                    <Text>Cũ (hóa)</Text>
                    <Text className={styles.helperText}>Quy tắc truyền thống</Text>
                  </div>
                }
              />
            </RadioGroup>
          </div>
        </Card>
      </div>

      {/* ───── Options Card ───── */}
      <Card className={styles.card}>
        <CardHeader
          image={<OptionsFilled className={styles.sectionIcon} fontSize={20} />}
          header={<Text weight="semibold">Tùy chọn</Text>}
        />
        <div className={styles.cardBody}>
          <div className={styles.optionsGrid}>
            <Tooltip
              content="Bật kiểm tra chính tả cơ bản để chặn gõ tiếng Việt sai"
              relationship="description"
            >
              <Checkbox
                label="Kiểm tra chính tả"
                checked={config.spellCheck}
                onChange={(_, data) => onChange({ spellCheck: !!data.checked })}
              />
            </Tooltip>

            <Tooltip
              content="Cho phép đặt dấu thanh ở bất cứ đâu trong từ"
              relationship="description"
            >
              <Checkbox
                label="Bỏ dấu tự do"
                checked={config.freeToneMarking}
                onChange={(_, data) => onChange({ freeToneMarking: !!data.checked })}
              />
            </Tooltip>

            <div className={styles.macroInputWrapper}>
              <Tooltip content="Sử dụng bảng gõ tắt cá nhân" relationship="description">
                <Checkbox
                  label="Bật gõ tắt (Macro)"
                  checked={config.macroEnabled}
                  onChange={(_, data) => onChange({ macroEnabled: !!data.checked })}
                />
              </Tooltip>
              {config.macroEnabled && (
                <div className={`${styles.macroRow} macro-expand`}>
                  <Input
                    size="small"
                    style={{ flexGrow: 1 }}
                    value={config.macroFilePath}
                    placeholder="Đường dẫn file macro..."
                    readOnly
                  />
                  <Tooltip content="Chọn bộ gõ tắt mới" relationship="label">
                    <Button
                      size="small"
                      icon={<FolderRegular />}
                      aria-label="Chọn file macro"
                    />
                  </Tooltip>
                </div>
              )}
            </div>

            <Tooltip
              content="Tự động khởi chạy khi bạn đăng nhập vào Windows"
              relationship="description"
            >
              <Checkbox
                label="Khởi động cùng Windows"
                checked={autoStart}
                onChange={(_, data) => onAutoStartChange?.(!!data.checked)}
              />
            </Tooltip>
          </div>
        </div>
      </Card>
    </div>
  );
};
