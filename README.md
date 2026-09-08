# Blacad Docs

网站使用 Zensical 构建，`docs/CS` 同时是 Obsidian vault 和网站的 CS 内容目录。在 Obsidian 中直接打开 `docs/CS`，之后的编辑会直接修改网站使用的同一份 Markdown 文件。

Vault 已设置为使用相对 Markdown 链接，新附件保存到 `docs/CS/assets`，便于 Obsidian 和网站同时解析。导航根据目录自动生成，新建笔记不需要再手工维护 `zensical.toml` 中的页面列表。

除网站首页 `docs/index.md` 外，各目录的说明页统一命名为 `说明.md`。不要在子目录中创建 `index.md`，以免目录标题被渲染成页面链接，影响侧栏的展开操作。

## 本地预览

```bash
python3.12 -m pip install zensical
zensical serve
```

Zensical 需要 Python 3.10 或更高版本；本仓库建议直接使用 Python 3.12。

## 检查与发布

```bash
python3 scripts/check_links.py
./scripts/publish-notes.sh "更新课程笔记"
```

发布脚本会先检查本地链接并完整构建网站，然后只暂存笔记和相关网站配置，最后提交并推送到当前分支。`main` 分支的推送会由 GitHub Actions 自动构建并部署 GitHub Pages。
