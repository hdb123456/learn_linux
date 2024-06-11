#!/bin/bash

# 生成目录结构
tree > directory.md

# 更新 README.md
echo "# Directory Structure" > README.md
echo "" >> README.md
echo "Generated at: $(date)" >> README.md
echo "" >> README.md
cat directory.md >> README.md

# 提交更改到仓库
git config --global user.email "actions@github.com"
git config --global user.name "GitHub Actions"
git add README.md
git commit -m "Update directory structure in README"
git push
