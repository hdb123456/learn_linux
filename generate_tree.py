import os

def generate_directory_tree(dir_path, prefix=''):
    """生成目录树并返回 Markdown 格式的字符串。"""
    markdown = ""
    for entry in sorted(os.listdir(dir_path)):
        entry_path = os.path.join(dir_path, entry)
        if os.path.isdir(entry_path):
            markdown += f"- {entry}/\n"
            markdown += generate_directory_tree(entry_path, prefix=f'{prefix}{entry}/')
        else:
            markdown += f"- [{entry}]({prefix}{entry})\n"
    return markdown

if __name__ == "__main__":
    dir_path = '.'  # 当前目录
    tree = generate_directory_tree(dir_path)
    with open('README.md', 'w') as readme:
        readme.write(tree)
