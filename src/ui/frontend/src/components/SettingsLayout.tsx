import React from 'react';
import {
  Tab,
  TabList,
  makeStyles,
  shorthands,
  tokens,
  Title2,
} from '@fluentui/react-components';
import {
  SettingsRegular,
  InfoRegular,
  OptionsRegular,
} from '@fluentui/react-icons';

const useStyles = makeStyles({
  root: {
    display: 'flex',
    height: '100vh',
    backgroundColor: tokens.colorNeutralBackground1,
  },
  sidebar: {
    width: '200px',
    backgroundColor: tokens.colorNeutralBackground2,
    ...shorthands.borderRight('1px', 'solid', tokens.colorNeutralStroke1),
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.padding('20px', '0'),
  },
  content: {
    flexGrow: 1,
    display: 'flex',
    flexDirection: 'column',
    overflowY: 'auto',
  },
  header: {
    ...shorthands.padding('20px'),
    ...shorthands.borderBottom('1px', 'solid', tokens.colorNeutralStroke1),
  },
  tabList: {
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'flex-start',
    ...shorthands.padding('0', '10px'),
  },
});

interface SettingsLayoutProps {
  children: React.ReactNode;
  activeTab: string;
  onTabChange: (tab: string) => void;
}

export const SettingsLayout: React.FC<SettingsLayoutProps> = ({ children, activeTab, onTabChange }) => {
  const styles = useStyles();

  return (
    <div className={styles.root}>
      <div className={styles.sidebar}>
        <TabList
          vertical
          className={styles.tabList}
          selectedValue={activeTab}
          onTabSelect={(_, data) => onTabChange(data.value as string)}
        >
          <Tab value="basic" icon={<SettingsRegular />}>
            Cơ bản
          </Tab>
          <Tab value="advanced" icon={<OptionsRegular />}>
            Mở rộng
          </Tab>
          <Tab value="about" icon={<InfoRegular />}>
            Thông tin
          </Tab>
        </TabList>
      </div>
      <div className={styles.content}>
        <header className={styles.header}>
          <Title2>
            {activeTab === 'basic' ? 'Cài đặt cơ bản' : 
             activeTab === 'advanced' ? 'Cài đặt mở rộng' : 
             'Thông tin phần mềm'}
          </Title2>
        </header>
        <main style={{ flexGrow: 1 }}>
          {children}
        </main>
      </div>
    </div>
  );
};
