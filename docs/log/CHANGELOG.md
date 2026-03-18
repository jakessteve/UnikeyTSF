# Changelog

All notable changes to this project will be documented in this file.
Format follows [Keep a Changelog](https://keepachangelog.com/).

## [Unreleased]

### Added
- Agent framework token burn rate optimization (50% reduction target)
- Model routing enforcement via `-ModelTier` parameter
- `AGENTS-LITE.md` for lightweight CLI worker context
- Documentation restructure into `/docs/{tech,biz,log}/`

### Changed
- Model routing: 3-model setup (Gemini High, Opus, Sonnet)
- Swarm execution: max 4 workers, mandatory batching
- Mechanical task timeout: 60s with no retry

### Removed
- Emoji/icon characters from all framework files (~1,467 icons)
- Advisory-only model routing disclaimers
- `GEMINI-L/Fast`, `OPUS/Fast`, `SONNET/Plan` model codes
