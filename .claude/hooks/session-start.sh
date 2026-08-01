#!/bin/bash
set -euo pipefail

# Installs media tools needed by the claude-video-vision plugin.
# Only for Claude Code on the web containers — local machines manage their own tools.
if [ "${CLAUDE_CODE_REMOTE:-}" != "true" ]; then
  exit 0
fi

# ffmpeg/ffprobe: required for frame extraction and audio processing
if ! command -v ffmpeg >/dev/null 2>&1; then
  export DEBIAN_FRONTEND=noninteractive
  # update may partially fail on blocked third-party PPAs; install still works off main repos
  apt-get update -qq || true
  apt-get install -y -qq --no-install-recommends ffmpeg
fi

# yt-dlp: optional, enables YouTube URL support; release binary stays current.
# Non-fatal — video file analysis works without it.
if ! command -v yt-dlp >/dev/null 2>&1; then
  if curl -fsSL https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp \
    -o /usr/local/bin/yt-dlp; then
    chmod +x /usr/local/bin/yt-dlp
  else
    echo "yt-dlp download failed; YouTube URL support unavailable this session" >&2
  fi
fi

echo "video tools ready: $(ffmpeg -version | head -1)"
