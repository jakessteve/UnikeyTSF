import unittest
import time
import os
from pywinauto.application import Application
from pywinauto import timings

class TestSettingsUI(unittest.TestCase):
    def setUp(self):
        # Path to the newly built executable from CMake
        self.exe_path = os.path.abspath(r"..\..\build\Debug\UniKeyTSF.exe")
        # Ensure it exists before testing
        if not os.path.exists(self.exe_path):
            self.exe_path = os.path.abspath(r"..\..\out\build\x64-Debug\UniKeyTSF.exe")

        # Start the application
        try:
            self.app = Application(backend="uia").start(self.exe_path)
        except Exception as e:
            self.fail(f"Failed to start UniKeyTSF.exe: {e}")
            
        time.sleep(1) # Give it a moment to initialize tray and IPC

    def tearDown(self):
        # Clean up by killing the test process
        try:
            self.app.kill()
        except:
            pass

    def test_settings_dialog_opens(self):
        # Find the Settings dialog window
        try:
            # We assume it opens by default on start, or find it by title
            settings_dlg = self.app.window(title_re=".*Cấu hình.*")
            settings_dlg.wait("visible", timeout=5)
            self.assertTrue(settings_dlg.exists(), "Settings Dialog did not open or is not visible.")
            
            # Interact with a checkbox to prove IPC/UI is responsive
            # Enable Spell Check checkbox
            spellcheck_cb = settings_dlg.child_window(title="Bật kiểm tra chính tả", control_type="CheckBox")
            if spellcheck_cb.exists():
                spellcheck_cb.toggle()
                
            # Close the dialog
            settings_dlg.close()
        except timings.TimeoutError:
            self.fail("Settings dialog never became visible.")

if __name__ == '__main__':
    unittest.main()
