# Track 2 Report: Requirements & Content
**Agent:** @ba

## Findings
1. **Typography/Spelling Inconsistency:** In `src/UniKeyTSF.rc`, the labels for Tone Type are: `"Mới (hoá)"` and `"Cũ (hóa)"`. The placement of the accent on "oa" is inconsistent (hoá vs hóa).
2. **Technical Jargon in UI:** "Bảng mã" (Charset) includes options like "VNI Windows" and "TCVN3 (ABC)". For modern users, Unicode is 99.9% of use cases. This could be simplified or hidden behind an "Advanced" toggle.
3. **Lack of Tooltips:** Settings like "Bỏ dấu tự do" (Free tone marking) lack tooltips explaining their behavior.

## UX Severity
- Issue 1: Minor (Aesthetics/Attention to detail)
- Issue 2: Minor (Legacy support is necessary but clutters UI)
- Issue 3: Enhancement (Tooltips would help clarify features)
