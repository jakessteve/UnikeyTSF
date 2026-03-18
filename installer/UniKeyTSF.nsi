; =============================================================================
; UniKey TSF Reborn — NSIS Installer Script
; =============================================================================
; Compile with: makensis installer\UniKeyTSF.nsi
; Output:       installer\UniKeyTSF_Setup.exe
; =============================================================================

!include "MUI2.nsh"
!include "FileFunc.nsh"

; --- Product Info ---
!define PRODUCT_NAME    "UniKey TSF Reborn"
!define PRODUCT_VERSION "2.0.0"
!define PRODUCT_PUBLISHER "UniKey TSF Project"
!define PRODUCT_WEB     "https://github.com/user/UniKeyTSF"
!define EXE_NAME        "UniKeyTSF.exe"
!define INSTALL_DIR     "$PROGRAMFILES\UniKey TSF Reborn"
!define REG_UNINSTALL   "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define REG_AUTOSTART   "Software\Microsoft\Windows\CurrentVersion\Run"

; --- Installer Config ---
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "UniKeyTSF_Setup.exe"
InstallDir "${INSTALL_DIR}"
RequestExecutionLevel admin
SetCompressor /SOLID lzma

; --- MUI Pages ---
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Vietnamese"

; =============================================================================
; Installer Sections
; =============================================================================

Section "UniKey TSF Core (required)" SecCore
    SectionIn RO  ; Required, cannot be deselected

    ; --- Kill running instance ---
    nsExec::ExecToLog 'taskkill /F /IM UniKeyTSF.exe'

    ; --- Install files ---
    SetOutPath "$INSTDIR"
    File "..\production\UniKeyTSF.exe"
    File "..\production\uktsf_core64.dll"
    File "..\production\uktsf_core32.dll"

    ; --- Register TSF DLL ---
    nsExec::ExecToLog '"$INSTDIR\UniKeyTSF.exe" /register'

    ; --- Create uninstaller ---
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; --- Registry: Add/Remove Programs ---
    WriteRegStr HKLM "${REG_UNINSTALL}" "DisplayName" "${PRODUCT_NAME}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "DisplayVersion" "${PRODUCT_VERSION}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "Publisher" "${PRODUCT_PUBLISHER}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "URLInfoAbout" "${PRODUCT_WEB}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    WriteRegStr HKLM "${REG_UNINSTALL}" "InstallLocation" "$INSTDIR"
    WriteRegDWORD HKLM "${REG_UNINSTALL}" "NoModify" 1
    WriteRegDWORD HKLM "${REG_UNINSTALL}" "NoRepair" 1

    ; Calculate installed size
    ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
    IntFmt $0 "0x%08X" $0
    WriteRegDWORD HKLM "${REG_UNINSTALL}" "EstimatedSize" $0
SectionEnd

Section "Start Menu Shortcuts" SecShortcuts
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${EXE_NAME}"
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Auto-start with Windows" SecAutoStart
    WriteRegStr HKCU "${REG_AUTOSTART}" "${PRODUCT_NAME}" '"$INSTDIR\${EXE_NAME}"'
SectionEnd

; --- Section Descriptions ---
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} "Core UniKey TSF files (required)"
  !insertmacro MUI_DESCRIPTION_TEXT ${SecShortcuts} "Add shortcuts to the Start Menu"
  !insertmacro MUI_DESCRIPTION_TEXT ${SecAutoStart} "Start UniKey TSF automatically when Windows starts"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; =============================================================================
; Uninstaller
; =============================================================================

Section "Uninstall"
    ; --- Unregister TSF ---
    nsExec::ExecToLog '"$INSTDIR\UniKeyTSF.exe" /unregister'

    ; --- Kill running instance ---
    nsExec::ExecToLog 'taskkill /F /IM UniKeyTSF.exe'

    ; Wait for process to fully exit
    Sleep 1000

    ; --- Remove files ---
    Delete "$INSTDIR\UniKeyTSF.exe"
    Delete "$INSTDIR\uktsf_core64.dll"
    Delete "$INSTDIR\uktsf_core32.dll"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir "$INSTDIR"

    ; --- Remove shortcuts ---
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

    ; --- Remove registry ---
    DeleteRegKey HKLM "${REG_UNINSTALL}"
    DeleteRegValue HKCU "${REG_AUTOSTART}" "${PRODUCT_NAME}"

    ; --- Remove config data ---
    RMDir /r "$APPDATA\UniKeyTSF"
SectionEnd
