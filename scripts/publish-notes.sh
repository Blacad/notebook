#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
commit_message="${1:-更新 CS 笔记}"

cd "$repo_root"
python3 scripts/check_links.py
zensical build --clean

git add -- docs/CS docs/.nav.yml zensical.toml .gitignore README.md scripts .github/workflows/ci.yml

if git diff --cached --quiet; then
  echo "没有需要提交的笔记更新。"
  exit 0
fi

git commit -m "$commit_message"
git push origin HEAD
