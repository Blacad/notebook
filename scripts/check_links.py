#!/usr/bin/env python3
"""检查 docs 中的本地 Markdown 链接和 Obsidian 双链。"""

from __future__ import annotations

import re
import sys
from pathlib import Path
from urllib.parse import unquote


ROOT = Path(__file__).resolve().parents[1]
DOCS = ROOT / "docs"
WIKI_LINK = re.compile(r"!?\[\[([^\]]+)\]\]")
INLINE_CODE = re.compile(r"`[^`]*`")


def markdown_targets(text: str):
    """Yield destinations from inline Markdown links, including balanced parentheses."""
    cursor = 0
    while True:
        start = text.find("](", cursor)
        if start < 0:
            return
        pos = start + 2
        depth = 1
        escaped = False
        while pos < len(text) and depth:
            char = text[pos]
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == "(":
                depth += 1
            elif char == ")":
                depth -= 1
            pos += 1
        if depth == 0:
            yield text[start + 2 : pos - 1].strip()
        cursor = max(pos, start + 2)


def clean_target(raw: str) -> str:
    if raw.startswith("<") and ">" in raw:
        return raw[1 : raw.index(">")]
    # 允许 Markdown 链接在地址后携带可选标题。
    match = re.match(r'^(.*?)(?:\s+["\'].*["\'])$', raw)
    return match.group(1) if match else raw


def main() -> int:
    failures: list[str] = []
    checked = 0

    for note in sorted(DOCS.rglob("*.md")):
        text = note.read_text(encoding="utf-8")

        visible_text = INLINE_CODE.sub("", text)
        for match in WIKI_LINK.finditer(visible_text):
            failures.append(
                f"{note.relative_to(ROOT)}: Obsidian 双链未转为 Markdown 链接: {match.group(0)}"
            )

        for raw_target in markdown_targets(text):
            target = clean_target(raw_target)
            if not target or target.startswith(("#", "http://", "https://", "mailto:", "data:")):
                continue

            path_part = unquote(target.split("#", 1)[0].split("?", 1)[0])
            if not path_part:
                continue

            checked += 1
            destination = (note.parent / path_part).resolve()
            try:
                destination.relative_to(DOCS.resolve())
            except ValueError:
                failures.append(
                    f"{note.relative_to(ROOT)}: 链接超出 docs 目录: {target}"
                )
                continue
            if not destination.exists():
                failures.append(
                    f"{note.relative_to(ROOT)}: 找不到链接目标: {target}"
                )

    if failures:
        print("\n".join(failures))
        print(f"\n检查失败：{len(failures)} 个问题。")
        return 1

    print(f"链接检查通过：已验证 {checked} 个本地链接。")
    return 0


if __name__ == "__main__":
    sys.exit(main())
