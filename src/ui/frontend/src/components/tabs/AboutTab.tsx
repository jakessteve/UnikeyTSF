import React from 'react';
import {
  Card,
  CardHeader,
  makeStyles,
  shorthands,
  tokens,
  Text,
  Link,
  Divider,
  Badge,
} from '@fluentui/react-components';
import {
  InfoRegular,
  CodeRegular,
  ShieldCheckmarkRegular,
  OpenRegular,
  KeyboardRegular,
} from '@fluentui/react-icons';

const useStyles = makeStyles({
  container: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('16px'),
    ...shorthands.padding('20px', '24px'),
    maxWidth: '600px',
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
  heroSection: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('16px'),
    ...shorthands.padding('20px'),
  },
  heroIcon: {
    width: '56px',
    height: '56px',
    ...shorthands.borderRadius('14px'),
    backgroundColor: tokens.colorBrandBackground,
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    flexShrink: 0,
    color: tokens.colorNeutralForegroundOnBrand,
  },
  heroText: {
    display: 'flex',
    flexDirection: 'column',
    ...shorthands.gap('4px'),
  },
  heroTitle: {
    fontWeight: tokens.fontWeightBold,
    fontSize: tokens.fontSizeBase500,
    lineHeight: tokens.lineHeightBase500,
  },
  heroSubtitle: {
    fontSize: tokens.fontSizeBase200,
    color: tokens.colorNeutralForeground3,
  },
  infoRow: {
    display: 'flex',
    justifyContent: 'space-between',
    alignItems: 'center',
    ...shorthands.padding('6px', '0'),
  },
  infoLabel: {
    color: tokens.colorNeutralForeground3,
    fontSize: tokens.fontSizeBase300,
  },
  infoValue: {
    fontWeight: tokens.fontWeightSemibold,
    fontSize: tokens.fontSizeBase300,
  },
  linkRow: {
    display: 'flex',
    alignItems: 'center',
    ...shorthands.gap('6px'),
  },
});

export const AboutTab: React.FC = () => {
  const styles = useStyles();

  return (
    <div className={styles.container}>
      {/* ───── Hero Section ───── */}
      <Card className={styles.card}>
        <div className={styles.heroSection}>
          <div className={styles.heroIcon}>
            <KeyboardRegular fontSize={28} />
          </div>
          <div className={styles.heroText}>
            <Text className={styles.heroTitle}>UniKey TSF Reborn</Text>
            <Text className={styles.heroSubtitle}>
              Bộ gõ tiếng Việt thế hệ mới trên Windows
            </Text>
          </div>
        </div>
      </Card>

      {/* ───── Details Card ───── */}
      <Card className={styles.card}>
        <CardHeader
          image={<InfoRegular className={styles.sectionIcon} fontSize={20} />}
          header={<Text weight="semibold">Thông tin chi tiết</Text>}
        />
        <div className={styles.cardBody}>
          <div className={styles.infoRow}>
            <Text className={styles.infoLabel}>Phiên bản</Text>
            <Badge appearance="outline" size="medium">v0.1.0</Badge>
          </div>
          <Divider style={{ opacity: 0.3 }} />
          <div className={styles.infoRow}>
            <Text className={styles.infoLabel}>Kiến trúc</Text>
            <Text className={styles.infoValue}>Pure TSF (x86 + x64)</Text>
          </div>
          <Divider style={{ opacity: 0.3 }} />
          <div className={styles.infoRow}>
            <Text className={styles.infoLabel}>Công nghệ</Text>
            <Text className={styles.infoValue}>C++17 / Win32 / WebView2</Text>
          </div>
          <Divider style={{ opacity: 0.3 }} />
          <div className={styles.infoRow}>
            <Text className={styles.infoLabel}>Bản quyền</Text>
            <Text className={styles.infoValue}>GPL-3.0 License</Text>
          </div>
        </div>
      </Card>

      {/* ───── Links Card ───── */}
      <Card className={styles.card}>
        <CardHeader
          image={<CodeRegular className={styles.sectionIcon} fontSize={20} />}
          header={<Text weight="semibold">Liên kết</Text>}
        />
        <div className={styles.cardBody}>
          <div className={styles.linkRow}>
            <OpenRegular fontSize={16} />
            <Link
              href="https://github.com"
              target="_blank"
              rel="noopener noreferrer"
            >
              Mã nguồn trên GitHub
            </Link>
          </div>
          <div className={styles.linkRow}>
            <ShieldCheckmarkRegular fontSize={16} />
            <Link
              href="https://www.gnu.org/licenses/gpl-3.0.html"
              target="_blank"
              rel="noopener noreferrer"
            >
              Giấy phép GPL-3.0
            </Link>
          </div>
        </div>
      </Card>

      {/* ───── Footer ───── */}
      <Text
        style={{
          textAlign: 'center',
          fontSize: tokens.fontSizeBase200,
          color: tokens.colorNeutralForeground3,
          paddingBottom: '12px',
        }}
      >
        Made with ❤️ for the Vietnamese community
      </Text>
    </div>
  );
};
