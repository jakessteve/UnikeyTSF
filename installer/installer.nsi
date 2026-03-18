; =============================================================================
; UniKey TSF Reborn — NSIS Installer Script
; =============================================================================
; Build with: makensis installer.nsi
; Requires: NSIS 3.x (https://nsis.sourceforge.io/Download)
; =============================================================================

!include "MUI2.nsh"

; --- Product Info ---
!define PRODUCT_NAME "UniKey TSF Reborn"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_PUBLISHER "UniKey TSF"
!define PRODUCT_EXE "UniKeyTSF.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

; --- General ---
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "UniKeyTSF_Setup_${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES64\${PRODUCT_NAME}"
InstallDirRegKey HKCU "${PRODUCT_UNINST_KEY}" "InstallLocation"
RequestExecutionLevel user  ; Per-user install (no admin required)
Unicode true

; --- UI ---
!define MUI_ABORTWARNING
!define MUI_ICON "..\res\icon_v.ico"
!define MUI_UNICON "..\res\icon_e.ico"

; --- Pages ---
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; --- Language ---
!insertmacro MUI_LANGUAGE "English"

; =============================================================================
; Install Section
; =============================================================================
Section "Install"
    SetOutPath "$INSTDIR"

    ; Copy files
    File "..\production\UniKeyTSF.exe"
    File "..\production\uktsf_core64.dll"
    ; File "..\production\uktsf_core32.dll"  ; Uncomment when Win32 DLL is needed

    ; Create default blacklist.txt if it doesn't exist
    IfFileExists "$INSTDIR\blacklist.txt" +2
        File /oname=blacklist.txt "..\production\blacklist.txt"

    ; Create Start Menu shortcut
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_EXE}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

    ; Auto-start registry entry (HKCU — per-user, no admin)
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${PRODUCT_NAME}" '"$INSTDIR\${PRODUCT_EXE}"'

    ; Uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    WriteRegStr HKCU "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME}"
    WriteRegStr HKCU "${PRODUCT_UNINST_KEY}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    WriteRegStr HKCU "${PRODUCT_UNINST_KEY}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKCU "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
    WriteRegStr HKCU "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
    WriteRegDWORD HKCU "${PRODUCT_UNINST_KEY}" "NoModify" 1
    WriteRegDWORD HKCU "${PRODUCT_UNINST_KEY}" "NoRepair" 1
SectionEnd

; =============================================================================
; Uninstall Section
; =============================================================================
Section "Uninstall"
    ; Kill running instance
    nsExec::ExecToLog 'taskkill /f /im ${PRODUCT_EXE}'

    ; Unregister TSF COM server
    nsExec::ExecToLog '"$INSTDIR\${PRODUCT_EXE}" /unregister'

    ; Remove auto-start
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "${PRODUCT_NAME}"

    ; Remove files
    Delete "$INSTDIR\${PRODUCT_EXE}"
    Delete "$INSTDIR\uktsf_core64.dll"
    Delete "$INSTDIR\uktsf_core32.dll"
    Delete "$INSTDIR\blacklist.txt"
    Delete "$INSTDIR\config.dat"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir "$INSTDIR"

    ; Remove Start Menu shortcuts
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

    ; Remove registry
    DeleteRegKey HKCU "${PRODUCT_UNINST_KEY}"
SectionEnd
