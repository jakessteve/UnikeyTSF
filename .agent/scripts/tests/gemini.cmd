@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0mock-gemini.ps1" %*
