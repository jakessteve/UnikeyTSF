#pragma once
// =============================================================================
// UniKey TSF Reborn — Win32 Resource IDs
// =============================================================================

// Application icon
#define IDI_APP_ICON        100

// Tray icons
#define IDI_TRAY_V          101
#define IDI_TRAY_E          102

// Tray notification
#define WM_TRAYICON         (WM_USER + 1)
#define IDC_TRAY            1000

// Hotkey IDs
#define IDH_TOGGLE_VE       1
#define IDH_TOOLKIT_CONVERT 2
#define IDH_TOOLKIT_STRIP   3

// Context menu items
#define IDM_TOGGLE_VE       2001
#define IDM_INPUT_TELEX     2010
#define IDM_INPUT_VNI       2011
#define IDM_INPUT_VIQR      2012
#define IDM_CHARSET_UNICODE 2020
#define IDM_CHARSET_TCVN3   2021
#define IDM_CHARSET_VNI     2022
#define IDM_SETTINGS        2100
#define IDM_EXIT            2200

// =============================================================================
// Settings Dialog — UniKey-Style Layout (OLED Dark Mode)
// =============================================================================
#define IDD_SETTINGS_DIALOG     3000

// Core options (top section, matches UniKey's Bảng điều khiển)
#define IDC_CHK_VIETNAMESE      3001   // "Bật gõ tiếng Việt" toggle
#define IDC_LBL_CHARSET         3002
#define IDC_CBO_CHARSET         3003   // Bảng mã
#define IDC_LBL_INPUT_METHOD    3004
#define IDC_CBO_INPUT_METHOD    3005   // Kiểu gõ
#define IDC_GRP_BASIC           3009   // Cơ bản groupbox

// Toggle key section (Phím chuyển)
#define IDC_GRP_TOGGLE_KEY      3010
#define IDC_RAD_CTRL_SHIFT      3011   // Ctrl+Shift
#define IDC_RAD_ALT_Z           3012   // Alt+Z

// Options section
#define IDC_GRP_OPTIONS         3020
#define IDC_CHK_SPELL_CHECK     3021   // Kiểm tra chính tả
#define IDC_CHK_FREE_TONE       3022   // Bỏ dấu tự do
#define IDC_CHK_MACRO           3023   // Gõ tắt (Macro)
#define IDC_CHK_AUTO_START      3024   // Khởi động cùng Windows

// Tone type (Kiểu dấu)
#define IDC_GRP_TONE_TYPE       3030
#define IDC_RAD_MODERN_TONE     3031   // Kiểu mới (oà)
#define IDC_RAD_CLASSIC_TONE    3032   // Kiểu cũ (òa)

// Macro file
#define IDC_LBL_MACRO_FILE      3040
#define IDC_TXT_MACRO_FILE      3041
#define IDC_BTN_MACRO_FILE      3042

// Bottom buttons
#define IDC_BTN_EXPAND          3050   // Mở rộng >>
#define IDC_BTN_CLOSE           3051   // Đóng

// Expanded panel — Blacklist editor
#define IDC_GRP_BLACKLIST       3060   // Groupbox: "Danh sách loại trừ"
#define IDC_LST_BLACKLIST       3061   // ListBox showing blacklisted process names
#define IDC_TXT_BLACKLIST_ADD   3062   // Edit field for adding new process name
#define IDC_BTN_BLACKLIST_ADD   3063   // "Thêm" button
#define IDC_BTN_BLACKLIST_DEL   3064   // "Xóa" button
#define IDC_BTN_BLACKLIST_BROWSE 3065  // "..." button to browse exe

// Tab-based layout (redesign v2)
#define IDC_TAB_BASIC           3071
#define IDC_TAB_ADVANCED        3072   // Tab control
#define IDC_TOGGLE_VIET         3071   // Custom toggle switch area
#define IDC_STATUS_MODE         3072   // Status header region
#define IDC_LBL_SHORTCUT1       3073   // Shortcut label 1
#define IDC_LBL_SHORTCUT2       3074   // Shortcut label 2

#ifndef IDC_STATIC
#define IDC_STATIC -1
#endif
