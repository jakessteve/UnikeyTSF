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
  makeStyles,
  shorthands,
  tokens,
} from '@fluentui/react-components';
import { FolderRegular } from '@fluentui/react-icons';
import { UniKeyConfig, InputMethod, Charset, ToneType, ToggleKey } from '../../types/unikey';

const useStyles = makeStyles({
  container: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('20px'),
    ...shorthands.padding('20px'),
  },
  section: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('10px'),
    ...shorthands.border('1px', 'solid', tokens.colorNeutralStroke1),
    ...shorthands.padding('15px'),
    ...shorthands.borderRadius(tokens.borderRadiusMedium),
  },
  sectionTitle: {
    fontWeight: tokens.fontWeightSemibold,
    marginBottom: '5px',
  },
  row: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('10px'),
  },
  dropdown: {
    minWidth: '200px',
    flexGrow: 1,
  },
  macroRow: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('10px'),
    marginTop: '5px',
  },
});

interface BasicTabProps {
  config: UniKeyConfig;
  onChange: (updates: Partial<UniKeyConfig>) => void;
}

export const BasicTab: React.FC<BasicTabProps> = ({ config, onChange }) => {
  const styles = useStyles();

  return (
    <div className={styles.container}>
      <div className={styles.section}>
        <Label className={styles.sectionTitle}>Cài đặt gõ</Label>
        <div className={styles.row}>
          <Label htmlFor="input-method">Kiểu gõ:</Label>
          <Dropdown
            id="input-method"
            className={styles.dropdown}
            value={InputMethod[config.inputMethod]}
            selectedOptions={[config.inputMethod.toString()]}
            onOptionSelect={(_, data) => onChange({ inputMethod: Number(data.selectedOptions[0]) })}
          >
            <Option value={InputMethod.TELEX.toString()}>Telex</Option>
            <Option value={InputMethod.VNI.toString()}>VNI</Option>
            <Option value={InputMethod.VIQR.toString()}>VIQR</Option>
          </Dropdown>
        </div>
        <div className={styles.row}>
          <Label htmlFor="charset">Bảng mã:</Label>
          <Dropdown
            id="charset"
            className={styles.dropdown}
            value={Charset[config.charset]}
            selectedOptions={[config.charset.toString()]}
            onOptionSelect={(_, data) => onChange({ charset: Number(data.selectedOptions[0]) })}
          >
            <Option value={Charset.UNICODE.toString()}>Unicode</Option>
            <Option value={Charset.TCVN3.toString()}>TCVN3 (ABC)</Option>
            <Option value={Charset.VNI_WIN.toString()}>VNI Windows</Option>
          </Dropdown>
        </div>
      </div>

      <div style={{ display: 'flex', gap: '20px' }}>
        <div className={styles.section} style={{ flex: 1 }}>
          <Label className={styles.sectionTitle}>Phím chuyển</Label>
          <RadioGroup
            value={config.toggleKey.toString()}
            onChange={(_, data) => onChange({ toggleKey: Number(data.value) })}
          >
            <Radio value={ToggleKey.CTRL_SHIFT.toString()} label="Ctrl+Shift" />
            <Radio value={ToggleKey.ALT_Z.toString()} label="Alt+Z" />
          </RadioGroup>
        </div>

        <div className={styles.section} style={{ flex: 1 }}>
          <Label className={styles.sectionTitle}>Kiểu dấu</Label>
          <RadioGroup
            value={config.toneType.toString()}
            onChange={(_, data) => onChange({ toneType: Number(data.value) })}
          >
            <Radio value={ToneType.MODERN.toString()} label="Mới (hoá)" />
            <Radio value={ToneType.CLASSIC.toString()} label="Cũ (hóa)" />
          </RadioGroup>
        </div>
      </div>

      <div className={styles.section}>
        <Label className={styles.sectionTitle}>Tùy chọn</Label>
        <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '10px' }}>
          <Checkbox
            label="Kiểm tra chính tả"
            checked={config.spellCheck}
            onChange={(_, data) => onChange({ spellCheck: !!data.checked })}
          />
          <Checkbox
            label="Bỏ dấu tự do"
            checked={config.freeToneMarking}
            onChange={(_, data) => onChange({ freeToneMarking: !!data.checked })}
          />
          <Checkbox
            label="Bật gõ tắt (Macro)"
            checked={config.macroEnabled}
            onChange={(_, data) => onChange({ macroEnabled: !!data.checked })}
          />
          <Checkbox
            label="Khởi động cùng Windows"
            checked={false}
            disabled
          />
        </div>
        <div className={styles.macroRow}>
          <Label size="small">Macro:</Label>
          <Input
            size="small"
            style={{ flexGrow: 1 }}
            value={config.macroFilePath}
            readOnly
          />
          <Button size="small" icon={<FolderRegular />} />
        </div>
      </div>
    </div>
  );
};
