#!/usr/bin/env python3

import os
import re

# 输出的Markdown文件名
output_file = "output.md"

content = ""

content += "# 算法模板\n\n"

# 遍历所有目录和文件
for root, dirs, files in os.walk("."):
    # 忽略.git等隐藏目录
    dirs[:] = [d for d in dirs if not d.startswith(".")]

    # 获取相对路径作为目录名
    rel_dir = os.path.relpath(root, ".")
    if rel_dir == ".":
        rel_dir = ""

    # 收集当前目录下的C/C++文件
    cpp_files = [f for f in files if f.endswith((".c", ".cpp", ".h", ".hpp"))]

    if cpp_files:
        # 添加二级标题（目录名）
        content += f"## {rel_dir if rel_dir else '根目录'}\n\n"

        for file in cpp_files:
            filepath = os.path.join(root, file)
            try:
                with open(filepath, "r", encoding="utf-8") as f:
                    code = f.read()
            except UnicodeDecodeError:
                with open(filepath, "r", encoding="gbk", errors="ignore") as f:
                    code = f.read()

            # 提取头部注释中的name和description
            name = file  # 默认使用文件名
            description = ""
            header_match = re.search(r"/\*([\s\S]*?)\*/", code)
            if header_match:
                header_content = header_match.group(1)
                # 处理头部注释，逐行解析
                lines = header_content.split("\n")
                in_description = False
                description_lines = []
                for line in lines:
                    line = line.strip(" *")  # 去除行首的*号和空格
                    if line.startswith("name:"):
                        name = line[len("name:") :].strip()
                    elif line.startswith("description:"):
                        in_description = True
                    elif in_description:
                        description_lines.append(line)
                # 移除空行开头和结尾的空行，保留中间的空行
                while description_lines and not description_lines[0]:
                    description_lines.pop(0)
                while description_lines and not description_lines[-1]:
                    description_lines.pop()
                if description_lines:
                    description = "\n".join(description_lines)
                # 移除头部注释
                code = code.replace(header_match.group(0), "").lstrip()

            # 添加三级标题（文件名或name）
            content += f"### {name}\n\n"
            # 添加description
            if description:
                content += f"{description}\n\n"
            # 添加代码块，不含头部注释
            lang = "cpp" if file.endswith((".cpp", ".hpp")) else "c"
            code = code.rstrip()
            content += f"```{lang}\n{code}\n```\n\n"

# 检查是否有内容需要写入
if content:
    # 将合并的内容写入Markdown文件
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"\n已生成 {output_file}")
else:
    print("未找到任何C/C++代码，未生成输出文件。")
