import React from 'react';
import {
  Tab,
  TabList,
  makeStyles,
  shorthands,
  tokens,
  Text,
  Badge,
  Divider,
} from '@fluentui/react-components';
import {
  SettingsRegular,
  InfoRegular,
  ShieldCheckmarkRegular,
  KeyboardRegular,
} from '@fluentui/react-icons';

const useStyles = makeStyles({
  root: {
    display: 'flex',
    height: '100vh',
    backgroundColor: tokens.colorNeutralBackground1,
  },
  sidebar: {
    width: '240px',
    flexShrink: 0,
    backgroundColor: tokens.colorNeutralBackground3,
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.borderRight('1px', 'solid', tokens.colorNeutralStroke2),
  },
  sidebarHeader: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('12px'),
    ...shorthands.padding('24px', '20px', '20px', '20px'),
  },
  logoIcon: {
    width: '36px',
    height: '36px',
    ...shorthands.borderRadius('8px'),
    backgroundColor: tokens.colorBrandBackground,
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    flexShrink: 0,
    color: tokens.colorNeutralForegroundOnBrand,
  },
  headerText: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('0px'),
    minWidth: 0,
  },
  appName: {
    fontWeight: tokens.fontWeightSemibold,
    fontSize: tokens.fontSizeBase400,
    lineHeight: tokens.lineHeightBase400,
    color: tokens.colorNeutralForeground1,
    whiteSpace: 'nowrap',
    overflowX: 'hidden',
    textOverflow: 'ellipsis',
  },
  appVersion: {
    fontSize: tokens.fontSizeBase100,
    color: tokens.colorNeutralForeground3,
    lineHeight: tokens.lineHeightBase100,
  },
  content: {
    flexGrow: 1,
    minWidth: '300px',
    display: 'flex',
    flexDirection: 'column',
    overflowY: 'auto',
    overflowX: 'hidden',
    backgroundColor: tokens.colorNeutralBackground1,
  },
  header: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    ...shorthands.padding('20px', '24px'),
    ...shorthands.borderBottom('1px', 'solid', tokens.colorNeutralStroke2),
    flexShrink: 0,
  },
  headerTitle: {
    fontWeight: tokens.fontWeightSemibold,
    fontSize: tokens.fontSizeBase500,
    color: tokens.colorNeutralForeground1,
  },
  tabList: {
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'stretch',
    ...shorthands.padding('12px', '12px'),
    ...shorthands.gap('4px'),
    flexGrow: 1,
  },
  mainContent: {
    flexGrow: 1,
    overflowY: 'auto',
  },
  sidebarFooter: {
    ...shorthands.padding('12px', '16px'),
    ...shorthands.borderTop('1px', 'solid', tokens.colorNeutralStroke2),
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('8px'),
  },
  statusDot: {
    width: '8px',
    height: '8px',
    ...shorthands.borderRadius('50%'),
    flexShrink: 0,
  },
  statusDotActive: {
    backgroundColor: '#10b981',
  },
  statusDotInactive: {
    backgroundColor: tokens.colorNeutralForeground3,
  },
  statusText: {
    fontSize: tokens.fontSizeBase200,
    color: tokens.colorNeutralForeground3,
  },
});

const TAB_TITLES: Record<string, string> = {
  basic: 'Cài đặt cơ bản',
  advanced: 'Mở rộng',
  about: 'Thông tin',
};

interface SettingsLayoutProps {
  children: React.ReactNode;
  activeTab: string;
  onTabChange: (tab: string) => void;
  inputEnabled?: boolean;
}

export const SettingsLayout: React.FC<SettingsLayoutProps> = ({
  children,
  activeTab,
  onTabChange,
  inputEnabled = true,
}) => {
  const styles = useStyles();

  return (
    <div className={styles.root}>
      <div className={styles.sidebar}>
        {/* --- Branded Header --- */}
        <div className={styles.sidebarHeader}>
          <div className={styles.logoIcon}>
            <KeyboardRegular fontSize={20} />
          </div>
          <div className={styles.headerText}>
            <Text className={styles.appName}>UniKey TSF</Text>
            <Text className={styles.appVersion}>Reborn v0.1.0</Text>
          </div>
        </div>

        <Divider style={{ opacity: 0.4 }} />

        {/* --- Navigation Tabs --- */}
        <TabList
          vertical
          className={styles.tabList}
          selectedValue={activeTab}
          onTabSelect={(_, data) => onTabChange(data.value as string)}
          size="large"
        >
          <Tab value="basic" icon={<SettingsRegular />}>
            Cơ bản
          </Tab>
          <Tab value="advanced" icon={<ShieldCheckmarkRegular />}>
            Mở rộng
          </Tab>
          <Tab value="about" icon={<InfoRegular />}>
            Thông tin
          </Tab>
        </TabList>

        {/* --- Status Footer --- */}
        <div className={styles.sidebarFooter}>
          <div
            className={`${styles.statusDot} ${
              inputEnabled ? styles.statusDotActive : styles.statusDotInactive
            }`}
          />
          <Text className={styles.statusText}>
            {inputEnabled ? 'Tiếng Việt' : 'English'}
          </Text>
          <Badge
            size="small"
            appearance="filled"
            color={inputEnabled ? 'success' : 'informative'}
            style={{ marginLeft: 'auto' }}
          >
            {inputEnabled ? 'V' : 'E'}
          </Badge>
        </div>
      </div>

      {/* --- Main Content --- */}
      <div className={styles.content}>
        <header className={styles.header}>
          <Text className={styles.headerTitle}>
            {TAB_TITLES[activeTab] || 'Cài đặt'}
          </Text>
        </header>
        <main className={styles.mainContent}>
          <div className="tab-content-enter" key={activeTab}>
            {children}
          </div>
        </main>
      </div>
    </div>
  );
};
